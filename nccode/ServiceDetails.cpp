/**
 * @file ServiceDetails.cpp
 * @brief Displays service details.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "Settings.h"

#include <gameduino2/GD2.h>

static char serviceFlowCold[] = { 0, 0, '.', 0, ' ', 0, 0, 0, '\0' };
static char serviceFlowAmbi[] = { 0, 0, '.', 0, ' ', 0, 0, 0, '\0' };
static char serviceFlowHot[] = { 0, 0, '.', 0, ' ', 0, 0, 0, '\0' };

static unsigned char serviceTempIce = 0;
static unsigned char serviceTempHot = 0;
static bool serviceMetric;

static DateFormat serviceLastChanged;

struct ServiceLogEntry {
	char date[32];
	char description[64];
};

static ServiceLogEntry serviceLog[3];

static void getFlowRate(unsigned char v, char *s)
{
	s[3] = (v % 10) + '0';
	v /= 10;
	s[1] = (v % 10) + '0';
	v /= 10;
	s[0] = (v == 0) ? ' ' : (v % 10) + '0';

	if (serviceMetric) {
		s[5] = 'L';
	} else {
		s[5] = 'G';
		s[6] = 'P';
		s[7] = 'M';
	}
}

static void getServiceLog(ServiceLogEntry& e)
{
	e.date[0] = '\0';
	e.description[0] = '\0';

	unsigned int i = 0;

	serialPrintf("@f");
	if (serialGetchar() == '$') {

		// Pull date
		int c = serialGetchar();
		while (c != ',') {
			e.date[i++] = c;
			c = serialGetchar();
		}

		e.date[i] = '\0';
		if (serialGetchar() == '$')
			return;

		// Pull description
		c = serialGetchar();
		i = 0;
		while (c != '$') {
			e.description[i++] = c;
			c = serialGetchar();
		}

		e.description[i] = '\0';
	}
}

static Screen ServiceDetails (
	ScreenID::ServiceDetails,
	// Parent screen
	ScreenID::Advanced,
	// Initialization function
	[](void) {
		serialPrintf("@X");
		serviceMetric = (serialGet() != 0);

		serialPrintf("@a");
		getFlowRate(serialGet(), serviceFlowCold);
		serialPrintf("@b");
		getFlowRate(serialGet(), serviceFlowAmbi);
		serialPrintf("@c");
		getFlowRate(serialGet(), serviceFlowHot);
		serialPrintf("@d");
		serviceTempIce = serialGet();
		serialPrintf("@e");
		serviceTempHot = serialGet();

		serialPrintf("@(");
		int val = serialGet() << 16;
		val |= serialGet() << 8;
		val |= serialGet();
		serviceLastChanged.format(val);

		for (int i = 0; i < 3; i++)
			getServiceLog(serviceLog[i]);
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.ColorRGB(NC_FDGND_COLOR);
		GD.cmd_text(136, 70, FONT_LARGE, OPT_CENTER, LanguageString({
			"AVERAGE FLOW RATE",
			"DURCHSCHMITTLICHER",
			"D" E_ACUTE "BIT MOYEN",
			"TASA DE FLUJO PROMEDIO"
		})());
		GD.cmd_text(136, 95, FONT_LARGE, OPT_CENTER, LanguageString({
			"(LAST 24 HOURS)",
			"DURCHFLUSS",
			"(DERNI" E_GRAVE "RES 24 HEURES)",
			"(ULTIMAS 24 HORAS)"
		})());
		GD.cmd_text(20, 120, FONT_LIGHT, 0, Settings::getLabel(16));
		GD.cmd_text(20, 140, FONT_LIGHT, 0, Settings::getLabel(17));
		GD.cmd_text(20, 160, FONT_LIGHT, 0, Settings::getLabel(18));

		GD.cmd_text(120, 120, FONT_SMALL, 0, serviceFlowCold);
		GD.cmd_text(120, 140, FONT_SMALL, 0, serviceFlowAmbi);
		GD.cmd_text(120, 160, FONT_SMALL, 0, serviceFlowHot);

		GD.cmd_text(20, 195, FONT_LIGHT, 0, Settings::getLabel(19));
		GD.cmd_text(20, 245, FONT_LIGHT, 0, Settings::getLabel(20));
		GD.cmd_text(20, 295, FONT_LIGHT, 0, Settings::getLabel(22));
		GD.cmd_text(20, 345, FONT_LIGHT, 0, Settings::getLabel(21));

		GD.cmd_number(20, 215, FONT_SMALL, 0, serviceTempIce);
		GD.cmd_number(20, 265, FONT_SMALL, 0, serviceTempHot);
		GD.cmd_text(20, 315, FONT_SMALL, 0, serviceLastChanged.get());

		for (int i = 0; i < 3; i++) {
			if (*serviceLog[i].date == '\0')
				break;
			GD.cmd_text(20, 365 + i * 40, FONT_SMALL, 0,
				serviceLog[i].date);
			GD.cmd_text(30, 365 + i * 40 + 20, FONT_SMALL, 0,
				serviceLog[i].description);
		}
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Advanced);
	})
);

