#ifndef __POWERUPSHIELD__HPP__
#define __POWERUPSHIELD__HPP__

#include "PowerUp.hpp"

class PowerUpShield : public PowerUp
{
	public:
		enum { TYPE = 1 };
	protected:
		virtual void step_specific(GLfloat);
		virtual void render_specific();
	public:
		virtual ~PowerUpShield();
		virtual int type() const;
		GLfloat time() const;
};

#endif
