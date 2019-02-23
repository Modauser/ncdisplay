/**
 * @file Errors.h
 * @brief Provides error handling for main board errors.
 */
#ifndef ERROR_H_
#define ERROR_H_

enum class ErrorType : char {
	None,
	Green,
	Red,
	System
};

using ErrorCode = unsigned char;

struct ErrorData {
	ErrorCode code;
	ErrorType type;
	bool showOnStartup;

	constexpr ErrorData(ErrorCode _code, ErrorType _type, bool _startup)
		: code(_code), type(_type), showOnStartup(_startup) {}
};

class Error {
private:
	static char message[128];
	static ErrorData lastError;

	static bool dispenseFlag;

	static void loadMessage(unsigned int index);

public:
	static bool check(void);
	inline static bool hasError(void) {
		return lastError.code != 0x00;
	}

	static void show(void);
	static void showStartup(void);
	static void showSystemError(void);

	inline static bool shouldDispense(void) {
		return !dispenseFlag;
	}
	inline static void clearDispenseFlag(void) {
		dispenseFlag = false;
	}
};

#endif // ERROR_H_

