/**
 * @file CO2Tank.cpp
 * @brief The menu to select which tank is being used */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

extern unsigned int filterType;
extern unsigned int filterTimerMonths;
extern unsigned int filterTimerGallons;

static const LanguageString lSelect ({
	"SELECT SIZE",
	"UMFANG SELEKTIEREN",
	"S" E_ACUTE "LECTIONNER LA TAILLE",
	"SELECTIONNAR TAMA" N_TILDE "O"
});


static void goSodaProCO2(int type) 
{
	
	serialPrintf("@o0");   //set CO2 tank type
	ScreenManager::setCurrent(ScreenID::CO2Level);
}

static void go5lb(int type)
{
	serialPrintf("@o1");   //set CO2 tank type
	ScreenManager::setCurrent(ScreenID::CO2Level);
}

static void go10lb(int type)
{
	serialPrintf("@o2");   //set CO2 tank type
	ScreenManager::setCurrent(ScreenID::CO2Level);
}

static Screen CO2Tank (
	ScreenID::CO2Tank,
	// Parent screen
	ScreenID::CO2Level,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();
		GD.ColorRGB(NC_FDGND_COLOR);
		GD.cmd_text(136, 90, FONT_LARGE, OPT_CENTER, lSelect());
		//GD.cmd_text(194, 445, FONT_SMALL, OPT_CENTERX, lInstall());
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::CO2Level);
	}),
	Button({0, 120}, Button::drawMenuItem, "SodaProCO2",
	[](bool press) { if (!press) goSodaProCO2(0); } ),
	Button({0, 180}, Button::drawMenuItem, "5lb",
	[](bool press) { if (!press) go5lb(1); } ),
		Button({0, 240}, Button::drawMenuItem, "10lb",
	[](bool press) { if (!press) go10lb(2); } )
);


