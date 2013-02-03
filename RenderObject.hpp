#ifndef __RENDEROBJECT__HPP__
#define __RENDEROBJECT__HPP__

#include "v3.hpp"

class RenderObject
{
	public:
		struct Render
		{
			void operator()(RenderObject * s) { if (s) s->render(); }
		};
		class Step
		{
			private:
				GLfloat dt;
			public:
				Step(GLfloat dt) : dt(dt) {}
				void operator()(RenderObject * s) { if (s) s->step(dt); }
		};
	public:
		RenderObject();
		virtual ~RenderObject();
		virtual void step(GLfloat) = 0;
		virtual void render() = 0;
};

#endif
