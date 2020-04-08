/**
 * @file Sleep.cpp
 * @brief The sleeping screen. This is shown when the display times out from
 * user interaction.
 */
#include "type/Assets.h"
#include "MainBoard.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

static bool inSleepMode = false;
static char sleepImagePath[] = "screensaver1.jpg";
static int sleepImageCurrent = 1;
static int sleepImageLast = -1;
static unsigned int sleepImageCounter;
static char sleepBottlesUsed[8];

static Screen Sleep (
	ScreenID::Sleep,
	// Parent screen
	ScreenID::Sleep,
	// Initialization function
	[](void) {
		sleepImageCounter = 0;
		// inSleepMode = MainBoard::getSleepmodeEnabled();  //dpm changed, was going into sleep screens when sleep mode was enabled, NOT when board was asleep 
		inSleepMode = MainBoard::getInSleepmode();
		MainBoard::getBottlesSaved(sleepBottlesUsed);
	},
	// Pre-draw function
	[](void) {
		//inSleepMode = MainBoard::getInSleepmode();  //dpm 4_8_2020
		if (inSleepMode) {
			// If in sleep mode, only show sleep mode image
			if (sleepImageLast != 0x1234) {
				sleepImageLast = 0x1234;
				loadImage(SLEEP_HANDLE, "sleepmode.jpg", 0);
			}
		} else {
			// Load current sleep image
			if (sleepImageCurrent != sleepImageLast) {
				sleepImageLast = sleepImageCurrent;

				sleepImagePath[11] = sleepImageCurrent + '0';
				GD.BitmapHandle(SLEEP_HANDLE);
				GD.cmd_loadimage(0, 0);
				GD.load(sleepImagePath);
				GD.finish();
			}
		}

		// Draw the sleep image
		GD.Clear();
		GD.ColorRGB(WHITE);
		GD.Begin(BITMAPS);
		GD.Vertex2ii(0, 0, SLEEP_HANDLE);

		if (!inSleepMode) {
		
			// Show TOUCH TO BEGIN
			//GD.ColorRGB(NC_BKGND_COLOR);
			//GD.Begin(RECTS);
			//GD.Vertex2ii(0, 420);
			//GD.Vertex2ii(272, 480);

			//GD.ColorRGB(NC_FRGND_COLOR);
			//GD.cmd_text(136, 450, FONT_SMALL, OPT_CENTER, LanguageString({
			//	"TOUCH TO BEGIN"
			//})());

			// Show bottles save on proper image
			if (sleepImageCurrent == 2) {
				GD.ColorRGB(NC_FDGND_COLOR);
				GD.cmd_text(136, 130, FONT_SMALL, OPT_CENTER,
					sleepBottlesUsed);
			}
			
			// Switch images after timeout
			sleepImageCounter += 10;
			if (sleepImageCounter >= SLEEP_IMGSWITCH) {
				sleepImageCounter = 0;

				sleepImageCurrent++;
				if (sleepImageCurrent > 3)
					sleepImageCurrent = 1;
				inSleepMode = MainBoard::getInSleepmode();  //check to see if we are in sleep mode dpm 4_8_2020
			}
		}

		// Check for touch
		GD.get_inputs();
		if (GD.inputs.x != -32768 && GD.inputs.y != -32768) {
			// Wait for release
			while (GD.inputs.x != -32768 && GD.inputs.y != -32768) {
				GD.get_inputs();
				delay_ms(10);
			}

			// Force reload of image next time
			// (other images may be loaded into sleep's space)
			sleepImageLast = -1;

			// Wake up, go to main dispense screen
			ScreenManager::setCurrent(ScreenID::Dispense);
		}
	}
);

