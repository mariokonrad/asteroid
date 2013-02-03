#include "v3.hpp"
#include "util.hpp"
#include <cmath>

v3::v3(GLfloat v0, GLfloat v1, GLfloat v2)
{
	x[0] = v0;
	x[1] = v1;
	x[2] = v2;
}

v3 operator*(GLfloat f, const v3 & v)
{
	return v3(v.x[0] * f, v.x[1] * f, v.x[2] * f);
}

v3 operator*(const v3 & v, GLfloat f)
{
	return v3(v.x[0] * f, v.x[1] * f, v.x[2] * f);
}

v3 operator+(const v3 & a, const v3 & b)
{
	return v3(
		a.x[0] + b.x[0],
		a.x[1] + b.x[1],
		a.x[2] + b.x[2]);
}

v3 operator-(const v3 & a, const v3 & b)
{
	return v3(
		a.x[0] - b.x[0],
		a.x[1] - b.x[1],
		a.x[2] - b.x[2]);
}

v3 & v3::operator+=(const v3 & v)
{
	for (int i = 0; i < 3; ++i) x[i] += v.x[i];
	return *this;
}

v3 & v3::max(GLfloat v)
{
	for (int i = 0; i < 3; ++i)
		if (x[i] > v)
			x[i] = v;
	return *this;
}

v3 & v3::wrap(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1)
{
	if (x[0] < x0) x[0] = x1;
	if (x[0] > x1) x[0] = x0;
	if (x[1] < y0) x[1] = y1;
	if (x[1] > y1) x[1] = y0;
	if (x[2] < z0) x[2] = z1;
	if (x[2] > z1) x[2] = z0;
	return *this;
}

GLfloat v3::len() const
{
	return sqrt(len2());
}

GLfloat v3::len2() const
{
	return sqr(x[0]) + sqr(x[1]) + sqr(x[2]);
}

v3 & v3::normalize(GLfloat l)
{
	GLfloat a = len();
	if (fabs(a) > 1e-7) {
		x[0] /= a;
		x[1] /= a;
		x[2] /= a;
		if (l != 1.0f) {
			x[0] *= l;
			x[1] *= l;
			x[2] *= l;
		}
	}
	return *this;
}

