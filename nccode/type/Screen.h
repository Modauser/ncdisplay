/**
 * @file Screen.h
 * @brief Provides a screen object with support for buttons and custom render
 * code.
 */
#ifndef SCREEN_H_
#define SCREEN_H_

#include "Button.h"
#include "ScreenInterface.h"

#include <array>
#include <cstddef>
#include <functional>
#include <gameduino2/GD2.h>

/**
 * @class Screen
 * @brief Contains data necessary for a complete 'screen' that's drawn on the
 * display.
 */
template<std::size_t N>
class Screen : public ScreenInterface {
private:
	/**
	 * A user-defined function to be called when this screen is selected
	 * for viewing, before this screen is rendered.
	 */
	std::function<void(void)> prepareFunc;

	/**
	 * A user-defined function called each render loop. Buttons are rendered
	 * and handled after calling this.
	 */
	std::function<void(void)> showFunc;

	/**
	 * Counts time since last screen touch, to determine if we should
	 * timeout to sleep.
	 */
	unsigned int sleepCounter;

	/**
	 * The array of buttons shown on this screen.
	 */
	std::array<Button, N> buttons;

public:
	/**
	 * Creates a screen with the given values.
	 * @param _this This screen's ID
	 * @param _parent This screen's parent's ID
	 * @param _prepare Function called upon screen load
	 * @param _show Render function of screen (buttons drawn separately)
	 * @param _buttons Definitions of each button
	 * @see ScreenID
	 */
	template<typename... Args>
	constexpr Screen(ScreenID _this, ScreenID _parent,
		std::function<void(void)> _prepare,
		std::function<void(void)> _show, Args... _buttons)
		: ScreenInterface(_this, _parent), prepareFunc(_prepare),
		  showFunc(_show), sleepCounter(0), buttons{_buttons...} {}

	/**
	 * Calls the screen's prepare/initialization function if one was
	 * provided.
	 */
	void prepare(void) final {
		if (prepareFunc)
			prepareFunc();
	}
	
	/**
	 * Renders the entire screen and handles input.
	 *
	 * Custom render/show function is called first, then buttons are
	 * rendered. Input is handled after entire screen is rendered.
	 */
	void show(void) final {
		// Call the pre-render function
		if (showFunc)
			showFunc();
	
		// Render the buttons
		for (unsigned int i = 0; i < buttons.size(); i++) {
			GD.Tag(i + 1);
			buttons[i].draw();
		}
		GD.Tag(0);
	
		// Push to the display
		GD.swap();
	
		// Get display inputs
		GD.get_inputs();
		auto tag = GD.inputs.tag;
	
		// Handle each button (must handle all every time to catch release
		// events)
		for (unsigned int i = 0; i < buttons.size(); i++) {
			bool pressed = (i + 1 == tag);
			bool change = (pressed != buttons[i].getPressed());
			buttons[i].setPressed(pressed);
			if (change) {
				buttons[i].doAction();
				break; // Only one change at a time
			}
		}
	
		// If touched, reset the sleep countdown
		// If no touch, continue sleep countdown
		if (tag != 0) {
			sleepCounter = 0;
		} else {
			sleepCounter += 10;
			if (sleepCounter >= SLEEP_TIMEOUT) {
				// Timed out; go to parent
				sleepCounter = 0;
				ScreenManager::setCurrent(parent);
			}
		}
	}

	/**
	 * Retrieves a button from this screen, given the button index.
	 *
	 * The index is determined based on the order the buttons are passed to
	 * the constructor, i.e. the first button defined is at index 0.
	 * @param index The index of the desired button
	 * @return The button
	 */
	inline Button& getButton(std::size_t index) {
		return buttons[index];
	}
};

/**
 * Allows Screen objects to be defined without the N parameter. The number of
 * buttons passed to the constructor will determine N.
 */
template<typename... Args>
Screen(ScreenID _this, ScreenID _parent, std::function<void(void)> _prepare,
	std::function<void(void)> _show, Args... _buttons) -> Screen<sizeof...(Args)>;

#endif // SCREEN_H_

