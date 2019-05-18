/**
 * @file MainBoard.cpp
 * @brief Main interface for communicating with the main board.
 */
#include "type/Assets.h"
#include "MainBoard.h"

#include <fatfs/ff.h>
#include <hal_delay.h>

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
	"Unknown"
};

char MainBoard::date[Format::size::date];
char MainBoard::time[9] = "";
bool MainBoard::ampm = false;

bool MainBoard::inMetric = false;

int MainBoard::modelNumber = 0;
char MainBoard::serialNumber[16] = "";
char MainBoard::softwareVersion[6];

int MainBoard::filterType = 0;
int MainBoard::filterRemaining = 0;
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

int MainBoard::updateFilterRemaining(void) {
	serialPrintf("@&");
	filterRemaining = serialGet() << 8;
	filterRemaining |= serialGet();
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
	serialPrintf("#:");
	return serialGet() == 1;
}

bool MainBoard::isSparklingTankFull(void) {
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
	serialPrintf(yes ? "@J\x0" : "@J\x1");
}

void MainBoard::clearDispenseError(void)
{
	serialPrintf("$I$F");
}

void MainBoard::autofill(void)
{
	serialPrintf("@i3");
	while (1)
		delay_ms(100);
}

void MainBoard::factoryReset(void)
{
	serialPrintf("@Q1");
}

