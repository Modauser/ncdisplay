/**
 * @file Assets.h
 * @brief Provides items common between most screens
 */
#ifndef ASSETS_H_
#define ASSETS_H_

#include "Formatters.h"
#include "LanguageString.h"


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
#define HANDSHAKE_TIMEOUT 1000

/**
 * Defines how many milliseconds to wait before entering sleep mode.
 */
#define SLEEP_TIMEOUT   (30 * 1000)

/**
 * Defines how many milliseconds to wait between sleep mode images.
 */
#define SLEEP_IMGSWITCH (10 * 1000)

/**
 * Gallon to liter ratio.
 * Not sure about using floating point numbers.
 * 1 gal = 3.7854 L
 */
#define GAL_TO_L (4)


// Common colors
#define BLACK          0x000000
#define WHITE          0xFFFFFF
#define NC_BKGND_COLOR 0xD7ECF9
#define NC_FRGND_COLOR 0x0268BD
#define NC_FDGND_COLOR 0x002860
//#define NC_BKGND_COLOR 0xD8EFFC
//#define NC_BKGND_COLOR 0xD7ECF9
#define NC_ONTOG_COLOR 0x5EF6B8
#define NC_OFTOG_COLOR 0xBBBBBB
#define NC_GREEN_COLOR NC_ONTOG_COLOR

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

#define ASSETS_END 679962UL

// TODO startup; use sleep space
#define SLEEP_HANDLE 0
#define ANI1_HANDLE 1
#define ANI2_HANDLE 2
#define ANI3_HANDLE 3
#define HOMEWTR_HANDLE 4
#define IONLOGO_HANDLE 5
#define RESTART_HANDLE 6  // TODO
#define FLTSTRIP_HANDLE 7 // TODO in progress
#define DROPSTRP_HANDLE 8 // TODO
#define SETSTRIP_HANDLE 9
#define FREEFONT_HANDLE 10

/* Font used for smaller general text */
#define FONT_SMALL (FREEFONT_HANDLE)
/* Font to be used for larger general text */
#define FONT_LARGE (FREEFONT_HANDLE + 1)
/* A small font, lighter/thinner than Nirmala */
#define FONT_LIGHT (FREEFONT_HANDLE + 2)
/* A large, lighter/thinner font for headers (e.g. "Advanced, SleepMode") */
#define FONT_TITLE (FREEFONT_HANDLE + 3)
/* The font to be used for time/date on the dispense screen */
#define FONT_TIME  FONT_LIGHT
/* Font to be used in the message box, for things like errors */
#define FONT_MESG  18

/**
 * A two-dimensional vector, useful for storing xy pairs, or width/height pairs.
 */
struct vec2 {
	int x, y;
	constexpr vec2(int _x = 0, int _y = 0)
		: x(_x), y(_y) {}
};

#define PRESS_FORCED     (1 << 1)

/**
 * Creates a graphical button that can trigger custom actions.
 * Supports bitmaps or a custom function for rendering (with or without text).
 * Can trigger actions on presses or releases.
 */
class Button {
private:
	int tag;
	vec2 xy;

	int handle;
	LanguageString text;
	void *render; // Custom render function

	bool visible;
	bool pressed;
	bool forcePressed;

	// Points to custom action code
	// bool argument is true on presses, false on releases
	void (*action)(bool);

public:
	/**
	 * Creates a button that renders the given bitmap handle.
	 */
	constexpr Button(int _tag, vec2 _xy, int _handle, void (*_action)(bool)
		= nullptr)
		: tag(_tag),
		  xy(_xy),
		  handle(_handle),
		  render(nullptr),
		  visible(true),
		  pressed(false),
		  forcePressed(false),
		  action(_action) {}

	/**
	 * Creates a button with a custom render function that includes the
	 * given text.
	 */
	constexpr Button(int _tag, vec2 _xy, void (*_render)(const vec2&, bool,
		const LanguageString&), const LanguageString& _text,
		void (*_action)(bool) =	nullptr)
		: tag(_tag),
		  xy(_xy),
		  handle(-1),
		  text(_text),
		  render((void *)_render),
		  visible(true),
		  pressed(false),
		  forcePressed(false),
		  action(_action) {}

	/**
	 * Creates a button with a custom render function.
	 */
	constexpr Button(int _tag, vec2 _xy, void (*_render)(const vec2&, bool),
		void (*_action)(bool) = nullptr)
		: tag(_tag),
		  xy(_xy),
		  handle(-1),
		  render((void *)_render),
		  visible(true),
		  pressed(false),
		  forcePressed(false),
		  action(_action) {}

	// Renders the button
	void draw(void);
	// Runs the button's action code
	// (that gets passed the button's current press state)
	void doAction(void);

	// Gets the current press state of the button
	bool getPressed(void) const;
	// Gets the button's tag
	int getTag(void) const;

	// Forces pressed state for things like switches/toggles
	void setForcePressed(bool p);
	// Sets the press state of the button
	void setPressed(bool p);
	// Sets the button's text, only if button was constructed with text
	void setText(const LanguageString& t);
	// Sets the button's visibility (button can't be pressed when invisible)
	void setVisibility(bool v);

	// Below are provided custom render functions, that create commonly
	// used UI elements:

	// Draws nothing
	static void noDraw(const vec2& xy, bool);
	// Draws the three dots (settings icon)
	static void drawDots(const vec2& xy, bool);
	// Draws the top-left back arrow
	static void drawBackArrow(const vec2& xy, bool);
	// Draws a menu item (settings screen entries)
	static void drawMenuItem(const vec2& xy, bool, const LanguageString& text);
	// Draws a dispenser button
	static void drawDispenserItem(const vec2& xy, bool, const LanguageString& text);
	// Draws a scroll button (used in the language menu)
	static void drawScrollButton(const vec2& xy, bool);
	// Draws an up-arrow button (time/date selectors)
	static void drawUpButton(const vec2& xy, bool);
	// Draws a down-arrow button (time/date selectors)
	static void drawDownButton(const vec2& xy, bool);
	// Draws a full-width button with text ("SAVE" buttons at screen bottom)
	static void drawFullWidth(const vec2& xy, bool, const LanguageString& text);
	// Draws a small left arrow (temperature selector)
	static void drawSmallLeft(const vec2& xy, bool);
	// Draws a small right arrow (temperature selector)
	static void drawSmallRight(const vec2& xy, bool);
	// Draws a button that should be exclusive to its pair (AM/PM or 24HR)
	static void drawExclusiveOption(const vec2& xy, bool, const LanguageString& text);
	// Draws a togglable switch (SleepMode enable)
	static void drawToggle(const vec2& xy, bool);
};

class Screen;

// Points to the currently displayed screen.
// If null, display enters its sleep mode.
extern Screen *screenCurrent;

/**
 * Defines a display screen, that includes buttons, other UI elements, and
 * initialization so that buttons can act on variables or the main board.
 */
class Screen {
private:
	Screen *parent;
	Button *buttons;
	unsigned int buttonCount;
	void (*preShow)(void);
	void (*preDraw)(void);

	unsigned int sleepCounter;

public:
	/**
	 * Creates a screen with the given buttons, initialization function and
	 * render function.
	 * preShow is called once each time this screen is switched to.
	 * preDraw is called every render loop, before the buttons and input
	 * handling.
	 */
	Screen(Screen *_parent, Button *_buttons, unsigned int _buttonCount,
		void (*_preShow)(void), void (*_preDraw)(void))
		: parent(_parent), buttons(_buttons), buttonCount(_buttonCount),
		  preShow(_preShow), preDraw(_preDraw), 
		  sleepCounter(0) {}

	// Prepares the screen for usage (called on transition to this screen).
	void prepare(void);
	// Shows the screen, rendering it and handling user input.
	void show(void);

	// A provided routine for clearing the display and showing the ion logo.
	static void clearWithIonHeader(bool line = true);
};

#endif // ASSETS_H_
