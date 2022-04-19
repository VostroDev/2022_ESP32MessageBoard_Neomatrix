#ifndef Font12x16_h
#define Font12x16_h

/*
NOTE: Each character must be defined as maximum width & height
      and the first uint8_t on each line is the actual width
	  :;|- changed
	  10, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xC0,0x3F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// -
*/

const uint8_t Font12x16Data[] = {
			FONT_PROPORTIONAL | 12,  // Font Maximum Width
			16, // Font Height
			32, // Font First Character x20 hex
			127,// Font Last Character  x7f hex
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//    space 32 dec x20 hex
			12, 0x06,0x00,0x06,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,	// !
			12, 0x00,0x00,0x00,0x00,0x19,0x80,0x19,0x80,0x19,0x80,0x19,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// "
			12, 0x00,0x00,0x06,0x60,0x06,0x60,0x06,0x60,0x3F,0xF0,0x0C,0xC0,0x0C,0xC0,0x19,0x80,0x19,0x80,0x7F,0xC0,0x33,0x00,0x33,0x00,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// #
			12, 0x06,0x00,0x06,0x00,0x1F,0x80,0x3F,0xC0,0x36,0x00,0x36,0x00,0x3F,0x80,0x1F,0xC0,0x06,0xC0,0x06,0xC0,0x3F,0xC0,0x1F,0x80,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,	// $
			12, 0x00,0x00,0x00,0x00,0x00,0x10,0x38,0x30,0x38,0x70,0x38,0xE0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x00,0x1C,0x00,0x38,0xE0,0x70,0xE0,0x60,0xE0,0x00,0x00,0x00,0x00,	// %
			12, 0x00,0x00,0x07,0x00,0x0D,0x80,0x19,0x80,0x19,0x80,0x1B,0x00,0x0E,0x00,0x1E,0x00,0x3E,0x00,0x33,0x60,0x33,0xC0,0x31,0x80,0x3B,0xC0,0x1E,0x60,0x00,0x00,0x00,0x00,	// &
			12, 0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// '
			12, 0x03,0x80,0x06,0x00,0x0E,0x00,0x0C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x1C,0x00,0x0C,0x00,0x0E,0x00,0x06,0x00,0x03,0x80,0x00,0x00,0x00,0x00,	// (
			12, 0x1C,0x00,0x06,0x00,0x07,0x00,0x03,0x00,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x00,0x07,0x00,0x06,0x00,0x1C,0x00,0x00,0x00,0x00,0x00,	// )
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x36,0xC0,0x36,0xC0,0x1F,0x80,0x0F,0x00,0x3F,0xC0,0x0F,0x00,0x1F,0x80,0x36,0xC0,0x36,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// *
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x3F,0xC0,0x3F,0xC0,0x06,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// +
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x00,0x0C,0x00,	// ,
            10, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x80,0x3F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    // -
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,	// .
			12, 0x00,0x00,0x00,0x10,0x00,0x30,0x00,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x00,0x1C,0x00,0x38,0x00,0x70,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// /
			12, 0x0F,0x80,0x3F,0xE0,0x30,0x60,0x60,0x70,0x60,0xF0,0x61,0xB0,0x63,0x30,0x66,0x30,0x6C,0x30,0x78,0x30,0x70,0x30,0x30,0x60,0x3F,0xE0,0x0F,0x80,0x00,0x00,0x00,0x00,	// 0
			12, 0x03,0x00,0x07,0x00,0x1F,0x00,0x1F,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x1F,0xE0,0x1F,0xE0,0x00,0x00,0x00,0x00,	// 1
			12, 0x1F,0xC0,0x3F,0xE0,0x70,0x70,0x60,0x30,0x60,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x00,0x1C,0x00,0x38,0x00,0x7F,0xF0,0x7F,0xF0,0x00,0x00,0x00,0x00,	// 2
			12, 0x1F,0xC0,0x3F,0xE0,0x70,0x70,0x60,0x30,0x00,0x30,0x00,0x70,0x0F,0xE0,0x0F,0xC0,0x00,0x60,0x00,0x30,0x60,0x30,0x70,0x70,0x3F,0xE0,0x1F,0xC0,0x00,0x00,0x00,0x00,	// 3
			12, 0x01,0xC0,0x03,0xC0,0x07,0xC0,0x0E,0xC0,0x1C,0xC0,0x38,0xC0,0x70,0xC0,0x60,0xC0,0x7F,0xF0,0x7F,0xF0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,	// 4
			12, 0x7F,0xF0,0x7F,0xF0,0x60,0x00,0x60,0x00,0x60,0x00,0x7F,0xC0,0x3F,0xE0,0x00,0x70,0x00,0x30,0x00,0x30,0x60,0x30,0x70,0x70,0x3F,0xE0,0x1F,0xC0,0x00,0x00,0x00,0x00,	// 5
			12, 0x03,0xC0,0x07,0xC0,0x0E,0x00,0x1C,0x00,0x38,0x00,0x30,0x00,0x7F,0xC0,0x7F,0xE0,0x70,0x70,0x60,0x30,0x60,0x30,0x70,0x70,0x3F,0xE0,0x1F,0xC0,0x00,0x00,0x00,0x00,	// 6
			12, 0x7F,0xF0,0x7F,0xF0,0x00,0x60,0x00,0x60,0x00,0xC0,0x00,0xC0,0x01,0x80,0x01,0x80,0x03,0x00,0x03,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,	// 7
			12, 0x0F,0x80,0x1F,0xC0,0x38,0xE0,0x30,0x60,0x30,0x60,0x38,0xE0,0x1F,0xC0,0x3F,0xE0,0x70,0x70,0x60,0x30,0x60,0x30,0x70,0x70,0x3F,0xE0,0x1F,0xC0,0x00,0x00,0x00,0x00,	// 8
			12, 0x1F,0xC0,0x3F,0xE0,0x70,0x70,0x60,0x30,0x60,0x30,0x70,0x70,0x3F,0xF0,0x1F,0xF0,0x00,0x60,0x00,0xE0,0x01,0xC0,0x03,0x80,0x1F,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,	// 9
			10, 0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x00,0x0e,0x00,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x00,0x0e,0x00,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// : RW widen
			10, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,	// ;
			12, 0x00,0xC0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x00,0x1C,0x00,0x38,0x00,0x38,0x00,0x1C,0x00,0x0E,0x00,0x07,0x00,0x03,0x80,0x01,0xC0,0x00,0xC0,0x00,0x00,0x00,0x00,	// <
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0xE0,0x3F,0xE0,0x00,0x00,0x00,0x00,0x3F,0xE0,0x3F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// =
			12, 0x30,0x00,0x38,0x00,0x1C,0x00,0x0E,0x00,0x07,0x00,0x03,0x80,0x01,0xC0,0x01,0xC0,0x03,0x80,0x07,0x00,0x0E,0x00,0x1C,0x00,0x38,0x00,0x30,0x00,0x00,0x00,0x00,0x00,	// >
			12, 0x1F,0x80,0x3F,0xC0,0x70,0xE0,0x60,0x60,0x60,0xE0,0x01,0xC0,0x03,0x80,0x07,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,	// ?
			12, 0x1F,0xC0,0x3F,0xE0,0x30,0x60,0x67,0xB0,0x6F,0xB0,0x6D,0xB0,0x6D,0xB0,0x6D,0xB0,0x6D,0xB0,0x6F,0xE0,0x67,0xC0,0x70,0x00,0x3F,0xC0,0x0F,0xC0,0x00,0x00,0x00,0x00,	// @
			12, 0x06,0x00,0x06,0x00,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x19,0x80,0x19,0x80,0x19,0x80,0x30,0xC0,0x3F,0xC0,0x3F,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00,	// A
			12, 0x7F,0x00,0x7F,0x80,0x61,0xC0,0x60,0xC0,0x60,0xC0,0x61,0xC0,0x7F,0x80,0x7F,0xC0,0x60,0xE0,0x60,0x60,0x60,0x60,0x60,0xE0,0x7F,0xC0,0x7F,0x80,0x00,0x00,0x00,0x00,	// B
			12, 0x0F,0x80,0x1F,0xC0,0x38,0xE0,0x30,0x60,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x30,0x60,0x38,0xE0,0x1F,0xC0,0x0F,0x80,0x00,0x00,0x00,0x00,	// C
			12, 0x7F,0x00,0x7F,0x80,0x61,0xC0,0x60,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xC0,0x61,0xC0,0x7F,0x80,0x7F,0x00,0x00,0x00,0x00,0x00,	// D
			12, 0x7F,0xE0,0x7F,0xE0,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x7F,0x80,0x7F,0x80,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x7F,0xE0,0x7F,0xE0,0x00,0x00,0x00,0x00,	// E
			12, 0x7F,0xE0,0x7F,0xE0,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x7F,0x80,0x7F,0x80,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,	// F
			12, 0x0F,0xC0,0x1F,0xE0,0x38,0x60,0x30,0x00,0x60,0x00,0x60,0x00,0x63,0xE0,0x63,0xE0,0x60,0x60,0x60,0x60,0x30,0x60,0x38,0x60,0x1F,0xE0,0x0F,0xE0,0x00,0x00,0x00,0x00,	// G
			12, 0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x7F,0xE0,0x7F,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00,	// H
			12, 0x1F,0x80,0x1F,0x80,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x1F,0x80,0x1F,0x80,0x00,0x00,0x00,0x00,	// I
			12, 0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x60,0x60,0x60,0x60,0x70,0xC0,0x3F,0xC0,0x1F,0x80,0x00,0x00,0x00,0x00,	// J
			12, 0x60,0x60,0x60,0xE0,0x61,0xC0,0x63,0x80,0x67,0x00,0x6E,0x00,0x7C,0x00,0x7C,0x00,0x6E,0x00,0x67,0x00,0x63,0x80,0x61,0xC0,0x60,0xE0,0x60,0x60,0x00,0x00,0x00,0x00,	// K
			12, 0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x7F,0xE0,0x7F,0xE0,0x00,0x00,0x00,0x00,	// L
			12, 0x60,0x60,0x70,0xE0,0x70,0xE0,0x79,0xE0,0x79,0xE0,0x6F,0x60,0x6F,0x60,0x66,0x60,0x66,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00,	// M
			12, 0x60,0x60,0x70,0x60,0x70,0x60,0x78,0x60,0x6C,0x60,0x6C,0x60,0x66,0x60,0x66,0x60,0x63,0x60,0x63,0x60,0x61,0xE0,0x60,0xE0,0x60,0xE0,0x60,0x60,0x00,0x00,0x00,0x00,	// N
			12, 0x0F,0x00,0x1F,0x80,0x39,0xC0,0x30,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x30,0xC0,0x39,0xC0,0x1F,0x80,0x0F,0x00,0x00,0x00,0x00,0x00,	// O
			12, 0x7F,0x80,0x7F,0xC0,0x60,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xE0,0x7F,0xC0,0x7F,0x80,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x00,0x00,0x00,0x00,	// P
			12, 0x0F,0x00,0x1F,0x80,0x39,0xC0,0x30,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x63,0x60,0x33,0xC0,0x39,0xC0,0x1F,0xE0,0x0F,0x60,0x00,0x00,0x00,0x00,	// Q
			12, 0x7F,0x80,0x7F,0xC0,0x60,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xE0,0x7F,0xC0,0x7F,0x80,0x67,0x00,0x63,0x80,0x61,0xC0,0x60,0xE0,0x60,0x60,0x00,0x00,0x00,0x00,	// R
			12, 0x1F,0x80,0x3F,0xC0,0x70,0xE0,0x60,0x60,0x60,0x00,0x70,0x00,0x3F,0x80,0x1F,0xC0,0x00,0xE0,0x00,0x60,0x60,0x60,0x70,0xE0,0x3F,0xC0,0x1F,0x80,0x00,0x00,0x00,0x00,	// S
			12, 0x3F,0xC0,0x3F,0xC0,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,	// T
			12, 0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x30,0xC0,0x3F,0xC0,0x1F,0x80,0x00,0x00,0x00,0x00,	// U
			12, 0x60,0x60,0x60,0x60,0x60,0x60,0x30,0xC0,0x30,0xC0,0x30,0xC0,0x19,0x80,0x19,0x80,0x19,0x80,0x0F,0x00,0x0F,0x00,0x0F,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,	// V
			12, 0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x66,0x60,0x66,0x60,0x6F,0x60,0x79,0xE0,0x70,0xE0,0x70,0xE0,0x60,0x60,0x00,0x00,0x00,0x00,	// W
			12, 0x60,0x60,0x60,0x60,0x30,0xC0,0x30,0xC0,0x19,0x80,0x0F,0x00,0x06,0x00,0x06,0x00,0x0F,0x00,0x19,0x80,0x30,0xC0,0x30,0xC0,0x60,0x60,0x60,0x60,0x00,0x00,0x00,0x00,	// X
			12, 0x60,0x60,0x60,0x60,0x30,0xC0,0x30,0xC0,0x19,0x80,0x19,0x80,0x0F,0x00,0x0F,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x00,0x00,	// Y
			12, 0x7F,0xE0,0x7F,0xE0,0x00,0xC0,0x00,0xC0,0x01,0x80,0x03,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,0x30,0x00,0x30,0x00,0x7F,0xE0,0x7F,0xE0,0x00,0x00,0x00,0x00,	// Z
			12, 0x1F,0x80,0x1F,0x80,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x1F,0x80,0x1F,0x80,0x00,0x00,0x00,0x00,	// [
			12, 0x00,0x00,0x40,0x00,0x60,0x00,0x70,0x00,0x38,0x00,0x1C,0x00,0x0E,0x00,0x07,0x00,0x03,0x80,0x01,0xC0,0x00,0xE0,0x00,0x70,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,	// Backslash
			12, 0x1F,0x80,0x1F,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x1F,0x80,0x1F,0x80,0x00,0x00,0x00,0x00,	// ]
			12, 0x70,0x00,0xd8,0x00,0xd8,0x00,0x70,0x00,0x00,0x00,0x0f,0xc0,0x1f,0xe0,0x38,0x30,0x30,0x00,0x30,0x00,0x30,0x00,0x38,0x30,0x3f,0xe0,0x1f,0xc0,0x00,0x00,0x00,0x00,	// ^  RW degC
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xF0,0x7F,0xF0,	// _
			12, 0x00,0x00,0x07,0x00,0x07,0x00,0x07,0x00,0x06,0x00,0x06,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// `
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xC0,0x3F,0xE0,0x00,0x60,0x1F,0xE0,0x3F,0xE0,0x60,0x60,0x60,0x60,0x7F,0xE0,0x3F,0xE0,0x00,0x00,0x00,0x00,	// a
			12, 0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x6F,0x80,0x7F,0xC0,0x70,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xE0,0x7F,0xC0,0x7F,0x80,0x00,0x00,0x00,0x00,	// b
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x3F,0xC0,0x70,0x60,0x60,0x00,0x60,0x00,0x60,0x00,0x70,0x60,0x3F,0xC0,0x1F,0x80,0x00,0x00,0x00,0x00,	// c
			12, 0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x1F,0x60,0x3F,0xE0,0x71,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,0x70,0x60,0x3F,0xE0,0x1F,0xE0,0x00,0x00,0x00,0x00,	// d
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x3F,0xC0,0x70,0x60,0x7F,0xE0,0x7F,0xC0,0x60,0x00,0x70,0x00,0x3F,0xC0,0x1F,0x80,0x00,0x00,0x00,0x00,	// e
			12, 0x07,0x80,0x0F,0x80,0x1C,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x7F,0x00,0x7F,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x00,0x00,0x00,0x00,	// f
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xE0,0x3F,0xE0,0x70,0x60,0x60,0x60,0x70,0xE0,0x3F,0xE0,0x1F,0x60,0x00,0x60,0x00,0xE0,0x3F,0xC0,0x3F,0x80,	// g
			12, 0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x6F,0x00,0x7F,0x80,0x71,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x00,0x00,0x00,0x00,	// h
			12, 0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x00,0x00,0x0E,0x00,0x0E,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x1F,0x80,0x1F,0x80,0x00,0x00,0x00,0x00,	// i
			12, 0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x00,0x00,0x03,0x80,0x03,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x19,0x80,0x1F,0x80,0x0F,0x00,	// j
			12, 0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x31,0x80,0x33,0x80,0x37,0x00,0x3E,0x00,0x3E,0x00,0x37,0x00,0x33,0x80,0x31,0xC0,0x30,0xC0,0x00,0x00,0x00,0x00,	// k
			12, 0x0E,0x00,0x0E,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,0x1F,0x80,0x1F,0x80,0x00,0x00,0x00,0x00,	// l
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x59,0x80,0x7F,0xC0,0x7F,0xE0,0x66,0x60,0x66,0x60,0x66,0x60,0x66,0x60,0x66,0x60,0x66,0x60,0x00,0x00,0x00,0x00,	// m
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x80,0x3F,0xC0,0x30,0xE0,0x30,0x60,0x30,0x60,0x30,0x60,0x30,0x60,0x30,0x60,0x30,0x60,0x00,0x00,0x00,0x00,	// n
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x80,0x3F,0xC0,0x70,0xE0,0x60,0x60,0x60,0x60,0x60,0x60,0x70,0xE0,0x3F,0xC0,0x1F,0x80,0x00,0x00,0x00,0x00,	// o
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x80,0x7F,0xC0,0x60,0xE0,0x60,0x60,0x60,0x60,0x70,0xE0,0x7F,0xC0,0x6F,0x80,0x60,0x00,0x60,0x00,0x60,0x00,	// p
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xE0,0x3F,0xE0,0x70,0x60,0x60,0x60,0x60,0x60,0x70,0xE0,0x3F,0xE0,0x1F,0x60,0x00,0x60,0x00,0x60,0x00,0x60,	// q
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x37,0xC0,0x3F,0xE0,0x38,0x60,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,	// r
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x7F,0x80,0x60,0x00,0x7F,0x00,0x3F,0x80,0x01,0x80,0x01,0x80,0x7F,0x80,0x3F,0x00,0x00,0x00,0x00,0x00,	// s
			12, 0x00,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x7F,0x00,0x7F,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,0x1F,0x80,0x0F,0x80,0x00,0x00,0x00,0x00,	// t
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x70,0xE0,0x3F,0xE0,0x1F,0x60,0x00,0x00,0x00,0x00,	// u
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x60,0x60,0x30,0xC0,0x30,0xC0,0x19,0x80,0x19,0x80,0x0F,0x00,0x0F,0x00,0x06,0x00,0x00,0x00,0x00,0x00,	// v
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x60,0x66,0x60,0x66,0x60,0x66,0x60,0x66,0x60,0x6F,0x60,0x3F,0xC0,0x39,0xC0,0x10,0x80,0x00,0x00,0x00,0x00,	// w
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0xC0,0x71,0xC0,0x3B,0x80,0x1F,0x00,0x0E,0x00,0x1F,0x00,0x3B,0x80,0x71,0xC0,0x60,0xC0,0x00,0x00,0x00,0x00,	// x
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0xC0,0x30,0xC0,0x19,0x80,0x19,0x80,0x0F,0x00,0x0F,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,0x0C,0x00,0x18,0x00,	// y
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xC0,0x7F,0x80,0x03,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,0x30,0x00,0x7F,0xC0,0x7F,0xC0,0x00,0x00,0x00,0x00,	// z
			12, 0x03,0xC0,0x07,0xC0,0x0E,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x1C,0x00,0x38,0x00,0x1C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0E,0x00,0x07,0xC0,0x03,0xC0,0x00,0x00,	// {
			10, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x00,0x0e,0x00,0x0e,0x00,0x0e,0x00,0x0e,0x00,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// | RW colon dot
			12, 0x3C,0x00,0x3E,0x00,0x07,0x00,0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x80,0x01,0xC0,0x03,0x80,0x03,0x00,0x03,0x00,0x03,0x00,0x07,0x00,0x3E,0x00,0x3C,0x00,0x00,0x00,	// }
			12, 0x00,0x00,0x00,0x00,0x1C,0x60,0x36,0xC0,0x63,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// ~
			12, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x00,0x19,0x80,0x30,0xC0,0x60,0x60,0x60,0x60,0x7F,0xE0,0x7F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	    // 
			};
#endif
