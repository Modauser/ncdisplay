/**
 * @file NirmalaMod.h
 * @brief Font table data that pairs with the other font data on the SD card.
 *
 * These fonts were modified to contain accents to support multiple languages,
 * as a font on the FT81x can only have 126 characters (0x01 to 0x7F).
 *
 * Example command line used to generate these:
 * 	fnt_cvt.exe -i Nirmala.ttf -s 14 -u nirmala-mod-utf8.txt 
 *
 * Header blocks are 148 bytes long:
 *  * 128 bytes of character width for each character
 *  * 4 byte word for format (L8 = 3)
 *  * 4 byte word for stride 
 *  * 4 byte word for max character width
 *  * 4 byte word for max character height
 *  * 4 byte word for video RAM address of font data (unused)
 */ 

using byte = unsigned char;

/**
 * Font header block for the Nirmala font, size 14.
 */
static const byte NirmalaBold14Header[148] = { 
/* Widths */
0,5,10,11,11,7,7,5,11,11,10,10,10,11,9,9,8,8,8,8,8,5,9,9,8,8,8,8,7,8,5,9,4,4,6,9,8,13,13,4,6,6,6,11,4,6,4,8,9,8,8,7,9,8,9,9,9,9,4,4,10,11,10,7,14,10,9,9,11,7,7,11,10,4,7,10,8,13,11,11,9,12,10,8,10,10,10,14,10,10,10,5,7,6,10,6,5,8,9,7,9,8,7,9,9,4,5,9,4,14,9,9,9,9,6,7,7,9,8,12,8,9,8,6,5,6,10,0,
/* Format */
3,0,0,0,
/* Stride */
14,0,0,0,
/* Max Width */
14,0,0,0,
/* Max Height */
19,0,0,0,
/* Raw Data Address in Decimal: <582106> */
218,225,8,0
};

/**
 * Font header block for the Nirmala font, size 18.
 */
static const byte NirmalaBold18Header[148] = { 
/* Widths */
0,7,13,13,14,10,10,6,14,14,13,13,13,14,11,11,10,10,10,10,10,6,11,11,11,11,11,11,9,10,7,12,5,5,10,11,11,16,16,6,7,7,8,13,5,7,5,10,11,10,10,10,12,11,11,11,11,11,5,5,12,13,12,8,17,13,11,12,13,9,9,13,13,5,8,12,9,16,14,14,11,15,12,10,12,13,12,19,12,11,12,7,9,8,13,7,6,10,11,9,11,10,8,11,11,5,6,11,5,17,11,11,11,11,8,8,8,11,10,15,10,11,10,6,6,6,13,0,
/* Format */
3,0,0,0,
/* Stride */
20,0,0,0,
/* Max Width */
20,0,0,0,
/* Max Height */
24,0,0,0,
/* Raw Data Address in Decimal: <581892> */ 
4,225,8,0
};

/**
 * Font header block for the Roboto font, size 14.
 */
static const byte Nirmala14Header[148] = { 
/* Widths */
0,5,9,9,9,7,7,4,11,11,10,10,10,10,9,8,7,7,7,7,7,4,8,8,8,8,8,8,7,7,5,6,4,3,5,8,3,12,11,3,5,5,6,10,3,6,3,7,8,8,7,7,9,8,8,8,8,8,3,3,10,10,10,6,13,9,8,9,10,7,6,10,9,3,5,8,6,12,10,11,8,11,9,8,9,9,9,13,9,9,9,5,7,4,10,6,4,7,8,7,8,8,5,8,8,3,5,7,3,11,8,9,8,8,5,6,5,8,7,10,7,8,8,5,3,5,10,0,
/* Format */
3,0,0,0,
/* Stride */
14,0,0,0,
/* Max Width */
14,0,0,0,
/* Max Height */
19,0,0,0,
/* Raw Data Address in Decimal: <-118> */ 
138,255,255,255
};

/**
 * Font header block for the Roboto font, size 20.
 */
static const byte Nirmala20Header[148] = { 
/* Widths */
0,8,13,14,14,10,10,6,15,15,14,14,14,15,12,11,10,10,10,10,10,5,12,12,11,11,11,11,9,10,5,9,5,6,8,12,6,17,16,5,6,7,8,13,4,7,4,9,11,10,11,10,12,11,11,11,11,11,4,4,14,13,14,9,20,13,12,13,14,10,10,14,14,5,7,12,10,18,15,15,11,16,13,11,12,14,13,18,12,12,13,6,9,7,14,8,5,10,12,9,12,10,7,12,12,4,7,11,5,18,12,12,12,12,8,8,8,11,10,15,9,11,10,6,5,6,14,0,
/* Format */
3,0,0,0,
/* Stride */
20,0,0,0,
/* Max Width */
20,0,0,0,
/* Max Height */
27,0,0,0,
/* Raw Data Address in Decimal: <-392> */ 
120,254,255,255
};

