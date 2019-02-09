#ifndef ERROR_H_
#define ERROR_H_

enum class ErrorType : char {
	None,
	Green,
	Red,
	System
};

class Error {
private:
	unsigned char code;
	ErrorType type;
	bool showOnStartup;
	char message[64];

	void loadErrorText(void);

public:
	constexpr Error(unsigned char _code, ErrorType _type, bool _startup)
		: code(_code), type(_type), showOnStartup(_startup),
       		  message{'\0'} {}

	constexpr operator bool(void) const {
		return code != 0;
	}

	constexpr bool operator==(unsigned char c) const {
		return code == c;
	}

	constexpr bool shouldShowOnStartup(void) const {
		return showOnStartup;
	}

	void show(void) const;

	static const Error& get(unsigned char code);
};

#endif // ERROR_H_

