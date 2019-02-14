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
	currentLanguage = l;
}

