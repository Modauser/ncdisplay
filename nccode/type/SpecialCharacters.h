/**
 * @file SpecialCharacters.h
 * @brief Defines font characters for special symbols.
 *
 * Custom font files had to be made due to the design of the FT81x graphics
 * chip. So, non-English symbols had to be moved to different locations. All
 * relocated symbols are defined in this file.
 */
#ifndef SPECIALCHARACTERS_H_
#define SPECIALCHARACTERS_H_

#define DEGREE     "\x01"	// 0xB0

//#define A_ACUTE
#define E_ACUTE    "\x05"	// 0xC9
#define e_ACUTE    "\x13"	// 0xE9
#define I_ACUTE    "\x07"	// 0xCD
#define i_ACUTE    "\x15"	// 0xED
#define O_ACUTE    "\x08"	// 0xD3
#define o_ACUTE    "\x16"	// 0xF3
#define U_ACUTE    "\x0A"	// 0xDA
#define u_ACUTE    "\x18"	// 0xFA
#define A_GRAVE    "\x02"	// 0xC0
#define a_GRAVE    "\x10"	// 0xE0
#define E_GRAVE    "\x06"	// 0xC8
#define e_GRAVE    "\x14"	// 0xE8
#define A_UMLAUT   "\x03"	// 0xC4
#define a_UMLAUT   "\x11"	// 0xE4
#define O_UMLAUT   "\x09"	// 0xD6
#define o_UMLAUT   "\x17"	// 0xF6
#define U_UMLAUT   "\x0B"	// 0xDC
#define u_UMLAUT   "\x19"	// 0xFC
#define U_HAT      "\x0C"	// 0xDB
#define u_HAT      "\x1A"	// 0xFB
#define A_ORING    "\x04"	// 0xC5
#define a_ORING    "\x12"	// 0xE5
#define N_TILDE    "\x0D"	// 0xD1
#define n_TILDE    "\x1B"	// 0xF1
#define CEDILLA    "\x0E"	// 0xC7
#define cEDILLA    "\x1C"	// 0xE7
#define ESZETT     "\x0F"	// 0xDF

#define COPYRIGHT  "\x1D"	// 0xA9
#define REGCOPYR   "\x1E"	// 0xAE

#endif // SPECIALCHARACTERS_H_

