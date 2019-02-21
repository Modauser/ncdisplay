/**
 * @file Lock.cpp
 * @brief Passcode enable screen.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

bool lockEnabled = false;

static void updateToggle(void);

static Screen Lock (
	ScreenID::Lock,
	// Parent screen
	ScreenID::Advanced,
	// Initialization function
#ifdef USE_SERIAL
	[](void) {
		serialPrintf("@K");
		lockEnabled = (serialGet() != 0);
	},
#else
	nullptr,
#endif // USE_SERIAL
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(20, 90, FONT_TITLE, OPT_CENTERY, LanguageString({
			"Passcode",
			"Passcode",
			"Code d'acc" e_GRAVE "s",
			"C" o_ACUTE "digo de acceso"
		})());

		GD.cmd_text(136, 240, FONT_LARGE, OPT_CENTER, lockEnabled ?
			"LOCKED" : "UNLOCKED");
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Advanced);
	}),
	Button({180, 80}, Button::drawToggle, [](bool press) {
		if (!press) {
			lockEnabled ^= true;
			updateToggle();
		}
	}),
	Button({0, 420}, Button::drawFullWidth, lStringSave,
#ifdef USE_SERIAL
	[](bool press) {
		if (!press) {
			serialPrintf("@L%1u", lockEnabled ? 1 : 0);
			ScreenManager::setCurrent(ScreenID::Advanced);
		}
	}
#else
	nullptr
#endif // USE_SERIAL
	)
);

void updateToggle(void)
{
	Lock.getButton(1).setForcePressed(lockEnabled);
}

