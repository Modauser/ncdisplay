#include <gameduino2/GD2.h>

#include "Assets.h"
#include "MainBoard.h"
#include "Screens.h"

static unsigned char sysOptionToggles[5] = {
	0, 0, 0, 0, 0
};
static bool sysOptionMetric = false;

static Button buttonsSystemOptions[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			screenCurrent = &screenAdvanced;
	}),
	Button(2, {215, 80}, Button::drawToggle, [](bool press) {
		if (!press) {
			sysOptionToggles[0] ^= 1;
			buttonsSystemOptions[1].setForcePressed(
				sysOptionToggles[0]);
		}
	}),
	Button(3, {215, 140}, Button::drawToggle, [](bool press) {
		if (!press) {
			sysOptionToggles[1] ^= 1;
			buttonsSystemOptions[2].setForcePressed(
				sysOptionToggles[1]);
		}
	}),
	Button(4, {215, 200}, Button::drawToggle, [](bool press) {
		if (!press) {
			sysOptionToggles[2] ^= 1;
			buttonsSystemOptions[3].setForcePressed(
				sysOptionToggles[2]);
		}
	}),
	Button(5, {215, 260}, Button::drawToggle, [](bool press) {
		if (!press) {
			sysOptionToggles[3] ^= 1;
			buttonsSystemOptions[4].setForcePressed(
				sysOptionToggles[3]);
		}
	}),
	Button(6, {215, 315}, Button::drawToggle, [](bool press) {
		if (!press) {
			sysOptionToggles[4] ^= 1;
			buttonsSystemOptions[5].setForcePressed(
				sysOptionToggles[4]);
		}
	}),
	Button(7, {14, 360}, Button::drawExclusiveOption, {
		"US Standard",
		"US-Standard",
		"Standard USA",
		"Standard USA"
	}, [](bool press) {
		if (!press) {
			sysOptionMetric = false;
			buttonsSystemOptions[6].setForcePressed(
				!sysOptionMetric);
			buttonsSystemOptions[7].setForcePressed(
				sysOptionMetric);
		}
	}),
	Button(8, {134, 360}, Button::drawExclusiveOption, {
		"Metric",
		"Metrik",
		"M" e_ACUTE "trique",
		"M" e_ACUTE "trico"
	}, [](bool press) {
		if (!press) {
			sysOptionMetric = true;
			buttonsSystemOptions[6].setForcePressed(
				!sysOptionMetric);
			buttonsSystemOptions[7].setForcePressed(
				sysOptionMetric);
		}
	}),
	Button(9, {0, 420}, Button::drawFullWidth, lStringSave, [](bool press) {
		if (!press) {
#ifdef USE_SERIAL
			serialPrintf("@Y%1u@Z%1u@A%1u@B%1u@g%1u@C%1u",
				sysOptionToggles[0], sysOptionToggles[1],
				sysOptionToggles[2], sysOptionToggles[3],
				sysOptionToggles[4], sysOptionMetric);
#endif // USE_SERIAL
			screenCurrent = &screenAdvanced;
		}
	})
};

Screen screenSystemOptions (
	// Parent screen
	&screenAdvanced,
	// Buttons
	buttonsSystemOptions, 9,
	// Initialization function
#ifdef USE_SERIAL
	[](void)  {
		serialPrintf("@S");
		sysOptionToggles[0] = serialGet();
		serialPrintf("@U");
		sysOptionToggles[1] = serialGet();
		serialPrintf("@V");
		sysOptionToggles[2] = serialGet();
		serialPrintf("@W");
		sysOptionToggles[3] = serialGet();
		serialPrintf("@h");
		sysOptionToggles[4] = serialGet();
		serialPrintf("@X");
		sysOptionMetric = serialGet();
	},
#else
	nullptr,
#endif // USE_SERIAL
	// Pre-draw function
	[](void) {
		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FDGND_COLOR);
		GD.cmd_text(30, 75, FONT_SMALL, 0, LanguageString({
			"ICE TANK OVER",
			"EIS-TANK",
			"ERREUR DE SURCHAUFFE",
			"ERROR DE SOBRECALENTAMIENTO"
		})());
		GD.cmd_text(30, 95, FONT_SMALL, 0, LanguageString({
			"TEMP ERROR:",
			"TEMPERATURFEHLER",
			"DU BAC " A_GRAVE " GLACE",
			"DEL TANQUE DE HIELO"
		})());
		GD.cmd_text(30, 135, FONT_SMALL, 0, LanguageString({
			"DRAIN LEVEL",
			"NIVEAU DER AUFFANGVORRICHTUNG",
			"ERREUR NIVEAU DE",
			"ERROR NIVEL DE",
		})());
		GD.cmd_text(30, 155, FONT_SMALL, 0, LanguageString({
			"FULL ERROR:",
			"ABSOLUT FALSCH",
			"DRAINAGE PLEIN",
			"DRENAJE LLENO"
		})());
		GD.cmd_text(30, 195, FONT_SMALL, 0, LanguageString({
			"COOLING FAN",
			"K" U_UMLAUT "HLUNGSVENTILATOR",
			"ERREUR VENTILATEUR",
			"ERROR VENTILADOR",
		})());
		GD.cmd_text(30, 215, FONT_SMALL, 0, LanguageString({
			"FAILURE ERROR:",
			"VERSAGT",
			"DE REFROIDISSEMENT",
			"DE ENFRIAMIENTO"
		})());
		GD.cmd_text(30, 255, FONT_SMALL, 0, LanguageString({
			"BOOSTER PUMP",
			"DRUCKERH" O_UMLAUT "HUNGS-",
			"POMPE DE SURPRESSION",
			"BOMBA ELEVADORA DE",
		})());
		GD.cmd_text(30, 275, FONT_SMALL, 0, LanguageString({
			"ENABLED:",
			"PUMPE AKTIV",
			"ACTIV" E_ACUTE "E",
			"PRESI" O_ACUTE "N ENCENDIDA"
		})());
		GD.cmd_text(30, 310, FONT_SMALL, 0, LanguageString({
			"SERVICE REMINDER:",
			"SERVICEERINNERUNG",
			"RAPPEL DE SERVICE",
			"SVC REMINDER"
		})());
	}
);

