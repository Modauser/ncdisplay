/**
 * @file SystemOptions.cpp
 * @brief Screen for configuring system options.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

static bool sysOptionMetric = false;
static unsigned char sysOptionToggles[5] = {
	0, 0, 0, 0, 0
};

static void setMetric(bool metric);
static void toggleOption(unsigned int index);

static Screen SystemOptions (
	ScreenID::SystemOptions,
	// Parent screen
	ScreenID::Advanced,
	// Initialization function
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
		setMetric(sysOptionMetric);
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.ColorRGB(NC_FDGND_COLOR);
		GD.cmd_text(185, 75, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"ICE TANK OVER",
			"EIS-TANK",
			"ERREUR DE SURCHAUFFE",
			"ERROR DE SOBRECALENTAMIENTO"
		})());
		GD.cmd_text(185, 95, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"TEMP ERROR:",
			"TEMPERATURFEHLER",
			"DU BAC " A_GRAVE " GLACE",
			"DEL TANQUE DE HIELO"
		})());
		GD.cmd_text(185, 135, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"DRAIN LEVEL",
			"NIVEAU DER AUFFANGVORRICHTUNG",
			"ERREUR NIVEAU DE",
			"ERROR NIVEL DE",
		})());
		GD.cmd_text(185, 155, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"FULL ERROR:",
			"ABSOLUT FALSCH",
			"DRAINAGE PLEIN",
			"DRENAJE LLENO"
		})());
		GD.cmd_text(185, 195, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"COOLING FAN",
			"K" U_UMLAUT "HLUNGSVENTILATOR",
			"ERREUR VENTILATEUR",
			"ERROR VENTILADOR",
		})());
		GD.cmd_text(185, 215, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"FAILURE ERROR:",
			"VERSAGT",
			"DE REFROIDISSEMENT",
			"DE ENFRIAMIENTO"
		})());
		GD.cmd_text(185, 255, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"BOOSTER PUMP",
			"DRUCKERH" O_UMLAUT "HUNGS-",
			"POMPE DE SURPRESSION",
			"BOMBA ELEVADORA DE",
		})());
		GD.cmd_text(185, 275, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"ENABLED:",
			"PUMPE AKTIV",
			"ACTIV" E_ACUTE "E",
			"PRESI" O_ACUTE "N ENCENDIDA"
		})());
		GD.cmd_text(185, 310, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"SERVICE REMINDER:",
			"SERVICEERINNERUNG",
			"RAPPEL DE SERVICE",
			"SVC REMINDER"
		})());
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Advanced);
	}),
	Button({215, 80}, Button::drawToggle, [](bool press) {
		if (!press)
			toggleOption(0);
	}),
	Button({215, 140}, Button::drawToggle, [](bool press) {
		if (!press)
			toggleOption(1);
	}),
	Button({215, 200}, Button::drawToggle, [](bool press) {
		if (!press)
			toggleOption(2);
	}),
	Button({215, 260}, Button::drawToggle, [](bool press) {
		if (!press)
			toggleOption(3);
	}),
	Button({215, 315}, Button::drawToggle, [](bool press) {
		if (!press)
			toggleOption(4);
	}),
	Button({14, 360}, Button::drawExclusiveOption, {
		"US Standard",
		"US-Standard",
		"Standard USA",
		"Standard USA"
	}, [](bool press) {
		if (!press)
			setMetric(false);
	}),
	Button({134, 360}, Button::drawExclusiveOption, {
		"Metric",
		"Metrik",
		"M" e_ACUTE "trique",
		"M" e_ACUTE "trico"
	}, [](bool press) {
		if (!press)
			setMetric(true);
	}),
	Button({0, 420}, Button::drawFullWidth, lStringSave, [](bool press) {
		if (!press) {
			serialPrintf("@Y%1u@Z%1u@A%1u@B%1u@g%1u@C%1u",
				sysOptionToggles[0], sysOptionToggles[1],
				sysOptionToggles[2], sysOptionToggles[3],
				sysOptionToggles[4], sysOptionMetric);
			ScreenManager::setCurrent(ScreenID::Advanced);
		}
	})
);

void setMetric(bool metric)
{
	sysOptionMetric = metric;
	SystemOptions.getButton(6).setForcePressed(
		!sysOptionMetric);
	SystemOptions.getButton(7).setForcePressed(
		sysOptionMetric);
}

void toggleOption(unsigned int index)
{
	sysOptionToggles[index] ^= 1;
	SystemOptions.getButton(index + 1).setForcePressed(
		sysOptionToggles[index]);
}

