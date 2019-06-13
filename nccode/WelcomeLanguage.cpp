/**
 * @file WelcomeLanguage.cpp
 * @brief Welcome screen for after reset, selects language.
 */
#include "type/Assets.h"
#include "type/LanguageString.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

static const char *welcomeGreetings[4] = {
	"Hello",
	"Bonjour",
	"Hola",
	"Willkommen"
};

static const char *welcomeLanguages[4] = {
	"ENGLISH",
	"FRAN" CEDILLA "AIS",
	"ESPA" N_TILDE "OL",
	"DEUTSCH"
};

static void setLanguage(Language lang);

static Screen WelcomeLanguage (
	ScreenID::WelcomeLanguage,
	// Parent screen
	ScreenID::WelcomeLanguage,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		GD.ClearColorRGB(NC_BKGND_COLOR);
		GD.Clear();
		
		// Show hello/language text
		GD.ColorRGB(NC_FRGND_COLOR);
		for (int i = 0; i < 4; i++) {
			GD.cmd_text(20, 120 * i + 40, FONT_LIGHT, 0,
				welcomeGreetings[i]);
			GD.cmd_text(20, 120 * i + 60, FONT_LARGE, 0,
				welcomeLanguages[i]);
		}

		// Draw lines dividing each language
		GD.ColorRGB(WHITE);
		GD.Begin(RECTS);
		for (int i = 120; i < 480; i += 120) {
			GD.Vertex2ii(0, i);
			GD.Vertex2ii(272, i);
		}
	},
	// Buttons
	Button({0, 0}, Button::drawWelcomeSelect, [](bool pressed) {
		if (!pressed)
			setLanguage(Language::English);
	}),
	Button({0, 120}, Button::drawWelcomeSelect, [](bool pressed) {
		if (!pressed)
			setLanguage(Language::French);
	}),
	Button({0, 240}, Button::drawWelcomeSelect, [](bool pressed) {
		if (!pressed)
			setLanguage(Language::Spanish);
	}),
	Button({0, 360}, Button::drawWelcomeSelect, [](bool pressed) {
		if (!pressed)
			setLanguage(Language::German);
	})
);

void setLanguage(Language lang)
{
	LanguageString::setCurrentLanguage(lang);
	MainBoard::setLanguage(static_cast<int>(lang));

	// Advance to time/date screen with forced parent of Setup
	ScreenManager::setCurrent(ScreenID::TimeDate, ScreenID::Setup);
}

