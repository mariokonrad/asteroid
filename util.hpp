#ifndef __UTIL__HPP__
#define __UTIL__HPP__

#include <vector>

template <typename T>
T sqr(const T & a)
{
	return a * a;
}

template <typename T>
void delete_all(std::vector<T> & c)
{
	while (c.size()) {
		T a = c.back();
		c.erase(c.end()-1);
		if (a) delete a;
	}
}

#endif
