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
 * @see SystemCalls.h
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

/**
 * Color: Black.
 */
constexpr uint32_t BLACK          = 0x000000;
/**
 * Color: White.
 */
constexpr uint32_t WHITE          = 0xFFFFFF;
/**
 * Color: Red tint (e.g. Warning screen).
 */
constexpr uint32_t TINT_RED       = 0xDE0D14;
/**
 * Color: Green tint (e.g. Sparkling fill screen).
 */
constexpr uint32_t TINT_GREEN     = 0x12A834;
/**
 * Color: Blue tint (e.g. Restart screen).
 */
constexpr uint32_t TINT_BLUE      = 0x024FA6;
/**
 * Color: Common background color.
 */
constexpr uint32_t NC_BKGND_COLOR = 0xD6ECF9;
/**
 * Color: Common foreground color.
 */
constexpr uint32_t NC_FRGND_COLOR = 0x026FB6;
/**
 * Color: Common darker foreground color.
 */
constexpr uint32_t NC_FDGND_COLOR = 0x0C3D6B;
/**
 * Color: 'On' color for toggles (light green).
 */
constexpr uint32_t NC_ONTOG_COLOR = 0x5EF6B8;
/**
 * Color: 'Off' color for toggles (light gray).
 */
constexpr uint32_t NC_OFTOG_COLOR = 0xBBBBBB;
/**
 * Color: Common green color.
 */
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
 * Loads an image to the specified address, binding it to the given handle.
 *
 * The address defaults to zero, where sleep images are usually loaded.
 * @param handle The bitmap handle to bind to
 * @param path The path of the image to load
 * @param addr The graphic memory address to load the image to
 */
void loadImage(int handle, const char *path, uint32_t addr = 0);

//
// Defines from gd3asset
// gd3asset arguments:
// 	sleep1.jpg ani1.jpg ani2.jpg ani3.jpg HomeWtr.png IonLogo.jpg
// 	restart.png,format=ARGB2 fltstrip.png,format=L8
// 	dropstrp.png,format=ARGB2 setstrip.png
//

/**
 * Defines where assets end in video memory. Assets start at address zero.
 */
constexpr uint32_t ASSETS_END = 629252UL;

/**
 * @enum BitmapHandles
 * @brief A list of bitmaps from the assets file.
 */
enum BitmapHandles {
	SLEEP_HANDLE = 0, /**< Sleep screen images loaded here */
	ANI1_HANDLE,      /**< Flow animation image 1 */
	ANI2_HANDLE,      /**< Flow animation image 2 */
	ANI3_HANDLE,      /**< Flow animation image 3 */
	HOMEWTR_HANDLE,   /**< The water splash image */
	IONLOGO_HANDLE,   /**< The small ION logo */
	RESTART_HANDLE,   /**< Two arrows making a circle */
	FLTSTRIP_HANDLE,  /**< Two images showing filter replacement. */
	DROPSTRP_HANDLE,  /**< Strip of drop images (used in fill screens) */
	SETSTRIP_HANDLE,  /**< Strip of settings menu icons */

	FONT_SMALL,       /**< Font used for smaller general text */
	FONT_LARGE,       /**< Font used for larger general text */
	FONT_LIGHT,       /**< A smaller font, lighter/thinner than Nirmala */
	FONT_TITLE,       /**< A larger font, lighter/thinner than Nirmala */

	FREE_HANDLE       /**< To be used for images loaded at runtime */
};

/**
 * The font to be used for time/date on the dispense screen.
 */
constexpr unsigned int FONT_TIME = FONT_SMALL;
/**
 * Font to be used in the message box, for things like errors
 */
constexpr unsigned int FONT_MESG = FONT_SMALL;

/**
 * Clears the screen and puts the ION logo in the top-right corner.
 * @param line True if white line should be drawn across screen, under the logo.
 */
void clearScreenWithIonHeader(bool line = true);

#endif // ASSETS_H_
