#include <algorithm>
#include <cmath>
#include "Ship.hpp"
#include "util.hpp"
#include "ShipDecoInvulnerable.hpp"
#include "PowerUpShield.hpp"

Ship::Ship(const v3 & x)
	: Sprite(x, v3())
	, scale(10.0f, 10.0f, 1.0f)
	, angle(0.0f)
	, max_bullets(2)
	, bullet_speed(300.0f)
	, bullet_time(2.0f)
	, accel(200.0f)
	, rot(10.0f)
	, v_max(200.0f)
	, decorator(N_DECO, NULL)
{
	color[0] = 1.0f;
	color[1] = 1.0f;
	color[2] = 1.0f;
	color[3] = 1.0f;
}

Ship::~Ship()
{
	delete_all(decorator);
}

void Ship::step(GLfloat dt)
{
	// velocity
	v += dt * a;
	v.max(v_max);

	// position
	x += dt * v;

	for_each(decorator.begin(), decorator.end(), RenderObject::Step(dt));
	destroy_dead_decorators();
	for_each(decorator.begin(), decorator.end(), ShipDecorator::Update(x, scale, angle));
}

void Ship::wrap(GLfloat x0, GLfloat x1, GLfloat y0, GLfloat y1, GLfloat z0, GLfloat z1)
{
	x.wrap(x0, x1, y0, y1, z0, z1);
}

void Ship::render()
{
	glPushMatrix();

	// translate to position
	glTranslatef(x[0], x[1], x[2]);

	// rotate
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	// draw
	glEnable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glScalef(scale[0], scale[1], scale[2]);
	glColor4fv(color);
	glBegin(GL_TRIANGLES);
		glVertex3f( 1, 0, 0); glVertex3f(-1, 1, 0); glVertex3f( 0,  0, 0);
		glVertex3f( 1, 0, 0); glVertex3f( 0, 0, 0); glVertex3f(-1, -1, 0);
	glEnd();

	glPopMatrix();

	for_each(decorator.begin(), decorator.end(), RenderObject::Render());
}

Boundary Ship::boundary() const
{
	return Boundary(x, 10.0f);
}

void Ship::rot_left()
{
	angle += +rot;
	if (angle >= 360.0) angle -= 360.0;
}

void Ship::rot_right()
{
	angle += -rot;
	if (angle < 0.0) angle += 360.0;
}

void Ship::accelerate(bool flag)
{
	if (flag) {
		a[0] = accel * cos(angle * M_PI / 180.0f);
		a[1] = accel * sin(angle * M_PI / 180.0f);
		a[2] = 0.0;
	} else {
		a[0] = a[1] = a[2] = 0.0f;
	}
}

void Ship::shoot(std::vector<Bullet *> & bullet)
{
	if (bullet.size() >= max_bullets) return;

	v3 vel(
		bullet_speed * cos(angle * M_PI / 180.0f),
		bullet_speed * sin(angle * M_PI / 180.0f),
		0.0f);

	bullet.push_back(new Bullet(bullet_time, angle, x, vel));
}

void Ship::invulnerable(GLfloat t)
{
	// delete previous decorator
	ShipDecorator * o = decorator[DECO_INVULNERABLE];
	if (o) {
		delete o;
		decorator[DECO_INVULNERABLE] = NULL;
	}

	// new decorator
	if (t > 0.0f) decorator[DECO_INVULNERABLE] = new ShipDecoInvulnerable(t);
}

bool Ship::invulnerable() const
{
	return decorator[DECO_INVULNERABLE] != NULL;
}

void Ship::destroy_dead_decorators()
{
	for (std::vector<ShipDecorator *>::iterator i = decorator.begin(); i != decorator.end(); ++i) {
		ShipDecorator * d = *i;
		if (d == NULL) continue;
		if (!d->alive()) {
			delete d;
			*i = NULL;
		}
	}
}

void Ship::powerup(const PowerUp & p)
{
	switch (p.type()) {
		case PowerUpShield::TYPE:
			invulnerable(dynamic_cast<const PowerUpShield &>(p).time());
			break;
	}
}

