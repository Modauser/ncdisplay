/**
 * @file Dispense.cpp
 * @brief The main dispense screen.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "Errors.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

static bool mainDispensing = false;
static bool hotDispensing = false;
static unsigned int hotTimeout = 0;
static unsigned int mainAniImage = 0;
static unsigned int mainAniCounter = 0;
static unsigned int mainAniCounterMax = 2;
static unsigned int timeDateCounter = 0;

static void showHotDispense(bool show);
static void setVisibilities(void);

static void doPress(char letter, bool pressed)
{
	if (pressed) {
		if (Error::shouldDispense() && !mainDispensing) {
			mainDispensing = true;
			mainAniImage = ANI1_HANDLE;
			mainAniCounter = 0;
			mainAniCounterMax = hotDispensing ? 4 : 2;
			serialPrintf("$%c", letter);
		}
	} else {
		mainDispensing = false;
		serialPrintf("$R$R");
	}
}

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
		timeDateCounter = 2000;
		showHotDispense(false);
		setVisibilities();
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader(false);

		if (!hotDispensing) {
			GD.Begin(RECTS);
			GD.Vertex2ii(0, 296);
			GD.Vertex2ii(272, 440);
		} else if (!mainDispensing && ++hotTimeout >= 200) {
			showHotDispense(false);
		}

		GD.Begin(BITMAPS);
		if (mainDispensing) {
			GD.Vertex2ii(76, 0, mainAniImage); // Flow animation image

			if (++mainAniCounter >= mainAniCounterMax) {
				mainAniCounter = 0;
				if (++mainAniImage > ANI3_HANDLE)
					mainAniImage = ANI1_HANDLE;
			}
		} else {
			GD.Vertex2ii(0, 130, HOMEWTR_HANDLE);
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
			
				// Put error message
				GD.ColorRGB(0xFF0000);
				GD.cmd_text(136, 238, FONT_MESG, OPT_CENTER,
					warningHot());
			} else if (Error::hasError()) {
				Error::show();
			}
		}

		++timeDateCounter;
		// Check for errors
		if ((timeDateCounter % 500) == 0)
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
		if (!pressed)
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
	Button({0, 298}, Button::drawRedFullWidth, dispenseHot,
	[](bool pressed) {
		hotTimeout = 0;
		doPress('H', pressed);
	}),
	Button({0, 0}, Button::drawDots, [](bool pressed) {
		// On release
		if (!pressed) {
			if (hotDispensing) {
				showHotDispense(false);
			} else {
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
	for (unsigned int i = 0; i < 4; i++)
		Dispense.getButton(i).setVisibility(!show);
	Dispense.getButton(4).setVisibility(show);
	Dispense.getButton(5).setRender(show ? Button::drawBackArrow :
		Button::drawDots);
}

void setVisibilities(void)
{
	Dispense.getButton(1).setVisibility(MainBoard::canDispenseHot());
	Dispense.getButton(2).setVisibility(MainBoard::canDispenseSparkling());
}

