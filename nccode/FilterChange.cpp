#include <gameduino2/GD2.h>

#include "Assets.h"
#include "Screens.h"

extern unsigned int filterType;
extern unsigned int filterTimerMonths;
extern unsigned int filterTimerGallons;

static void goTimerShort(int type)
{
	filterType = type;
	filterTimerMonths = 6;
	filterTimerGallons = 750;
	screenCurrent = &screenFilterChangeTimer;
}
// L 2850 5700
static void goTimerLong(int type)
{
	filterType = type;
	filterTimerMonths = 12;
	filterTimerGallons = 1500;
	screenCurrent = &screenFilterChangeTimer;
}

static Button buttonsFilterChange[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool pressed) {
		if (!pressed)
			screenCurrent = &screenFilter;
	}),
	Button(2, {0, 60}, Button::drawMenuItem, "CarbonPlus",
	[](bool press) { if (!press) goTimerLong(0); } ),
	Button(3, {0, 120}, Button::drawMenuItem, "CarbonPro",
	[](bool press) { if (!press) goTimerShort(1); } ),
	Button(4, {0, 180}, Button::drawMenuItem, "FiberTek",
	[](bool press) { if (!press) goTimerLong(2); } ),
	Button(5, {0, 240}, Button::drawMenuItem, "CarbonSilv",
	[](bool press) { if (!press) goTimerShort(4); } ),
	Button(6, {0, 300}, Button::drawMenuItem, "CarbonPhos",
	[](bool press) { if (!press) goTimerShort(3); } ),
};

Screen screenFilterChange (
	// Parent screen
	&screenFilter,
	// Buttons
	buttonsFilterChange, 6,
	// Initialization function
	nullptr,
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();
	}
);


