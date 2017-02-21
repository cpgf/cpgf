#ifndef CPGF_GFIXEDTYPE_H
#define CPGF_GFIXEDTYPE_H


#include "cpgf/gifelse.h"


namespace cpgf {


struct GFixedTypeInt8
{
	typedef char Signed;
	typedef unsigned char Unsigned;
};


struct GFixedTypeInt16
{
	typedef GIfElse<sizeof(short) == 2, short,
		GIfElse<sizeof(int) == 2, int,
			GIfElse<sizeof(long) == 2, long,
				void // error
			>::Result
		>::Result
	>::Result
	Signed;

	typedef GIfElse<sizeof(unsigned short) == 2, unsigned short,
		GIfElse<sizeof(unsigned int) == 2, unsigned int,
			GIfElse<sizeof(unsigned long) == 2, unsigned long,
				void // error
			>::Result
		>::Result
	>::Result
	Unsigned;

};


struct GFixedTypeInt32
{
	typedef GIfElse<sizeof(int) == 4, int,
		GIfElse<sizeof(short) == 4, short,
			GIfElse<sizeof(long) == 4, long,
				void // error
			>::Result
		>::Result
	>::Result
	Signed;

	typedef GIfElse<sizeof(unsigned int) == 4, unsigned int,
		GIfElse<sizeof(unsigned short) == 4, unsigned short,
			GIfElse<sizeof(unsigned long) == 4, unsigned long,
				void // error
			>::Result
		>::Result
	>::Result
	Unsigned;
};


struct GFixedTypeInt64
{
	typedef GIfElse<sizeof(long long) == 8, long long,
		GIfElse<sizeof(int) == 8, int,
			GIfElse<sizeof(short) == 8, short,
				void // error
			>::Result
		>::Result
	>::Result
	Signed;

	typedef GIfElse<sizeof(unsigned long long) == 8, unsigned long long,
		GIfElse<sizeof(unsigned int) == 8, unsigned int,
			GIfElse<sizeof(unsigned short) == 8, unsigned short,
				void // error
			>::Result
		>::Result
	>::Result
	Unsigned;
};


struct GFixedTypeFloat32
{
	typedef GIfElse<sizeof(float) == 4, float,
		GIfElse<sizeof(double) == 4, double,
			GIfElse<sizeof(long double) == 4, long double,
				void // error
			>::Result
		>::Result
	>::Result
	Signed;
};


struct GFixedTypeFloat64
{
	typedef GIfElse<sizeof(double) == 8, double,
		GIfElse<sizeof(float) == 8, float,
			GIfElse<sizeof(long double) == 8, long double,
				void // error
			>::Result
		>::Result
	>::Result
	Signed;
};

struct GFixedTypeFloat80
{
	typedef long double Signed;
};


} // namespace cpgf



#endif
