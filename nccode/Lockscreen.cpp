/**
 * @file Lockscreen.cpp
 * @brief The "Enter Passcode" screen.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "Settings.h"

#include <gameduino2/GD2.h>

static ScreenID lockscreenReturn = ScreenID::Settings;

static int lockscreenIndex = 0;
static char lockscreenInput[4] = {
	0, 0, 0, 0
};

static void lockscreenEnterKey(char key)
{
	if (key == '<') {
		if (lockscreenIndex > 0)
			lockscreenIndex--;
	} else {
		lockscreenInput[lockscreenIndex++] = key;
	}
}

static Screen Lockscreen (
	ScreenID::Lockscreen,
	// Parent screen
	ScreenID::Settings,
	// Initialization function
	[](void) {
		for (int i = 0; i < 4; i++)
			lockscreenInput[i] = 0;
		lockscreenIndex = 0;
	},
	// Pre-draw function
	[](void) {
		if (lockscreenIndex > 4) {
			delay_ms(500);
			if (Settings::isCorrectPassword(lockscreenInput)) {
				ScreenManager::setCurrent(lockscreenReturn);
			} else {
				for (int i = 0; i < 4; i++)
					lockscreenInput[i] = 0;
				lockscreenIndex = 0;
			}
		}

		clearScreenWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 90, FONT_TITLE, OPT_CENTER, "Enter Passcode");

		GD.ColorRGB(NC_FDGND_COLOR);
		for (int i = 0; i < lockscreenIndex; i++) {
			GD.cmd_text(54 + 54 * i, 120, FONT_LARGE, OPT_CENTER,
				"*");
		}

		GD.ColorRGB(WHITE);
		GD.cmd_fgcolor(NC_FRGND_COLOR);
		GD.cmd_keys(0, 140, 272, 78, FONT_SMALL, OPT_FLAT, "123");
		GD.cmd_keys(0, 220, 272, 78, FONT_SMALL, OPT_FLAT, "456");
		GD.cmd_keys(0, 300, 272, 78, FONT_SMALL, OPT_FLAT, "789");
		GD.cmd_keys(0, 380, 272, 78, FONT_SMALL, OPT_FLAT, " 0<");

		if (lockscreenIndex == 4) {
			lockscreenIndex++;
			return;
		}

		// Get display inputs
		GD.get_inputs();
		if (int tag = GD.inputs.tag; isdigit(tag) || tag == '<') {
			while (GD.inputs.tag == tag) {
				delay_ms(10);
				GD.get_inputs();
			}
			lockscreenEnterKey(tag);
		}

	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Settings);
	})
);

void doPasscodeTest(ScreenID id)
{
	if (MainBoard::isLocked()) {
		lockscreenReturn = id;
		ScreenManager::setCurrent(ScreenID::Lockscreen);
	} else {
		ScreenManager::setCurrent(id);
	}
}

