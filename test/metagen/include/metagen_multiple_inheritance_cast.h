#ifndef __METAGEN_MULTIPLE_INHERITANCE_CAST_H
#define __METAGEN_MULTIPLE_INHERITANCE_CAST_H


#include <typeinfo>


class MultipleInheritanceCastR
{
public:
	virtual bool isR() const { return true; }
	virtual bool isA() const { return false; }
	virtual bool isB() const { return false; }
	virtual bool isC() const { return false; }
	virtual bool isD() const { return false; }

	int r[20];
};

class MultipleInheritanceCastA : public MultipleInheritanceCastR
{
public:
	virtual ~MultipleInheritanceCastA() {}
	virtual bool isR() const { return false; }
	virtual bool isA() const { return true; }
	virtual bool isB() const { return false; }
	virtual bool isC() const { return false; }
	virtual bool isD() const { return false; }

	int a;
};

class MultipleInheritanceCastB : virtual public MultipleInheritanceCastA
{
public:
	virtual ~MultipleInheritanceCastB() {}
	virtual void c1() {}
	virtual bool isR() const { return false; }
	virtual bool isA() const { return false; }
	virtual bool isB() const { return true; }
	virtual bool isC() const { return false; }
	virtual bool isD() const { return false; }

	double c;
};

class MultipleInheritanceCastC : virtual public MultipleInheritanceCastA
{
public:
	virtual ~MultipleInheritanceCastC() {}
	virtual bool isR() const { return false; }
	virtual bool isA() const { return false; }
	virtual bool isB() const { return false; }
	virtual bool isC() const { return true; }
	virtual bool isD() const { return false; }

	long long d[20];
};

class MultipleInheritanceCastD : virtual public MultipleInheritanceCastB, virtual public MultipleInheritanceCastC
{
public:
	virtual ~MultipleInheritanceCastD() {}
	virtual bool isR() const { return false; }
	virtual bool isA() const { return false; }
	virtual bool isB() const { return false; }
	virtual bool isC() const { return false; }
	virtual bool isD() const { return true; }

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
//	return r->isD();
//	return typeid(*r) == typeid(MultipleInheritanceCastD);
}



#endif
