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

static void setFilter(bool isFilter);
static bool isFilterPressed = false;   


static Screen C02Level (  
	ScreenID::C02Level,
	// Parent screen
	ScreenID::Filter,
	// Initialization function
	[](void) {
		setFilter(false);  //setup button  
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();
		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(120, 112, FONT_LARGE, OPT_CENTERX, LanguageString({
			"SodaPro CO2 Level",
			"Niveau der CO2 SodaPro",
			"Niveau de CO2 SodaPro",
			"Nivel de CO2 SodaPro"
		})());
		
		GD.cmd_text(8, 376, FONT_SMALL, 0, LanguageString({
			"Last Reset Date",
			"Letzer Resetten Datum",
			"Derni" e_ACUTE "re Date de R" e_ACUTE "initialisation",
			// "" U_TILDE "ltima Fecha de Reinicio"
			"Ultima Fecha de Reinicio"
		})());

			// Print text labels
			
		GD.cmd_text(180, 376, FONT_SMALL, 0, MainBoard::getFilterLastChanged());
		
		
		GD.ColorRGB(WHITE);
		GD.Begin(BITMAPS);
		//GD.Vertex2ii(44, 375, FLTSTRIP_HANDLE, 0);
		//GD.Vertex2ii(160, 375, FLTSTRIP_HANDLE, 1);
		GD.ColorRGB(NC_FDGND_COLOR);
		//GD.cmd_text(78, 445, FONT_SMALL, OPT_CENTERX, lRemove());
		//GD.cmd_text(194, 445, FONT_SMALL, OPT_CENTERX, lInstall());
		// Draw a white rectangle for button borders
		GD.ColorRGB(WHITE);
		GD.Begin(RECTS);
		GD.Vertex2ii(0, 418);
		GD.Vertex2ii(272, 480);
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::Settings);
	}),
	Button({15, 70}, Button::drawExclusiveOption, {  //dpm 3_17_2020 added for sodapro update
		"FILTER", 
		"FILTER",
		"FILTRE",
		"FILTRO"
	}, [](bool press) {
		if (!press){
		setFilter(true);		
		ScreenManager::setCurrent(ScreenID::Filter);
		}	
	}),
	Button({136, 70}, Button::drawExclusiveOption, "C02", [](bool press) {
		if (!press){
		//ScreenManager::setCurrent(ScreenID::C02Level);
		//setFilter(false);
		}
	}),
	Button({0, 420}, Button::drawDispenserItem, {
		"SELECT TANK",
		"BEH" A_UMLAUT "LTER\n\nSELEKTIEREN",
		"S" E_ACUTE "LECTIONNER\n\nLE R" E_ACUTE "SERVOIR",
		"SELECTIONNAR\n\nEL TANQUE"
	}, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::C02Tank);
	}),
	Button({138, 420}, Button::drawDispenserItem, {
		"RESET",
		"RESETTEN",
		"R" E_ACUTE "INITIALISER",
		"REINICAR"
	}, [](bool pressed) {
		//doPress('A', pressed);
	})	
);



void setFilter(bool isFilter)  //toggle the button
{

	isFilterPressed = isFilter;
	C02Level.getButton(1).setForcePressed(isFilterPressed);  //filter button we want it to be white when activated
	C02Level.getButton(2).setForcePressed(!isFilterPressed); //c02 button

}


