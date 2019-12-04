/**
 * @file TimeDate.cpp
 * @brief Controls to configure the board's date and time.
 */
#include "type/Assets.h"
#include "type/Screen.h"
#include "MainBoard.h"

#include <gameduino2/GD2.h>

static bool timeSetting = true;
static bool ampm = true; // true: AM/PM, false: 24hr
static int slideDiff = 0;
static int slideIndex = 0;
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
static void handleSlide(void);

static int newTDValue(int index, int change);
static void increment(int index);
static void decrement(int index);

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
        handleSlide();
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
        GD.Tag(254);
		GD.ColorRGB(WHITE);
		GD.Begin(RECTS);
		GD.Vertex2ii(0, 204); GD.Vertex2ii(272, 304);
        GD.ColorRGB(0xBBBBBB);
		GD.Vertex2ii(0, 245); GD.Vertex2ii(272, 263);

		// Print time/date values
		GD.ColorRGB(BLACK);
        GD.ScissorSize(70, 100);
        int end = timeSetting ? (ampm ? 3 : 2) : 6;
        int x = spot1;
        for (int i = timeSetting ? 0 : 3; i < end; i++) {
            int y = slideIndex != i ? 204 : 204 + slideDiff;
            GD.ScissorXY(x, 204);
            for (int j = -2; j < 3; j++) {
                int v = newTDValue(i, j);
                if (i == 2) {
                    GD.cmd_text(x + 35, y, FONT_SMALL, OPT_CENTERX,
                                (v & 1) ? "PM" : "AM");
                } else {
                    int o = OPT_CENTERX;
                    if (i == 5)
                        v += 2000;
                    else
                        o |= 2;
		            GD.cmd_number(x + 35, y, FONT_SMALL, o, v);
                }
                y += 20;
            }

            x += 84;
        }
        GD.Tag(0);
        GD.ScissorXY(0, 0);
        GD.ScissorSize(272, 480);
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

	TimeDate.getButton(1).setForcePressed(ampm);
	TimeDate.getButton(2).setForcePressed(!ampm);
}

void setTimeView(void)
{
	timeSetting = true;
	TimeDate.getButton(1).setVisibility(true);
	TimeDate.getButton(2).setVisibility(true);
	TimeDate.getButton(3).setText(lStringNext);
	set24Hour(!ampm);
}

void setDateView(void)
{
	timeSetting = false;
	TimeDate.getButton(1).setVisibility(false);
	TimeDate.getButton(2).setVisibility(false);
	TimeDate.getButton(3).setText(lStringSave);
}

void handleSlide(void)
{
    static int startY = -1;

    if (GD.inputs.touching) {
        if (startY == -1 && GD.inputs.y >= 204 && GD.inputs.y <= 304) {
		    int spot1 = (timeSetting && !ampm) ? 62 : 14;
		    int spot2 = (timeSetting && !ampm) ? 140 : 100;
            if (GD.inputs.x > spot1 && GD.inputs.x < spot1 + 70)
                slideIndex = timeSetting ? 0 : 3;
            else if (GD.inputs.x > spot2 && GD.inputs.x < spot2 + 70)
                slideIndex = timeSetting ? 1 : 4;
            else if ((!timeSetting || ampm) && GD.inputs.x > 186 && GD.inputs.x < 256)
                slideIndex = timeSetting ? 2 : 5;
            else 
                slideIndex = -1;

            startY = GD.inputs.y;
        } else if (slideIndex != -1) {
            slideDiff = GD.inputs.y - startY;
            if (slideDiff > 20) {
                decrement(slideIndex);
                startY = GD.inputs.y;
                slideDiff = 0;
            } else if (slideDiff < -20) {
                increment(slideIndex);
                startY = GD.inputs.y;
                slideDiff = 0;
            }
        }
    } else if (startY != -1) {
        if (slideDiff > 10)
            decrement(slideIndex);
        else if (slideDiff < -10)
            increment(slideIndex);
        startY = -1;
        slideIndex = -1;
        slideDiff = 0;
    }
}

int newTDValue(int index, int change)
{
    if (index < 0 || index > 5)
        return 0;
    else if (change == 0)
        return timeDate[index];

    int ret = timeDate[index] + change;
    if (change > 0) {
        switch (index) {
        case 0:
	        if (ampm) {
	        	if (ret > 12)
	        		ret -= 12;
	        } else if (ret > 23) {
	        	ret -= 24;
	        }
            break;
        case 1:
            if (ret > 59)
                ret -= 60;
            break;
        case 2:
            ret &= 1;
            break;
        case 3:
           	if (ret > 12)
	       	    ret -= 12;
            break;
        case 4:
	        if (int d = daysInMonth(timeDate[3], timeDate[5]); ret > d)
	        	ret -= d;
            break;
        case 5:
	        if (ret > 99)
	       		ret -= 100;
            break;
        }
    } else {
        switch (index) {
        case 0:
	        if (ampm) {
	        	if (ret < 1)
	        		ret += 12;
	        } else if (ret < 0) {
	        	ret += 24;
	        }
            break;
        case 1:
            if (ret < 0)
                ret += 60;
            break;
        case 2:
            ret &= 1;
            break;
        case 3:
           	if (ret < 1)
	       	    ret += 12;
            break;
        case 4:
	        if (ret < 1)
                ret += daysInMonth(timeDate[3], timeDate[5]);
            break;
        case 5:
	        if (ret < 0)
	       		ret += 100;
            break;
        }
    }

    return ret;
}

void increment(int index)
{
    if (index < 0 || index > 5)
        return;

    timeDate[index] = newTDValue(index, 1);
    if (index == 3 || index == 5) {
	    timeDate[4] = std::min(timeDate[4],
                               daysInMonth(timeDate[3], timeDate[5]));
    }
}

void decrement(int index)
{
    if (index < 0 || index > 5)
        return;

    timeDate[index] = newTDValue(index, -1);
    if (index == 3 || index == 5) {
	    timeDate[4] = std::min(timeDate[4],
                               daysInMonth(timeDate[3], timeDate[5]));
    }
}

