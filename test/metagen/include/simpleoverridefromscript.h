#ifndef __SIMPLEOVERRIDEFROMSCRIPT_H
#define __SIMPLEOVERRIDEFROMSCRIPT_H

class SimpleOverride
{
public:
	explicit SimpleOverride(int n);

	virtual int getValue();

//private:
	int n;
};


#endif
