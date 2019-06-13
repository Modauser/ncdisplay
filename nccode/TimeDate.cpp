/**
 * @file TimeDate.cpp
 * @brief Controls to configure the board's date and time.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

static bool timeSetting = true;
static bool ampm = true;
static int timeDate[] = {
	8, // Hour
	0, // Minute
	0, // AM/PM
	1, // Month
	1, // Day
	0  // Year
};

static const LanguageString tdSetTime ({
	"Set Time",
	"Zeit",
	"R" e_ACUTE "glage de L'Heure",
	"Programar Hora"
});
static const LanguageString tdSetDate ({
	"Set Date",
	"Datum",
	"R" e_ACUTE "glage de la Date",
	"Programar Fecha"
});
static const LanguageString tdMonth ({
	"MM", "MM", "MM", "MM"
});
static const LanguageString tdDay ({
	"DD", "TT", "JJ", "DD"
});
static const LanguageString tdYear ({
	"YYYY", "JJJJ", "AAAA", "AAAA"
});

// Returns days in given month of given year
static int daysInMonth(int month, int year);

static void set24Hour(bool is24);
static void setTimeView(void);
static void setDateView(void);

static Screen TimeDate (
	ScreenID::TimeDate,
	// Parent screen
	ScreenID::Settings,
	// Initialization function
	[](void) {
		timeSetting = true;

		serialPrintf("@T");
		timeDate[0] = serialGet(); // Hour
		timeDate[1] = serialGet(); // Minute
		serialGet(); // Second, unused
		timeDate[2] = serialGet(); // AM/PM
		ampm = timeDate[2] < 2;
	
		serialPrintf("@D");
		timeDate[3] = serialGet(); // Month
		timeDate[4] = serialGet(); // Day
		timeDate[5] = serialGet(); // Year

		setTimeView();
		set24Hour(!ampm);
	},
	// Pre-draw function
	[](void) {
		clearScreenWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 90, FONT_TITLE, OPT_CENTER, timeSetting ?
			tdSetTime() : tdSetDate());

		if (!timeSetting) {
			// Print MM / DD / YYYY text if setting date
			GD.cmd_text(49, 164, FONT_SMALL, OPT_CENTER, tdMonth());
			GD.cmd_text(92, 164, FONT_SMALL, OPT_CENTER, "/");
			GD.cmd_text(135, 164, FONT_SMALL, OPT_CENTER, tdDay());
			GD.cmd_text(178, 164, FONT_SMALL, OPT_CENTER, "/");
			GD.cmd_text(221, 164, FONT_SMALL, OPT_CENTER, tdYear());
		}

		// Draw white boxes between up/down arrows
		unsigned int spot1 = (timeSetting && !ampm) ? 62 : 14;
		unsigned int spot2 = (timeSetting && !ampm) ? 140 : 100;
		GD.ColorRGB(WHITE);
		GD.Begin(RECTS);
		GD.Vertex2ii(spot1, 224); GD.Vertex2ii(spot1 + 70, 258);
		GD.Vertex2ii(spot2, 224); GD.Vertex2ii(spot2 + 70, 258);
		if (!timeSetting || ampm) {
			GD.Vertex2ii(186, 224);
			GD.Vertex2ii(256, 258);
		}

		// Print time/date values
		int idx = timeSetting ? 0 : 3;
		GD.ColorRGB(BLACK);
		GD.cmd_number(spot1 + 35, 244, FONT_SMALL, OPT_CENTER,
			timeDate[idx]);
		GD.cmd_number(spot2 + 35, 244, FONT_SMALL, OPT_CENTER | 2,
			timeDate[idx + 1]);
		if (timeSetting) {
			if (ampm) {
				GD.cmd_text(221, 244, FONT_SMALL, OPT_CENTER,
					(timeDate[idx + 2] > 0) ? "PM" : "AM");
			}
		} else {
			// Century 2000 appended here
			GD.cmd_number(221, 244, FONT_SMALL, OPT_CENTER,
				2000 + timeDate[idx + 2]);
		}
	},
	// Buttons
	Button({0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press) {
			if (!timeSetting)
				setTimeView();
			else
				ScreenManager::setCurrent(ScreenID::Settings);
		}
	}),
	Button({14, 190}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		if (timeSetting) {
			// Increment hour
			++timeDate[0];
			if (ampm) {
				if (timeDate[0] > 12)
					timeDate[0] = 1;
			} else if (timeDate[0] > 23) {
				timeDate[0] = 0;
			}
		} else {
			// Increment month
	       		if (++timeDate[3] > 12)
				timeDate[3] = 1;
			timeDate[4] = std::min(timeDate[4], daysInMonth(
				timeDate[3], timeDate[5]));
		}
	}),
	Button({14, 262}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		if (timeSetting) {
			// Decrement hour
			--timeDate[0];
			if (ampm) {
				if (timeDate[0] < 1)
					timeDate[0] = 12;
			} else if (timeDate[0] < 0) {
				timeDate[0] = 23;
			}
		} else {
			// Decrement month
			if (--timeDate[3] < 1)
				timeDate[3] = 12;
			timeDate[4] = std::min(timeDate[4], daysInMonth(
				timeDate[3], timeDate[5]));
		}
	}),
	Button({100, 190}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		// Increment minute/day
		if (timeSetting && ++timeDate[1] > 59) {
			timeDate[1] = 0;
		} else {
			int days = daysInMonth(timeDate[3], timeDate[5]);
			if (++timeDate[4] > days)
				timeDate[4] = 1;
		}
	}),
	Button({100, 262}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		// Decrement minute/day
		if (timeSetting && --timeDate[1] < 0) {
			timeDate[1] = 59;
		} else {
			if (--timeDate[4] < 1) {
				timeDate[4] = daysInMonth(timeDate[3],
					timeDate[5]);
			}
		}
	}),
	Button({186, 190}, Button::drawUpButton, [](bool press) {
		if (press)
			return;

		// Toggle 24-hour or increment year
		if (timeSetting) {
			timeDate[2] ^= 1;
		} else {
			if (++timeDate[5] > 99)
				timeDate[5] = 0;
			timeDate[4] = std::min(timeDate[4], daysInMonth(
				timeDate[3], timeDate[5]));
		}
	}),
	Button({186, 262}, Button::drawDownButton, [](bool press) {
		if (press)
			return;

		// Toggle 24-hour or decrement year
		if (timeSetting) {
			timeDate[2] ^= 1;
		} else {
			if (--timeDate[5] < 0)
				timeDate[5] = 99;
			timeDate[4] = std::min(timeDate[4], daysInMonth(
				timeDate[3], timeDate[5]));
		}
	}),
	Button({15, 120}, Button::drawExclusiveOption, "AM/PM", [](bool press) {
		if (!press)
			set24Hour(false);
	}),
	Button({136, 120}, Button::drawExclusiveOption, "24 HR", [](bool press) {
		if (!press)
			set24Hour(true);
	}),
	Button({0, 420}, Button::drawFullWidth, lStringNext,
	[](bool press) {
		if (!press) {
			if (timeSetting) {
				setDateView();
			} else {
				// Set time/date on main board
				serialPrintf("@.%02u%02u%02u",
					timeDate[3], timeDate[4], timeDate[5]);
				if (ampm) {
					serialPrintf("@<%02u%02u%01u", timeDate[0],
						timeDate[1], timeDate[2]);
				} else {
					serialPrintf("@-%02u%02u", timeDate[0],
						timeDate[1]);
				}
				ScreenManager::setCurrent(ScreenID::Settings);
			}
		}
	})
);

int daysInMonth(int month, int year)
{
	bool isLeapYear;
	switch (month) {
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
		break;
	case 2:
		isLeapYear = ((year % 4) == 0 && (year % 100) != 0) ||
			((year % 400) == 0);
		return 28 + isLeapYear;
		break;
	default:
		return 31;
		break;
	}
}

void set24Hour(bool is24)
{
	ampm = !is24;
	if (ampm) {
		if (timeDate[0] > 12) {
			timeDate[0] -= 12;
			timeDate[2] = 1;
		}

		if (timeDate[2] > 1)
			timeDate[2] = 0;
	} else {
		if (timeDate[2] != 0 && timeDate[0] < 12)
			timeDate[0] += 12;
	}

	TimeDate.getButton(1).setPosition({ampm ? 14 : 62, 190});
	TimeDate.getButton(2).setPosition({ampm ? 14 : 62, 262});
	TimeDate.getButton(3).setPosition({ampm ? 100 : 140, 190});
	TimeDate.getButton(4).setPosition({ampm ? 100 : 140, 262});
	TimeDate.getButton(5).setVisibility(ampm);
	TimeDate.getButton(6).setVisibility(ampm);
	TimeDate.getButton(7).setForcePressed(ampm);
	TimeDate.getButton(8).setForcePressed(!ampm);
}

void setTimeView(void)
{
	timeSetting = true;
	TimeDate.getButton(7).setVisibility(true);
	TimeDate.getButton(8).setVisibility(true);
	TimeDate.getButton(9).setText(lStringNext);
	set24Hour(!ampm);
}

void setDateView(void)
{
	timeSetting = false;
	TimeDate.getButton(1).setPosition({14, 190});
	TimeDate.getButton(2).setPosition({14, 262});
	TimeDate.getButton(3).setPosition({100, 190});
	TimeDate.getButton(4).setPosition({100, 262});
	TimeDate.getButton(5).setVisibility(true);
	TimeDate.getButton(6).setVisibility(true);
	TimeDate.getButton(7).setVisibility(false);
	TimeDate.getButton(8).setVisibility(false);
	TimeDate.getButton(9).setText(lStringSave);
}

