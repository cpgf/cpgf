#include "sprite.h"

#if defined(_WIN32)
    #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


Sprite::Sprite()
	: x(0), y(0), z(0), size(100), rotate(0), color(0), alpha(1), visible(true), shape(ssTeapot)
{
}

void Sprite::setX(int x)
{
	this->x = x;
}

int Sprite::getX() const
{
	return this->x;
}

void Sprite::setY(int y)
{
	this->y = y;
}

int Sprite::getY() const
{
	return this->y;
}

void Sprite::setZ(int z)
{
	this->z = z;
}

int Sprite::getZ() const
{
	return this->z;
}

void Sprite::setSize(int size)
{
	this->size = size;
}

int Sprite::getSize() const
{
	return this->size;
}

void Sprite::setRotate(float rotate)
{
	this->rotate = rotate;
}

float Sprite::getRotate() const
{
	return this->rotate;
}

void Sprite::setColor(unsigned int color)
{
	this->color = color;
}

unsigned int Sprite::getColor() const
{
	return this->color;
}

void Sprite::setAlpha(float alpha)
{
	if(alpha < 0) {
		alpha = 0;
	}
	if(alpha > 1) {
		alpha = 1;
	}
	this->alpha = alpha;
}

float Sprite::getAlpha() const
{
	return this->alpha;
}

void Sprite::setVisible(bool visible)
{
	this->visible = visible;
}

bool Sprite::getVisible() const
{
	return this->visible;
}

void Sprite::setShape(int shape)
{
	this->shape = shape;
}

int Sprite::getShape() const
{
	return this->shape;
}

void Sprite::render(int viewWidth, int viewHeight)
{
	if(! this->visible) {
		return;
	}

	float tx = this->x;
	float ty = this->y;
	float tz = 0;
	tx /= SpriteBoardSize;
	ty /= SpriteBoardSize;
	tx *= 2;
	ty *= 2;
	float s = this->size;
	s = s / viewWidth;
	glPushMatrix();

	glColor4ub((this->color >> 16) & 0xff, (this->color >> 8) & 0xff, this->color & 0xff, this->alpha * 255);

	glTranslatef(tx, ty, tz);
	glRotatef(rotate, 1, 1, 1);

	switch(this->shape) {
	case ssTeapot:
		glutSolidTeapot(s);
		break;

	default:
		glutSolidSphere(s, 36, 18);
		break;
	}

	glPopMatrix();
}
