#include <gameduino2/GD2.h>
#include <stdio.h>

#include "Assets.h"
#include "Screens.h"

void languageSet(const Language& l)
{
#ifdef USE_SERIAL
	serialPrintf("@2%1u", static_cast<unsigned int>(l));
#endif // USE_SERIAL

	LanguageString::setCurrentLanguage(l);
	screenCurrent = &screenSettings;
}

void languageShowPage(bool top);

static Button buttonsLanguage[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			screenCurrent = &screenSettings;
	}),
	Button(2, {0, 122}, Button::drawMenuItem, "ENGLISH", [](bool press) {
		if (!press)
			languageSet(Language::English);
	}),
	Button(3, {0, 182}, Button::drawMenuItem, "FRAN" CEDILLA "AIS",
	[](bool press) {
		if (!press)
			languageSet(Language::French);
	}),
	Button(4, {0, 242}, Button::drawMenuItem, "ESPA" N_TILDE "OL",
	[](bool press) {
		if (!press)
			languageSet(Language::Spanish);
	}),
	Button(5, {0, 302}, Button::drawMenuItem, "DEUTSCH", [](bool press) {
		if (!press)
			languageSet(Language::German);
	}),
	Button(6, {0, 362}, Button::drawMenuItem, "NEDERLANDS",
	[](bool press) {
		if (!press)
			languageSet(Language::Dutch);
	}),
	Button(7, {-1, 422}, Button::drawScrollButton, [](bool press) {
		if (!press)
			languageShowPage(false);
	}),

	Button(8, {1, 122}, Button::drawScrollButton, [](bool press) {
		if (!press)
			languageShowPage(true);
	}),
	Button(9, {0, 182}, Button::drawMenuItem, "SVENSKT", [](bool press) {
		if (!press)
			languageSet(Language::Swedish);
	}),
	Button(10, {0, 242}, Button::drawMenuItem, "NORSK", [](bool press) {
		if (!press)
			languageSet(Language::Norweigan);
	}),
	Button(11, {0, 302}, Button::drawMenuItem, "DANSK", [](bool press) {
		if (!press)
			languageSet(Language::Danish);
	}),
};

void languageShowPage(bool top)
{
	for (unsigned int i = 1; i < 7; i++)
		buttonsLanguage[i].setVisibility(top);
	for (unsigned int i = 7; i < 11; i++)
		buttonsLanguage[i].setVisibility(!top);
}

static const LanguageString lSelectLang ({
	"Select Language",
	"Sprache Selektieren",
	"S" e_ACUTE "lectionner la Langue",
	"Seleccionar idioma"
});

Screen screenLanguage (
	// Parent screen
	&screenSettings,
	// Buttons
	buttonsLanguage, 11,
	// Initialization function
	[](void) {
		languageShowPage(true);
	},
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 70, FONT_TITLE, OPT_CENTERX, lSelectLang());
	}
);

