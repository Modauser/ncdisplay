/**
 * @file Language.cpp
 * @brief The language selection menu.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "Settings.h"

#include <gameduino2/GD2.h>



// Sets current language, loads lang's label text, then exits Language screen
void languageSet(const Language& l)
{
	LanguageString::setCurrentLanguage(l);
	MainBoard::setLanguage(static_cast<int>(l));
	Settings::loadLabels();
	ScreenManager::setCurrent(ScreenID::Settings);
}

static const LanguageString lSelectLang ({
	"Select Language",
	"Sprache Selektieren",
	"S" e_ACUTE "lectionner la Langue",
	"Seleccionar idioma"
});


// Show top or bottom of language list
void languageShowPage(bool top);

static Screen Language (
	// This screen
	ScreenID::Language,
	// Parent screen
	ScreenID::Settings,
	
	// Initialization function
	[](void) {
		languageShowPage(true);
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		// Header text
		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 90, FONT_TITLE, OPT_CENTER, lSelectLang());
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Settings);
	}),
	Button({0, 120}, Button::drawMenuItem, "ENGLISH", [](bool press) {
		if (!press)
			languageSet(Language::English);
	}),
	Button({0, 180}, Button::drawMenuItem, "FRAN" CEDILLA "AIS",
	[](bool press) {
		if (!press)
			languageSet(Language::French);
	}),
	Button({0, 240}, Button::drawMenuItem, "ESPA" N_TILDE "OL",
	[](bool press) {
		if (!press)
			languageSet(Language::Spanish);
	}),
	Button({0, 300}, Button::drawMenuItem, "DEUTSCH", [](bool press) {
		if (!press)
			languageSet(Language::German);
	}),
	Button({0, 360}, Button::drawMenuItem, "NEDERLANDS",
	[](bool press) {
		if (!press)
			languageSet(Language::Dutch);
	}),
	// -1 makes down arrow
	Button({-1, 420}, Button::drawScrollButton, [](bool press) {
		if (!press)
			languageShowPage(false);
	}),
	// 1 makes up arrow
	Button({1, 120}, Button::drawScrollButton, [](bool press) {
		if (!press)
			languageShowPage(true);
	}),
	Button({0, 180}, Button::drawMenuItem, "SVENSKT", [](bool press) {
		if (!press)
			languageSet(Language::Swedish);
	}),
	Button({0, 240}, Button::drawMenuItem, "NORSK", [](bool press) {
		if (!press)
			languageSet(Language::Norweigan);
	}),
	Button({0, 300}, Button::drawMenuItem, "DANSK", [](bool press) {
		if (!press)
			languageSet(Language::Danish);
	})
);

void languageShowPage(bool top)
{
	for (unsigned int i = 1; i < 7; i++)
		Language.getButton(i).setVisibility(top);
	for (unsigned int i = 7; i < 11; i++)
		Language.getButton(i).setVisibility(!top);
}

