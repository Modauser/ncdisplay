#include <gameduino2/GD2.h>
#include <cstdio>

#include "Assets.h"
#include "Settings.h"
#include "Screens.h"

/**
 * Puts the fatal error message on the screen and halts.
 * @param msg The error message
 */
void showFatalError(const char *msg);

/**
 * The currently displayed screen.
 * Modified by other screens to transition between them.
 */
Screen *screenCurrent = &screenDispense;

extern int initDisks(void);

void setup()
{
	// Do basic display preparation
	displayInit();
	GD.ClearColorRGB(NC_BKGND_COLOR);
	GD.Clear();
	GD.swap();
	
	// Show the loading spinner
	GD.ClearColorRGB(NC_BKGND_COLOR);
	GD.Clear();
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.cmd_spinner(GD.h / 2, (GD.w / 2), 0, 0);
	GD.swap();

	// Prepare SD card and USB drive
	int diskStatus = initDisks();
	if (diskStatus < 0)
		showFatalError("Failed to access SD card.");

	// Check for update
	extern bool USBUpdateCheck(void);
	USBUpdateCheck();

	// Handshake test
	// EDIT! made in hal/src/hal_usart_sync.c to prevent hang on USART read
	// (Undid the change, need to try again. TODO)

	//unsigned int tries = 0;
	//do {
	//	int check = getchar();
	//	if (check == '$') {
	//		check = getchar();
	//		if (check == '0')
	//			break;
	//	}

	//	delay_ms(10);
	//} while (++tries < HANDSHAKE_TIMEOUT);
	//
	//if (tries == HANDSHAKE_TIMEOUT)
	//	showFatalError("COM error; check the display cable.");

	// Load fonts and images from SD card
	loadAssets();

	Settings::loadLabels();

	// Get required language
	serialPrintf("#6");
	LanguageString::setCurrentLanguage(static_cast<Language>(serialGet()));
}

void loop()
{
	static int sleepImage = 0;
	static unsigned int sleepCounter = 0;

	auto oldScreen = screenCurrent;

	// Renders the screen and handles potential input
	screenCurrent->show();

	if (screenCurrent != oldScreen) {
		// Switched to a new screen, let it prepare for showing
		// This includes queries to the main board
		screenCurrent->prepare();
	}

	delay_ms(10);
}

void showFatalError(const char *msg)
{
	GD.ClearColorRGB(NC_BKGND_COLOR);
	GD.Clear();
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.cmd_text(0, 0, FONT_MESG, 0, msg);
	GD.swap();
	while (1)
		delay_ms(1000);
}

