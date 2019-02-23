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
 * Enables serial communication with the main board.
 * WARNING: Display will currently hang if this is enabled and the main board
 * is disconnected!
 */
//#define USE_SERIAL

/**
 * Defines how long to attempt handshaking with the main board, in 10ms
 * increments.
 */
constexpr unsigned int HANDSHAKE_TIMEOUT = 1000;

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

/**
 * Loads an image, binding it to the given handle.
 * The image is loaded to the next known available address.
 * @param handle The bitmap handle to bind to
 * @param path The path of the image to load
 */
void loadImage(int handle, const char *path);

/**
 * Defines from gd3asset
 * gd3asset command: gd3asset.exe -f assets.gd3 -o assets.h
 * 	sleep1.jpg ani1.jpg ani2.jpg ani3.jpg HomeWtr.jpg IonLogo.jpg
 * 	restart.png,format=ARGB2 fltstrip.png,format=L8
 * 	dropstrp.png,format=ARGB2 setstrip.png
 */

constexpr uint32_t ASSETS_END = 611844;

// TODO startup; use sleep space
constexpr unsigned int SLEEP_HANDLE = 0;
constexpr unsigned int ANI1_HANDLE = 1;
constexpr unsigned int ANI2_HANDLE = 2;
constexpr unsigned int ANI3_HANDLE = 3;
constexpr unsigned int HOMEWTR_HANDLE = 4;
constexpr unsigned int IONLOGO_HANDLE = 5;
constexpr unsigned int RESTART_HANDLE = 6;
constexpr unsigned int FLTSTRIP_HANDLE = 7; // TODO in progress
constexpr unsigned int DROPSTRP_HANDLE = 8; // TODO
constexpr unsigned int SETSTRIP_HANDLE = 9; 
constexpr unsigned int FREEFONT_HANDLE = 10;

/* Font used for smaller general text */
constexpr unsigned int FONT_SMALL = FREEFONT_HANDLE;
/* Font to be used for larger general text */
constexpr unsigned int FONT_LARGE = FREEFONT_HANDLE + 1;
/* A small font, lighter/thinner than Nirmala */
constexpr unsigned int FONT_LIGHT = FREEFONT_HANDLE + 2;
/* A large, lighter/thinner font for headers (e.g. "Advanced, SleepMode") */
constexpr unsigned int FONT_TITLE = FREEFONT_HANDLE + 3;
/* The font to be used for time/date on the dispense screen */
constexpr unsigned int FONT_TIME = FONT_LIGHT;
/* Font to be used in the message box, for things like errors */
constexpr unsigned int FONT_MESG = 18;

void clearScreenWithIonHeader(bool line = true);

#endif // ASSETS_H_
