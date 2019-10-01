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
static void loadLockImage(void);

static Screen Lock (
	ScreenID::Lock,
	// Parent screen
	ScreenID::Advanced,
	// Initialization function
	[](void) {
		lockEnabled = MainBoard::isLocked();
		loadLockImage();
		updateToggle();
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.Begin(RECTS);
		GD.Vertex2ii(0, 120);
		GD.Vertex2ii(272, 120);

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(20, 90, FONT_TITLE, OPT_CENTERY, LanguageString({
			"Passcode",
			"Passcode",
			"Code d'acc" e_GRAVE "s",
			"C" o_ACUTE "digo de acceso"
		})());

		// Draws lock image
		GD.ColorRGB(WHITE);
		GD.Begin(BITMAPS);
		GD.Vertex2ii(104, 197, FREE_HANDLE);
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Advanced);
	}),
	Button({180, 90}, Button::drawToggle, [](bool press) {
		if (!press) {
			lockEnabled ^= true;
			loadLockImage();
			updateToggle();
		}
	}),
	Button({0, 420}, Button::drawFullWidth, lStringSave,
	[](bool press) {
		if (!press) {
			// Save lock state
			MainBoard::setLocked(lockEnabled);
			ScreenManager::setCurrent(ScreenID::Advanced);
		}
	})
);

void updateToggle(void)
{
	Lock.getButton(1).setForcePressed(lockEnabled);
}

void loadLockImage(void)
{
	loadImage(FREE_HANDLE, lockEnabled ? "lock.jpg" : "unlock.jpg");
}

