/**
 * @file ServiceDetails.cpp
 * @brief Displays service details.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "Settings.h"
#include "SystemCalls.h"

#include <gameduino2/GD2.h>

static char serviceFlowCold[Format::size::flowRate];
static char serviceFlowAmbi[Format::size::flowRate];
static char serviceFlowHot[Format::size::flowRate];

static char serviceTempIce[Format::size::temperature];
static char serviceTempHot[Format::size::temperature];
static bool serviceMetric;

static char serviceLastChanged[Format::size::date];

struct ServiceLogEntry {
	char date[32];
	char description[64];
};

static ServiceLogEntry serviceLog[3];

// Does an "@f" request and reads a log entry into the given object
static void getServiceLog(ServiceLogEntry& e);

static Screen ServiceDetails (
	ScreenID::ServiceDetails,
	// Parent screen
	ScreenID::Advanced,
	// Initialization function
	[](void) {
		// Show spinner because service log requests take time
		clearScreenWithIonHeader();
		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_spinner(GD.h / 2, (GD.w / 2), 0, 0);
		GD.swap();

		serialPrintf("@X");
		serviceMetric = (serialGet() != 0);

		serialPrintf("@a");
		Format::flowRate(serviceFlowCold, serialGet());
		serialPrintf("@b");
		Format::flowRate(serviceFlowAmbi, serialGet());
		serialPrintf("@c");
		Format::flowRate(serviceFlowHot, serialGet());
		serialPrintf("@d");
		Format::temperature(serviceTempIce, serialGet());
		serialPrintf("@e");
		Format::temperature(serviceTempHot, serialGet());

		serialPrintf("@(");
		int val = serialGet() << 16;
		val |= serialGet() << 8;
		val |= serialGet();
		Format::date(serviceLastChanged, val);

		// Get last three log entries
		for (int i = 0; i < 3; i++)
			getServiceLog(serviceLog[i]);
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.Begin(RECTS);
		GD.Vertex2ii(0, 185);
		GD.Vertex2ii(272, 185);

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
		GD.cmd_text(20, 120, FONT_LIGHT, 0, Settings::getLabel(Label::Cold));
		GD.cmd_text(20, 140, FONT_LIGHT, 0, Settings::getLabel(Label::Ambient));
		GD.cmd_text(20, 160, FONT_LIGHT, 0, Settings::getLabel(Label::Hot));

		GD.cmd_text(120, 120, FONT_SMALL, 0, serviceFlowCold);
		GD.cmd_text(120, 140, FONT_SMALL, 0, serviceFlowAmbi);
		GD.cmd_text(120, 160, FONT_SMALL, 0, serviceFlowHot);

		GD.cmd_text(20, 195, FONT_LIGHT, 0, Settings::getLabel(Label::IceTemp));
		GD.cmd_text(20, 240, FONT_LIGHT, 0, Settings::getLabel(Label::HotTemp));
		GD.cmd_text(20, 285, FONT_LIGHT, 0, Settings::getLabel(Label::ServiceLastChange));
		GD.cmd_text(20, 330, FONT_LIGHT, 0, Settings::getLabel(Label::ErrorLog));

		GD.cmd_text(20, 215, FONT_SMALL, 0, serviceTempIce);
		GD.cmd_text(20, 260, FONT_SMALL, 0, serviceTempHot);
		GD.cmd_text(20, 305, FONT_SMALL, 0, serviceLastChanged);

		// Show each service log entry
		for (int i = 0; i < 3; i++) {
			if (*serviceLog[i].date == '\0')
				break;
			GD.cmd_text(20, 350 + i * 40, FONT_SMALL, 0,
				serviceLog[i].date);
			GD.cmd_text(30, 350 + i * 40 + 20, FONT_SMALL, 0,
				serviceLog[i].description);
		}
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Advanced);
	})
);

void getServiceLog(ServiceLogEntry& e)
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


