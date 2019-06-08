/**
 * @class SleepMode.cpp
 * @brief Screen for configuring SleepMode.
 */
#include "type/Assets.h"
#include "type/Formatters.h"
#include "type/Screen.h"
#include "MainBoard.h"
#include "Settings.h"

#include <cctype>
#include <cstring>
#include <gameduino2/GD2.h>

static bool sleepModeOn = false;
static bool sleepSettingHours = false;
static bool sleepSettingWeekday = false;

static char sleepHourStrings[4][Format::size::hour + 2] = {
	"", "", "", ""
};
static unsigned int sleepHours[4] = {
	25, 25, 25, 25
};

static const LanguageString smWeekdays ({
	"Monday - Friday:",
	"Montag - Freitag:",
	"Lundi - Vendredi:",
	"Lunes - Viernes:"
});
static const LanguageString smWeekend ({
	"Saturday - Sunday:",
	"Samstag - Sonntag:",
	"Samedi - Dimanche:",
	"Sabado - Domingo:"
});

static void sleepSetVisibilities(void);
static void updateSaveButtonText(void);
static void updateSleepHourText(void);

static Screen SleepMode (
	ScreenID::SleepMode,
	// Parent screen
	ScreenID::Settings,
	// Initialization function
	[](void) {
		MainBoard::getSleepmodeHours(sleepHours);
		sleepModeOn = MainBoard::getSleepmodeEnabled();

		sleepSettingHours = false;
		sleepSetVisibilities();

		if (sleepHours[0] == 25)
			sleepHours[1] = 25;
		else if (sleepHours[1] == 25)
			sleepHours[0] = 25;
		if (sleepHours[2] == 25)
			sleepHours[3] = 25;
		else if (sleepHours[3] == 25)
			sleepHours[2] = 25;

		updateSleepHourText();
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.Begin(RECTS);
		GD.Vertex2ii(0, 120);
		GD.Vertex2ii(272, 120);

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(20, 90, FONT_TITLE, OPT_CENTERY, "SleepMode");

		if (!sleepSettingHours) {
			GD.cmd_text(8, 185, FONT_LARGE, 0, LanguageString({
				"OPERATING HOURS",
				"BETRIEBSSTUNDEN",
				"HEURES DE FONCTIONNEMENT",
				"HORAS DE FUNCIONAMIENTO"
			})());
			GD.cmd_text(8, 240, FONT_LIGHT, 0, smWeekdays());
			GD.cmd_text(8, 260, FONT_LIGHT, 0, smWeekend());

			GD.cmd_text(160, 240, FONT_SMALL, 0, sleepHourStrings[0]);
			if (isdigit(sleepHourStrings[0][0])) {
				GD.cmd_text(210, 240, FONT_SMALL, 0, "-");
				GD.cmd_text(220, 240, FONT_SMALL, 0, sleepHourStrings[1]);
			}
			GD.cmd_text(160, 260, FONT_SMALL, 0, sleepHourStrings[2]);
			if (isdigit(sleepHourStrings[2][0])) {
				GD.cmd_text(210, 260, FONT_SMALL, 0, "-");
				GD.cmd_text(220, 260, FONT_SMALL, 0, sleepHourStrings[3]);
			}
		} else {
			GD.cmd_text(136, 150, FONT_TITLE, OPT_CENTER, LanguageString({
				"Set Operating Hours",
				"Set Betriebsstunden",
				"Programmer Heures\nde Fonctionnement",
				"Programar Horas de\nFuncionamiento"
			})());
			GD.cmd_text(136, 290, FONT_SMALL, OPT_CENTER, "TO");

			GD.cmd_text(136, 210, FONT_TITLE, OPT_CENTER,
				sleepSettingWeekday ? smWeekdays() : smWeekend());

			GD.ColorRGB(WHITE);
			GD.Begin(RECTS);
			GD.Vertex2ii(34, 274); GD.Vertex2ii(104, 312);
			GD.Vertex2ii(168, 274); GD.Vertex2ii(238, 312);
			GD.ColorRGB(BLACK);

			int i = sleepSettingWeekday ? 0 : 2;
			GD.cmd_text(69, 293, FONT_SMALL, OPT_CENTER,
				sleepHourStrings[i]);
			GD.cmd_text(203, 293, FONT_SMALL, OPT_CENTER,
				sleepHourStrings[i + 1]);
		}
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press) {
			if (sleepSettingHours) {
				if (!sleepSettingWeekday)
					sleepSettingWeekday = true;
				else
					sleepSettingHours = false;

				sleepSetVisibilities();
			} else {
				ScreenManager::setCurrent(ScreenID::Settings);
			}
		}
	}),
	Button({180, 85}, Button::drawToggle, [](bool press) {
		if (!press) {
			sleepModeOn ^= true;
			sleepSetVisibilities();
			updateSleepHourText();
		}
	}),
	Button({0, 120}, Button::drawMenuItem, {
		"CUSTOMIZE HOURS",
		"ANPASSEN",
		"PERSONNALISER",
		"PERSONALIZAR"
	},
	[](bool press) {
		if (!press) {
			sleepSettingHours = true;
			sleepSettingWeekday = true;

			sleepSetVisibilities();
		}
	}),
	Button({0, 420}, Button::drawFullWidth, lStringSave, [](bool press) {
		if (press)
			return;

		if (sleepSettingHours & sleepSettingWeekday) {
			sleepSettingWeekday = false;
			updateSaveButtonText();
		} else {
			MainBoard::setSleepmodeHours(sleepHours);
			MainBoard::setSleepmodeEnabled(sleepModeOn);
			ScreenManager::setCurrent(ScreenID::Settings);
		}
	}),
	Button({34, 240}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		int index = sleepSettingWeekday ? 0 : 2;
		auto& h = sleepHours[index];
		h++;
		if (h > 25) {
			h = 0;
		} else if (h > 23) {
			h = 25;
			sleepHours[index + 1] = 25;
		}
		updateSleepHourText();
	}),
	Button({34, 312}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		int index = sleepSettingWeekday ? 0 : 2;
		auto& h = sleepHours[index];
		if (h == 24) {
			h = 23;
		} else if (h > 0) {
			h--;
		} else {
			h = 25;
			sleepHours[index + 1] = 25;
		}
		updateSleepHourText();
	}),
	Button({168, 240}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		int index = sleepSettingWeekday ? 1 : 3;
		auto& h = sleepHours[index];
		h++;
		if (h > 25) {
			h = 0;
		} else if (h > 23) {
			h = 25;
			sleepHours[index - 1] = 25;
		}
		updateSleepHourText();
	}),
	Button({168, 312}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		int index = sleepSettingWeekday ? 1 : 3;
		auto& h = sleepHours[index];
		if (h == 24) {
			h = 23;
		} else if (h > 0) {
			h--;
		} else {
			h = 25;
			sleepHours[index - 1] = 25;
		}
		updateSleepHourText();
	})
);

void sleepSetVisibilities(void)
{
	SleepMode.getButton(1).setForcePressed(sleepModeOn);
	SleepMode.getButton(1).setVisibility(!sleepSettingHours);
	SleepMode.getButton(2).setVisibility(sleepModeOn & !sleepSettingHours);
	SleepMode.getButton(4).setVisibility(sleepSettingHours);
	SleepMode.getButton(5).setVisibility(sleepSettingHours);
	SleepMode.getButton(6).setVisibility(sleepSettingHours);
	SleepMode.getButton(7).setVisibility(sleepSettingHours);

	updateSaveButtonText();
}

void updateSaveButtonText(void)
{
	SleepMode.getButton(3).setText((sleepSettingHours & sleepSettingWeekday) ?
		lStringNext : lStringSave);
}

void updateSleepHourText(void)
{
	if (!sleepModeOn) {
		strcpy(sleepHourStrings[0], "ALL");
		strcpy(sleepHourStrings[2], "ALL");
	} else {
		for (int i = 0; i < 4; i++) {
			if (sleepHours[i] == 25)
				strcpy(sleepHourStrings[i], "OFF");
			else
				Format::hour(sleepHourStrings[i], sleepHours[i]);
		}
	}
}

