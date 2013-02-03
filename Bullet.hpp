#ifndef __BULLET__HPP__
#define __BULLET__HPP__

#include "Sprite.hpp"

class Bullet : public Sprite
{
	private:
		GLfloat t;
		GLfloat angle;
	public:
		Bullet(GLfloat, GLfloat, const v3 &, const v3 &);
		virtual ~Bullet() {}
		virtual void step(GLfloat);
		virtual void render();
		virtual Boundary boundary() const;
		virtual void wrap(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
		bool alive() const;
		static bool alive(const Bullet *);
		static bool not_alive(const Bullet *);
};

#endif
