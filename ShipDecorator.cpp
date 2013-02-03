#include "ShipDecorator.hpp"

ShipDecorator::~ShipDecorator()
{}

void ShipDecorator::update(const v3 & x, const v3 & scale, GLfloat angle)
{
	this->x = x;
	this->scale = scale;
	this->angle = angle;
}

bool ShipDecorator::alive(const ShipDecorator * s)
{
	return s && s->alive();
}

bool ShipDecorator::not_alive(const ShipDecorator * s)
{
	return !(s && s->alive());
}

