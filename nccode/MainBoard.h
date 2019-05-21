/**
 * @file MainBoard.h
 * @brief Main interface for communicating with the main board.
 */
#ifndef MAINBOARD_H_
#define MAINBOARD_H_

#include "type/Formatters.h"

#include <array>

extern int (*serialPrintf)(const char *, ...);
extern int serialGetchar(void);
int serialGet(void);

constexpr int GaltoL(int gallons) {
	return gallons * 4;
}
constexpr int GPMtoL(int gpm) {
	// Flow rate readings are x10
	return gpm * 40;
}
constexpr int FtoC(int f) {
	return (f - 32) / 2;
}

enum class FilterType : char {
	CarbonPlus = 0,
	CarbonPro,
	FiberTek,
	CarbonPhos,
	CarbonSilv
};

class MainBoard {
private:
	static bool inMetric;

	static char date[Format::size::date];
	static char time[9];
	static bool ampm;

	static int modelNumber;
	static char serialNumber[16];
	static char softwareVersion[6];

	static int filterType;
	static int filterRemaining;
	static int filterMonthsRemaining;
	static char filterLastChanged[Format::size::date];
	static char flowRate[Format::size::flowRate];

	static char serviceContact[200];

	static unsigned int tankTemperatures[3];

	static const std::array<const char *, 5> filterTypes;
	static const std::array<const char *, 5> filterReorders;

public:
	static inline bool isMetric(void) {
		updateMetric();
		return inMetric;
	}
	static void updateMetric(void);

	static inline const char *getTime(void) {
		return time;
	}
	static inline const char *getDate(void) {
		return date;
	}
	static inline bool is24Hour(void) {
		return !ampm;
	}
	static void updateDateTime(void);

	static inline int getModelNumber(void) {
		return modelNumber;
	}
	static int updateModelNumber(void);

	static inline const char *getSerialNumber(void) {
		return serialNumber;
	}
	static const char *updateSerialNumber(void);

	static inline const char *getSoftwareVersion(void) {
		return softwareVersion;
	}
	static const char *updateSoftwareVersion(void);

	static inline FilterType getFilterType(void) {
		return static_cast<FilterType>(filterType);
	}
	static int updateFilterType(void);

	static inline const char *getFilterName(void) {
		return filterTypes[filterType];
	}
	static inline const char *getFilterReorder(void) {
		return filterReorders[filterType];
	}

	static inline int getFilterRemaining(void) {
		return inMetric ? GaltoL(filterRemaining) : filterRemaining;
	}
	static int updateFilterRemaining(void);

	static inline int getFilterMonthsRemaining(void) {
		return filterMonthsRemaining;
	}
	static int updateFilterMonthsRemaining(void);

	static inline const char *getFilterLastChanged(void) {
		return filterLastChanged;
	}
	static const char *updateFilterLastChanged(void);

	static inline const char *getFlowRate(void) {
		return flowRate;
	}
	static const char *updateFlowRate(void);

	static inline const char *getServiceContact(void) {
		return serviceContact;
	}
	static const char *updateServiceContact(void);

	static inline bool canDispenseHot(void) {
		updateModelNumber();
		return modelNumber & (1 << 0);
	}
	static inline bool canDispenseSparkling(void) {
		updateModelNumber();
		return modelNumber & (1 << 1);
	}

	static void getSleepmodeHours(unsigned int *buf);
	static void setSleepmodeHours(const unsigned int *hours);
	static bool getSleepmodeEnabled(void);
	static void setSleepmodeEnabled(bool en);

	static int getLanguage(void);
	static void setLanguage(int);

	static const unsigned int *getTankTemperatures(void);
	static void setTankTemperatures(const unsigned int *temps);

	static bool isColdTankFull(void);
	static bool isHotTankFull(void);
	static bool isSparklingTankFull(void);

	static bool isLocked(void);
	static void setLocked(bool locked);

	static void allowDispenseError(bool yes = true);
	static void clearDispenseError(void);

	[[noreturn]]
	static void autofill(void);

	static void factoryReset(void);

	static void getBottlesSaved(char *buf);
};

#endif // MAINBOARD_H_

