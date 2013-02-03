#ifndef __SHIPDECORATOR__HPP__
#define __SHIPDECORATOR__HPP__

#include "RenderObject.hpp"

class ShipDecorator : public RenderObject
{
	public:
		class Update
		{
			private:
				const v3 & x;
				const v3 & scale;
				GLfloat angle;
			public:
				Update(const v3 & x, const v3 & scale, GLfloat angle)
					: x(x)
					, scale(scale)
					, angle(angle)
				{}
				void operator()(ShipDecorator * s)
				{ if (s) s->update(x, scale, angle); }
		};
	protected:
		v3 x;
		v3 scale;
		GLfloat angle;
	public:
		virtual ~ShipDecorator();
		virtual void update(const v3 &, const v3 &, GLfloat);
		virtual bool alive() const = 0;
		static bool alive(const ShipDecorator *);
		static bool not_alive(const ShipDecorator *);
};

#endif
