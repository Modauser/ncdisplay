/**
 * @file Init.cpp
 * @brief Initializes the display and contains the main loop for screen viewing.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "Settings.h"

#include <gameduino2/GD2.h>

/**
 * Puts the fatal error message on the screen and halts.
 * @param msg The error message
 */
void showFatalError(const char *msg);

extern int initDisks(void);
extern bool USBUpdateCheck(void);

static void handshakeTest(void);

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
	Settings::loadPassword();

	// Get required language
	serialPrintf("#6");
	LanguageString::setCurrentLanguage(static_cast<Language>(serialGet()));

	//checkTankLevels();
	//ScreenManager::setCurrent(ScreenID::Fill);

	ScreenManager::setCurrent(ScreenID::Dispense);
}

void loop()
{
	// Renders the screen and handles potential input
	ScreenManager::showCurrent();
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

namespace std {
	void __throw_bad_function_call(void)
	{
		while (1);
	}
} // namespace std

extern "C" {
void __cxa_pure_virtual(void)
{
	while (1);
}
}


