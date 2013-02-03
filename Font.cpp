#include "Font.hpp"
#include <ctype.h>
#include <cstdlib>

static const GLfloat LETTER[][3] = {
/*
	10  28  29  30  21
	9       38      20
	8       37      19
	7       36      18
	6       35      17
	5   25  26  27  16
	4       34      15
	3       33      14
	2       32      13
	1       31      12
	0   22  23  24  11
*/
	// {{{
	{ 0.0f, 0.0f, 0.0f }, // [0..10] : left
	{ 0.0f, 0.1f, 0.0f },
	{ 0.0f, 0.2f, 0.0f },
	{ 0.0f, 0.3f, 0.0f },
	{ 0.0f, 0.4f, 0.0f },
	{ 0.0f, 0.5f, 0.0f },
	{ 0.0f, 0.6f, 0.0f },
	{ 0.0f, 0.7f, 0.0f },
	{ 0.0f, 0.8f, 0.0f },
	{ 0.0f, 0.9f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },

	{ 1.0f, 0.0f, 0.0f }, // [11..21] : right
	{ 1.0f, 0.1f, 0.0f },
	{ 1.0f, 0.2f, 0.0f },
	{ 1.0f, 0.3f, 0.0f },
	{ 1.0f, 0.4f, 0.0f },
	{ 1.0f, 0.5f, 0.0f },
	{ 1.0f, 0.6f, 0.0f },
	{ 1.0f, 0.7f, 0.0f },
	{ 1.0f, 0.8f, 0.0f },
	{ 1.0f, 0.9f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },

	{ 0.25f, 0.0f, 0.0f }, // [22..24] : bottom
	{ 0.50f, 0.0f, 0.0f },
	{ 0.75f, 0.0f, 0.0f },

	{ 0.25f, 0.5f, 0.0f }, // [25..27] : middle
	{ 0.50f, 0.5f, 0.0f },
	{ 0.75f, 0.5f, 0.0f },

	{ 0.25f, 1.0f, 0.0f }, // [28..30] : top
	{ 0.50f, 1.0f, 0.0f },
	{ 0.75f, 1.0f, 0.0f },

	{ 0.5f, 0.1f, 0.0f }, // [31..38] : center
	{ 0.5f, 0.2f, 0.0f },
	{ 0.5f, 0.3f, 0.0f },
	{ 0.5f, 0.4f, 0.0f },
	{ 0.5f, 0.6f, 0.0f },
	{ 0.5f, 0.7f, 0.0f },
	{ 0.5f, 0.8f, 0.0f },
	{ 0.5f, 0.9f, 0.0f },
	// }}}
};
static const unsigned int LETTER_A[] = { 0,10, 10,21, 21,11, 5,16 };
static const unsigned int LETTER_B[] = { 0,10, 5,27, 10,30, 30,20, 20,17, 17,27, 27,15, 15,12, 12,24, 24,0 };
static const unsigned int LETTER_C[] = { 20,30, 30,28, 28,9, 9,1, 1,22, 22,24, 24,12 };
static const unsigned int LETTER_D[] = { 0,10, 10,29, 29,19, 19,13, 13,23, 23,0 };
static const unsigned int LETTER_E[] = { 11,0, 0,10, 10,21, 5,26 };
static const unsigned int LETTER_F[] = { 0,10, 10,21, 5,26 };
static const unsigned int LETTER_G[] = { 26,16, 16,12, 12,24, 24,22, 22,1, 1,9, 9,28, 28,30, 30,20 };
static const unsigned int LETTER_H[] = { 0,10, 11,21, 5,16 };
static const unsigned int LETTER_I[] = { 0,11, 10,21, 23,29 };
static const unsigned int LETTER_J[] = { 10,21, 21,11, 11,22, 22,1, 1,3 };
static const unsigned int LETTER_K[] = { 0,10, 5,21, 5,11 };
static const unsigned int LETTER_L[] = { 10,0, 0,11 };
static const unsigned int LETTER_M[] = { 0,10, 10,26, 26,21, 21,11 };
static const unsigned int LETTER_N[] = { 0,10, 10,11, 11, 21 };
static const unsigned int LETTER_O[] = { 1,9, 9,28, 28,30, 30,20, 20,12, 12,24, 24,22, 22,1 };
static const unsigned int LETTER_P[] = { 0,10, 10,30, 30,20, 20,17, 17,27, 27,5 };
static const unsigned int LETTER_Q[] = { 1,9, 9,28, 28,30, 30,20, 20,12, 12,24, 24,22, 22,1, 11,26 };
static const unsigned int LETTER_R[] = { 0,10, 5,27, 10,30, 30,20, 20,17, 17,27, 26,11 };
static const unsigned int LETTER_S[] = { 1,22, 22,24, 24,12, 12,15, 15,27, 27,25, 25,6, 6,9, 9,28, 28,30, 30,20 };
static const unsigned int LETTER_T[] = { 10,21, 29,23 };
static const unsigned int LETTER_U[] = { 10,1, 1,22, 22,24, 24,12, 12,21 };
static const unsigned int LETTER_V[] = { 10,5, 5,23, 23,16, 16,21 };
static const unsigned int LETTER_W[] = { 10,0, 0,26, 26,11, 11,21 };
static const unsigned int LETTER_X[] = { 10,11, 0,21 };
static const unsigned int LETTER_Y[] = { 10,26, 26,21, 26,23 };
static const unsigned int LETTER_Z[] = { 10,21, 21,0, 0,11 };
static const unsigned int LETTER_0[] = { 1,9, 9,28, 28,30, 30,20, 20,12, 12,24, 24,22, 22,1, 22,30 };
static const unsigned int LETTER_1[] = { 7,30, 30,21, 21,11 };
static const unsigned int LETTER_2[] = { 8,9, 9,28, 28,30, 30,20, 20,17, 17,0, 0,11 };
static const unsigned int LETTER_3[] = { 9,28, 28,30, 30,20, 20,17, 17,27, 27,15, 15,12, 12,24, 24,22, 22,1, 27,26 };
static const unsigned int LETTER_4[] = { 10,5, 5,16, 21,11 };
static const unsigned int LETTER_5[] = { 21,10, 10,5, 5,27, 27,15, 15,12, 12,24, 24,0 };
static const unsigned int LETTER_6[] = { 20,30, 30,28, 28,9, 9,1, 1,22, 22,24, 24,12, 12,15, 15,27, 27,5 };
static const unsigned int LETTER_7[] = { 10,21, 21,11 };
static const unsigned int LETTER_8[] = { 1,4, 4,25, 25,6, 6,9, 9,28, 28,30, 30,20, 20,17, 17,27, 27,15, 15,12, 12,24, 24,22, 22,1, 25,27 };
static const unsigned int LETTER_9[] = { 1,22, 22,24, 24,12, 12,20, 20,30, 30,28, 28,9, 9,6, 6,25, 25,16 };
static const unsigned int LETTER_SPACE[] = {};
static const unsigned int LETTER_DASH[] = { 5,16 };
static const unsigned int LETTER_COLON[] = { 36,37, 32,33 };
static const unsigned int LETTER_POINT[] = { 31,32 };

static const FontBase::letter_t LETTERS[] = {
	{ LETTER_A, sizeof(LETTER_A)/sizeof(unsigned int) },
	{ LETTER_B, sizeof(LETTER_B)/sizeof(unsigned int) },
	{ LETTER_C, sizeof(LETTER_C)/sizeof(unsigned int) },
	{ LETTER_D, sizeof(LETTER_D)/sizeof(unsigned int) },
	{ LETTER_E, sizeof(LETTER_E)/sizeof(unsigned int) },
	{ LETTER_F, sizeof(LETTER_F)/sizeof(unsigned int) },
	{ LETTER_G, sizeof(LETTER_G)/sizeof(unsigned int) },
	{ LETTER_H, sizeof(LETTER_H)/sizeof(unsigned int) },
	{ LETTER_I, sizeof(LETTER_I)/sizeof(unsigned int) },
	{ LETTER_J, sizeof(LETTER_J)/sizeof(unsigned int) },
	{ LETTER_K, sizeof(LETTER_K)/sizeof(unsigned int) },
	{ LETTER_L, sizeof(LETTER_L)/sizeof(unsigned int) },
	{ LETTER_M, sizeof(LETTER_M)/sizeof(unsigned int) },
	{ LETTER_N, sizeof(LETTER_N)/sizeof(unsigned int) },
	{ LETTER_O, sizeof(LETTER_O)/sizeof(unsigned int) },
	{ LETTER_P, sizeof(LETTER_P)/sizeof(unsigned int) },
	{ LETTER_Q, sizeof(LETTER_Q)/sizeof(unsigned int) },
	{ LETTER_R, sizeof(LETTER_R)/sizeof(unsigned int) },
	{ LETTER_S, sizeof(LETTER_S)/sizeof(unsigned int) },
	{ LETTER_T, sizeof(LETTER_T)/sizeof(unsigned int) },
	{ LETTER_U, sizeof(LETTER_U)/sizeof(unsigned int) },
	{ LETTER_V, sizeof(LETTER_V)/sizeof(unsigned int) },
	{ LETTER_W, sizeof(LETTER_W)/sizeof(unsigned int) },
	{ LETTER_X, sizeof(LETTER_X)/sizeof(unsigned int) },
	{ LETTER_Y, sizeof(LETTER_Y)/sizeof(unsigned int) },
	{ LETTER_Z, sizeof(LETTER_Z)/sizeof(unsigned int) },
	{ LETTER_0, sizeof(LETTER_0)/sizeof(unsigned int) },
	{ LETTER_1, sizeof(LETTER_1)/sizeof(unsigned int) },
	{ LETTER_2, sizeof(LETTER_2)/sizeof(unsigned int) },
	{ LETTER_3, sizeof(LETTER_3)/sizeof(unsigned int) },
	{ LETTER_4, sizeof(LETTER_4)/sizeof(unsigned int) },
	{ LETTER_5, sizeof(LETTER_5)/sizeof(unsigned int) },
	{ LETTER_6, sizeof(LETTER_6)/sizeof(unsigned int) },
	{ LETTER_7, sizeof(LETTER_7)/sizeof(unsigned int) },
	{ LETTER_8, sizeof(LETTER_8)/sizeof(unsigned int) },
	{ LETTER_9, sizeof(LETTER_9)/sizeof(unsigned int) },
	{ LETTER_SPACE, sizeof(LETTER_SPACE)/sizeof(unsigned int) },
	{ LETTER_DASH, sizeof(LETTER_DASH)/sizeof(unsigned int) },
	{ LETTER_COLON, sizeof(LETTER_COLON)/sizeof(unsigned int) },
	{ LETTER_POINT, sizeof(LETTER_POINT)/sizeof(unsigned int) },
};

Font::Font(const v3 & scale, const color_t & color,
	GLfloat space, GLfloat linewidth)
	: _scale(scale)
	, _color(color)
	, _space(space)
	, _linewidth(linewidth)
{}

Font::~Font()
{}

v3 Font::scale() const
{
	return _scale;
}

GLfloat Font::space() const
{
	return _space;
}

GLfloat Font::linewidth() const
{
	return _linewidth;
}

const GLfloat * Font::vertices() const
{
	return reinterpret_cast<const GLfloat *>(LETTER);
}

const FontBase::letter_t * Font::letter(char c) const
{
	c = toupper(c);
	if (isdigit(c)) return &LETTERS[c-'0'+('Z'-'A'+1)];
	if (isalpha(c)) return &LETTERS[c-'A'];
	if (c == ' ') return &LETTERS['Z'-'A'+1+10];
	if (c == '-') return &LETTERS['Z'-'A'+1+10+1];
	if (c == ':') return &LETTERS['Z'-'A'+1+10+2];
	if (c == '.') return &LETTERS['Z'-'A'+1+10+3];
	return NULL;
}

const GLfloat * Font::color() const
{
	return reinterpret_cast<const GLfloat *>(&_color);
}

