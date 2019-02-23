/**
 * @file Button.h
 * @brief Provides support for customizable buttons for screens.
 */
#ifndef BUTTON_H_
#define BUTTON_H_

#include "LanguageString.h"

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
	constexpr Button(vec2 _xy, int _handle, void (*_action)(bool)
		= nullptr)
		: xy(_xy),
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
	constexpr Button(vec2 _xy, void (*_render)(const vec2&, bool,
		const LanguageString&), const LanguageString& _text,
		void (*_action)(bool) =	nullptr)
		: xy(_xy),
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
	constexpr Button(vec2 _xy, void (*_render)(const vec2&, bool),
		void (*_action)(bool) = nullptr)
		: xy(_xy),
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

	// Forces pressed state for things like switches/toggles
	void setForcePressed(bool p);
	// Sets the press state of the button
	void setPressed(bool p);
	// Sets the button's text, only if button was constructed with text
	void setText(const LanguageString& t);
	// Sets the button's visibility (button can't be pressed when invisible)
	void setVisibility(bool v);

	void setRender(void (*_render)(const vec2&, bool));

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
	// Used for welcome language select buttons
	static void drawWelcomeSelect(const vec2& xy, bool);
	// Used for cautionary buttons (warning screen, hot dispense)
	static void drawRedFullWidth(const vec2& xy, bool, const LanguageString& text);
};

#endif // BUTTON_H_

