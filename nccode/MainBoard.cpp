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

DateFormat MainBoard::date ('-');
char MainBoard::time[8] = { 0, 0, ':', 0, 0, 0, 'M', '\0' };

bool MainBoard::inMetric = false;

int MainBoard::modelNumber = 0;
char MainBoard::serialNumber[16] = "";
char MainBoard::softwareVersion[4] = { '0', '.', '0', '\0' };

int MainBoard::filterType = 0;
int MainBoard::filterRemaining = 0;
int MainBoard::filterMonthsRemaining = 0;
DateFormat MainBoard::filterLastChanged;
GPMFormat MainBoard::flowRate;

char MainBoard::serviceContact[200] = "";

unsigned int MainBoard::tankTemperatures[3] = { 0, 0, 0 };

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
	int val;

	serialPrintf("@T");
	val = serialGet(); // Hour
	time[0] = val / 10 + '0';
	time[1] = val % 10 + '0';
	val = serialGet(); // Minute
	time[3] = val / 10 + '0';
	time[4] = val % 10 + '0';
	serialGet(); // Second, unused
	val = serialGet(); // AM/PM
	if (val == 2)
		time[5] = '\0';
	else
		time[5] = (val ? 'P' : 'A');

	serialPrintf("@D");
	val = serialGet() << 16; // Month
	val |= serialGet() << 8; // Day
	val |= serialGet(); // Year
	date.format(val);
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
	softwareVersion[0] = serialGet();
	softwareVersion[2] = serialGet();
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
	filterLastChanged.format(val);
	return filterLastChanged.get();
}

const char *MainBoard::updateFlowRate(void) {
	serialPrintf("@F");
	flowRate.format(serialGet());
	flowRate.setUnits(inMetric);
	return flowRate.get();
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
		serialPrintf("@%1u%1u", i + 3, tankTemperatures[i]);
}


bool MainBoard::isColdTankFull(void) {
	serialPrintf("#9");
	serialPrintf("%?");
	return serialGet() == 1 ? true : false;
}

bool MainBoard::isHotTankFull(void) {
	serialPrintf("#:");
	serialPrintf("%?");
	return serialGet() == 1 ? true : false;
}

bool MainBoard::isSparklingTankFull(void) {
	serialPrintf("#;");
	serialPrintf("%?");
	return serialGet() == 1 ? true : false;
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

