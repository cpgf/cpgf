#ifndef __GGEOMETRY_H
#define __GGEOMETRY_H


namespace cpgf {


class GPoint2D
{
public:
	GPoint2D() : x(0), y(0) {}
	GPoint2D(gcoord x, gcoord y) : x(x), y(y) {}
	GPoint2D(const GPoint2D &other) : x(other.x), y(other.y) {}

	inline void set(gcoord x, gcoord y) {
		this->x = x;
		this->y = y;
	}

public:
	gcoord x;
	gcoord y;
};

class GSize
{
public:
	GSize() : width(0), height(0) {}
	GSize(gcoord width, gcoord height) : width(width), height(height) {}
	GSize(const GSize &other) : width(other.width), height(other.height) {}

	inline void set(gcoord width, gcoord height) {
		this->width = width;
		this->height = height;
	}

public:
	gcoord width;
	gcoord height;
};

class GRect
{
public:
	GRect() : left(0), top(0), right(0), bottom(0) {}
	GRect(gcoord left, gcoord top, gcoord right, gcoord bottom) : left(left), top(top), right(right), bottom(bottom) {}
	GRect(const GRect &other) : left(other.left), top(other.top), right(other.right), bottom(other.bottom) {}

	inline gcoord getWidth() const {
		return this->right - this->left;
	}

	inline gcoord getHeight() const {
		return this->bottom - this->top;
	}

	inline GSize getSize() const {
		return GSize(this->getWidth(), this->getHeight());
	}

	inline bool isNormalized() const {
		return this->left <= this->right && this->top <= this->bottom;
	}

	inline void set(gcoord left, gcoord top, gcoord right, gcoord bottom) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}

public:
	gcoord left;
	gcoord top;
	gcoord right;
	gcoord bottom;
};



}


#endif
