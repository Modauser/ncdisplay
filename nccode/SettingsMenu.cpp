/**
 * @file SettingsMenu.cpp
 * @brief The settings menu.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "Lockscreen.h"
#include "Mainboard.h"

#include <gameduino2/GD2.h>
int a;

static Screen Settings (
	// This screen
	ScreenID::Settings,
	// Parent screen
	ScreenID::Dispense,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();
	},
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
	Button({2, 120}, Button::drawMenuItemCo2,  {  //dpm 3_17_2020 added CO2 to filter for sodapro update, need to fix titles, should only be "FILTER" etc.. if model 100 or model 200
		"FILTER",
		"FILTER",
		"FILTRE",
		"FILTRO"
	}, [](bool press) {
		if (!press) {
			if (MainBoard::canDispenseSparkling())	doPasscodeTest(ScreenID::Filter);
			else 	doPasscodeTest(ScreenID::Filter100);
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


