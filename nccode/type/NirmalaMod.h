/**
 * @file NirmalaMod.h
 * @brief Font table data that pairs with the other font data on the SD card.
 *
 * These fonts were modified to contain accents to support multiple languages,
 * as a font on the FT81x can only have 127 characters (0x01 to 0x7F).
 *
 * Example command line used to generate these:
 * 	fnt_cvt.exe -i font -s size -u nirmala-mod-utf8.txt -d 582224
 *
 * Header blocks are 148 bytes long:
 *  * 128 bytes of character width for each character
 *  * 4 byte word for format (L8 = 3)
 *  * 4 byte word for stride 
 *  * 4 byte word for max character width
 *  * 4 byte word for max character height
 *  * 4 byte word for video RAM address of font data 
 */ 

using byte = unsigned char;

/**
 * Font header block for the Nirmala font, size 14.
 */
static const byte Nirmala14Header[148] = { 
/* Widths */
 0,  6, 11, 11, 11,  8,  8,  5, 11, 11,
10, 10, 10, 11, 10,  9,  7,  7,  7,  8,
 8,  5,  9,  9,  9,  9,  9,  9,  8, 14,
14,  9,  5,  5,  6,  9,  8, 13, 14,  4,
 6,  5,  7, 10,  4,  6,  4,  7,  9,  6,
 8,  8,  9,  8,  9,  9,  9,  9,  4,  4,
10, 10, 10,  8, 14, 11,  9, 10, 11,  8,
 8, 11, 10,  4,  7, 10,  8, 13, 11, 11,
 9, 10, 10,  9, 10, 10, 10, 14, 10,  9,
10,  6,  7,  5,  9,  7,  5,  8,  9,  8, // 100
 9,  8,  7,  9,  9,  4,  4,  9,  4, 14,
 9,  9,  9,  9,  7,  8,  7,  9,  9, 12,
 9,  8,  8,  6,  5,  7, 10,          0,

/* Format */
3,0,0,0,
/* Stride */
14,0,0,0,
/* Max Width */
14,0,0,0,
/* Max Height */
19,0,0,0,
/* Raw Data Address in Decimal: <582106> */ 
0x00, 0xe4, 0x08, 0x00
};

/**
 * Font header block for the Nirmala font, size 18.
 */
static const byte Nirmala18Header[148] = { 
/* Widths */
0,7,13,13,14,10,10,6,14,14,13,13,13,14,11,11,10,10,10,10,10,6,11,11,11,11,11,11,9,16,16,12,5,5,10,11,11,16,16,6,7,7,8,13,5,7,5,10,11,10,10,10,12,11,11,11,11,11,5,5,12,13,12,8,17,13,11,12,13,9,9,13,13,5,8,12,9,16,14,14,11,15,12,10,12,13,12,19,12,11,12,7,9,8,13,7,6,10,11,9,11,10,8,11,11,5,6,11,5,17,11,11,11,11,8,8,8,11,10,15,10,11,10,6,6,6,13, 0,
/* Format */
3,0,0,0,
/* Stride */
20,0,0,0,
/* Max Width */
20,0,0,0,
/* Max Height */
24,0,0,0,
/* Raw Data Address in Decimal: <581892> */ 
4,225,8,0,
};

/**
 * Font header block for the Roboto font, size 14.
 */
static const byte Roboto14Header[148] = { 
/* Widths */
0,5,9,10,9,8,8,4,10,10,9,9,9,10,9,8,8,8,8,7,7,4,8,8,8,8,8,8,7,11,11,6,3,3,5,9,8,11,9,3,5,5,7,9,3,5,3,6,8,8,8,8,9,8,8,9,8,8,3,3,8,8,8,7,13,9,8,9,9,8,7,9,9,3,8,9,7,12,9,10,9,10,9,9,10,9,9,13,9,9,9,4,6,4,6,8,4,8,8,7,8,8,6,8,8,3,4,7,3,13,8,8,8,8,5,8,6,8,7,11,7,7,8,5,3,5,10, 0,
/* Format */
3,0,0,0,
/* Stride */
14,0,0,0,
/* Max Width */
14,0,0,0,
/* Max Height */
16,0,0,0,
/* Raw Data Address in Decimal: <582148> */ 
4,226,8,0,
};

/**
 * Font header block for the Roboto font, size 20.
 */
static const byte Roboto20Header[148] = { 
/* Widths */
0,7,13,14,14,12,12,7,14,14,13,13,13,14,13,12,11,11,11,11,11,6,11,11,11,11,11,11,10,16,16,9,5,6,6,13,11,15,13,4,7,7,10,12,4,7,4,8,11,11,11,11,12,12,11,12,11,11,4,4,10,10,10,10,18,13,13,13,13,12,12,13,15,6,12,13,11,18,15,13,13,13,13,12,13,12,13,18,13,12,12,5,9,6,8,11,6,10,11,11,11,11,8,11,10,5,5,10,5,17,10,12,11,12,7,10,8,10,10,15,10,10,10,7,6,8,13, 0,
/* Format */
3,0,0,0,
/* Stride */
18,0,0,0,
/* Max Width */
18,0,0,0,
/* Max Height */
23,0,0,0,
/* Raw Data Address in Decimal: <581958> */ 
70,225,8,0,
};

