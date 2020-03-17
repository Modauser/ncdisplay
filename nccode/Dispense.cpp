/**
 * @file Dispense.cpp
 * @brief The main dispense screen.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "Errors.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

constexpr unsigned int ANICOUNT_LONG = 4;
constexpr unsigned int ANICOUNT_SHORT = 2;

// True if dispensing water
static bool mainDispensing = false;
// True if showing dispense hot button
static bool hotDispensing = false;

static unsigned int hotTimeout = 0;
static unsigned int mainAniImage = 0;
static unsigned int mainAniCounter = 0;
static unsigned int mainAniCounterMax = ANICOUNT_SHORT;
static unsigned int timeDateCounter = 0;

/**
 * Shows or hides the dispense hot button and warning.
 * @param show True to show dispense hot button/warning
 */
static void showHotDispense(bool show);

/**
 * Refreshes visibilities of buttons/text based on current state.
 */
static void setVisibilities(void);

/**
 * Render function for a full-width dispense button.
 * @see Button.h
 */
void bigDispenseButton(const vec2& xy, bool pressed, const LanguageString& text);

/**
 * Does the appropriate action when a dispense button is pressed.
 * @param letter Letter associated with button (e.g. 'C' for cold)
 * @param pressed Button pressed/released state
 */
static void doPress(char letter, bool pressed);

static const LanguageString mainDispense1 ({
	"PRESS AND HOLD",
	"DR" U_UMLAUT "CKEN UND",
	"APPUYER ET MAINTENIR",
	"PRESIONAR Y MANTENER",
	"DRUKKEN EN",
	"H" A_ORING "LL KNAPP INTRYCKT",
	"VELD DRIKK",
	"TRYK OG HOLD"
});
static const LanguageString mainDispense2 ({
	"TO DISPENSE",
	"HALTEN",
	"POUR VERSER",
	"PARA DISPENSAR",
	"VASTHOUDEN",
	"F" O_UMLAUT "R VAL AV DRYCK",
	" ",
	"FOR VAND"
});

static const LanguageString warningHot ({
	"WARNING: HOT!",
	"VORSICHT: HEI" ESZETT "!",
	"ATTENTION: CHAUD!",
	"ADVERTENCIA: CALIENTE!",
	"WAARSCHUWING: HEET!",
	"VARNING: HETT!",
	"ADVARSEL: VARMT VANN!",
	"ADVARSEL: VARMT!",
});
static const LanguageString dispenseHot ({
	"DISPENSE HOT",
	"AUSGABE HEI" ESZETT,
	"VERSER CHAUD",
	"DISPENSAR AGUA CALIENTE",
	"HEET WATER",
	"LEVERERA HETVATTEN",
	"VARMT VANN",
	"TRYK FOR VARMT VAND"
});

// Estimated widths for "WARNING: HOT!" message box
static const unsigned int warningHotWidths[] = {
	11 * 13, 11 * 15, 11 * 17, 11 * 22,
	11 * 19, 11 * 14, 11 * 21, 11 * 16
};

static Screen Dispense (
	// Our ID
	ScreenID::Dispense,
	// Parent screen
	ScreenID::Sleep,
	// Initialization function
	[](void) {
		MainBoard::updateModelNumber();
        // GD.cmd_calibrate();
		// Trigger time/date update on first render
		timeDateCounter = 2000 - 1;
		Error::check();

		showHotDispense(false);
		setVisibilities();
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader(false);

		GD.Begin(BITMAPS);
		if (mainDispensing) {
			// Draw flow animation
			GD.Vertex2ii(76, 0, mainAniImage);

			if (++mainAniCounter >= mainAniCounterMax) {
				mainAniCounter = 0;
				if (++mainAniImage > ANI3_HANDLE)
					mainAniImage = ANI1_HANDLE;
			}
		} else {
			// Show dispense text
			GD.Vertex2ii(0, 130, HOMEWTR_HANDLE);  //paints image based on handle
			GD.ColorRGB(NC_FDGND_COLOR);
			GD.cmd_text(136, 80, FONT_LARGE, OPT_CENTER,
				mainDispense1());
			GD.cmd_text(136, 110, FONT_LARGE, OPT_CENTER,
				mainDispense2());

			// Time and date
			GD.ColorRGB(BLACK);
			GD.cmd_text(90, 460, FONT_TIME, OPT_CENTER,
				MainBoard::getTime());
			GD.cmd_text(186, 460, FONT_TIME, OPT_CENTER,
				MainBoard::getDate());

			// Show hot dispense warning if hot was selected
			if (hotDispensing) {
				auto halfwidth = warningHotWidths[static_cast<int>(
					LanguageString::getCurrentLanguage())] / 2;
				// Draw message box
				GD.ColorRGB(0xFF0000);
				GD.Begin(RECTS);
				GD.Vertex2ii(134 - halfwidth, 225);
				GD.Vertex2ii(138 + halfwidth, 255);
				GD.ColorRGB(WHITE);
				GD.Vertex2ii(136 - halfwidth, 227);
				GD.Vertex2ii(136 + halfwidth, 253);
			
				// Draw warning text
				GD.ColorRGB(0xFF0000);
				GD.cmd_text(136, 238, FONT_MESG, OPT_CENTER,
					warningHot());
			} else if (Error::hasError()) {
				// Show errors outside of hot dispense
				Error::show();
			}
		}

		if (!hotDispensing) {
			// Draw a white rectangle for button borders
			GD.ColorRGB(WHITE);
			GD.Begin(RECTS);
			GD.Vertex2ii(0, 296);
			GD.Vertex2ii(272, 440);
		} else if (!mainDispensing && ++hotTimeout >= 200) {
			// Exit hot dispense mode on timeout
			showHotDispense(false);
		}

		++timeDateCounter;
		// Check for errors
		if ((timeDateCounter % 250) == 0)
			Error::check();

		// Update date/time
		if (timeDateCounter >= 2000) {
			timeDateCounter = 0;
			MainBoard::updateDateTime();
		}
	},
	// Buttons
	Button({0,   298}, Button::drawDispenserItem, {
		"COLD",
		"KALT",
		"FROID",
		"FR" I_ACUTE "A",
		"KOUD",
		"KYLT",
		"KALDT",
		"KOLDT"
	}, [](bool pressed) {
		doPress('C', pressed);
	}),
	Button({0,   370}, Button::drawDispenserItem, {
		"HOT",
		"HEI" ESZETT,
		"CHAUD",
		"CALIENTE",
		"HEET",
		"HETVATTEN",
		"VARMT",
		"VARMT"
	}, [](bool pressed) {
		// Show hot dispense button/warning
		if (pressed)
			showHotDispense(true);
	}),
	Button({138, 298}, Button::drawDispenserItem, {
		"SPARKLING",
		"SODAWASSER",
		"P" E_ACUTE "TILLANT",
		"CARBONATADA",
		"KOOLZUUR\n\nHOUDEND",
		"KOLSYRA",
		"KULLSYRE",
		"MED BRUS"
	}, [](bool pressed) {
		doPress('S', pressed);
	}),
	Button({138, 370}, Button::drawDispenserItem, {
		"AMBIENT",
		"AMBIENT",
		"AMBIANTE",
		"AMBIENTE",
		"KAMER\n\nTEMPERATUUR",
		"SVALT",
		"TEMPERERT",
		"STUETEMP"
	}, [](bool pressed) {
		doPress('A', pressed);
	}),
	Button({0xF, 298}, Button::drawRedFullWidth, dispenseHot,
	[](bool pressed) {
		hotTimeout = 0;
		doPress('H', pressed);
	}),
	Button({0, 0}, Button::drawDots, [](bool pressed) {
		if (!pressed) {
			// If dispensing hot, this is actually a back button
			if (hotDispensing) {
				// Go back to normal dispense screen
				showHotDispense(false);
			} else {
				// Clear dispense error if necessary
				if (!Error::shouldDispense())
					Error::clearDispenseFlag();

				ScreenManager::setCurrent(ScreenID::Settings);
			}
		}
	})
);

void showHotDispense(bool show)
{
	hotDispensing = show;
	hotTimeout = 0;

	Dispense.getButton(4).setVisibility(show);
	Dispense.getButton(5).setRender(show ? Button::drawBackArrow :
		Button::drawDots);

	if (show) {
		// Hide other dispense buttons
		for (unsigned int i = 0; i < 4; i++)
			Dispense.getButton(i).setVisibility(false);
	} else {
		// Show dispense buttons
		Dispense.getButton(0).setVisibility(true);
		Dispense.getButton(3).setVisibility(true);
		setVisibilities();
	}
}

void setVisibilities(void)
{
	// Prepare dispense buttons based on model number
	auto model = MainBoard::getModelNumber();
	if (model < 3)
		Dispense.getButton(0).setRender(bigDispenseButton);
	Dispense.getButton(1).setVisibility(MainBoard::canDispenseHot());
	Dispense.getButton(2).setVisibility(MainBoard::canDispenseSparkling());

	if (!MainBoard::canDispenseHot()) {
		if (MainBoard::canDispenseSparkling()) {
			Dispense.getButton(2).setPosition({0, 370});
		} else {
			Dispense.getButton(3).setPosition({0, 370});
			Dispense.getButton(3).setRender(bigDispenseButton);
		}
	}
}

void doPress(char letter, bool pressed)
{
	// Begin dispensing on press
	Dispense.getButton(5).setVisibility(!pressed);
	if (pressed) {
		if (Error::shouldDispense() && !mainDispensing) {
			mainDispensing = true;
			mainAniImage = ANI1_HANDLE;
			mainAniCounter = 0;
			mainAniCounterMax = (hotDispensing ||
				!MainBoard::canDispenseHot() ||
				!MainBoard::canDispenseSparkling()) ?
				ANICOUNT_LONG : ANICOUNT_SHORT;
			serialPrintf("$%c", letter);
		}
	} else {
		// Send release code
		mainDispensing = false;
		serialPrintf("$R$R");
	}
}


void bigDispenseButton(const vec2& xy, bool pressed, const LanguageString& text)
{
	// Rectangle
	GD.Begin(RECTS);
	GD.ColorRGB(pressed ? 0x0c3d6b : NC_FRGND_COLOR);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 272, xy.y + 68);

	GD.ColorRGB(WHITE);
	GD.cmd_text(xy.x + 136, xy.y + 33, FONT_SMALL, OPT_CENTER, text(), 20);
}

