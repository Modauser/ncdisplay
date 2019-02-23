/**
 * @file Settings.h
 * @brief Display-wide settings.
 */
#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "type/LanguageString.h"

#include <array>

class Settings {
private:
	static std::array<char[50], 23> Labels;
	static char Password[4];

public:
	/**
	 * Loads labels for the current language.
	 * These are from the "*Set00.txt" files
	 */
	static void loadLabels(void);

	inline static const char *getLabel(unsigned int index) {
		return Labels[index];
	}

	static void loadPassword(void);
	static bool isCorrectPassword(const char *guess);
};

#endif // SETTINGS_H_

