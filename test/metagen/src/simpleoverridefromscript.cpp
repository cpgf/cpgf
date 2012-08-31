#include "../include/simpleoverridefromscript.h"

int SimpleOverrideBase::baseOnly()
{
	return 38;
}

int SimpleOverrideBase::getValue()
{
	return 0;
}


SimpleOverride::SimpleOverride(int n)
	: n(n)
{
}

int SimpleOverride::getValue()
{
	return this->n;
}

std::string SimpleOverride::getName()
{
	return "";
}
