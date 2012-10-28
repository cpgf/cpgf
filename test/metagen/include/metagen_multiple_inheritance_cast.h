#ifndef __METAGEN_MULTIPLE_INHERITANCE_CAST_H
#define __METAGEN_MULTIPLE_INHERITANCE_CAST_H


#include <typeinfo>


class MultipleInheritanceCastR
{
public:
	int r[20];
};

class MultipleInheritanceCastA : public MultipleInheritanceCastR
{
public:
	virtual ~MultipleInheritanceCastA() {}

	int a;
};

class MultipleInheritanceCastB : virtual public MultipleInheritanceCastA
{
public:
	virtual ~MultipleInheritanceCastB() {}
	virtual void c1() {}

	double c;
};

class MultipleInheritanceCastC : virtual public MultipleInheritanceCastA
{
public:
	virtual ~MultipleInheritanceCastC() {}

	long long d[20];
};

class MultipleInheritanceCastD : virtual public MultipleInheritanceCastB, virtual public MultipleInheritanceCastC
{
public:
	virtual ~MultipleInheritanceCastD() {}

	int e[10];
};

inline MultipleInheritanceCastD * getMultipleInheritanceCastD()
{
	static MultipleInheritanceCastD d;
	MultipleInheritanceCastD * pd = &d;
	return pd;
}

inline bool checkMultipleInheritanceCastDAsR(MultipleInheritanceCastR * r)
{
	return true;
//	return typeid(*r) == typeid(MultipleInheritanceCastD);
}



#endif
