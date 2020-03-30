/**
 * @file FilterChangeTimer100.cpp
 * @brief Screen to set timer for filter change; applies new filter settings.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

unsigned int filter100Type = 0;
unsigned int filter100TimerMonths = 3;
unsigned int filter100TimerGallons = 750;

static const LanguageString gallons ({
	"GALLONS",
	"GALLONEN",
	"GALLONS",
	"GALONES"
});

static const LanguageString liters ({
	"LITERS",
	"LITERS",
	"LITRES",
	"LITROS"
});

static Screen FilterChangeTimer100 (
	ScreenID::FilterChangeTimer100,
	// Parent screen
	ScreenID::FilterChange100,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 90, FONT_TITLE, OPT_CENTER, LanguageString({
			"Set Filter Timer",
			"Filtertimer Einstellen",
			"Programmer le\ntemporisateur du filtre",
			"Programar temporizador\ndel Filtro"
		})());

		GD.cmd_text(69, 185, FONT_SMALL, OPT_CENTERX, LanguageString({
			"MONTHS",
			"MONATE",
			"MOIS",
			"MESES"
		})());
		GD.cmd_text(136, 260, FONT_SMALL, OPT_CENTER, LanguageString({
			"OR",
			"ODER",
			"OU",
			"O"
		})());
		GD.cmd_text(203, 300, FONT_SMALL, OPT_CENTERX,
			MainBoard::isMetric() ? liters() : gallons());

		GD.ColorRGB(WHITE);
		GD.Begin(RECTS);
		GD.Vertex2ii(34, 244); GD.Vertex2ii(104, 282);
		GD.Vertex2ii(168, 244); GD.Vertex2ii(238, 282);

		GD.ColorRGB(BLACK);
		GD.cmd_number(69, 263, FONT_SMALL, OPT_CENTER,
			filter100TimerMonths);
		GD.cmd_number(203, 263, FONT_SMALL, OPT_CENTER,
			filter100TimerGallons);
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::FilterChange100);
	}),
	Button({0, 420}, Button::drawFullWidth, {
		"SET TIMER",
		"TIMER EINSTELLEN",
		"PROGRAMMER LE TEMPORISATEUR",
		"PROGRAMAR TEMPORIZADOR"
	}, [](bool press) {
		if (!press) {
			serialPrintf("@+%c@*%01u@$%01u",
				filter100TimerGallons == 750 ? '1' : '3',
				filter100TimerMonths / 3 - 1,
				filter100Type);
			ScreenManager::setCurrent(ScreenID::Filter100);
		}
	}),
	Button({34, 210}, Button::drawUpButton, [](bool press) {
		if (!press && filter100TimerMonths < (filter100TimerGallons == 750 ?
			6 : 12)) {
			filter100TimerMonths += 3;
		}
	}),
	Button({34, 282}, Button::drawDownButton, [](bool press) {
		if (!press && filter100TimerMonths > 3)
			filter100TimerMonths -= 3;
	})
);

