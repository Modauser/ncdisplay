#include <gameduino2/GD2.h>
#include "Assets.h"
#include "Errors.h"
#include "MainBoard.h"
#include "Screens.h"

void showErrorStatus(void);
void showFlowAnimation(int on);

static bool mainDispensing = false;
static int mainAniImage = 0;
static int mainAniCounter = 0;

void doPress(char letter, bool pressed)
{
	if (pressed) {
		if (!mainDispensing) {
			mainDispensing = true;
			mainAniImage = ANI1_HANDLE;
			mainAniCounter = 0;
#ifdef USE_SERIAL
			serialPrintf("$%c", letter);
#endif // USE_SERIAL
		}
	} else {
		mainDispensing = false;
#ifdef USE_SERIAL
		serialPrintf("$R$R");
#endif // USE_SERIAL
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

static Button buttonsDispense[] = {
	Button(1, {0,   298}, Button::drawDispenserItem, {
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
	Button(2,  {0,   370}, Button::drawDispenserItem, {
		"HOT",
		"HEI" ESZETT,
		"CHAUD",
		"CALIENTE",
		"HEET",
		"HETVATTEN",
		"VARMT",
		"VARMT"
	}, [](bool pressed) {
		doPress('H', pressed);
	}),
	Button(3, {138, 298}, Button::drawDispenserItem, {
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
	Button(4, {138, 370}, Button::drawDispenserItem, {
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
	Button(5, {0, 0}, Button::drawDots, [](bool pressed) {
		// On release
		if (!pressed)
			screenCurrent = &screenSettings;
	})
};

static unsigned int timeDateCounter = 3000;

Screen screenDispense (
	// Parent screen
	nullptr,
	// Buttons
	buttonsDispense, 5,
	// Initialization function
	[](void) {
		timeDateCounter = 2000;
	},
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader(false);

		GD.Begin(RECTS);
		GD.Vertex2ii(0, 296);
		GD.Vertex2ii(272, 372);

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
		}

		// Time and date
		GD.ColorRGB(BLACK);
		GD.cmd_text(15, 450, FONT_TIME, 0, MainBoard::getTime());
		GD.cmd_text(202, 450, FONT_TIME, 0, MainBoard::getDate());

#ifdef USE_SERIAL
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
#endif // USE_SERIAL
	}
);

