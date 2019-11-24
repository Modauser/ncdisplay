/**
 * @file Init.cpp
 * @brief Initializes the display and contains the main loop for screen viewing.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "Errors.h"
#include "MainBoard.h"
#include "Settings.h"
#include "SystemCalls.h"

#include <gameduino2/GD2.h>

/**
 * Checks USB for a firmware update.
 * @see USBUpdate.cpp
 * @return True if USB is mounted, false if not
 */
extern bool USBUpdateCheck(void);

/**
 * Checks USB for files that need to be copied to the SD card.
 * @see USBUpdateFiles.cpp
 */
extern void USBUpdateFiles(void);

/**
 * Puts the fatal error message on the screen and halts.
 * @param msg The error message
 */
void showFatalError(const char *msg);

/**
 * Attempts to communicate with the main board, showing a fatal error on
 * failure.
 *
 * First attempts to read "$0" put out by main board when powered on. If that
 * fails, the current language is then requested and a valid response is checked
 * for.
 * @return Zero on success
 */
//static int handshakeTest(void);

void playVideo(const char *fileName)
{
    FIL fd;
    if (FRESULT r = f_open(&fd, fileName, FA_READ); r != FR_OK)
      return;

    UINT br = 0;
    GD.cmd_playvideo(OPT_FULLSCREEN);
    for (FSIZE_t fileSize = f_size(&fd); fileSize > 0; fileSize -= br) {
        uint8_t buffer[512];
        if (FRESULT r = f_read(&fd, buffer, 512, &br); r != FR_OK)
            break;
        GD.cmd_n(buffer, 512);
    }

    GD.finish();
    f_close(&fd);
}

/**
 * Basic display setup, run before entering the main loop.
 */
void setup()
{
	// Do basic display preparation
	displayInit();
	GD.ClearColorRGB(NC_BKGND_COLOR);
	GD.Clear();
	GD.wr32(REG_ROTATE, 1);//2);
	GD.swap();

    while (1) {
        playVideo("fish.avi");
        delay_ms(2000);
    }

	//USBUpdateCheck();
	//USBUpdateFiles();
	//unmount(DRV_USB);

	//// Show the loading spinner
	//GD.ClearColorRGB(NC_BKGND_COLOR);
	//GD.Clear();
	//GD.ColorRGB(NC_FRGND_COLOR);
	//GD.cmd_spinner(GD.h / 2, (GD.w / 2), 0, 0);
	//GD.swap();

	//handshakeTest();

	//// Set backlight brightness (value between 0-128)
	//GD.wr32(REG_PWM_DUTY, 64);

	//// Load fonts and images from SD card
	//loadAssets();

	//// Get the current language
	//auto lang = MainBoard::getLanguage();
	//LanguageString::setCurrentLanguage(static_cast<Language>(lang));
	//MainBoard::allowDispenseError();

	//// Load language text
	//Settings::loadLabels();
	//Settings::loadPassword();

	//// Show WelcomeLanguage if just had factory reset
	//// Do normal setup otherwise
	//ScreenManager::setCurrent(lang != 9 ? ScreenID::Setup :
	//	ScreenID::WelcomeLanguage);
}

/**
 * Main display loop.
 */
void loop()
{
	// Renders the current screen and handles potential input
	//ScreenManager::showCurrent();

	delay_ms(10);
}

//int handshakeTest(void)
//{
//	// Try to get "$0" given by main board on boot
//	int answer = serialTest();
//	if (answer == -1) {
//		// Maybe board is already on? Try querying language
//		delay_ms(2000);
//		serialPrintf("#6");
//		answer = serialTest();
//		// Valid language?
//		if (answer != -1)
//			return 0;
//
//		showFatalError("COM error check display cable");
//	}
//
//	return 0;
//}
//
//void showFatalError(const char *msg)
//{
//	// Put message on screen then halt.
//	GD.ClearColorRGB(NC_BKGND_COLOR);
//	GD.Clear();
//	GD.ColorRGB(NC_FRGND_COLOR);
//	GD.cmd_text(0, 0, 18, 0, msg);
//	GD.swap();
//	while (1)
//		delay_ms(1000);
//}

namespace std {
	// Needed because we use std::function within the program
	void __throw_bad_function_call(void)
	{
		while (1);
	}
} // namespace std

extern "C" {
// Needed because we use pure virtual functions for Screen
void __cxa_pure_virtual(void)
{
	while (1);
}
}


