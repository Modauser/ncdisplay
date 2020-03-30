/**
 * @file CO2Level.cpp
 * @brief The screen that displays the CO2 tank level.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "CO2Level.h"
//#include <cstring>


#include <gameduino2/GD2.h>

extern unsigned int filterType;
extern unsigned int filterTimerMonths;
extern unsigned int filterTimerGallons;
int CO2LevelValue;  //value 
int bars; //counter for bar graph
int tank; //tank type 0=sodapro, 1=5lb, 2=10lb

static void setFilter(bool isFilter);
static void plotGraph(int level);
static bool isFilterPressed = false;   

const char *CO2Level::TankString[] = {"Sodapro", "5lb","10lb"};

//const char *Tankstring(void) {"Sodapro", "5lb","10lb"};

static Screen CO2Level (  
	ScreenID::CO2Level,
	// Parent screen
	ScreenID::Filter,
	// Initialization function
	[](void) {
		setFilter(false);  //setup button  
		serialPrintf("@j");
		CO2LevelValue = serialGet();
		MainBoard::updateCO2LastReset();  //get CO2 date
		serialPrintf("@p");
		tank = serialGet();		
		
		//  CO2LevelValue=0;  //dpm debug
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();
		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 112, FONT_LARGE, OPT_CENTERX, CO2Level::TankString[tank]);
		//GD.cmd_text(136, 112, FONT_LARGE, OPT_CENTERX, tank);
		
		/*
		GD.cmd_text(136, 112, FONT_LARGE, OPT_CENTERX, LanguageString({
			"SodaPro CO2 Level",
			"Niveau der CO2 SodaPro",
			"Niveau de CO2 SodaPro",
			"Nivel de CO2 SodaPro"
		})()); */
		
		GD.cmd_text(136, 376, FONT_SMALL, OPT_CENTERX	, LanguageString({
			"LAST RESET:",
			"LETZER RESETTEN:",
			"DERNI" E_ACUTE "RE R" E_ACUTE" INITIALISATION:",
			// "" U_TILDE "ltima Fecha de Reinicio"
			"ULTIMA REINICIO:"   //Need a U_TILDE
		})());

			// Print text labels
		//MainBoard::updateCO2LastReset();	
		GD.cmd_text(136, 390, FONT_SMALL, OPT_CENTERX, MainBoard::getCO2LastReset());
		
		
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
		plotGraph(CO2LevelValue);  //plot the graph
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
	Button({136, 70}, Button::drawExclusiveOption, "CO2", [](bool press) {
		if (!press){
		//ScreenManager::setCurrent(ScreenID::CO2Level);
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
			ScreenManager::setCurrent(ScreenID::CO2Tank);
	}),
	Button({138, 420}, Button::drawDispenserItem, {
		"RESET",
		"RESETTEN",
		"R" E_ACUTE "INITIALISER",
		"REINICAR"
	}, [](bool pressed) {
		if (!pressed) {
		serialPrintf("@k1");  //reset CO2 counter.  Make sure the date is reset also when doing this on the main board.
		serialPrintf("@j");
		CO2LevelValue = serialGet();
		MainBoard::updateCO2LastReset();  //get CO2 date
		GD.Begin(RECTS);
		GD.Vertex2ii(0, 418);
		GD.Vertex2ii(272, 480);
		plotGraph(CO2LevelValue);  //plot the graph
		}
	})	
);



void setFilter(bool isFilter)  //toggle the button
{

	isFilterPressed = isFilter;
	CO2Level.getButton(1).setForcePressed(isFilterPressed);  //filter button we want it to be white when activated
	CO2Level.getButton(2).setForcePressed(!isFilterPressed); //CO2 button

}

void plotGraph(int level)
{
//brute forced this to make it easy to read
//plots graph with colors and text
		GD.Vertex2ii(92, 148); //bar graph border top left  //make graph 42 pixels each with 2 pixel dividers
		GD.Vertex2ii(177, 366); //bot right		
		switch (level) {
			case 5: //full
				GD.Begin(RECTS);
				GD.ColorRGB(NC_GREEN_COLOR);
					GD.Vertex2ii(95, 151); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 191); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 195); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 234); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 238); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 277); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 281); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 320); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 324); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 363); //bot right, leave grid of 3 pixels	
					GD.ColorRGB(WHITE);
					GD.cmd_text(135, 171, FONT_LARGE, OPT_CENTER, "100%");
				break;  //case 5
			case 4:
				GD.Begin(RECTS);
				GD.ColorRGB(NC_BKGND_COLOR);  //set the top bar empty
					GD.Vertex2ii(95, 151); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 191); //bot right, leave grid of 3 pixels	
				GD.ColorRGB(NC_GREEN_COLOR);	
					GD.Vertex2ii(95, 195); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 234); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 238); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 277); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 281); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 320); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 324); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 363); //bot right, leave grid of 3 pixels	
					GD.ColorRGB(WHITE);
					GD.cmd_text(135, 215, FONT_LARGE, OPT_CENTER, "80%");
				break;  //case 4
			case 3:
				GD.Begin(RECTS);
				GD.ColorRGB(NC_BKGND_COLOR);  //set the top bar empty
					GD.Vertex2ii(95, 151); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 191); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 195); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 234); //bot right, leave grid of 3 pixels	
				GD.ColorRGB(NC_GREEN_COLOR);	
					GD.Vertex2ii(95, 238); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 277); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 281); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 320); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 324); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 363); //bot right, leave grid of 3 pixels	
					GD.ColorRGB(WHITE);
					GD.cmd_text(135, 258, FONT_LARGE, OPT_CENTER, "60%");
				break;  //case 3
			case 2:
				GD.Begin(RECTS);
				GD.ColorRGB(NC_BKGND_COLOR);  //set the top bar empty
					GD.Vertex2ii(95, 151); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 191); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 195); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 234); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 238); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 277); //bot right, leave grid of 3 pixels	
					GD.ColorRGB(NC_GREEN_COLOR);	
					GD.Vertex2ii(95, 281); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 320); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 324); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 363); //bot right, leave grid of 3 pixels	
					GD.ColorRGB(WHITE);
					GD.cmd_text(135, 301, FONT_LARGE, OPT_CENTER, "40%");
				break;  //case 2
			case 1:
				GD.Begin(RECTS);
				GD.ColorRGB(NC_BKGND_COLOR);  //set the top bar empty
					GD.Vertex2ii(95, 151); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 191); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 195); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 234); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 238); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 277); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 281); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 320); //bot right, leave grid of 3 pixels	
					GD.ColorRGB(NC_GREEN_COLOR);	
					GD.Vertex2ii(95, 324); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 363); //bot right, leave grid of 3 pixels	
					GD.ColorRGB(WHITE);
					GD.cmd_text(135, 344, FONT_LARGE, OPT_CENTER, "20%");
				break;  //case 1
			case 0:
				GD.Begin(RECTS);
				GD.ColorRGB(NC_BKGND_COLOR);  //set the top bar empty
					GD.Vertex2ii(95, 151); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 191); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 195); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 234); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 238); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 277); //bot right, leave grid of 3 pixels	
					GD.Vertex2ii(95, 281); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 320); //bot right, leave grid of 3 pixels	
					//1/2 size block
					GD.Vertex2ii(95, 324); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 343); //bot right, leave grid of 3 pixels	
					GD.ColorRGB(TINT_RED);		//make last half bar red for <5%				
					GD.Vertex2ii(95, 343); //top left  //make bars 80x40 pixels
					GD.Vertex2ii(174, 363); //bot right, leave grid of 3 pixels	
					GD.cmd_text(135, 331, FONT_LARGE, OPT_CENTER, "<5%");

				break;  //case 0

				
			default:
			break;
			}	
	
}

