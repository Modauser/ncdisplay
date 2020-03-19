/**
 * @file Filter.cpp
 * @brief The filter info screen.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "Settings.h"

#include <gameduino2/GD2.h>

static void setC02(bool isC02);
static bool isFilterPressed = true;

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
		setC02(false);  //setup button  
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		// Header
		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(8, 110, FONT_LARGE, 0, LanguageString({
			"FILTER INFO",
			"FILTERINFO",
			"INFORMATIONS SUR LE FILTRE",
			"INFOMACI" O_ACUTE "N DEL FILTRO"
		})());

		// Print text labels
		GD.cmd_text(8,  140, FONT_SMALL, 0, Settings::getLabel(Label::CurrentType));  //dpm 3_17_2020 moved labels for sodaapro update
		GD.cmd_text(8,  160, FONT_SMALL, 0, Settings::getLabel(Label::ReOrder));
		GD.cmd_text(8,  180, FONT_SMALL, 0, Settings::getLabel(Label::GallonsRem));
		GD.cmd_text(8,  200, FONT_SMALL, 0, Settings::getLabel(Label::MonthsRem));
		GD.cmd_text(8,  220, FONT_SMALL, 0, Settings::getLabel(Label::LastChange));

		// Print corresponding values
		GD.cmd_text(180, 140, FONT_SMALL, 0, MainBoard::getFilterName());
		GD.cmd_text(180, 160, FONT_SMALL, 0, MainBoard::getFilterReorder());
        GD.cmd_number(180, 180, FONT_SMALL, 0, MainBoard::getFilterRemainingCount());
	    GD.cmd_number(180, 200, FONT_SMALL, 0, MainBoard::getFilterMonthsRemaining());
		GD.cmd_text(180, 220, FONT_SMALL, 0, MainBoard::getFilterLastChanged());

		GD.cmd_text(8, 250, FONT_LARGE, 0, LanguageString({
			"CONTAMINANTS REMOVED", 
			"ENTFERNTE",
			"CONTAMINANTS " E_ACUTE "LIMIN" E_ACUTE "S",
			"CONTAMINANTES\nELIMINADOS"
		})());

		// Show proper contaminant text based on filter's type
		auto filterType = MainBoard::getFilterType();
		switch (filterType) {
		case FilterType::CarbonPro:
			GD.cmd_text(8, 290, FONT_SMALL, 0, Settings::getLabel(
				Label::Contaminants21));
			GD.cmd_text(8, 310, FONT_SMALL, 0, Settings::getLabel(
				Label::Contaminants22));
			GD.cmd_text(8, 330, FONT_SMALL, 0, Settings::getLabel(
				Label::Contaminants23));
			break;
		case FilterType::CarbonPhos:
			GD.cmd_text(8, 290, FONT_SMALL, 0, Settings::getLabel(
				Label::Contaminants31));
			GD.cmd_text(8, 310, FONT_SMALL, 0, Settings::getLabel(
				Label::Contaminants32));
			break;
		default:
			GD.cmd_text(8, 290, FONT_SMALL, 0, Settings::getLabel(
				Label::Contaminants11));
			GD.cmd_text(8, 310, FONT_SMALL, 0, Settings::getLabel(
				Label::Contaminants12));
			break;
		}
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
	}),
	Button({15, 70}, Button::drawExclusiveOption, {  //dpm 3_17_2020 added for sodapro update
		"FILTER", 
		"FILTER",
		"FILTRE",
		"FILTRO"
	}, [](bool press) {
		if (!press){
		//setC02(false);
		//ScreenManager::setCurrent(ScreenID::Filter);
		//setC02(true);
		}	
	}),
	Button({136, 70}, Button::drawExclusiveOption, "C02", [](bool press) {
		if (!press){
		setC02(true);
		ScreenManager::setCurrent(ScreenID::C02Level);

		}
	})	
);


void setC02(bool isC02)  //toggle the button
{
	isFilterPressed = !isC02;

	Filter.getButton(2).setForcePressed(isFilterPressed);
	Filter.getButton(3).setForcePressed(!isFilterPressed);
}
