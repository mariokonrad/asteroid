#include "Boundary.hpp"
#include "util.hpp"

Boundary::Boundary(const v3 & x, GLfloat r)
	: x(x)
	, r(r)
{}

bool Boundary::overlap(const Boundary & o) const
{
	return (o.x - x).len2() <= (sqr(r) + sqr(o.r));
}

