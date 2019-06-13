/**
 * @file ProductInfo.cpp
 * @brief Displays product info.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "Settings.h"

#include <fatfs/ff.h>
#include <gameduino2/GD2.h>
#include <cstdio>

// Format string, %1u replaced with model number + 1
constexpr const char *piModelStringFormat = "ION TS%1u00";
static char piModelString[10];

static Screen ProductInfo (
	ScreenID::ProductInfo,
	// Parent screen
	ScreenID::Settings,
	// Initialization function
	[](void) {
		MainBoard::updateMetric();
		MainBoard::updateModelNumber();
		MainBoard::updateSerialNumber();
		MainBoard::updateSoftwareVersion();
		MainBoard::updateFilterType();
		MainBoard::updateFilterRemaining();
		MainBoard::updateFlowRate();
		MainBoard::updateServiceContact();

		sprintf(piModelString, piModelStringFormat, 1 + MainBoard::
			getModelNumber());
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(20, 80, FONT_LARGE, 0, LanguageString({
			"PRODUCT INFO",
			"INFO",
			"INFO",
			"INFO"
		})());

		GD.cmd_text(20,  110, FONT_SMALL, 0, Settings::getLabel(Label::Model));
		GD.cmd_text(21,  130, FONT_SMALL, 0, Settings::getLabel(Label::Serial));
		GD.cmd_text(20,  150, FONT_SMALL, 0, Settings::getLabel(Label::Software));

		GD.cmd_text(140, 110, FONT_SMALL, 0, piModelString);
		GD.cmd_text(140, 130, FONT_SMALL, 0, MainBoard::getSerialNumber());
		GD.cmd_text(140, 150, FONT_SMALL, 0, MainBoard::getSoftwareVersion());

		GD.cmd_text(20, 190, FONT_LARGE, 0, LanguageString({
			"FILTER INFO",
			"FILTERINFO",
			"INFORMATION DU FILTRE",
			"INFORMACION DEL FILTRO"
		})());

		GD.cmd_text(20,  220, FONT_SMALL, 0, Settings::getLabel(Label::Type));
		GD.cmd_text(20,  240, FONT_SMALL, 0, Settings::getLabel(Label::Remaining));
		GD.cmd_text(20,  260, FONT_SMALL, 0, Settings::getLabel(Label::FlowRate));

	        GD.cmd_text(140, 220, FONT_SMALL, 0, MainBoard::getFilterName());
	      GD.cmd_number(140, 240, FONT_SMALL, 0, MainBoard::getFilterRemaining());
		GD.cmd_text(140, 260, FONT_SMALL, 0, MainBoard::getFlowRate());

		GD.cmd_text(20, 380, FONT_SMALL, 0, MainBoard::getServiceContact(), 20);
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Settings);
	})
);

