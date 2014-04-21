#ifndef CPGF_SIMPLEOVERRIDEFROMSCRIPT_H
#define CPGF_SIMPLEOVERRIDEFROMSCRIPT_H

#include <string>
#include <stdexcept>

struct SimpleOverrideHelperData {
};

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

	virtual SimpleOverrideHelperData *createHelperData() {
		throw std::runtime_error("test should override this method");
	}

	virtual std::string getName();

	void consumeHelperData() {
		delete this->createHelperData();
	}

//private:
	int n;
};


#endif
