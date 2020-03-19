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

char *LanguageString::convertFileText(char *buffer)
{
	for (uint32_t i = 0; buffer[i] != '\0'; i++) {
		if (buffer[i] < 0x80) {
			if (buffer[i] == '\t')
				buffer[i] = ' ';
			continue;
		}

		// Cases are Unicode values for their respective characters
		switch (buffer[i]) {
		case 0xB0: buffer[i] = *DEGREE; break;
		case 0xC9: buffer[i] = *E_ACUTE; break;
		case 0xE9: buffer[i] = *e_ACUTE; break;
		case 0xCC: buffer[i] = *I_GRAVE; break;
		case 0xCD: buffer[i] = *I_ACUTE; break;
		case 0xED: buffer[i] = *i_ACUTE; break;
		case 0xD3: buffer[i] = *O_ACUTE; break;
		case 0xF3: buffer[i] = *o_ACUTE; break;
		case 0xDA: buffer[i] = *U_ACUTE; break;
		case 0xFA: buffer[i] = *u_ACUTE; break;
		case 0xC0: buffer[i] = *A_GRAVE; break;
		case 0xE0: buffer[i] = *a_GRAVE; break;
		case 0xE1: buffer[i] = *a_ACUTE; break;
		case 0xC8: buffer[i] = *E_GRAVE; break;
		case 0xE8: buffer[i] = *e_GRAVE; break;
		case 0xC4: buffer[i] = *A_UMLAUT; break;
		case 0xE4: buffer[i] = *a_UMLAUT; break;
		case 0xD6: buffer[i] = *O_UMLAUT; break;
		case 0xF6: buffer[i] = *o_UMLAUT; break;
		case 0xDC: buffer[i] = *U_UMLAUT; break;
		case 0xFC: buffer[i] = *u_UMLAUT; break;
		case 0xDB: buffer[i] = *U_HAT; break;
		case 0xFB: buffer[i] = *u_HAT; break;
		case 0xC5: buffer[i] = *A_ORING; break;
		case 0xE5: buffer[i] = *a_ORING; break;
		case 0xD1: buffer[i] = *N_TILDE; break;
		case 0xF1: buffer[i] = *n_TILDE; break;
//		case 0x168: buffer[i] = *U_TILDE; break;
		case 0xC7: buffer[i] = *CEDILLA; break;
		case 0xE7: buffer[i] = *cEDILLA; break;
		case 0xDF: buffer[i] = *ESZETT; break;
		default: break;
		}
	}

	return buffer;
}

