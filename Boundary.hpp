#ifndef __BOUNDARY__HPP__
#define __BOUNDARY__HPP__

#include "v3.hpp"

class Boundary
{
	private:
		v3 x;
		GLfloat r;
	public:
		Boundary(const v3 &, GLfloat);
		bool overlap(const Boundary &) const;
};

#endif
