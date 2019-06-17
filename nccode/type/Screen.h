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

	unsigned int pressedButton;

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
		  showFunc(_show), sleepCounter(0), pressedButton(0),
		  buttons{_buttons...} {}

	/**
	 * Calls the screen's prepare/initialization function if one was
	 * provided.
	 */
	void prepare(void) final {
		if (prepareFunc)
			prepareFunc();
		pressedButton = 0;
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


		if (GD.inputs.x == -32768 && GD.inputs.y == -32768) {
			if (pressedButton != 0) {
				buttons[pressedButton - 1].setPressed(false);
				buttons[pressedButton - 1].doAction();
				pressedButton = 0;
			} else {
				// No recent presses, count for sleep
				sleepCounter += 10;
				if (sleepCounter >= SLEEP_TIMEOUT) {
					// Timed out; go to parent
					sleepCounter = 0;
					ScreenManager::setCurrent(parent);
				}
			}
		} else if (GD.inputs.tag > 0 && GD.inputs.tag <= buttons.size()) {
			// Button press, keep screen on
			sleepCounter = 0;

			if (pressedButton == 0) {
				pressedButton = GD.inputs.tag;
				buttons[pressedButton - 1].setPressed(true);
				buttons[pressedButton - 1].doAction();
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

