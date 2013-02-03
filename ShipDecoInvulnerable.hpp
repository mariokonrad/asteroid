#ifndef __SHIPDECOINVULNERABLE__HPP__
#define __SHIPDECOINVULNERABLE__HPP__

#include "ShipDecorator.hpp"

class ShipDecoInvulnerable : public ShipDecorator
{
	private:
		GLfloat t;
		GLfloat c[4];
	public:
		ShipDecoInvulnerable(GLfloat);
		virtual ~ShipDecoInvulnerable() {}
		virtual void step(GLfloat);
		virtual void render();
		virtual bool alive() const;
};

#endif
