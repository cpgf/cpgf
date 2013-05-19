#ifndef __CHAINEDOBJECTACCESS_H
#define __CHAINEDOBJECTACCESS_H

class ChainedObjectA
{
public:
	ChainedObjectA() : a(1) {}

	int getA() const { return this->doGetA(); }
	
protected:
	virtual int doGetA() const { return this->a; }
private:
	int a;
};

class ChainedObjectB
{
public:
	ChainedObjectB() {}
	
	ChainedObjectA & getA() { return this->a; }
	const ChainedObjectA & getConstA() const { return this->a; }

private:
	ChainedObjectA a;
};


class ChainedObjectC
{
public:
	ChainedObjectC() {}
	
	ChainedObjectB & getB() { return this->b; }
	const ChainedObjectB & getConstB() const { return this->b; }

private:
	ChainedObjectB b;
};


#endif
