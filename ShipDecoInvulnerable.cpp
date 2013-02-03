#include "ShipDecoInvulnerable.hpp"
#include "util.hpp"
#include <GL/gl.h>
#include <cmath>

ShipDecoInvulnerable::ShipDecoInvulnerable(GLfloat t)
	: t(t)
{
	c[0] = 0.0f;
	c[1] = 0.0f;
	step(0.0f);
}

void ShipDecoInvulnerable::step(GLfloat dt)
{
	static const GLfloat OMEGA = 2.0 * M_PI * 1.0f;
	t -= dt;
	if (t < 0.0f) t = 0.0f;

	// blinking during the last second
	if (t < 1.0f) {
		GLfloat a = sqr(sin(OMEGA * t));
		c[2] = 0.8f + 0.2f * a;
		c[3] = 0.2f + 0.4f * a;
	} else {
		c[2] = 0.8f;
		c[3] = 0.4f;
	}
}

void ShipDecoInvulnerable::render()
{
	static const int S = 6;

	v3 s = 2.0f * scale;

	glPushMatrix();
	glTranslatef(x[0], x[1], x[2]);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glScalef(s[0], s[1], s[2]);
	glColor4fv(c);
	glEnable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		for (int i = 0; i <= S; ++i) {
			double phi = M_PI * 2.0 / S * i;
			glVertex3f(cos(phi), sin(phi), 0.0f);
		}
	glEnd();
	glPopMatrix();
}

bool ShipDecoInvulnerable::alive() const
{
	return t > 0.0f;
}

