#include <gameduino2/GD2.h>

#include "Assets.h"
#include "MainBoard.h"
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
extern bool USBUpdateCheck(void);

void handshakeTest(void);
void checkTankLevels(void);

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

	USBUpdateCheck();

	handshakeTest();

	// Load fonts and images from SD card
	loadAssets();

	Settings::loadLabels();

	checkTankLevels();

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

void handshakeTest(void)
{
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
}

void checkTankLevels(void)
{
#ifdef USE_SERIAL
	// Cold first
	if (!MainBoard::isColdTankFull()) {
		fillMessage = &fillCold;
		fillTint = 0x0000FF;
		do {
			screenFill.show();
			delay_ms(1000);
		} while (!MainBoard::isColdTankFull());
	}

	// Hot next
	if (!MainBoard::isHotTankFull()) {
		fillMessage = &fillHot;
		fillTint = 0xFF0000;
		do {
			screenFill.show();
			delay_ms(1000);
		} while (!MainBoard::isHotTankFull());
	}

	// Lastly, sparkling
	if (!MainBoard::isSparklingTankFull()) {
		fillMessage = &fillSparkling;
		fillTint = 0x00FF00;
		do {
			screenFill.show();
			delay_ms(1000);
		} while (!MainBoard::isSparklingTankFull());
	}

#endif // USE_SERIAL
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

