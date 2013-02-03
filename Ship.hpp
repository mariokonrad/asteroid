#ifndef __SHIP__HPP__
#define __SHIP__HPP__

#include "Sprite.hpp"
#include "Bullet.hpp"
#include "ShipDecorator.hpp"
#include "PowerUp.hpp"
#include <vector>

class Ship : public Sprite
{
	public:
		enum Decorators {
			 DECO_INVULNERABLE = 0
			,N_DECO
		};
	private:
		v3 a;
		v3 scale;
		GLfloat angle;
		unsigned int max_bullets;
		GLfloat bullet_speed;
		GLfloat bullet_time;
		GLfloat accel;
		GLfloat rot;
		GLfloat v_max;
		GLfloat color[4];
		std::vector<ShipDecorator *> decorator;
	private:
		void destroy_dead_decorators();
	public:
		Ship(const v3 & = v3());
		virtual ~Ship();
		virtual void step(GLfloat);
		virtual void render();
		virtual Boundary boundary() const;
		virtual void wrap(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

		void rot_left();
		void rot_right();
		void accelerate(bool);

		void shoot(std::vector<Bullet *> &);

		void invulnerable(GLfloat);
		bool invulnerable() const;

		void powerup(const PowerUp &);
};

#endif
