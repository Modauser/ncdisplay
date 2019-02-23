/**
 * @file Button.cpp
 * @brief Provides support for customizable buttons for screens.
 */
#include "Assets.h"
#include "Button.h"

#include <gameduino2/GD2.h>

void Button::draw(void)
{
	if (!visible)
		return;

	if (render != nullptr) {
		GD.LineWidth(16);
		if (!text.empty()) {
			((void (*)(const vec2&, bool, const LanguageString&))
				render)(xy, pressed | forcePressed, text);
		} else {
			((void (*)(const vec2&, bool))render)(xy,
				pressed | forcePressed);
		}
		GD.ColorRGB(WHITE);
	} else if (handle != -1) {
		GD.Vertex2ii(xy.x, xy.y, handle);
	}
}

void Button::setForcePressed(bool p)
{
	forcePressed = p;
}

void Button::setVisibility(bool v)
{
	visible = v;
}

void Button::setText(const LanguageString& t)
{
	if (!text.empty())
		text = t;
}

bool Button::getPressed(void) const
{
	return pressed;
}

void Button::setPressed(bool p)
{
	pressed = p;
}

void Button::setRender(void (*_render)(const vec2&, bool))
{
	render = (void *)_render;
}

void Button::doAction(void)
{
	if (action != nullptr)
		action(pressed);
}

void Button::noDraw(const vec2& xy, bool pressed)
{
	(void)xy;
	(void)pressed;
}

void Button::drawDots(const vec2& xy, bool pressed)
{
	(void)xy;
	(void)pressed;

	GD.ColorRGB(NC_BKGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(0, 0);
	GD.Vertex2ii(57, 57);

	GD.ColorRGB(NC_FRGND_COLOR);
	GD.PointSize(20 * 4);
	GD.Begin(POINTS);
	GD.Vertex2ii(20, 30);
	GD.Vertex2ii(33, 30);
	GD.Vertex2ii(46, 30);
	GD.ColorRGB(NC_BKGND_COLOR);
	GD.PointSize(14 * 3);
	GD.Vertex2ii(20, 30);
	GD.Vertex2ii(33, 30);
	GD.Vertex2ii(46, 30);
}

void Button::drawBackArrow(const vec2& xy, bool pressed)
{
	(void)xy;
	(void)pressed;

	GD.ColorRGB(NC_BKGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(0, 0);
	GD.Vertex2ii(57, 57);

	GD.ColorRGB(NC_FRGND_COLOR);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(29, 23);
	GD.Vertex2ii(22, 30);
	GD.Vertex2ii(29, 37);
}

void Button::drawMenuItem(const vec2& xy, bool pressed, const LanguageString& text)
{
	(void)pressed;

	// Rectangle
	GD.Begin(RECTS);
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Vertex2ii(0, xy.y + 1);
	GD.Vertex2ii(272, xy.y + 59);
	GD.ColorRGB(WHITE);
	GD.Vertex2ii(0, xy.y);
	GD.Vertex2ii(272, xy.y);

	// Right arrow
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(272 - 29, xy.y + 23);
	GD.Vertex2ii(272 - 22, xy.y + 30);
	GD.Vertex2ii(272 - 29, xy.y + 37);

	// Text
	GD.cmd_text(xy.x == 0 ? 30 : 60, xy.y + 30, FONT_SMALL, OPT_CENTERY, text());
	if (xy.x != 0) {
		GD.Begin(BITMAPS);
		GD.Vertex2ii(20, xy.y + 17, SETSTRIP_HANDLE, xy.x - 1);
	}
}

void Button::drawScrollButton(const vec2& xy, bool pressed)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(0, xy.y + 1);
	GD.Vertex2ii(272, xy.y + 59);
	GD.ColorRGB(WHITE);
	GD.Vertex2ii(0, xy.y);
	GD.Vertex2ii(272, xy.y);

	// Arrow (x > 0 -> up, x < 0 -> down)
	GD.ColorRGB(WHITE);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	int a = (xy.x > 0) ? 40 : 20;
	int b = (xy.x > 0) ? 20 : 40;
	GD.Vertex2ii(136 - 30, xy.y + a);
	GD.Vertex2ii(136,      xy.y + b);
	GD.Vertex2ii(136 + 30, xy.y + a);
}

void Button::drawDispenserItem(const vec2& xy, bool pressed, const LanguageString& text)
{
	// Rectangle
	GD.Begin(RECTS);
	GD.ColorRGB(pressed ? 0x0c3d6b : NC_FRGND_COLOR);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 134, xy.y + 68);

	GD.ColorRGB(WHITE);
	GD.cmd_text(xy.x + 67, xy.y + 33, FONT_SMALL, OPT_CENTER, text(), 20);
}

void Button::drawFullWidth(const vec2& xy, bool pressed, const LanguageString& text)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(0, xy.y + 1);
	GD.Vertex2ii(272, xy.y + 59);
	GD.ColorRGB(WHITE);
	GD.Vertex2ii(0, xy.y);
	GD.Vertex2ii(272, xy.y);

	// Text
	GD.ColorRGB(WHITE);
	GD.cmd_text(136, xy.y + 30, FONT_SMALL, OPT_CENTER, text());
}

void Button::drawSmallLeft(const vec2& xy, bool pressed)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 40, xy.y + 40);

	// Left arrow
	GD.ColorRGB(WHITE);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(24, xy.y + 14);
	GD.Vertex2ii(17, xy.y + 21);
	GD.Vertex2ii(24, xy.y + 28);
}

void Button::drawSmallRight(const vec2& xy, bool pressed)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 40, xy.y + 40);

	// Right arrow
	GD.ColorRGB(WHITE);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(272 - 24, xy.y + 14);
	GD.Vertex2ii(272 - 17, xy.y + 21);
	GD.Vertex2ii(272 - 24, xy.y + 28);
}

void Button::drawUpButton(const vec2& xy, bool pressed)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 70, xy.y + 34);

	// Arrow
	GD.ColorRGB(WHITE);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(xy.x + 28, xy.y + 18);
	GD.Vertex2ii(xy.x + 35, xy.y + 11);
	GD.Vertex2ii(xy.x + 42, xy.y + 18);
}

void Button::drawDownButton(const vec2& xy, bool pressed)
{
	(void)pressed;

	// Rectangle
	GD.ColorRGB(NC_FRGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 70, xy.y + 34);

	// Arrow
	GD.ColorRGB(WHITE);
	GD.LineWidth(12 * 2);
	GD.Begin(LINE_STRIP);
	GD.Vertex2ii(xy.x + 28, xy.y + 11);
	GD.Vertex2ii(xy.x + 35, xy.y + 18);
	GD.Vertex2ii(xy.x + 42, xy.y + 11);
}

void Button::drawExclusiveOption(const vec2& xy, bool pressed, const LanguageString& text)
{
	// Rectangle
	GD.ColorRGB(NC_FDGND_COLOR);
	GD.Begin(RECTS);
	GD.Vertex2ii(xy.x, xy.y);
	GD.Vertex2ii(xy.x + 122, xy.y + 34);

	// Inner Rectangle
	GD.ColorRGB(pressed ? WHITE : NC_FRGND_COLOR);
	GD.Vertex2ii(xy.x + 1, xy.y + 1);
	GD.Vertex2ii(xy.x + 120, xy.y + 32);

	// Text
	GD.ColorRGB(pressed ? NC_FDGND_COLOR : WHITE);
	GD.cmd_text(xy.x + 62, xy.y + 17, FONT_SMALL, OPT_CENTER, text());
}

void Button::drawToggle(const vec2& xy, bool pressed)
{
	//if (pressed)
	//	extraState ^= true;

	GD.cmd_fgcolor(0xdddddd);
	GD.cmd_bgcolor(pressed ? NC_ONTOG_COLOR : NC_OFTOG_COLOR);
	GD.cmd_toggle(xy.x, xy.y, 30, 27, OPT_FLAT, pressed ? 65535 : 0,
		"\0\xFF\0");
}

void Button::drawWelcomeSelect(const vec2& xy, bool pressed)
{
	(void)pressed;
	(void)xy;
}

void Button::drawRedFullWidth(const vec2& xy, bool pressed, const LanguageString& text)
{
	// Rectangle
	GD.ColorRGB(pressed ? 0xB00000 : 0xFF0000);
	GD.Begin(RECTS);
	GD.Vertex2ii(0, xy.y + 1);
	GD.Vertex2ii(272, xy.y + 59);
	GD.ColorRGB(WHITE);
	GD.Vertex2ii(0, xy.y);
	GD.Vertex2ii(272, xy.y);

	// Text
	GD.cmd_text(136, xy.y + 30, FONT_SMALL, OPT_CENTER, text());
}

