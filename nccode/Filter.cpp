#include <gameduino2/GD2.h>

#include "Assets.h"
#include "MainBoard.h"
#include "Settings.h"
#include "Screens.h"

static Button buttonsFilter[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			screenCurrent = &screenSettings;
	}),
	Button(2, {0, 420}, Button::drawFullWidth, {
		"CHANGE FILTER",
		"FILTER WECHSELN",
		"CHANGER LE FILTRE",
		"CAMBIAR EL FILTRO"
	}, [](bool press) {
		if (!press)
			screenCurrent = &screenFilterChange;
	})
};

Screen screenFilter (
	// Parent screen
	&screenSettings,
	// Buttons
	buttonsFilter, 2,
	// Initialization function
	[](void) {
		MainBoard::updateMetric();
		MainBoard::updateFilterType();
		MainBoard::updateFilterRemaining();
		MainBoard::updateFilterMonthsRemaining();
		MainBoard::updateFilterLastChanged();
	},
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(20, 80, FONT_LARGE, 0, LanguageString({
			"FILTER INFO",
			"FILTERINFO",
			"INFORMATIONS SUR LE FILTRE",
			"INFOMACI" o_ACUTE "N DEL FILTRO"
		})());

		GD.cmd_text(20,  110, FONT_SMALL, 0, Settings::getLabel(6));
		GD.cmd_text(20,  130, FONT_SMALL, 0, Settings::getLabel(7));
		GD.cmd_text(20,  150, FONT_SMALL, 0, Settings::getLabel(8));
		GD.cmd_text(20,  170, FONT_SMALL, 0, Settings::getLabel(9));
		GD.cmd_text(20,  190, FONT_SMALL, 0, Settings::getLabel(10));

		GD.cmd_text(180, 110, FONT_SMALL, 0, MainBoard::getFilterName());
		GD.cmd_text(180, 130, FONT_SMALL, 0, MainBoard::getFilterReorder());
	      GD.cmd_number(180, 150, FONT_SMALL, 0, MainBoard::getFilterRemaining());
	      GD.cmd_number(180, 170, FONT_SMALL, 0, MainBoard::getFilterMonthsRemaining());
		GD.cmd_text(180, 190, FONT_SMALL, 0, MainBoard::getFilterLastChanged());

		GD.cmd_text(20, 230, FONT_LARGE, 0, LanguageString({
			"CONTAMINANTS REMOVED",
			"ENTFERNTE",
			"CONTAMINANTS " E_ACUTE "LIMIN" e_ACUTE "S",
			"CONTAMINANTES ELIMINADOS"
		})());

		// TODO
		GD.cmd_text(20, 260, FONT_SMALL, 0, LanguageString({
			"Chlorine, Taste, Odor, Lead, Cysts",
			"Chlor, Geschmack, Geruch, Blei, Zysten",
			"Chlore, Go" u_HAT "t, Odeur, Plomb, Kystes",
			"Clorina, Sabor, Olor, Plomo, Quistes"
		})());
	}
);

