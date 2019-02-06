#include <gameduino2/GD2.h>
#include <stdio.h>

#include "Assets.h"
#include "Screens.h"

static int lockscreenInput[4] = {
	0, 0, 0, 0
};
static int lockscreenIndex = 0;

Screen *lockscreenReturn = nullptr;

void lockscreenEnterKey(char key)
{
	if (key == '<') {
		if (lockscreenIndex > 0)
			lockscreenIndex--;
	} else {
		lockscreenInput[lockscreenIndex++] = key;
	}
}

static Button buttonsLockscreen[] = {
	Button(1, {0, 0}, Button::drawBackArrow, [](bool press) {
		if (!press)
			screenCurrent = &screenSettings;
	}),
	Button('1', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('1'); } }),
	Button('2', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('2'); } }),
	Button('3', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('3'); } }),
	Button('4', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('4'); } }),
	Button('5', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('5'); } }),
	Button('6', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('6'); } }),
	Button('7', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('7'); } }),
	Button('8', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('8'); } }),
	Button('9', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('9'); } }),
	Button('0', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('0'); } }),
	Button('<', {0, 0}, Button::noDraw, [](bool press) {
		if (!press) { lockscreenEnterKey('<'); } }),
};

Screen screenLockscreen (
	// Parent screen
	&screenSettings,
	// Buttons
	buttonsLockscreen, 12,
	// Initialization function
	[](void) {
		if (lockscreenReturn == nullptr)
			lockscreenReturn = &screenSettings;

		for (int i = 0; i < 4; i++)
			lockscreenInput[i] = 0;
		lockscreenIndex = 0;
	},
	// Pre-draw function
	[](void) {
		if (lockscreenIndex > 4) {
			delay_ms(500);
			screenCurrent = lockscreenReturn;
		}

		Screen::clearWithIonHeader();

		GD.ColorRGB(NC_FRGND_COLOR);
		GD.cmd_text(136, 70, FONT_TITLE, OPT_CENTERX, "Enter Passcode");

		GD.ColorRGB(NC_FDGND_COLOR);
		for (int i = 0; i < lockscreenIndex; i++) {
			GD.cmd_text(54 + 54 * i, 120, FONT_LARGE, OPT_CENTER,
				"*");
		}

		GD.ColorRGB(WHITE);
		GD.cmd_keys(0, 140, 272, 78, FONT_SMALL, OPT_FLAT, "123");
		GD.cmd_keys(0, 220, 272, 78, FONT_SMALL, OPT_FLAT, "456");
		GD.cmd_keys(0, 300, 272, 78, FONT_SMALL, OPT_FLAT, "789");
		GD.cmd_keys(0, 380, 272, 78, FONT_SMALL, OPT_FLAT, " 0<");

		if (lockscreenIndex == 4)
			lockscreenIndex++;
	}
);

