#include "Asteroid.hpp"
#include <cmath>
#include <cstdlib>

Asteroid::Asteroid(int sides, int level,
	const v3 & x, const v3 & v, GLfloat angle, GLfloat omega)
	: Sprite(x, v)
	, sides(sides)
	, _level(level)
	, scale(1.0f)
	, angle(angle)
	, omega(omega)
{
	switch (_level) {
		case 0: scale = 10.0f; break;
		case 1: scale = 20.0f; break;
		case 2: scale = 40.0f; break;
	}
}

void Asteroid::step(GLfloat dt)
{
	// rotation
	angle += dt * omega;
	while (angle > 360.0f) angle -= 360.0f;
	while (angle <   0.0f) angle += 360.0f;

	// position
	x += dt * v;
}

void Asteroid::wrap(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1)
{
	x.wrap(x0, x1, y0, y1, z0, z1);
}

void Asteroid::render()
{
	glPushMatrix();

	// translate to position
	glTranslatef(x[0], x[1], x[2]);

	// rotation
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	// draw
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glScalef(scale, scale, 1.0f);
	glColor4f(0.5f, 0.5f, 0.5f, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0, 0, 0);
		for (int i = 0; i <= sides; ++i) {
			glVertex3f(cos(M_PI * 2.0f / sides * i), sin(M_PI * 2.0f / sides * i), 0.0f);
		}
	glEnd();

	glPopMatrix();
}

Boundary Asteroid::boundary() const
{
	return Boundary(x, scale);
}

int Asteroid::level() const
{
	return _level;
}

bool Asteroid::crack(std::vector<Asteroid *> & c) const
{
	if (_level <= 0) return false;

	GLfloat speed = v.len();

	// add new asteroid objects to the specified container
	// the asteroid does crack into new smaller asteroids,
	// number of the new asteroids is the same as the side count
	// of this one. direction and velocity are random.
	for (unsigned int i = sides; i > 0; --i) {
		GLfloat phi = M_PI / 180.0 * static_cast<GLfloat>(abs(rand()) % 360);
		GLfloat start_angle = static_cast<GLfloat>(abs(rand()) % 360);
		GLfloat omega = static_cast<GLfloat>(abs(rand()) % 40) - 20.0f;
		int sides = _level > 1 ? 3 + abs(rand()) % 4 : 3;
		c.push_back(new Asteroid(sides, _level-1, x,
			v3(speed * cos(phi), speed * sin(phi), 0.0f),
			start_angle, omega));
	}

	return true;
}

unsigned int Asteroid::score() const
{
	switch (_level) {
		case 0: return 50;
		case 1: return 30;
		case 2: return 10;
	}
	return 0;
}

