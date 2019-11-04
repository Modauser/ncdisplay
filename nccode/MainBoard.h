/**
 * @file MainBoard.h
 * @brief Main interface for communicating with the main board.
 */

#ifndef MAINBOARD_H_
#define MAINBOARD_H_

#include "type/Formatters.h"

#include <array>

/**
 * Printf that communicates with UART to the main board.
 * @param s Format string
 * @param ... Arguments for format string
 */
extern int (*serialPrintf)(const char *s, ...);

/**
 * Fetches a 'response' character from the main board.
 *
 * Main board responds in bytes separated by '$', this consumes the '$' then
 * returns the next character.
 * @return Byte from main board
 */
int serialGet(void);

/**
 * Converts gallons to liters.
 *
 * Does a rough conversion to avoid floating-point math.
 */
constexpr int GaltoL(int gallons) {
	return gallons * 38 / 10;
}

/**
 * Converts degrees Fahrenheit to Celsius.
 *
 * Does a rough conversion to avoid floating-point math.
 */
constexpr int FtoC(int f) {
	return (f - 32) / 2;
}

/**
 * @enum FilterType
 * @brief Enumerates possible filter types.
 */
enum class FilterType : char {
	CarbonPlus = 0,
	CarbonPro,
	FiberTek,
	CarbonPhos,
	CarbonSilv
};

/**
 * @class MainBoard
 * @brief An interface to handle most (or all) main board communication.
 *
 * Most main board queries have an update...() and get...() pair. 'Update' calls
 * request this class to get the latest value from the main board, 'get' calls
 * return the latest stored value. 'get' calls without 'update' pairs retrieve
 * data from the main board within the 'get' call.
 */
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
	static char filterRemaining[5];
    static int filterRemainingCount;
	static int filterMonthsRemaining;
	static char filterLastChanged[Format::size::date];
	static char flowRate[Format::size::flowRate];

	static char serviceContact[200];

	static unsigned int tankTemperatures[3];

	static const std::array<const char *, 5> filterTypes;
	static const std::array<const char *, 5> filterReorders;

public:
	// Metric
	static inline bool isMetric(void) {
		updateMetric();
		return inMetric;
	}
	static void updateMetric(void);

	// Time/Date
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

	// Model number
	static inline int getModelNumber(void) {
		return modelNumber;
	}
	static int updateModelNumber(void);

	// Serial number
	static inline const char *getSerialNumber(void) {
		return serialNumber;
	}
	static const char *updateSerialNumber(void);

	// Software version
	static inline const char *getSoftwareVersion(void) {
		return softwareVersion;
	}
	static const char *updateSoftwareVersion(void);

	// Filter type
	static inline FilterType getFilterType(void) {
		return static_cast<FilterType>(filterType);
	}
	static int updateFilterType(void);

	// Filter name
	static inline const char *getFilterName(void) {
		return filterTypes[filterType];
	}
	// Filter Re-order number text
	static inline const char *getFilterReorder(void) {
		return filterReorders[filterType];
	}

	// Filter gallons/liters remaining
    static inline int getFilterRemainingCount(void) {
        return filterRemainingCount;
    }
	static inline const char *getFilterRemaining(void) {
		return filterRemaining;
	}
	static const char *updateFilterRemaining(void);

	// Filter months remaining
	static inline int getFilterMonthsRemaining(void) {
		return filterMonthsRemaining;
	}
	static int updateFilterMonthsRemaining(void);

	// Filter last changed date
	static inline const char *getFilterLastChanged(void) {
		return filterLastChanged;
	}
	static const char *updateFilterLastChanged(void);

	// Flow rate
	static inline const char *getFlowRate(void) {
		return flowRate;
	}
	static const char *updateFlowRate(void);

	// Service contact text
	static inline const char *getServiceContact(void) {
		return serviceContact;
	}
	static const char *updateServiceContact(void);

	// Returns true if hot water can be dispensed
	static inline bool canDispenseHot(void) {
		updateModelNumber();
		return modelNumber & (1 << 0);
	}
	// Returns true if sparkling water can be dispensed
	static inline bool canDispenseSparkling(void) {
		updateModelNumber();
		return modelNumber & (1 << 1);
	}

	// SleepMode
	static void getSleepmodeHours(unsigned int *buf);
	static void setSleepmodeHours(const unsigned int *hours);
	static bool getSleepmodeEnabled(void);
	static void setSleepmodeEnabled(bool en);

	// Language
	static int getLanguage(void);
	static void setLanguage(int);

	// Tank temperatures
	static const unsigned int *getTankTemperatures(void);
	static void setTankTemperatures(const unsigned int *temps);

	// Tank full-ness
	static bool isColdTankFull(void);
	static bool isHotTankFull(void);
	static bool isSparklingTankFull(void);

	// Passcode
	static bool isLocked(void);
	static void setLocked(bool locked);

	// Dispense error
	static void allowDispenseError(bool yes = true);
	static void clearDispenseError(void);

	// Autofill routine
	[[noreturn]]
	static void autofill(void);

	// Factory reset request
	static void factoryReset(void);

	// Fills given buffer with bottles saved count text
	static void getBottlesSaved(char *buf);
};

#endif // MAINBOARD_H_

