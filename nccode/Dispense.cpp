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

static void showHotDispense(bool show);

static void doPress(char letter, bool pressed)
{
	if (pressed) {
		if (Error::shouldDispense() && !mainDispensing) {
			mainDispensing = true;
			mainAniImage = ANI1_HANDLE;
			mainAniCounter = 0;
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

static unsigned int timeDateCounter;

static Screen Dispense (
	// Our ID
	ScreenID::Dispense,
	// Parent screen
	ScreenID::Sleep,
	// Initialization function
	[](void) {
		timeDateCounter = 2000;
		showHotDispense(false);
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader(false);

		if (!hotDispensing) {
			GD.Begin(RECTS);
			GD.Vertex2ii(0, 296);
			GD.Vertex2ii(272, 440);
		} else if (!mainDispensing && ++hotTimeout >= 300) {
			showHotDispense(false);
		}

		GD.Begin(BITMAPS);
		if (mainDispensing) {
			GD.Vertex2ii(76, 0, mainAniImage); // Flow animation image

			if (++mainAniCounter >= 6) {
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
			GD.cmd_text(15, 450, FONT_TIME, 0, MainBoard::getTime());
			GD.cmd_text(202, 450, FONT_TIME, 0, MainBoard::getDate());
		}

		++timeDateCounter;
		// Check for errors
		if ((timeDateCounter % 500) == 0)
			Error::check();

		if (Error::hasError())
			Error::show();

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
	Button({0, 298}, Button::drawRedFullWidth, "DISPENSE HOT",
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

