/**
 * @file Settings.h
 * @brief Display-wide settings, including text labels and passcode.
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "type/LanguageString.h"

#include <array>

enum class Label : unsigned int {
	Model = 0,
	Serial,
	Software,
	Type,
	Remaining,
	FlowRate,
	CurrentType,
	ReOrder,
	GallonsRem,
	MonthsRem,
	LastChange,
	Contaminants11,
	Contaminants12,
	Contaminants21,
	Contaminants22,
	Contaminants23,
	Contaminants31,
	Contaminants32,
	MonFri,
	SatSun,
	Cold,
	Ambient,
	Hot,
	IceTemp,
	HotTemp,
	ErrorLog,
	ServiceLastChange,

	Count // Used to count labels
};

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
	static std::array<char[LabelSize],
		static_cast<unsigned int>(Label::Count)> Labels;

	/**
	 * The current password/passcode.
	 */
	static char Password[4];

public:
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
	inline static const char *getLabel(Label label) {
		if (label < Label::Count)
			return Labels[static_cast<unsigned int>(label)];

		return "";
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

