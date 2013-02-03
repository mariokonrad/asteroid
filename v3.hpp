#ifndef __V3__HPP__
#define __V3__HPP__

#include <GL/gl.h>

class v3
{
	private:
		GLfloat x[3];
	public:
		v3(GLfloat = 0.0f, GLfloat = 0.0f, GLfloat = 0.0f);
		GLfloat & operator[](unsigned int i) { return x[i]; }
		const GLfloat & operator[](unsigned int i) const { return x[i]; }
		v3 & operator+=(const v3 &);
		v3 & max(GLfloat);
		v3 & wrap(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
		v3 & normalize(GLfloat = 1.0f);
		GLfloat len() const;
		GLfloat len2() const;
		friend v3 operator*(GLfloat, const v3 &);
		friend v3 operator*(const v3 &, GLfloat);
		friend v3 operator+(const v3 &, const v3 &);
		friend v3 operator-(const v3 &, const v3 &);
};

#endif
