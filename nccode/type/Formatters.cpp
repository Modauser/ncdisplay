#include "Formatters.h"

#include "../MainBoard.h"

namespace Format {

char *flowRate(char *buf, int number)
{
	sprintf(buf, "%d.%1d %s", number / 10, number % 10,
		MainBoard::isMetric() ? "L" : "GPM");
	return buf;
}

char *temperature(char *buf, int number)
{
	sprintf(buf, "%u %c", number, MainBoard::isMetric() ? 'C' : 'F');
	return buf;
}

char *date(char *buf, int mmddyy, char splitter)
{ 
	sprintf(buf, "%d%c%d%c20%02d", ((mmddyy >> 16) & 0xFF), splitter,
		((mmddyy >> 8) & 0xFF), splitter, mmddyy & 0xFF);
	return buf;
}

char *hour(char *buf, int hour)
{
	if (MainBoard::is24Hour()) {
		sprintf(buf, "%d", hour);
	} else {
		bool pm = hour > 11;
		if (pm)
			hour -= 12;
		if (hour == 0)
			hour = 12;
		sprintf(buf, "%d %cM", hour, pm ? 'P' : 'A');
	}
	return buf;
}

} // namespace Format

