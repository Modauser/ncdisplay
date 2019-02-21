/**
 * @file Filter.cpp
 * @brief The filter info screen.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "Settings.h"

#include <gameduino2/GD2.h>

static Screen Filter (
	ScreenID::Filter,
	// Parent screen
	ScreenID::Settings,
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
		clearScreenWithIonHeader();

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
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::Settings);
	}),
	Button({0, 420}, Button::drawFullWidth, {
		"CHANGE FILTER",
		"FILTER WECHSELN",
		"CHANGER LE FILTRE",
		"CAMBIAR EL FILTRO"
	}, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::FilterChange);
	})
);

