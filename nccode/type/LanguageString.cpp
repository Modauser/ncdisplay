/**
 * @file LanguageString.cpp
 * @brief Provides a class for handling strings in multiple languages.
 */
#include "LanguageString.h"

Language LanguageString::currentLanguage = Language::English;

const LanguageString lStringNext ({
	"NEXT",
	"N" A_UMLAUT "CHSTE",
	"PROCHAIN",
	"SIGUIENTE"
});

const LanguageString lStringSave ({
	"SAVE",
	"SPEICHERN",
	"SAUVEGARDER",
	"GAURDAR"
});

const char *LanguageString::operator()(Language lang) const
{
	unsigned int index = static_cast<unsigned int>(lang);
	return (*strings[index] != '\0') ? strings[index] : strings[0];
}

bool LanguageString::empty(void) const
{
	return *strings[0] == '\0';
}

void LanguageString::setCurrentLanguage(Language l)
{
	if (l >= Language::Count)
		l = Language::English;
	currentLanguage = l;
}

