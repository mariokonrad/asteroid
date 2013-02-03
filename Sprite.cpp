#include "Sprite.hpp"

Sprite::Sprite()
{}

Sprite::Sprite(const v3 & x, const v3 & v)
	: x(x)
	, v(v)
{}

Sprite::~Sprite()
{}

const v3 & Sprite::pos() const
{
	return x;
}

Sprite & Sprite::pos(const v3 & x)
{
	this->x = x;
	return *this;
}

const v3 & Sprite::vel() const
{
	return v;
}

Sprite & Sprite::vel(const v3 & v)
{
	this->v = v;
	return *this;
}

