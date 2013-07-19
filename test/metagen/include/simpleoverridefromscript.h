#ifndef __SIMPLEOVERRIDEFROMSCRIPT_H
#define __SIMPLEOVERRIDEFROMSCRIPT_H

#include <string>

class SimpleOverrideBase
{
public:
	virtual ~SimpleOverrideBase() {}

	int baseOnly();
	virtual int getValue();
};

class SimpleOverride : public SimpleOverrideBase
{
public:
	explicit SimpleOverride(int n);

	virtual int getValue();

	virtual int getAnother() { return 1; }

	virtual std::string getName();

//private:
	int n;
};


#endif
