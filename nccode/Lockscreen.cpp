/**
 * @file Lockscreen.cpp
 * @brief The "Enter Passcode" screen.
 */
#include "type/Assets.h"
#include "type/Screen.h"

#include <gameduino2/GD2.h>

static ScreenID lockscreenReturn = ScreenID::Settings;

static int lockscreenIndex = 0;
static int lockscreenInput[4] = {
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
			ScreenManager::setCurrent(lockscreenReturn);
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
		GD.cmd_keys(0, 140, 272, 78, FONT_SMALL, OPT_FLAT, "123");
		GD.cmd_keys(0, 220, 272, 78, FONT_SMALL, OPT_FLAT, "456");
		GD.cmd_keys(0, 300, 272, 78, FONT_SMALL, OPT_FLAT, "789");
		GD.cmd_keys(0, 380, 272, 78, FONT_SMALL, OPT_FLAT, " 0<");

		// Get display inputs
		GD.get_inputs();
		if (int tag = GD.inputs.tag; isdigit(tag) || tag == '<')
			lockscreenEnterKey(tag);

		if (lockscreenIndex == 4)
			lockscreenIndex++;
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Settings);
	})
);

void doPasscodeTest(ScreenID id)
{
#ifdef USE_SERIAL
	serialPrintf("@K");
	if (serialGet() == 0) {
		ScreenManager::setCurrent(id);
	} else {
		lockscreenReturn = s;
		ScreenManager::setCurrent(ScreenID::Lockscreen);
	}
#else
	ScreenManager::setCurrent(id);
#endif
}

