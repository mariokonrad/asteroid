#include "PowerUp.hpp"
#include <cmath>

PowerUp::PowerUp(GLfloat t)
	: t(t)
	, scale(10.0f)
	, angle(0.0f)
{}

PowerUp::~PowerUp()
{}

Boundary PowerUp::boundary() const
{
	return Boundary(x, scale);
}

void PowerUp::wrap(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1)
{
	x.wrap(x0, x1, y0, y1, z0, z1);
}

void PowerUp::step(GLfloat dt)
{
	// rotation of the ring
	angle += 360.0f * 0.5f * dt;
	while (angle > 360.0f) angle -= 360.0f;

	// life time decrease
	t -= dt;
	if (t < 0.0f) t = 0.0f;

	// position
	x += dt * v;
	step_specific(dt);
}

void PowerUp::render()
{
	static const int SIDES = 12;
	static const GLfloat R0 = 1.2f;
	static const GLfloat R1 = 1.5f;

	glPushMatrix();

	// translate to position
	glTranslatef(x[0], x[1], x[2]);
	glScalef(scale, scale, 1.0f);

	render_specific();

	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		for (int i = 0; i < SIDES; i += 2) {
			GLfloat p0 = M_PI * 2.0f / SIDES * (i+0);
			GLfloat p1 = M_PI * 2.0f / SIDES * (i+1);
			GLfloat c0= cos(p0);
			GLfloat s0= sin(p0);
			GLfloat c1= cos(p1);
			GLfloat s1= sin(p1);
			glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
			glVertex3f(R0 * c1, R0 * s1, 0.0f);
			glVertex3f(R1 * c1, R1 * s1, 0.0f);
			glVertex3f(R1 * c0, R1 * s0, 0.0f);
			glVertex3f(R0 * c0, R0 * s0, 0.0f);
		}
	glEnd();

	glPopMatrix();
}

bool PowerUp::alive() const
{
	return t > 0.0f;
}

bool PowerUp::alive(const PowerUp * o)
{
	return o && o->alive();
}

bool PowerUp::not_alive(const PowerUp * o)
{
	return !(o && o->alive());
}

