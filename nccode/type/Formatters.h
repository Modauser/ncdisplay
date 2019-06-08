/**
 * @file Formatters.h
 * @brief Provides utilities for converting numbers to various textual forms.
 */

#ifndef FORMATTERS_H_
#define FORMATTERS_H_

#include <cstdio>

/**
 * @namespace Format
 * @brief Contains special formatting functions and constants.
 */
namespace Format {
	/**
	 * @namespace size
	 * @brief Contains max buffer size constants for each formatter.
	 */
	namespace size {
		/**
		 * Max buffer size necessary for flow rate.
		 * @see Format::flowRate()
		 */
		constexpr std::size_t flowRate = 9;
		/**
		 * Max buffer size necessary for temperature.
		 * @see Format::temperature()
		 */
		constexpr std::size_t temperature = 6;
		/**
		 * Max buffer size necessary for date.
		 * @see Format::date()
		 */
		constexpr std::size_t date = 11;
		/**
		 * Max buffer size necessary for hour.
		 * @see Format::hour()
		 */
		constexpr std::size_t hour = 6;
	}

	/**
	 * Formats number as a flow rate string, handling metric conversion.
	 * @param buf Buffer to store string in
	 * @param number Flow rate x10 (e.g. 234 = 23.4 GPM)
	 * @return The buffer
	 */
	char *flowRate(char *buf, int number);

	/**
	 * Formats number as a temperature, handling metric conversion.
	 * @param buf Buffer to store string in
	 * @param number Temperature in degrees Fahrenheit 
	 * @return The buffer
	 */
	char *temperature(char *buf, int number);

	/**
	 * Formats the given number as a date, separating numbers with the given
	 * splitter.
	 * Current century is added to the year, producing 20YY.
	 * @param buf Buffer to store string in
	 * @param mmddyy Date number, in the format 0xMMDDYY
	 * @param splitter Number separator (e.g. '/' gives MM/DD/YYYY)
	 * @return The buffer
	 */
	char *date(char *buf, int mmddyy, char splitter = '/');

	/**
	 * Formats the given number as an hour, adding AM/PM if not in 24-hour
	 * mode.
	 * @param buf Buffer to store string in
	 * @param hour The hour, 0-23
	 * @return The buffer
	 */
	char *hour(char *buf, int hour);
}		

#endif // FORMATTERS_H_

