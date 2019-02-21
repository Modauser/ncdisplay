/**
 * @file FilterChange.cpp
 * @brief The menu to select which filter is being changed to.
 */
#include "type/Assets.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

extern unsigned int filterType;
extern unsigned int filterTimerMonths;
extern unsigned int filterTimerGallons;

static void goTimerShort(int type)
{
	filterType = type;
	filterTimerMonths = 6;
	filterTimerGallons = 750;
	ScreenManager::setCurrent(ScreenID::FilterChangeTimer);
}
// Liters: 2850 5700
static void goTimerLong(int type)
{
	filterType = type;
	filterTimerMonths = 12;
	filterTimerGallons = 1500;
	ScreenManager::setCurrent(ScreenID::FilterChangeTimer);
}

static Screen FilterChange (
	ScreenID::FilterChange,
	// Parent screen
	ScreenID::Filter,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.ColorRGB(WHITE);
		GD.Begin(BITMAPS);
		GD.Vertex2ii(20, 400, FLTSTRIP_HANDLE, 0);
		GD.Vertex2ii(136, 400, FLTSTRIP_HANDLE, 1);
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			ScreenManager::setCurrent(ScreenID::Filter);
	}),
	Button({0, 60}, Button::drawMenuItem, "CarbonPlus",
	[](bool press) { if (!press) goTimerLong(0); } ),
	Button({0, 120}, Button::drawMenuItem, "CarbonPro",
	[](bool press) { if (!press) goTimerShort(1); } ),
	Button({0, 180}, Button::drawMenuItem, "FiberTek",
	[](bool press) { if (!press) goTimerLong(2); } ),
	Button({0, 240}, Button::drawMenuItem, "CarbonSilv",
	[](bool press) { if (!press) goTimerShort(4); } ),
	Button({0, 300}, Button::drawMenuItem, "CarbonPhos",
	[](bool press) { if (!press) goTimerShort(3); } )
);


