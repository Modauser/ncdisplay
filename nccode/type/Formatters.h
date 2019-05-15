/**
 * @file Formatters.h
 * @brief Provides utilities for converting numbers to various textual forms.
 */
#ifndef FORMATTERS_H_
#define FORMATTERS_H_

#include <cstdio>

namespace Format {
	namespace size {
		constexpr std::size_t flowRate = 8;
		constexpr std::size_t temperature = 6;
		constexpr std::size_t date = 11;
		constexpr std::size_t hour = 6;
	}

	char *flowRate(char *buf, int number);
	char *temperature(char *buf, int number);
	char *date(char *buf, int mmddyy, char splitter = '/');
	char *hour(char *buf, int hour);
}		

#endif // FORMATTERS_H_

