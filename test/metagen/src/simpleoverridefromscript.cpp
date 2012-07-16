#include "../include/simpleoverridefromscript.h"

SimpleOverride::SimpleOverride(int n)
	: n(n)
{
}

int SimpleOverride::getValue()
{
	return this->n;
}
