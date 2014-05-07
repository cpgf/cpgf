#ifndef CPGF_SIMPLEOVERRIDABLEINCREMENTER_H
#define CPGF_SIMPLEOVERRIDABLEINCREMENTER_H

class SimpleOverridableIncrementer
{
public:
	~SimpleOverridableIncrementer() {}
	virtual int increment() = 0;
};

#endif
