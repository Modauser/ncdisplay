/**
 * @file Errors.h
 * @brief Provides error handling for main board errors.
 */
#ifndef ERROR_H_
#define ERROR_H_

/**
 * @enum ErrorType
 * @brief Defines all possible error types.
 */
enum class ErrorType : char {
	None,  /**< Not an error */
	Green, /**< Error shown at dispense screen with green border. */
	Red,   /**< Error shown at dispense screen with red border. */
	System /**< Full-screen, system-halting error. */
};

/**
 * Error codes are byte-sized.
 */
using ErrorCode = unsigned char;

/**
 * @struct ErrorData
 * @brief Contains data necessary to 'define' an error.
 */
struct ErrorData {
	ErrorCode code;     /**< The error code. */
	ErrorType type;     /**< The error's type. */
	bool showOnStartup; /**< True if error can occur during startup. */

	constexpr ErrorData(ErrorCode _code, ErrorType _type, bool _startup)
		: code(_code), type(_type), showOnStartup(_startup) {}
};

/**
 * @class Error
 * @brief An interface for managing errors.
 */
class Error {
private:
	/**
	 * @internal
	 * Stores error message text for non-system errors.
	 */
	static char message[128];

	/**
	 * @internal
	 * Contains the last obtained error from the main board.
	 */
	static ErrorData lastError;

	/**
	 * True if a dispense error was received.
	 */
	static bool dispenseFlag;

	/**
	 * @internal
	 * Loads error message text for the error at the given index
	 */
	static void loadMessage(unsigned int index);

	/**
	 * @internal
	 * Draws a message box to the screen.
	 * @param color The color of the box border, format 0xRRGGBB
	 * @param message The box's message
	 */
	static void messageBox(uint32_t color, const char *message);

	/**
	 * @internal
	 * Shows a system error on the display, then halts.
	 */
	[[noreturn]]
	static void showSystemError(void);

public:
	/**
	 * Fetches the latest error code from the main board.
	 * @return True if an error was received, false for no errors
	 */
	static bool check(void);

	/**
	 * Checks if there is a current error.
	 * @return True if there's an error
	 */
	inline static bool hasError(void) {
		return lastError.code != 0x00;
	}

	/**
	 * Shows an error if there is one, either as a message box or a system
	 * error.
	 */
	static void show(void);

	/**
	 * Shows an error if there is one, and it should be shown during
	 * startup.
	 */
	static void showStartup(void);

	/**
	 * Checks if dispensing should be allowed.
	 */
	inline static bool shouldDispense(void) {
		return !dispenseFlag;
	}

	/**
	 * Clears the dispense error flag, for both display and main board.
	 */
	static void clearDispenseFlag(void);
};

#endif // ERROR_H_

