#include <gameduino2/GD2.h>

#include <stdio.h>

#include "Assets.h"
#include "Screens.h"

static unsigned int tempLevels[3] = {
	0, // Hot
	0, // Cold On
	0  // Cold Off
};
static const char *tempNumbersF[12] = {
	"183" DEGREE, "193" DEGREE, "202" DEGREE, "206" DEGREE,
	 "34" DEGREE,  "35" DEGREE,  "36" DEGREE,  "38" DEGREE,
	 "29" DEGREE,  "30" DEGREE,  "31" DEGREE,  "32" DEGREE
};
static const char *tempNumbersC[12] = {
	"84" DEGREE, "89" DEGREE, "94" DEGREE, "97" DEGREE,
	 "1" DEGREE,  "2" DEGREE,  "3" DEGREE,  "4" DEGREE,
	 "0" DEGREE, "-1" DEGREE, "-2" DEGREE, "-3" DEGREE
};
static bool tempMetric;

void tempIncrease(unsigned int index)
{
	if (tempLevels[index] < 3)
		tempLevels[index]++;
}

void tempDecrease(unsigned int index)
{
	if (tempLevels[index] > 0)
		tempLevels[index]--;
}

static Button buttonsTemperature[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			screenCurrent = &screenSettings;
	}),
	Button(2, {0, 420}, Button::drawFullWidth, lStringSave, [](bool press) {
		if (!press) {
#ifdef USE_SERIAL
			for (int i = 0; i < 3; i++)
				serialPrintf("@%1u%1u", i + 3, tempLevels[i]);
#endif // USE_SERIAL
			screenCurrent = &screenSettings;
		}
	}),
	// Hot
	Button(3, {0, 100}, Button::drawSmallLeft,
		[](bool press) { if (!press) tempDecrease(0); }),
	Button(4, {232, 100}, Button::drawSmallRight,
		[](bool press) { if (!press) tempIncrease(0); }),
	// Cold On
	Button(5, {0, 200}, Button::drawSmallLeft,
		[](bool press) { if (!press) tempDecrease(1); }),
	Button(6, {232, 200}, Button::drawSmallRight,
		[](bool press) { if (!press) tempIncrease(1); }),
	// Cold Off
	Button(7, {0, 300}, Button::drawSmallLeft,
		[](bool press) { if (!press) tempDecrease(2); }),
	Button(8, {232, 300}, Button::drawSmallRight,
		[](bool press) { if (!press) tempIncrease(2); }),
};

static const LanguageString tHotTemp ({
	"Hot Temp.",
	"Heisse Temperatur",
	"Temp. Chaude",
	"Temp. Caliente"
});
static const LanguageString tColdTempOn ({
	"Cold Temp. ON",
	"Kalt Temperatur AN",
	"Temp. Froide Enclench" e_ACUTE  "e",
	"Temp. Fr" i_ACUTE "a Activiada"
});
static const LanguageString tColdTempOff ({
	"Cold Temp. OFF",
	"Kalt Temperatur AUS",
	"Temp. Froide " E_ACUTE "teinte",
	"Temp. Fr" i_ACUTE "a Desactivada"
});

Screen screenTemperature (
	// Parent screen
	&screenSettings,
	// Buttons
	buttonsTemperature, 8,
	// Initialization function
#ifdef USE_SERIAL
	[](void) {
		for (int i = 0; i < 3; i++) {
			serialPrintf("@%1u", i + 6);
			tempLevels[i] = serialGet();
		}

		serialPrintf("@X");
		tempMetric = serialGet() ? true : false;
	},
#else
	nullptr,
#endif // USE_SERIAL
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 70,  FONT_TITLE, OPT_CENTERX, tHotTemp());
		GD.cmd_text(136, 170, FONT_TITLE, OPT_CENTERX, tColdTempOn());
		GD.cmd_text(136, 270, FONT_TITLE, OPT_CENTERX, tColdTempOff());
		GD.cmd_text(252, 70, FONT_TITLE, OPT_CENTERX, tempMetric ? "(C)"
			: "(F)");

		for (int y = 100; y < 400; y += 100) {
			// Main box
			GD.ColorRGB(WHITE);
			GD.Begin(RECTS);
			GD.Vertex2ii(40, y + 13);
			GD.Vertex2ii(232, y + 26);
	
			// Outline
			GD.ColorRGB(NC_FRGND_COLOR / 2);
			GD.LineWidth(16);
			GD.Begin(LINES);
			GD.Vertex2ii(40, y + 13); GD.Vertex2ii(232, y + 13);
			GD.Vertex2ii(40, y + 26); GD.Vertex2ii(232, y + 26);
			GD.Vertex2ii(64, y + 13); GD.Vertex2ii(64, y + 26);
			GD.Vertex2ii(112, y + 13); GD.Vertex2ii(112, y + 26);
			GD.Vertex2ii(160, y + 13); GD.Vertex2ii(160, y + 26);
			GD.Vertex2ii(208, y + 13); GD.Vertex2ii(208, y + 26);

			// Points
			int px = tempLevels[y / 100 - 1] * 48 + 64;
			GD.PointSize(16 * 10);
			GD.Begin(POINTS);
			GD.Vertex2ii(px, y + 19);
			GD.ColorRGB(NC_GREEN_COLOR);
			GD.PointSize(16 * 8);
			GD.Vertex2ii(px, y + 19);
		}

		const char **tempNumbers = tempMetric ? tempNumbersC :
			tempNumbersF;
		GD.ColorRGB(NC_FRGND_COLOR);
		for (int i = 0; i < 12; i++) {
			GD.cmd_text((i % 4) * 48 + 62, (i / 4) * 100 + 130,
				FONT_LIGHT, OPT_CENTERX, tempNumbers[i]);
		}
	}
);

