/**
 * @file Formatters.h
 * @brief Provides utilities for converting numbers to various textual forms.
 */
#ifndef FORMATTERS_H_
#define FORMATTERS_H_

#include <cstddef>

template<std::size_t N>
class NumFormat {
protected:
	char buffer[N];

public:
	constexpr NumFormat(void)
		: buffer{} {}

	constexpr const char *get(void) const {
		return buffer;
	}

	virtual const char *format(int number) = 0;
};

class AMPMFormat : public NumFormat<5> {
public:
	constexpr AMPMFormat(void)
		: NumFormat() {
		buffer[3] = 'M';
		buffer[4] = '\0';
	}

	const char *format(int number) final {
		bool pm = number > 11;
		if (number > 11)
			number -= 12;
		if (number == 0)
			number = 12;

		if (auto div = number / 10; div != 0)
			buffer[0] = '0' + div;
		else
			buffer[0] = ' ';
		buffer[1] = '0' + number % 10;
		buffer[2] = pm ? 'P' : 'A';
		return get();
	}
};

class GPMFormat : public NumFormat<9> {
public:
	constexpr GPMFormat(void)
		: NumFormat() {
		buffer[2] = '.';
		buffer[4] = ' ';
	}

	const char *format(int number) final {
		buffer[3] = (number % 10) + '0';
		number /= 10;
		buffer[1] = (number % 10) + '0';
		number /= 10;
		buffer[0] = (number == 0) ? ' ' : (number % 10) + '0';
		return get();
	}

	void setUnits(bool metric) {
		if (metric) {
			buffer[5] = 'L';
		} else {
			buffer[5] = 'G';
			buffer[6] = 'P';
			buffer[7] = 'M';
		}
	}
};
		
class DateFormat : public NumFormat<11> {
public:
	constexpr DateFormat(char splitter = '/')
		: NumFormat() {
		buffer[2] = splitter;
		buffer[5] = splitter;
		buffer[6] = '2';
		buffer[7] = '0';
		buffer[10] = '\0';
	}

	// Takes in 24-bit number: 0xMMDDYY
	const char *format(int number) final {
		int n = (number >> 16) & 0xFF;

		buffer[0] = '0' + n / 10;
		buffer[1] = '0' + n % 10;

		n = (number >> 8) & 0xFF;
		buffer[3] = '0' + n / 10;
		buffer[4] = '0' + n % 10;

		n = number & 0xFF;
		buffer[8] = '0' + n / 10;
		buffer[9] = '0' + n % 10;

		return get();
	}
};

#endif // FORMATTERS_H_

