#include <gameduino2/GD2.h>

#include "Assets.h"
#include "Screens.h"

static Button buttonsSettings[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			screenCurrent = &screenDispense;
	}),
	Button(2, {0, 60}, Button::drawMenuItem, {
		"PRODUCT INFO",
		"INFO",
		"INFO",
		"INFO"
	}, [](bool press) {
		if (!press)
			screenCurrent = &screenProductInfo;
	}),
	Button(3, {0, 120}, Button::drawMenuItem, {
		"FILTER",
		"FILTER",
		"FILTRE",
		"FILTRO"
	}, [](bool press) {
		if (!press)
			doPasscodeTest(&screenFilter);
	}),
	Button(4, {0, 180}, Button::drawMenuItem, {
		"TIME + DATE",
		"ZEIT + DATUM",
		"HEURE + DATE",
		"HORA + FECHA"
	}, [](bool press) {
		if (!press)
			screenCurrent = &screenTimeDate;
	}),
	Button(5, {0, 240}, Button::drawMenuItem, {
		"LANGUAGE",
		"SPRACHE",
		"LANGUE",
		"IDIOMA"
	}, [](bool press) {
		if (!press)
			screenCurrent = &screenLanguage;
	}),
	Button(6, {0, 300}, Button::drawMenuItem, "SLEEPMODE", [](bool press) {
		if (!press)
			doPasscodeTest(&screenSleepMode);
	}),
	Button(7, {0, 360}, Button::drawMenuItem, {
		"TEMPERATURE",
		"TEMPERATUR",
		"TEMP" E_ACUTE "RATURE",
		"TEMPERATURA"
	}, [](bool press) {
		if (!press)
			doPasscodeTest(&screenTemperature);
	}),
	Button(8, {0, 420}, Button::drawMenuItem, {
		"ADVANCED",
		"FORTSCHRITT",
		"AVANC" E_ACUTE,
		"AVANZADO"
	}, [](bool press) {
		if (!press)
			doPasscodeTest(&screenAdvanced);
	})
};

Screen screenSettings (
	// Parent screen
	&screenDispense,
	// Buttons
	buttonsSettings, 8,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();
	}
);

