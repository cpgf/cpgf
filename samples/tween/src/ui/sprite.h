#ifndef __SPRITE_H
#define __SPRITE_H

#include "wx/dc.h"

class Sprite
{
public:
	Sprite();
	
	void setX(int x);
	int getX() const;
	void setY(int y);
	int getY() const;
	void setZ(int z);
	int getZ() const;
	void setSize(int size);
	int getSize() const;
	void setColor(unsigned int color);
	unsigned int getColor() const;
	void setAlpha(float alpha);
	float getAlpha() const;
	void setVisible(bool visible);
	bool getVisible() const;

	void render(int viewWidth, int viewHeight);

private:
	int x;
	int y;
	int z;
	int size;
	unsigned int color;
	float alpha;
	bool visible;
};


#endif
