#include "type/Assets.h"
#include "MainBoard.h"
#include "SystemCalls.h"

#include <fatfs/ff.h>
#include <gameduino2/GD2.h>
#include <hal_delay.h>
#include <cstring>

constexpr const char *piServiceFile = DRV_SD "Service0.txt";

const std::array<const char *, 5> MainBoard::filterTypes = {
	"CarbonPlus",
	"CarbonPro",
	"FiberTek",
	"CarbonPhos",
	"CarbonSilv"
};

const std::array<const char *, 5> MainBoard::filterReorders = {
	"5335",
	"6905",
	"6906",
	"5341",
	"6900"
};

char MainBoard::date[Format::size::date];
char MainBoard::time[9] = "";
bool MainBoard::ampm = false;

bool MainBoard::inMetric = false;

int MainBoard::modelNumber = 0;
char MainBoard::serialNumber[16] = "";
char MainBoard::softwareVersion[6];

int MainBoard::filterType = 0;
char MainBoard::filterRemaining[5] = "";
int MainBoard::filterRemainingCount = 0;
int MainBoard::filterMonthsRemaining = 0;
char MainBoard::filterLastChanged[Format::size::date];
char MainBoard::flowRate[Format::size::flowRate];

char MainBoard::serviceContact[200] = "";

unsigned int MainBoard::tankTemperatures[3] = { 0, 0, 0 };

NOOPTIMIZE
int serialGet(void)
{
	if (serialGetchar() != '$')
		return 0;
	return serialGetchar();
}

void MainBoard::updateMetric(void)
{
	serialPrintf("@X");
	inMetric = serialGet() != 0;
}

void MainBoard::updateDateTime(void)
{
	char timeSuffix[3] = {
		'\0', 'A', 'M'
	};

	serialPrintf("@T");
	int h = serialGet();
	int m = serialGet();
	/*int s =*/serialGet(); // Seconds unused
	int rawampm = serialGet();
	ampm = rawampm < 2;
	if (ampm) {
		timeSuffix[0] = ' ';
		timeSuffix[1] = (rawampm == 1) ? 'P' : 'A';
	} else {
		timeSuffix[0] = '\0';
	}

	sprintf(time, "%d:%02d%s", h, m, timeSuffix);

	serialPrintf("@D");
	uint32_t val = serialGet() << 16; // Month
	val |= serialGet() << 8; // Day
	val |= serialGet(); // Year
	Format::date(date, val, '-');
}

int MainBoard::updateModelNumber(void) {
	serialPrintf("@!");
	modelNumber = serialGet();
	return modelNumber;
}

const char *MainBoard::updateSerialNumber(void) {
	serialPrintf("@E");
	for (int i = 0; i < 15; i++)
		serialNumber[i] = serialGet();
	return serialNumber;
}

const char *MainBoard::updateSoftwareVersion(void) {
	serialPrintf("@#");
	int major = serialGet();
	int minor = serialGet();
	sprintf(softwareVersion, "%d.%d", major, minor);
	return softwareVersion;
}

int MainBoard::updateFilterType(void) {
	serialPrintf("@%%");
	filterType = serialGet();
	return filterType;
}

const char *MainBoard::updateFilterRemaining(void) {
    // Get number of remaining
	serialPrintf("@&");
	filterRemainingCount = serialGet() << 8;
	filterRemainingCount |= serialGet();
    if (isMetric())
        filterRemainingCount = GaltoL(filterRemainingCount);

    // Convert to percentage
    int div;
    if (int type = updateFilterType(); type == 0 || type == 2)
        div = inMetric ? 5700 : 1500;
    else
        div = inMetric ? 2850 : 750;

    if (filterRemainingCount < div) {
        div /= 10;
        int n = filterRemainingCount / div;
        filterRemaining[0] = n + '0';
        filterRemaining[1] = filterRemainingCount / (div / 10) - n * 10 + '0';
        filterRemaining[2] = '%';
        filterRemaining[3] = '\0';
    } else {
        strcpy(filterRemaining, "100%");
    }

    return filterRemaining;
}

int MainBoard::updateFilterMonthsRemaining(void)
{
	serialPrintf("@`");
	filterMonthsRemaining = serialGet();
	return filterMonthsRemaining;
}

const char *MainBoard::updateFilterLastChanged(void)
{
	serialPrintf("@(");
	int val = serialGet() << 16; // Month
	val |= serialGet() << 8; // Day
	val |= serialGet(); // Year
	Format::date(filterLastChanged, val);
	return filterLastChanged;
}

const char *MainBoard::updateFlowRate(void) {
	serialPrintf("@F");
	Format::flowRate(flowRate, serialGet());
	return flowRate;
}

const char *MainBoard::updateServiceContact(void) {
	FIL fil;
	auto result = f_open(&fil, piServiceFile, FA_READ);
	if (result == FR_OK) {
		UINT unused;
		f_read(&fil, serviceContact, sizeof(serviceContact), &unused);
		unsigned int i;
		for (i = 0; serviceContact[i] != '\0' &&
			serviceContact[i] != ':'; i++) {
			if (serviceContact[i] == '\r')
				serviceContact[i] = ' ';
		}
		serviceContact[i] = '\0';
		f_close(&fil);
	}

	return serviceContact;
}

const unsigned int *MainBoard::getTankTemperatures(void)
{
	for (int i = 0; i < 3; i++) {
		serialPrintf("@%1u", i + 6);
		tankTemperatures[i] = serialGet();
	}
	return tankTemperatures;
}

void MainBoard::setTankTemperatures(const unsigned int *temps)
{
	for (int i = 0; i < 3; i++)
		serialPrintf("@%1u%1u", i + 3, temps[i]);
}

int MainBoard::getLanguage(void)
{
	serialPrintf("#6");
	return serialGet();
}

void MainBoard::setLanguage(int lang)
{
	serialPrintf("@2%1u", lang);
}

bool MainBoard::isColdTankFull(void) {
	serialPrintf("#9");
	return serialGet() == 1;
}

bool MainBoard::isHotTankFull(void) {
	if (!canDispenseHot())
		return true;

	serialPrintf("#:");
	return serialGet() == 1;
}

bool MainBoard::isSparklingTankFull(void) {
	if (!canDispenseSparkling())
		return true;

	serialPrintf("#;");
	return serialGet() == 1;
}

bool MainBoard::isLocked(void)
{
	serialPrintf("@K");
	return serialGet() == 1;
}

void MainBoard::setLocked(bool locked)
{
	serialPrintf("@L%c", locked ? '1' : '0');
}

void MainBoard::getSleepmodeHours(unsigned int *buf)
{
	serialPrintf("@M");
	for (int i = 0; i < 4; i++)
		buf[i] = serialGet();
}

void MainBoard::setSleepmodeHours(const unsigned int *hours)
{
	serialPrintf("@N%02u%02u%02u%02u", hours[0], hours[1], hours[2], hours[3]);
}

bool MainBoard::getSleepmodeEnabled(void)
{
	serialPrintf("@H");
	return serialGet() != 0;
}

void MainBoard::setSleepmodeEnabled(bool en)
{
	serialPrintf("@G%01u", en ? 1 : 0);
}

void MainBoard::allowDispenseError(bool yes)
{
	serialPrintf("@J%01u", yes ? 0 : 1);
}

void MainBoard::clearDispenseError(void)
{
	serialPrintf("$I$F");
}

void MainBoard::autofill(void)
{
	serialPrintf("@i3");

	clearScreenWithIonHeader();
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.cmd_spinner(GD.h / 2, (GD.w / 2), 0, 0);
	GD.swap();

	delay_ms(3000);

	NVIC_SystemReset();
	while (1);
}

void MainBoard::factoryReset(void)
{
	serialPrintf("@Q1");
}

void MainBoard::getBottlesSaved(char *buf)
{
	buf[0] = '\0';
	serialPrintf("^B");

	if (serialGetchar() != '$')
		return;

	while (1) {
		auto c = serialGetchar();
		if (c == '$') {
			*buf = '\0';
			break;
		}

		*buf++ = c;
	}
}

