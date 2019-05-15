/**
 * @file Assets.h
 * @brief Miscellaneous constants, functions for loading visual assets, and
 * other miscellaneous things.
 */
#ifndef ASSETS_H_
#define ASSETS_H_

#include <cstdint>

/**
 * Function attribute to prevent optimization in critical areas.
 * (see SystemCalls.h)
 */
#define NOOPTIMIZE __attribute__((optimize(0)))

/**
 * Defines how many milliseconds to wait before entering sleep mode.
 */
constexpr unsigned int SLEEP_TIMEOUT = 30 * 1000;

/**
 * Defines how many milliseconds to wait between sleep mode images.
 */
constexpr unsigned int SLEEP_IMGSWITCH = 10 * 1000;

// Common colors
constexpr uint32_t BLACK          = 0x000000;
constexpr uint32_t WHITE          = 0xFFFFFF;
constexpr uint32_t NC_BKGND_COLOR = 0xD6ECF9;
constexpr uint32_t NC_FRGND_COLOR = 0x0268BD;
constexpr uint32_t NC_FDGND_COLOR = 0x002860;
constexpr uint32_t NC_ONTOG_COLOR = 0x5EF6B8;
constexpr uint32_t NC_OFTOG_COLOR = 0xBBBBBB;
constexpr uint32_t NC_GREEN_COLOR = NC_ONTOG_COLOR;

/**
 * Does basic initialization of the display.
 */
void displayInit(void);

/**
 * Loads all assets (images and fonts) from the SD card.
 */
void loadAssets(void);

extern uint32_t FREE_MEM;

/**
 * Loads an image to the specified address, binding it to the given handle.
 * The address defaults to free memory immediately after loaded images/fonts.
 * @param handle The bitmap handle to bind to
 * @param path The path of the image to load
 * @param addr The graphic memory address to load the image to
 */
void loadImage(int handle, const char *path, uint32_t addr = FREE_MEM);

/**
 * Defines from gd3asset
 * gd3asset command: gd3asset.exe -f assets.gd3 -o assets.h
 * 	sleep1.jpg ani1.jpg ani2.jpg ani3.jpg HomeWtr.jpg IonLogo.jpg
 * 	restart.png,format=ARGB2 fltstrip.png,format=L8
 * 	dropstrp.png,format=ARGB2 setstrip.png
 */

constexpr uint32_t ASSETS_END = 707044;

// TODO startup; use sleep space
enum BitmapHandles {
	SLEEP_HANDLE = 0,
	STARTUP_HANDLE,
	ANI1_HANDLE,
	ANI2_HANDLE,
	ANI3_HANDLE,
	HOMEWTR_HANDLE,
	IONLOGO_HANDLE,
	RESTART_HANDLE,
	FLTSTRIP_HANDLE, // TODO in progress
	DROPSTRP_HANDLE, // TODO
	SETSTRIP_HANDLE, 

	FONT_SMALL, // Font used for smaller general text
	FONT_LARGE, // Font to be used for larger general text
	FONT_LIGHT, // A small font, lighter/thinner than Nirmala
	FONT_TITLE, // A large, lighter/thinner font for headers (e.g. "Advanced, SleepMode")

	FREE_HANDLE
	
	// No more handles may be assigned (maximum of 16)
};

/* The font to be used for time/date on the dispense screen */
constexpr unsigned int FONT_TIME = FONT_SMALL;
/* Font to be used in the message box, for things like errors */
constexpr unsigned int FONT_MESG = FONT_SMALL; //18;

void clearScreenWithIonHeader(bool line = true);

#endif // ASSETS_H_
