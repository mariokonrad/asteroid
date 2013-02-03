#ifndef __ASTEROID__HPP__
#define __ASTEROID__HPP__

#include "Sprite.hpp"
#include <vector>

class Asteroid : public Sprite
{
	public:
		enum { MAX_LEVEL = 2 };
	private:
		int sides;
		int _level;
		GLfloat scale;
		GLfloat angle;
		GLfloat omega;
	public:
		Asteroid(int, int, const v3 &, const v3 &, GLfloat = 0.0f, GLfloat = 20.0f);
		virtual ~Asteroid() {}
		virtual void step(GLfloat);
		virtual void render();
		virtual Boundary boundary() const;
		virtual void wrap(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

		int level() const;
		unsigned int score() const;
		bool crack(std::vector<Asteroid *> &) const;
};

#endif
