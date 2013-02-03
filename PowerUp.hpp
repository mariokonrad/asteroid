#ifndef __POWERUP__HPP__
#define __POWERUP__HPP__

#include "Sprite.hpp"

class PowerUp : public Sprite
{
	private:
		GLfloat t;
		GLfloat scale;
		GLfloat angle;
	protected:
		virtual void step_specific(GLfloat) = 0;
		virtual void render_specific() = 0;
	public:
		PowerUp(GLfloat = 20.0f);
		virtual ~PowerUp();
		virtual Boundary boundary() const;
		virtual void wrap(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
		virtual int type() const = 0;
		virtual void step(GLfloat);
		virtual void render();
		bool alive() const;
		static bool alive(const PowerUp *);
		static bool not_alive(const PowerUp *);
};

#endif
