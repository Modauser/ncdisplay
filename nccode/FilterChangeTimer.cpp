#include <gameduino2/GD2.h>

#include "Assets.h"
#include "Screens.h"

unsigned int filterType = 0;
unsigned int filterTimerMonths = 3;
unsigned int filterTimerGallons = 750;

static Button buttonsFilterChangeTimer[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			screenCurrent = &screenFilterChange;
	}),
	Button(2, {0, 62}, Button::drawFullWidth, {
		"SET TIMER",
		"TIMER EINSTELLEN",
		"PROGRAMMER LE TEMPORISATEUR",
		"PROGRAMAR TEMPORIZADOR"
	}, [](bool press) {
		if (!press) {
#ifdef USE_SERIAL
			//serialPrintf("@+%1u@*%1u@$%1u",,, filterType);
#endif // USE_SERIAL
			screenCurrent = &screenFilter;
		}
	}),
	Button(3, {34, 210}, Button::drawUpButton, [](bool press) {
		if (!press && filterTimerMonths < 12)
			filterTimerMonths += 3;
	}),
	Button(4, {34, 282}, Button::drawDownButton, [](bool press) {
		if (!press && filterTimerMonths > 3)
			filterTimerMonths -= 3;
	})
};

Screen screenFilterChangeTimer (
	// Parent screen
	&screenFilter,
	// Buttons
	buttonsFilterChangeTimer, 4,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 70, FONT_TITLE, OPT_CENTERX, LanguageString({
			"Set Filter Timer",
			"Filtertimer Einstellen",
			"Programmer le temporisateur du filtre",
			"Programar temporizador del Filtro"
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
		GD.cmd_text(203, 300, FONT_SMALL, OPT_CENTERX, LanguageString({
			"GALLONS",
			"GALLONEN",
			"GALLONS",
			"GALONES"
		})());

		GD.ColorRGB(WHITE);
		GD.Begin(RECTS);
		GD.Vertex2ii(34, 244); GD.Vertex2ii(104, 282);
		GD.Vertex2ii(168, 244); GD.Vertex2ii(238, 282);

		GD.ColorRGB(BLACK);
		GD.cmd_number(69, 263, FONT_SMALL, OPT_CENTER,
			filterTimerMonths);
		GD.cmd_number(203, 263, FONT_SMALL, OPT_CENTER,
			filterTimerGallons);
	}
);

