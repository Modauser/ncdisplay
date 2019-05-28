/**
 * @file Settings.h
 * @brief Display-wide settings, including text labels and passcode.
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "type/LanguageString.h"

#include <array>

/**
 * @class Settings
 * @brief Provides text labels and passcode loaded from SD card files.
 */
class Settings {
private:
	/**
	 * Maximum size of a label.
	 */
	constexpr static unsigned int LabelSize = 64;

	/**
	 * An array storing all labels.
	 */
	static std::array<char[LabelSize], 23> Labels;

	/**
	 * The current password/passcode.
	 */
	static char Password[4];

public:
	/**
	 * Defines which label contains the 1st line of contaminants.
	 */
	constexpr static unsigned int Contaminants1 = 11;
	/**
	 * Defines which label contains the 2st line of contaminants.
	 */
	constexpr static unsigned int Contaminants2 = 12;
	/**
	 * Defines which label contains the 3st line of contaminants.
	 */
	constexpr static unsigned int Contaminants3 = 13;

	/**
	 * Loads labels for the current language.
	 * These are from the "*Set00.txt" files
	 */
	static void loadLabels(void);

	/**
	 * Gets the desired label.
	 * @param index Index of the label
	 * @return Label text
	 */
	inline static const char *getLabel(unsigned int index) {
		return Labels[index];
	}

	/**
	 * Loads the password from its file.
	 */
	static void loadPassword(void);

	/**
	 * Tests if the given password matches the actual.
	 * @param guess The password to test
	 * @return True if correct password
	 */
	static bool isCorrectPassword(const char *guess);
};

#endif // SETTINGS_H_

