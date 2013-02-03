#include "PowerUpShield.hpp"

PowerUpShield::~PowerUpShield()
{}

int PowerUpShield::type() const
{ return TYPE; }

GLfloat PowerUpShield::time() const
{ return 10.0f; }

void PowerUpShield::step_specific(GLfloat)
{}

void PowerUpShield::render_specific()
{
	glPushMatrix();
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
		glVertex3f( 0.0f, -0.7f, 0.0f);
		glVertex3f( 0.5f, -0.5f, 0.0f);
		glVertex3f( 0.5f,  0.5f, 0.0f);
		glVertex3f(-0.5f,  0.5f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);
		glVertex3f( 0.0f, -0.7f, 0.0f);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();
}

