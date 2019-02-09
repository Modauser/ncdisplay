#include "Errors.h"
#include "Assets.h"

#include <algorithm>
#include <array>
#include <fatfs/ff.h>
#include <gameduino2/GD2.h>

/**
 * Known error codes.
 */
static std::array<Error, 24> ErrorTable = {{
/* LED_NO_ERRORS */ 		{0x00, ErrorType::None, false},

/* LED_ICE_LEVEL_ERROR */	{0x75, ErrorType::Red, false},
/* LED_ICE_FILL */		{0x7A, ErrorType::None, false},

/* LED_HOT_LEVEL_ERROR */ 	{0x19, ErrorType::Red, false},
/* LED_HOT_FILL */		{0x7B, ErrorType::Red, true}, // 3?

/* LED_CARB_FILL */		{0x7E, ErrorType::None, false},
/* LED_CARB_FILL_TIME */	{0x7E, ErrorType::Red, true}, // 3?

/* LED_FILT_ERROR */		{0x15, ErrorType::Green, false},
/* LED_LEAK_ERROR */		{0x31, ErrorType::Green, false},

/* LED_INIT */			{0x1F, ErrorType::None, false},
/* LED_NORMAL */		{0x1A, ErrorType::None, false},
/* LED_DARK_MODE */		{0x0A, ErrorType::None, false},
/* LED_DISP_ERROR */		{0x5A, ErrorType::Green, false}, // DISPense
/* LED_REFRESH */		{0xFF, ErrorType::None, false},
/* LED_LOW_FLOW */		{0x1B, ErrorType::Red, false},
/* LED_CLEAN_FAN_ERROR */	{0x33, ErrorType::Green, false},

/* LED_BOARD_ERROR */		{0x7F, ErrorType::System, true},
/* LED_NO_CAL_ERROR */		{0x9F, ErrorType::System, true},
/* LED_CARB_LVL_ERROR */	{0x95, ErrorType::System, true},
/* LED_ICE_ERROR */	   	{0x55, ErrorType::System, true},
/* LED_ICE_COMP_ERROR */	{0x56, ErrorType::System, true},
/* LED_HOT_ERROR */	   	{0x35, ErrorType::System, false},
/* LED_HOT_TANK_ERROR */	{0x36, ErrorType::System, true},
/* LED_HOT_FILL_ERROR */	{0x54, ErrorType::/*System*/None, true}
}};

void Error::show(void) const
{
	auto messageBox = [this](uint32_t border) {
		// Draw message box
		GD.ColorRGB(border);
		GD.Begin(RECTS);
		GD.Vertex2ii(20, 210);
		GD.Vertex2ii(250, 266);
		GD.ColorRGB(WHITE);
		GD.Vertex2ii(22, 212);
		GD.Vertex2ii(248, 264);

		// Put error message
		GD.ColorRGB(BLACK);
		GD.cmd_text(135, 238, FONT_MESG, OPT_CENTER, message);
	};

	switch (type) {
	case ErrorType::None:
		break;
	case ErrorType::Green:
		messageBox(0x00FF00);
		break;
	case ErrorType::Red:
		messageBox(0xFF0000);
		break;
	case ErrorType::System:
		break;
	}
}

const Error& Error::get(unsigned char code)
{
	for (auto& e : ErrorTable) {
		if (e == code) {
			e.loadErrorText();
			return e;
		}
	}

	return ErrorTable[0];
}

//This opens a text file with lines terminated by a LF character
//The file must end with a : FOLLOWED BY A LF !!!!!!!!!!!!!!!!!
//Reads each line into a string ErrorString, uses string pointer ErrorStringPointer
//and selects the line that matches ErrorCode
//0 is the first line

static const std::array<char, 16> hexDigits = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F'
};

int getHexByte(const char *s)
{
	if (s[0] != '0' || s[1] != 'x')
		return 0;

	return (std::distance(hexDigits.begin(), std::find(
			hexDigits.begin(), hexDigits.end(), s[2])) << 4)
		| std::distance(hexDigits.begin(), std::find(
			hexDigits.begin(), hexDigits.end(), s[3]));
}

static const LanguageString errorFile ({
	DRV_SD "English.txt",
	DRV_SD "German.txt",
	DRV_SD "French.txt",
	DRV_SD "Spanish.txt"
});

void Error::loadErrorText(void) 
{
	if (message[0] != '\0')
		return;

	FIL fil;
	auto result = f_open(&fil, errorFile(), FA_READ);
	if (result != FR_OK)
		return; 
	
	do {
		f_gets(message, 64, &fil);
	} while (getHexByte(message) != code && !f_eof(&fil));

	if (f_eof(&fil))
		message[0] = '\0';

	f_close(&fil);
}
