/**
 * @file LanguageString.h
 * @brief Provides a class for handling strings in multiple languages
 */
#ifndef LANGSTR_H_
#define LANGSTR_H_

#include <array>

#include "SpecialCharacters.h"

/**
 * Defines possible languages.
 * Order is important; it matches the language value on the main board.
 */
enum class Language : unsigned int {
	English = 0,
	German,
	French,
	Spanish,
	Dutch,
	Swedish,
	Norweigan,
	Danish,
	Count
};

/**
 * Allows text to be defined for multiple languages.
 * When text is requested from an object of this class, the proper language
 * string will be returned according to the overall current language.
 */
class LanguageString {
private:

	// States the current language being used
	static Language currentLanguage;

	// An array for each language's text
	const char *strings[static_cast<unsigned int>(Language::Count)];

public:
	// Fills all text entries with empty strings
	constexpr LanguageString(void)
		: strings{} {
		for (auto& s : strings)
			s = "";
	}

	// Fills the strings array for as many languages as provided
	// Undefined entries will point to the first (English) entry
	template<typename... T>
	constexpr LanguageString(const T&... s)
		: strings{s...} {
		for (unsigned int i = sizeof...(T); i <
			static_cast<unsigned int>(Language::Count); i++) {
			strings[i] = strings[0];
		}
	}

	// Fetches the string for the current language
	const char *operator()(Language lang = currentLanguage) const; 

	// Returns true if this object was initialized with non-empty values
	bool empty(void) const; 

	// Sets the current language for all LanguageStrings
	static void setCurrentLanguage(Language l);
};

// Some common strings predefined to save space
extern const LanguageString lStringNext;
extern const LanguageString lStringSave;

#endif // LANGSTR_H_
