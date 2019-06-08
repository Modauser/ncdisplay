#include <iostream>
#include <cstdio>

static void handleRequest(void);
static void handleStartup(void);
static void handleDispense(void);

class Serial
{
private:
	FILE *fd;

public:
	Serial(void) {
		fd = fopen("/dev/ttyUSB0", "ab+");
		if (fd != nullptr)
			std::cout << "Success" << std::endl;
	}
	~Serial(void) {
		fclose(fd);
	}

	void write(char c) {
		std::cout << ">>> " << (int)c << std::endl;
		if (fd == nullptr) {
			std::cout << "ouch" << std::endl;
			return;
		}
		fputc('$', fd);
		fputc(c, fd);
	}
	void write(const char *s, unsigned int count) {
		for (unsigned int i = 0; i < count; i++)
			write(s[i]);
		fflush(fd);
	}

	int read(void) {
		if (fd == nullptr) {
			std::cout << "ouch" << std::endl;
			return 0;
		}
		int c = fgetc(fd);
		std::cout << "> " << c << std::endl;
		return c;
	}
};

static Serial serial;

int main(void)
{
	bool shouldRun = true;
	char input = 0;

	while (shouldRun) {
		input = serial.read();
		switch (input) {
		case '\0':
			shouldRun = false;
			break;
		case '@':
			std::cout << "Request code" << std::endl;
			handleRequest();
			break;
		case '#':
			std::cout << "Startup code" << std::endl;
			handleStartup();
			break;
		case '$':
			std::cout << "Dispense code" << std::endl;
			handleDispense();
			break;
		case '%':
			std::cout << "Error code" << std::endl;
			input = serial.read();
			if (input == '?')
				serial.write(0);
			break;
		case '^':
			std::cout << "Bottle code" << std::endl;
			input = serial.read();
			if (input == 'B')
				serial.write("123", 3);
			break;
		default:
			std::cout << "Unknown code" << std::endl;
			break;
		}
	}

	return 0;
}

void handleRequest(void)
{
	char input;
	input = serial.read();
	switch (input) {
	case '!':
		// Model number
		serial.write(3);
		break;
	case 'E':
		// Serial number?
		serial.write("Current        ", 15);
		break;
	case '#':
		// Version number
		serial.write("\x1\x4", 2);
		break;
	case '%':
		// Filter type
		serial.write(0);
		break;
	case '&':
		// Gals rem
		serial.write(750 >> 8);
		serial.write(750 & 0xFF);
		break;
	case '`':
		// Months rem
		serial.write(12);
		break;
	case '(':
		// m/d/y filter date
		serial.write("\x2\x1\x14", 3);
		break;
	case 'T':
		// Time
		serial.write("\x5\xA\x0\x1", 4);
		break;
	case 'D':
		// Date
		serial.write("\x6\xD\x13", 3);
		break;
	case '6':
		// Temp Hot
		serial.write(0);
		break;
	case '7':
		// Temp Cold On
		serial.write(1);
		break;
	case '8':
		// Temp Cold Off
		serial.write(2);
		break;
	case 'F':
	case 'a':
	case 'b':
	case 'c':
		// Flow rates
		serial.write(32);
		break;
	case 'M':
		// SleepMode hours
		serial.write("\x19\x19\x19\x19", 4);
		break;
	case 'O':
		// In SleepMode
		serial.write(0);
		break;
	case 'H':
		// SleepMode enabled
		serial.write(0);
		break;
	case 'I':
		// Dispense error enabled
		serial.write(1);
		break;
	case 'K':
		// Lock enable
		serial.write(0);
		break;
	case 'S':
	case 'U':
	case 'V':
	case 'W':
	case 'h':
		// Error enables
		serial.write(0);
		break;
	case 'X':
		// Metric?
		serial.write(0);
		break;
	case 'd':
		// Ice tank temp
		serial.write(32);
		break;
	case 'e':
		// Hot tank temp
		serial.write(200);
		break;
	case 'f':
		// Service log, don't support by sending not '$'
		serial.write(0);
		break;
	default:
		std::cout << "Unknown request";
		break;
	}
}

void handleStartup(void)
{
	char input = serial.read();
	switch (input) {
	case '6':
		// English
		serial.write(0);
		break;
	case '7':
		// Model 3
		serial.write('\x83');
		break;
	case '9':
	case ':':
	case ';':
		// Full
		serial.write(1);
		break;
	default:
		break;
	}
}

void handleDispense(void)
{
	// Consume next character
	serial.read();

	// We don't write anything back to the display
}

