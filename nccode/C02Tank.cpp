/**
 * @file C02Tank.cpp
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


static void goSodaProC02(int type) 
{
	
	serialPrintf("@o0");   //set c02 tank type
	ScreenManager::setCurrent(ScreenID::C02Level);
}

static void go5lb(int type)
{
	serialPrintf("@o1");   //set c02 tank type
	ScreenManager::setCurrent(ScreenID::C02Level);
}

static void go10lb(int type)
{
	serialPrintf("@o2");   //set c02 tank type
	ScreenManager::setCurrent(ScreenID::C02Level);
}

static Screen C02Tank (
	ScreenID::C02Tank,
	// Parent screen
	ScreenID::C02Level,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();
		GD.ColorRGB(NC_FDGND_COLOR);
		GD.cmd_text(120, 90, FONT_LARGE, OPT_CENTER, lSelect());
		//GD.cmd_text(194, 445, FONT_SMALL, OPT_CENTERX, lInstall());
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::C02Level);
	}),
	Button({0, 120}, Button::drawMenuItem, "SodaProC02",
	[](bool press) { if (!press) goSodaProC02(0); } ),
	Button({0, 180}, Button::drawMenuItem, "5lb",
	[](bool press) { if (!press) go5lb(1); } ),
		Button({0, 240}, Button::drawMenuItem, "10lb",
	[](bool press) { if (!press) go10lb(2); } )
);


