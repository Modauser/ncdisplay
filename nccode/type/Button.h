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

/**
 * @class Button
 * @brief Creates a graphical button that can trigger custom actions.
 *
 * Supports bitmaps or a custom function for rendering (with or without text).
 * Can trigger actions on presses or releases.
 */
class Button {
private:
	/**
	 * The button's location.
	 */
	vec2 xy;

	/**
	 * The bitmap handle for the button (-1 if none).
	 */
	int handle;

	/**
	 * The button's text (empty if no text).
	 */
	LanguageString text;

	/**
	 * The button's custom render function (nullptr if none).
	 */
	void *render;

	/**
	 * True if the button should be seen.
	 */
	bool visible;

	/**
	 * True if the button is pressed; false if released.
	 */
	bool pressed;

	/**
	 * If true, button will always be rendered in a pressed state.
	 */
	bool forcePressed;

	/**
	 * Points to custom action code.
	 * @param True on button press, false no button release
	 */
	void (*action)(bool);

public:
	/**
	 * Creates a button that renders the given bitmap handle.
	 * @param _xy The button's location
	 * @param _handle The bitmap handle of the button's icon
	 * @param _action The button's press/release action
	 */
	 Button(vec2 _xy, int _handle, void (*_action)(bool)
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
	 * @param _xy The button's location
	 * @param _render The button's custom render function, with text
	 * @param _text The button's text
	 * @param _action The button's press/release action
	 */
	 Button(vec2 _xy, void (*_render)(const vec2&, bool,
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
	 * @param _xy The button's location
	 * @param _render The button's custom render function
	 * @param _action The button's press/release action
	 */
	 Button(vec2 _xy, void (*_render)(const vec2&, bool),
		void (*_action)(bool) = nullptr)
		: xy(_xy),
		  handle(-1),
		  render((void *)_render),
		  visible(true),
		  pressed(false),
		  forcePressed(false),
		  action(_action) {}

	/**
	 * Renders the button.
	 */
	void draw(void);

	/**
	 * Runs the button's action code if it has any.
	 */
	void doAction(void);

	/**
	 * Gets the current press state of the button.
	 * @return True for pressed, false for released
	 */
	bool getPressed(void) const;

	/**
	 * Forces pressed state, keeping button rendered as pressed.
	 * @param p True to force pressed state.
	 */ 
	void setForcePressed(bool p);

	/**
	 * Sets the press state of the button.
	 * @param p True for pressed, false for released
	 */
	void setPressed(bool p);

	/**
	 * Sets the button's text (only if button was constructed with text).
	 * @param t New text for the button
	 */
	void setText(const LanguageString& t);

	/**
	 * Sets the button's visibility.
	 * If not visible, button won't be shown and can't be pressed.
	 */ 
	void setVisibility(bool v);

	/**
	 * Sets the button's position.
	 * @param _xy New position for the button
	 */
	inline void setPosition(vec2 _xy) {
		xy = _xy;
	}

	/**
	 * Sets a new render function for the button (excluding text).
	 * If button started with text, render function should not change to one
	 * without text.
	 * @param _render New render function
	 */
	void setRender(void (*_render)(const vec2&, bool));

	/**
	 * Sets a new render function for the button (including text).
	 * If button started without text, render function should not change to
	 * one with text.
	 * @param _render New render function
	 */
	void setRender(void (*_render)(const vec2&, bool, const LanguageString&));

	/**
	 * Pre-defined button render function:
	 * Draws the three dots (settings icon).
	 */
	static void drawDots(const vec2& xy, bool);
	/**
	 * Pre-defined button render function:
	 * Draws the top-left back arrow.
	 */
	static void drawBackArrow(const vec2& xy, bool);

	/**
	 * Pre-defined button render function:
	 * Draws a menu item (settings screen entries).
	 */
	static void drawMenuItem(const vec2& xy, bool, const LanguageString& text);

/**
	 * Pre-defined button render function:
	 * Draws a menu item (settings screen entries).
	 * Appends + CO2 to the text if it is a model 300 or 400
	 * pass true to bool if CO2 is enabled
	 */
	static void drawMenuItemCo2(const vec2& xy, bool, const LanguageString& text);

	/**
	 * Pre-defined button render function:
	 * Draws a dispenser button.
	 */
	static void drawDispenserItem(const vec2& xy, bool, const LanguageString& text );

	/**
	 * Pre-defined button render function:
	 * Draws a scroll button (used in the language menu).
	 */
	static void drawScrollButton(const vec2& xy, bool);

	/**
	 * Pre-defined button render function:
	 * Draws an up-arrow button (e.g. time/date selectors).
	 */
	static void drawUpButton(const vec2& xy, bool);

	/**
	 * Pre-defined button render function:
	 * Draws a down-arrow button (e.g. time/date selectors).
	 */
	static void drawDownButton(const vec2& xy, bool);

	/**
	 * Pre-defined button render function:
	 * Draws a full-width button with text (e.g. "SAVE" buttons at screen
	 * bottom).
	 */
	static void drawFullWidth(const vec2& xy, bool, const LanguageString& text);

	/**
	 * Pre-defined button render function:
	 * Draws a small left arrow (e.g. temperature selector).
	 */
	static void drawSmallLeft(const vec2& xy, bool);

	/**
	 * Pre-defined button render function:
	 * Draws a small right arrow (e.g. temperature selector).
	 */
	static void drawSmallRight(const vec2& xy, bool);

	/**
	 * Pre-defined button render function:
	 * Draws a button that should be exclusive to its pair (e.g. AM/PM or
	 * 24HR toggle).
	 */
	static void drawExclusiveOption(const vec2& xy, bool, const LanguageString& text);

	/**
	 * Pre-defined button render function:
	 * Draws a togglable switch (e.g. SleepMode enable).
	 */
	static void drawToggle(const vec2& xy, bool);

	/**
	 * Pre-defined button render function:
	 * Used for welcome language select buttons.
	 */
	static void drawWelcomeSelect(const vec2& xy, bool);

	/**
	 * Pre-defined button render function:
	 * Used for cautionary buttons (warning screen, hot dispense).
	 */
	static void drawRedFullWidth(const vec2& xy, bool, const LanguageString& text);
};

#endif // BUTTON_H_

