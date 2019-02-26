/**
 * @file Errors.cpp
 * @brief Defines known board errors, and provides code to handle them.
 */
#include "type/Assets.h"
#include "type/LanguageString.h"
#include "Errors.h"
#include "MainBoard.h"

#include <array>
#include <fatfs/ff.h>
#include <gameduino2/GD2.h>

static const LanguageString errorFile ({
	DRV_SD "English0.txt",
	DRV_SD "German00.txt",
	DRV_SD "French00.txt",
	DRV_SD "Spanish0.txt"
});
static const LanguageString systemErrorFile ({
	DRV_SD "EngErr00.txt",
	DRV_SD "GerErr00.txt",
	DRV_SD "FreErr00.txt",
	DRV_SD "SpaErr00.txt"
});

/**
 * Known error codes.
 * Order is important and should not be changed.
 */
static const std::array<ErrorData, 25> ErrorTable = {{
/* LED_ICE_ERROR */	   	ErrorData(0x55, ErrorType::System, true),
/* LED_ICE_COMP_ERROR */	ErrorData(0x56, ErrorType::System, true),
/* LED_HOT_ERROR */	   	ErrorData(0x35, ErrorType::System, false),
/* LED_HOT_TANK_ERROR */	ErrorData(0x36, ErrorType::System, true),
/* LED_ICE_LEVEL_ERROR */	ErrorData(0x75, ErrorType::Red, false),
/* LED_HOT_FILL_ERROR */	ErrorData(0x54, ErrorType::/*System*/None, true),
/* LED_CARB_LVL_ERROR */	ErrorData(0x95, ErrorType::System, true),
/* LED_BOARD_ERROR */		ErrorData(0x7F, ErrorType::System, true),
/* LED_NO_CAL_ERROR */		ErrorData(0x9F, ErrorType::System, true),
/* LED_CARB_FILL_TIME */	ErrorData(0x96, ErrorType::Red, true), // 3?
/* LED_FILT_ERROR */		ErrorData(0x15, ErrorType::Green, false),
/* LED_LEAK_ERROR */		ErrorData(0x31, ErrorType::Green, false),
/* LED_INIT */			ErrorData(0x1F, ErrorType::None, false),
/* LED_HOT_FILL */		ErrorData(0x7B, ErrorType::Red, true), // 3?
/* LED_ICE_FILL */		ErrorData(0x7A, ErrorType::None, false),
/* LED_NORMAL */		ErrorData(0x1A, ErrorType::None, false),
/* LED_DARK_MODE */		ErrorData(0x0A, ErrorType::None, false),
/* LED_DISP_ERROR */		ErrorData(0x5A, ErrorType::Green, false), // DISPense
/* LED_REFRESH */		ErrorData(0xFF, ErrorType::None, false),
/* LED_LOW_FLOW */		ErrorData(0x1B, ErrorType::Red, false),
/* LED_CLEAN_FAN_ERROR */	ErrorData(0x33, ErrorType::Green, false),
/* LED_CARB_FILL */		ErrorData(0x7E, ErrorType::None, false),
/* LED_HOT_LEVEL_ERROR */ 	ErrorData(0x19, ErrorType::Red, false),
/* LED_ICE_LEVEL_ERROR */	ErrorData(0x74, ErrorType::Red, false), // again?
/* LED_NO_ERRORS */ 		ErrorData(0x00, ErrorType::None, false),
}};

char Error::message[128] = "";
ErrorData Error::lastError = ErrorTable.back();
bool Error::dispenseFlag = false;

static unsigned int GetErrorCodeIndex(ErrorCode code)
{
	for (unsigned int i = 0; i < ErrorTable.size(); i++) {
		if (ErrorTable[i].code == code)
			return i;
	}

	return ErrorTable.size() - 1;
}

void Error::loadMessage(unsigned int index)
{
	FIL fil;
	auto result = f_open(&fil, errorFile(), FA_READ);
	if (result != FR_OK)
		return; 

	for (unsigned int i = 0; !f_eof(&fil) && i <= index; i++)
		f_gets(message, sizeof(message) / sizeof(char), &fil);
	
	if (f_eof(&fil))
		message[0] = '\0';

	f_close(&fil);
	LanguageString::convertFileText(message);
}

bool Error::check(void)
{
	serialPrintf("%%?");
	if (auto code = serialGet(); code != lastError.code) {
		auto index = GetErrorCodeIndex(code);
		lastError = ErrorTable[index];
		loadMessage(index);

		if (lastError.code == 0x5A)
			dispenseFlag = true;
	}

	return lastError.code != 0x00;
}

void Error::showStartup(void)
{
	if (!lastError.showOnStartup)
		return;
	show();
}

void Error::show(void)
{
	auto messageBox = [](uint32_t border) {
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

	switch (lastError.type) {
	case ErrorType::Green:
		messageBox(0x00FF00);
		break;
	case ErrorType::Red:
		messageBox(0xFF0000);
		break;
	case ErrorType::System:
		showSystemError();
		break;
	case ErrorType::None:
		break;
	}
}

void Error::showSystemError(void)
{
	GD.ClearColorRGB(NC_BKGND_COLOR);
	GD.Clear();

	GD.Begin(RECTS);
	GD.ColorRGB(0xFF0000);
	GD.Vertex2ii(0, 0);
	GD.Vertex2ii(272, 90);

	GD.ColorRGB(WHITE);
	GD.cmd_text(136, 30, FONT_LARGE, OPT_CENTER, "SYSTEM ERROR");
	GD.cmd_text(136, 60, FONT_MESG, OPT_CENTERX, message);

	FIL fil;
	auto result = f_open(&fil, systemErrorFile(), FA_READ);
	if (result != FR_OK) {
		GD.cmd_text(0, 100, FONT_MESG, 0, "Failed to open error message file");
	} else {
		auto targetLine = GetErrorCodeIndex(lastError.code) * 10;
		for (unsigned int i = 0; !f_eof(&fil) && i < targetLine; i++)
			f_gets(message, sizeof(message) / sizeof(char), &fil);
		
		if (f_eof(&fil)) {
			GD.cmd_text(0, 100, FONT_MESG, 0, "Failed to load error message file");
			GD.swap();
			while (1);
		} else {
			GD.ColorRGB(NC_FDGND_COLOR);
			for (unsigned int i = 0; i < 10; i++) {
				f_gets(message, sizeof(message) / sizeof(char), &fil);
				LanguageString::convertFileText(message);
				GD.cmd_text(10, 100 + i * 20, FONT_SMALL, 0, message);
			}
		}
	}

	GD.cmd_text(20, 380, FONT_SMALL, 0, MainBoard::updateServiceContact(), 20);
	GD.swap();

	f_close(&fil);
	while (1);
}

