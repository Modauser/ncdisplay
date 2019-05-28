/**
 * @file LanguageString.h
 * @brief Provides a class for handling strings in multiple languages.
 */

#ifndef LANGSTR_H_
#define LANGSTR_H_

#include <array>

#include "SpecialCharacters.h"

/**
 * Defines possible languages.
 *
 * Order is important; it matches the language value on the main board.
 */
enum class Language : unsigned int {
	English = 0, /**< English */
	German,      /**< German (Deutsch) */
	French,      /**< French (Francais) */
	Spanish,     /**< Spanish (Espanol) */
	Dutch,       /**< Dutch (Nederlands) */
	Swedish,     /**< Swedish (Svenskt) */
	Norweigan,   /**< Norweigan (Norsk) */
	Danish,      /**< Danish (Dansk) */
	Count        /**< Language count for testing language number validity */
};

/**
 * @class LanguageString
 * @brief Handles string switching based on language.
 *
 * Allows text to be defined for multiple languages. When text is requested from
 * this class, the proper language string will be returned according to the
 * global current language.
 */
class LanguageString {
private:

	/**
	 * Stores the current language being used system-wide.
	 */
	static Language currentLanguage;

	/**
	 * An array for text in each language.
	 */
	const char *strings[static_cast<unsigned int>(Language::Count)];

public:
	/**
	 * Fills all string entries with empty strings.
	 */
	constexpr LanguageString(void)
		: strings{} {
		for (auto& s : strings)
			s = "";
	}

	/**
	 * Fills the strings array for the languages provided.
	 * String order must match that of the Language enum. Undefined entries
	 * will be set to the first (i.e. English).
	 */
	template<typename... T>
	constexpr LanguageString(const T&... s)
		: strings{s...} {
		for (unsigned int i = sizeof...(T); i <
			static_cast<unsigned int>(Language::Count); i++) {
			strings[i] = strings[0];
		}
	}

	/**
	 * Fetches text for the current or given language.
	 * @param lang The desired language
	 * @return The text in the given language
	 */
	const char *operator()(Language lang = currentLanguage) const; 

	/**
	 * Checks if the object was initialized with text.
	 * @return True if object contains no text
	 */
	bool empty(void) const; 

	/**
	 * Sets the current language, used by all LanguageString instances.
	 * @param l The new current language
	 */
	static void setCurrentLanguage(Language l);

	/**
	 * Converts Unicode characters in the given text buffer to renderable
	 * characters.
	 * Special/accented characters have different values in the display's
	 * custom font files.
	 * @param Text to convert
	 * @return The text, now converted
	 */
	static char *convertFileText(char *);

	/**
	 * Retrieves the current language.
	 * @return The current language
	 */
	inline static const Language& getCurrentLanguage(void) {
		return currentLanguage;
	}
};

/**
 * Next text ("NEXT"), defined once to prevent extra declarations.
 */
extern const LanguageString lStringNext;

/**
 * Save text ("SAVE"), defined once to prevent extra declarations.
 */
extern const LanguageString lStringSave;

#endif // LANGSTR_H_
