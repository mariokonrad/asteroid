#include "Bullet.hpp"

Bullet::Bullet(GLfloat t, GLfloat angle, const v3 & x, const v3 & v)
	: Sprite(x, v)
	, t(t)
	, angle(angle)
{}

bool Bullet::alive() const
{
	return t > 0.0f;
}

void Bullet::step(GLfloat dt)
{
	// calc lifetime
	if (t > 0.0f) t -= dt;

	// position
	x += dt * v;
}

void Bullet::wrap(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1)
{
	x.wrap(x0, x1, y0, y1, z0, z1);
}

void Bullet::render()
{
	glPushMatrix();

	// translate to position
	glTranslatef(x[0], x[1], x[2]);

	// rotate to draw in flight direction
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	// draw bullet
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glScalef(5.0f, 2.0f, 1.0f);
	glBegin(GL_POLYGON);
		glVertex3f( 1.0f,  0.0f, 0.0f);
		glVertex3f( 0.0f,  1.0f, 0.0f);
		glVertex3f(-1.0f,  0.0f, 0.0f);
		glVertex3f( 0.0f, -1.0f, 0.0f);
	glEnd();

	glPopMatrix();
}

Boundary Bullet::boundary() const
{
	return Boundary(x, 2.0f);
}

bool Bullet::alive(const Bullet * b)
{
	return b->alive();
}

bool Bullet::not_alive(const Bullet * b)
{
	return !b->alive();
}

