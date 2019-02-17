#include <gameduino2/GD2.h>
#include <stdio.h>

#include "Assets.h"
#include "Screens.h"

bool lockEnabled = false;

static Button buttonsLock[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			screenCurrent = &screenAdvanced;
	}),
	Button(2, {180, 80}, Button::drawToggle, [](bool press) {
		if (!press) {
			lockEnabled ^= true;
			buttonsLock[1].setForcePressed(lockEnabled);
		}
	}),
	Button(3, {0, 420}, Button::drawFullWidth, lStringSave,
#ifdef USE_SERIAL
	[](bool press) {
		if (!press) {
			serialPrintf("@L%1u", lockEnabled ? 1 : 0);
			screenCurrent = &screenAdvanced;
		}
	}
#else
	nullptr
#endif // USE_SERIAL
	)
};

Screen screenLock (
	// Parent screen
	&screenAdvanced,
	// Buttons
	buttonsLock, 3,
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
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(20, 90, FONT_TITLE, OPT_CENTERY, LanguageString({
			"Passcode",
			"Passcode",
			"Code d'acc" e_GRAVE "s",
			"C" o_ACUTE "digo de acceso"
		})());

		GD.cmd_text(136, 240, FONT_LARGE, OPT_CENTER, lockEnabled ?
			"LOCKED" : "UNLOCKED");
	}
);

void doPasscodeTest(Screen *s)
{
#ifdef USE_SERIAL
	serialPrintf("@K");
	if (serialGet() == 0) {
		screenCurrent = s;
	} else {
		extern Screen *lockscreenReturn;
		lockscreenReturn = s;
		screenCurrent = &screenLockscreen;
	}
#else
	screenCurrent = s;
#endif
}

