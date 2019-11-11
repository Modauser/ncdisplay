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
static void updateToggles(void);

static Screen SystemOptions (
	ScreenID::SystemOptions,
	// Parent screen
	ScreenID::Advanced,
	// Initialization function
	[](void) {
		// Load current values
        // 11/11/19 - Apparently some settings are stored opposite of their
        //            intended value, so !'s were added to those settings when
        //            accessed/modified.
		serialPrintf("@S");
		sysOptionToggles[0] = !serialGet();
		serialPrintf("@U");
		sysOptionToggles[1] = !serialGet();
		serialPrintf("@V");
		sysOptionToggles[2] = !serialGet();
		serialPrintf("@W");
		sysOptionToggles[3] = serialGet();
		serialPrintf("@h");
		sysOptionToggles[4] = serialGet();
		serialPrintf("@X");
		sysOptionMetric = serialGet();
		setMetric(sysOptionMetric);

		updateToggles();
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.ColorRGB(NC_FDGND_COLOR);
		GD.cmd_text(185, 75, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"ICE TANK OVER\nTEMP ERROR:",
			"EIS-TANK\nTEMPERATURFEHLER",
			"ERREUR DE SURCHAUFFE\nDU BAC " A_GRAVE " GLACE",
			"ERROR DE\nSOBRECALENTAMIENTO\nDEL TANQUE DE HIELO"
		})(), 16);
		GD.cmd_text(185, 135, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"DRAIN LEVEL\nFULL ERROR",
			"NIVEAU DER\nAUFFANGVORRICHTUNG\nABSOLUT FALSCH",
			"ERREUR NIVEAU DE\nDRAINAGE PLEIN",
			"ERROR NIVEL DE\nDRENAJE LLENO",
		})(), 16);
		GD.cmd_text(185, 195, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"COOLING FAN\nFAILURE ERROR:",
			"K" U_UMLAUT "HLUNGSVENTILATOR\nVERSAGT",
			"ERREUR VENTILATEUR\nDE REFROIDISSEMENT",
			"ERROR VENTILADOR DE\nENTRIAMIENTO",
		})(), 16);
		GD.cmd_text(185, 255, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"BOOSTER PUMP\nENABLED:",
			"DRUCKERH" O_UMLAUT "HUNGS-\nPUMPE AKTIV",
			"POMPE DE SURPRESSION\nACTIV" E_ACUTE "E",
			"BOMBA ELEVADORA DE\nPRESI" O_ACUTE "N ENCENDIDA",
		})(), 16);
		GD.cmd_text(185, 310, FONT_SMALL, OPT_RIGHTX, LanguageString({
			"SERVICE REMINDER:",
			"SERVICEERINNERUNG",
			"RAPPEL DE SERVICE",
			"SVC REMINDER"
		})(), 16);
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			ScreenManager::setCurrent(ScreenID::Advanced);
	}),
	Button({205, 90}, Button::drawToggle, [](bool press) {
		if (!press)
			toggleOption(0);
	}),
	Button({205, 150}, Button::drawToggle, [](bool press) {
		if (!press)
			toggleOption(1);
	}),
	Button({205, 210}, Button::drawToggle, [](bool press) {
		if (!press)
			toggleOption(2);
	}),
	Button({205, 270}, Button::drawToggle, [](bool press) {
		if (!press)
			toggleOption(3);
	}),
	Button({205, 325}, Button::drawToggle, [](bool press) {
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
			// Save values
			serialPrintf("@Y%1u@Z%1u@A%1u@B%1u@g%1u@C%1u",
				!sysOptionToggles[0], !sysOptionToggles[1],
				!sysOptionToggles[2], sysOptionToggles[3],
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

void updateToggles(void)
{
	for (unsigned int i = 0; i < 5; i++) {
		SystemOptions.getButton(i + 1).setForcePressed(
			sysOptionToggles[i]);
	}
}

