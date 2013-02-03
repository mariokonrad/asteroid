#include "glutil.hpp"
#include <stdarg.h>
#include <cstdio>
#include <cstring>

void glText(const v3 & x, const FontBase & font, const char * s, ...)
{
	if (!s) return;

	// handle format string
	char buf[128];
	char * p = buf;
	memset(buf, 0, sizeof(buf));
	va_list ap;
	va_start(ap, s);
	vsprintf(buf, s, ap);
	va_end(ap);

	// render text

	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), font.vertices());

	glPushMatrix();
	glColor4fv(font.color());
	glLineWidth(font.linewidth());

	v3 scale = font.scale();
	GLfloat a = x[0];
	for (; *p; ++p, a += scale[0] + font.space()) {
		glPushMatrix();
		glTranslatef(a, x[1], x[2]);
		glScalef(scale[0], scale[1], scale[2]);
		const FontBase::letter_t * l = font.letter(*p);
		if (l) glDrawElements(GL_LINES, l->size, GL_UNSIGNED_INT, l->p);
		glPopMatrix();
	}
	glLineWidth(1.0f);

	glLineWidth(1.0f);
	glPopMatrix();
	glPopClientAttrib();
}

