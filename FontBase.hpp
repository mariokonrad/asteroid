#ifndef __FONTBASE__HPP__
#define __FONTBASE__HPP__

#include "v3.hpp"

class FontBase
{
	public:
		struct letter_t {
			const unsigned int * p;
			const unsigned int size;
		};
		struct color_t {
			GLfloat r, g, b, a;

			color_t(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
				: r(r), g(g), b(b), a(a)
			{}
		};
	public:
		virtual ~FontBase();
		virtual v3 scale() const = 0;
		virtual GLfloat space() const = 0;
		virtual GLfloat linewidth() const = 0;
		virtual const GLfloat * vertices() const = 0;
		virtual const letter_t * letter(char) const = 0;
		virtual const GLfloat * color() const = 0;
};

#endif
