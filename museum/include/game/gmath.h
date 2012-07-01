#ifndef __GMATH_H
#define __GMATH_H

#undef max // disable max macro so no warning in matrix.hpp
#include "thirdparty/vmmlib/matrix.hpp"

#include <math.h>
#include <limits>
#include <float.h>

#include "cpgf/game/gcore.h"


namespace cpgf {

const greal32 epsilon32 = FLT_EPSILON;
const greal64 epsilon64 = DBL_EPSILON;

const gradian gPI = 3.141592653589793238f;
const gradian g2PI = gPI * 2;
const gradian gPI_2 = gPI / 2;
const gradian gPI_4 = gPI / 4;
const gradian gPI_8 = gPI / 8;

inline gradian degreeToRadian(gfloat degree) {
	return degree * g2PI / 360;
}

inline gfloat radianToDegree(gradian radian) {
	return radian * 360 / g2PI;
}

inline gradian normalizeRadian(gradian radian) {
	while(radian >= g2PI) {
		radian -= g2PI;
	}

	return radian;
}

template <typename T1, typename T2>
inline int compareValue(const T1 v1, const T2 v2)
{
	if(v1 < v2) {
		return -1;
	}

	if(v2 < v1) {
		return 1;
	}

	return 0;
}

template <typename T>
inline bool numberIsNaN(T n)
{
	return n != n;
}

template <typename T>
inline bool numberIsInfinity(T n)
{
	T infinity = std::numeric_limits<T>::infinity();
	return !(-infinity < n && n < infinity);
}

template <typename T>
inline bool numberIsValid(T n)
{
	return !numberIsNaN(n) && !numberIsInfinity(n);
}

inline bool numberIsZero(greal32 n)
{
	return n < epsilon32 && n > -epsilon32;
}

inline bool numberIsZero(greal64 n)
{
	return n < epsilon64 && n > -epsilon64;
}

template <typename T>
inline T squareRoot(T n)
{
	return sqrt(n);
}

inline int makeInt32From16(gint16 high, gint16 low) {
	return (high << 16) + low;
}

inline int getHigh16(gint32 n) {
	return n >> 16;
}

inline int getLow16(gint32 n) {
	return n & 0xffff;
}

template <typename T, int N>
inline int sizeOfArray( T(&)[N] ) {
	return N;
}


template <typename T>
class GTypedVector2
{
private:
	typedef GTypedVector2<T> ThisType;

public:
	GTypedVector2() {
		this->loadZero();
	}

	GTypedVector2(T value) {
		this->loadValue(value);
	}

	GTypedVector2(const ThisType &other) {
		this->copyFrom(other);
	}

	~GTypedVector2() {
	}

	inline void copyFrom(const ThisType &other) {
		this->x = other.x;
		this->y = other.y;
	}

	inline void loadZero() {
		this->loadValue(static_cast<T>(0));
	}

	inline void loadValue(T value) {
		this->x = value;
		this->y = value;
	}

	inline void setX(T x) {
		this->x = x;
	}

	inline T getX() const {
		return this->x;
	}

	inline void setY(T y) {
		this->y = y;
	}

	inline T getY() const {
		return this->y;
	}

	inline T getSquaredLength() const {
		return x * x + y * y;
	}

	inline T getLength() const {
		return squareRoot(this->getSquaredLength());
	}

	inline T normalize() {
		T length = this->getLength();

		if(numberIsZero(length)) {
			return 0.0f;
		}

		T invertLength = 1.0f / length;

		this->x *= invertLength;
		this->y *= invertLength;

		return length;
	}

	inline bool isValid() const {
		return numberIsValid(this->x) && numberIsValid(this->y);
	}


public:
	T x;
	T y;
};

typedef GTypedVector2<gcoord> GVector2;

template <typename T>
inline T vectorDot2(const GTypedVector2<T> & a, const GTypedVector2<T> & b)
{
	return a.x * b.x + a.y * b.y;
}

template <typename T>
inline T vectorCross2(const GTypedVector2<T> & a, const GTypedVector2<T> & b)
{
	return a.x * b.y - a.y * b.x;
}


template <typename T, int N>
class GMatrix
{
private:
	typedef GMatrix<T, N> ThisType;
	typedef vmml::matrix<N, N, T> MatType;


public:
	GMatrix() {
		this->loadIdentity();
	}

	GMatrix(const ThisType &other) {
		this->implementMatrix = other.implementMatrix;
	}

	~GMatrix() {
	}

	void copyFrom(const ThisType &other) {
		this->implementMatrix = other.implementMatrix;
	}

	inline void loadZero() {
		MatType::zero(this->implementMatrix);
	}

	inline void loadIdentity() {
		vmml::identity(this->implementMatrix);
	}

	inline void translate(T x, T y, T z) {
		MatType m;
		vmml::identity(m);
		m.set_translation(x, y, z);
		this->implementMatrix *= m;
	}

	inline void translate(T x, T y) {
		this->translate(x, y, T(0));
	}

	inline void rotateZ(gradian radian) {
		this->implementMatrix.rotate_z(radian);
	}

	inline void rotateZ(gradian radian, T centerX, T centerY) {
		this->translate(centerX, centerY);
		this->rotateZ(radian);
		this->translate(-centerX, -centerY);
	}

	inline void scale(T x, T y, T z) {
		this->implementMatrix.scale(x, y, z);
	}

	inline void scale(T x, T y) {
		this->scale(x, y, T(1));
	}

	inline void multiply(const ThisType &other) {
		this->implementMatrix *= other.implementMatrix;
	}

	inline void multiply(const ThisType *other) {
		this->implementMatrix *= other->implementMatrix;
	}

	inline static ThisType multiply(const ThisType &m1, const ThisType &m2) {
		return ThisType(m1.implementMatrix * m2.implementMatrix);
	}

	void getDataColumnMajor(T *data) {
		for(int c = 0; c < N; ++c) {
			for(int r = 0; r < N; ++r) {
				data[c * N + r] = this->implementMatrix.at(r, c);
			}
		}
	}

	void setDataColumnMajor(T *data) {
		for(int c = 0; c < N; ++c) {
			for(int r = 0; r < N; ++r) {
				this->implementMatrix.at(r, c) = data[c * N + r];
			}
		}
	}

protected:
	GMatrix(const MatType &otherImplementMatrix) {
		this->implementMatrix = otherImplementMatrix;
	}

private:
	ThisType & operator = (const ThisType &other) {
		this->implementMatrix = other.implementMatrix;

		return *this;
	}

private:
	MatType implementMatrix;
};

typedef GMatrix<gfloat, 4> GMatrix4;



} // namespace cpgf


#endif

