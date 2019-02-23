/**
 * @file Lock.cpp
 * @brief Passcode enable screen.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

static bool lockEnabled = false;

static void updateToggle(void);

static Screen Lock (
	ScreenID::Lock,
	// Parent screen
	ScreenID::Advanced,
	// Initialization function
#ifdef USE_SERIAL
	[](void) {
		lockEnabled = MainBoard::isLocked();
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
	[](bool press) {
		if (!press) {
			MainBoard::setLocked(lockEnabled);
			ScreenManager::setCurrent(ScreenID::Advanced);
		}
	})
);

void updateToggle(void)
{
	Lock.getButton(1).setForcePressed(lockEnabled);
}

