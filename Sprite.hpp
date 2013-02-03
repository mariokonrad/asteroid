#ifndef __SPRITE__HPP__
#define __SPRITE__HPP__

#include "RenderObject.hpp"
#include "Boundary.hpp"
#include <vector>

class Sprite : public RenderObject
{
	public:
		class Wrap
		{
			private:
				GLfloat x0;
				GLfloat x1;
				GLfloat y0;
				GLfloat y1;
				GLfloat z0;
				GLfloat z1;
			public:
				Wrap(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1)
					: x0(x0), x1(x1)
					, y0(y0), y1(y1)
					, z0(z0), z1(z1)
				{}
				void operator()(Sprite * s) { s->wrap(x0, x1, y0, y1, z0, z1); }
		};
		class Collision
		{
			private:
				Sprite * s;
				std::vector<Sprite *> & c;
				Boundary b;
			public:
				Collision(Sprite * s, std::vector<Sprite *> & c)
					: s(s)
					, c(c)
					, b(s->boundary())
				{}

				void operator()(Sprite * a)
				{ if (b.overlap(a->boundary())) c.push_back(a); }
		};
	protected:
		v3 x;
		v3 v;
	public:
		Sprite();
		Sprite(const v3 &, const v3 &);
		virtual ~Sprite();
		virtual Boundary boundary() const = 0;
		virtual void wrap(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat) = 0;
		const v3 & pos() const;
		Sprite & pos(const v3 &);
		const v3 & vel() const;
		Sprite & vel(const v3 &);
};

#endif
