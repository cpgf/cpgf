#ifndef CPGF_CHAINEDOBJECTACCESS_H
#define CPGF_CHAINEDOBJECTACCESS_H

class ChainedObjectA
{
public:
	ChainedObjectA() : a(1) {}
	virtual ~ChainedObjectA() {}

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
	virtual ~ChainedObjectB() {}
	
	ChainedObjectA & getA() { return this->a; }
	const ChainedObjectA & getConstA() const { return this->a; }

private:
	ChainedObjectA a;
};


class ChainedObjectC
{
public:
	ChainedObjectC() {}
	virtual ~ChainedObjectC() {}
	
	ChainedObjectB & getB() { return this->b; }
	const ChainedObjectB & getConstB() const { return this->b; }

private:
	ChainedObjectB b;
};


#endif
