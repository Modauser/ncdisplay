#include <gameduino2/GD2.h>
#include "Assets.h"
#include "Errors.h"
#include "Screens.h"

// UART redirected to stdio
#include <cstdio>

void showErrorStatus(void);
void showFlowAnimation(int on);
void updateDateTime(void);

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
	Button(1, {0,   296}, Button::drawDispenserItem, {
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
	Button(2,  {0,   369}, Button::drawDispenserItem, {
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
	Button(3, {137, 296}, Button::drawDispenserItem, {
		"SPARKLING",
		"SODAWASSER",
		"P" E_ACUTE "TILLANT",
		"CARBONATADA",
		"KOOLZUUR HOUDEND",
		"KOLSYRA",
		"KULLSYRE",
		"MED BRUS"
	}, [](bool pressed) {
		doPress('S', pressed);
	}),
	Button(4, {137, 369}, Button::drawDispenserItem, {
		"AMBIENT",
		"AMBIENT",
		"AMBIANTE",
		"AMBIENTE",
		"KAMER TEMPERATUUR",
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

static DateFormat dDate ('-');
static char dTime[] = { 0, 0, ':', 0, 0, 0, 'M', '\0' };
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
		Screen::clearWithIonHeader();

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
		GD.cmd_text(15, 450, FONT_TIME, 0, dTime);
		GD.cmd_text(202, 450, FONT_TIME, 0, dDate.get());

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
			updateDateTime();
		}
#endif // USE_SERIAL
	}
);

void updateDateTime(void)
{
	int val;

	serialPrintf("@T");
	val = serialGet(); // Hour
	dTime[0] = val / 10 + '0';
	dTime[1] = val % 10 + '0';
	val = serialGet(); // Minute
	dTime[3] = val / 10 + '0';
	dTime[4] = val % 10 + '0';
	serialGet(); // Second, unused
	val = serialGet(); // AM/PM
	if (val == 2)
		dTime[5] = '\0';
	else
		dTime[5] = (val ? 'P' : 'A');

	serialPrintf("@D");
	val = serialGet() << 16; // Month
	val |= serialGet() << 8; // Day
	val |= serialGet(); // Year
	dDate.format(val);
}

