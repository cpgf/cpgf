#include "cpgf/scriptbind/gscriptbind.h"

namespace cpgf {


GScriptObject::GScriptObject(const GScriptConfig & config)
	: config(config), owner(NULL)
{
}

GScriptObject::~GScriptObject()
{
}

const GScriptConfig & GScriptObject::getConfig() const
{
	return this->config;
}

GScriptObject * GScriptObject::getOwner() const
{
	return this->owner;
}

bool GScriptObject::isGlobal() const
{
	return this->owner == NULL;
}

const char * GScriptObject::getName() const
{
	return this->name.c_str();
}


} // namespace cpgf


