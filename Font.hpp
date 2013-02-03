#ifndef __FONT__HPP__
#define __FONT__HPP__

#include "FontBase.hpp"

class Font : public FontBase
{
	private:
		v3 _scale;
		color_t _color;
		GLfloat _space;
		GLfloat _linewidth;
	public:
		Font(const v3 &, const color_t &, GLfloat = 1.0f, GLfloat = 1.0f);
		virtual ~Font();
		virtual v3 scale() const;
		virtual GLfloat space() const;
		virtual GLfloat linewidth() const;
		virtual const GLfloat * vertices() const;
		virtual const letter_t * letter(char) const;
		virtual const GLfloat * color() const;
};

#endif
