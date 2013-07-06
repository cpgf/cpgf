#include "builderutil.h"
#include "model/cppitem.h"
#include "config.h"


std::string getReflectionClassName(const Config * config)
{
	return config->getMetaDefineParamName() + "::ClassType";
}

std::string getReflectionScope(const CppItem * item)
{
	if(item->isGlobal()) {
		return "";
	}
	else {
		return getReflectionClassName(item->getConfig()) + "::";
	}
}
