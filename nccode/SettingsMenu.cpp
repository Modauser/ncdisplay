/**
 * @file SettingsMenu.cpp
 * @brief The settings menu.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "Lockscreen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

static void settingsMenuInit(void);

static Screen Settings (
	// This screen
	ScreenID::Settings,
	// Parent screen
	ScreenID::Dispense,
	// Initialization function
	nullptr,
	// Pre-draw function
    settingsMenuInit,
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Dispense);
	}),
	Button({1, 60}, Button::drawMenuItem, {
		"PRODUCT INFO",
		"INFO",
		"INFO",
		"INFO"
	}, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::ProductInfo);
	}),
	Button({2, 120}, Button::drawMenuItem, "", [](bool press) {
		if (!press) {
			if (MainBoard::canDispenseSparkling())
                doPasscodeTest(ScreenID::Filter);
			else
                doPasscodeTest(ScreenID::Filter100);
		}		
	}),
	Button({3, 180}, Button::drawMenuItem, {
		"TIME + DATE",
		"ZEIT + DATUM",
		"HEURE + DATE",
		"HORA + FECHA"
	}, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::TimeDate);
	}),
	Button({4, 240}, Button::drawMenuItem, {
		"LANGUAGE",
		"SPRACHE",
		"LANGUE",
		"IDIOMA"
	}, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Language);
	}),
	Button({5, 300}, Button::drawMenuItem, "SLEEPMODE", [](bool press) {
		if (!press)
			doPasscodeTest(ScreenID::SleepMode);
	}),
	Button({6, 360}, Button::drawMenuItem, {
		"TEMPERATURE",
		"TEMPERATUR",
		"TEMP" E_ACUTE "RATURE",
		"TEMPERATURA"
	}, [](bool press) {
		if (!press)
			doPasscodeTest(ScreenID::Temperature);
	}),
	Button({7, 420}, Button::drawMenuItem, {
		"ADVANCED",
		"FORTSCHRITT",
		"AVANC" E_ACUTE,
		"AVANZADO"
	}, [](bool press) {
		if (!press)
			doPasscodeTest(ScreenID::Advanced);
	})
);

static void settingsMenuInit(void)
{
    clearScreenWithIonHeader();
    auto filterText = MainBoard::canDispenseSparkling() ?
        LanguageString {"FILTER + CO2", "FILTER + CO2", "FILTRE + CO2", "FILTRO + CO2"} :
        LanguageString {"FILTER", "FILTER", "FILTRE", "FILTRO"};
    Settings.getButton(2).setText(filterText);
}

