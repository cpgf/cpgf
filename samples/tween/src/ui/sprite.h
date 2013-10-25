#ifndef CPGF_SPRITE_H
#define CPGF_SPRITE_H

#include "wx/dc.h"

const int SpriteBoardSize = 500;

class Sprite
{
public:
	// use int instead of enum so we can tween the shape
	static const int ssSphere = 0;
	static const int ssTeapot = 1;

public:
	Sprite();
	
	void setX(int x);
	int getX() const;
	void setY(int y);
	int getY() const;
	void setZ(int z);
	int getZ() const;
	void setRotate(float rotate);
	float getRotate() const;
	void setSize(int size);
	int getSize() const;
	void setColor(unsigned int color);
	unsigned int getColor() const;
	void setAlpha(float alpha);
	float getAlpha() const;
	void setVisible(bool visible);
	bool getVisible() const;

	void setShape(int shape);
	int getShape() const;

	void render(int viewWidth, int viewHeight);

private:
	int x;
	int y;
	int z;
	int size;
	float rotate;
	unsigned int color;
	float alpha;
	bool visible;
	int shape;
};


#endif
