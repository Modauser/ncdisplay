/**
 * @file C02Level.cpp
 * @brief The screen that displays the C02 tank level.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

extern unsigned int filterType;
extern unsigned int filterTimerMonths;
extern unsigned int filterTimerGallons;

static void setC02(bool isC02);
static bool isFilter = true;


static Screen C02Level (  
	ScreenID::C02Level,
	// Parent screen
	ScreenID::Filter,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.ColorRGB(WHITE);
		GD.Begin(BITMAPS);
		GD.Vertex2ii(44, 375, FLTSTRIP_HANDLE, 0);
		GD.Vertex2ii(160, 375, FLTSTRIP_HANDLE, 1);
		GD.ColorRGB(NC_FDGND_COLOR);
		//GD.cmd_text(78, 445, FONT_SMALL, OPT_CENTERX, lRemove());
		//GD.cmd_text(194, 445, FONT_SMALL, OPT_CENTERX, lInstall());
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::Filter);
	}),
	Button({15, 70}, Button::drawExclusiveOption, {  //dpm 3_17_2020 added for sodapro update
		"FILTER", 
		"FILTER",
		"FILTRE",
		"FILTRO"
	}, [](bool press) {
		if (!press){
		setC02(false);
		ScreenManager::setCurrent(ScreenID::Filter);
		setC02(true);
		}	
	}),
	Button({136, 70}, Button::drawExclusiveOption, "C02", [](bool press) {
		if (!press){
		ScreenManager::setCurrent(ScreenID::C02Level);
		setC02(true);
		}
	})	
);



void setC02(bool isC02)  //toggle the button
{
	isFilter = !isC02;

	C02Level.getButton(2).setForcePressed(isFilter);
	C02Level.getButton(3).setForcePressed(!isFilter);
}


