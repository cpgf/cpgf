#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gglobal.h"

#include "../pinclude/gbindcommon.h"


namespace cpgf {


GScriptFunction::GScriptFunction(const GSharedPointer<bind_internal::GBindingContext> & context)
	: context(context)
{
}

GScriptFunction::~GScriptFunction()
{
}

GSharedPointer<bind_internal::GBindingContext> GScriptFunction::getContext()
{
	return this->context.get();
}


GScriptObject::GScriptObject(const GSharedPointer<bind_internal::GBindingContext> & context, const GScriptConfig & config)
	: context(context), config(config), owner(NULL)
{
}

GScriptObject::GScriptObject(const GScriptObject & other)
	: context(other.context), config(other.config), owner(NULL)
{
}

GScriptObject::~GScriptObject()
{
}

const GSharedPointer<bind_internal::GBindingContext> & GScriptObject::getContext()
{
	return this->context;
}

const GScriptConfig & GScriptObject::getConfig() const
{
	return this->config;
}

GScriptObject * GScriptObject::getOwner() const
{
	return this->owner;
}

void GScriptObject::setOwner(GScriptObject * newOwner)
{
	this->owner = newOwner;
}

bool GScriptObject::isGlobal() const
{
	return this->owner == NULL;
}

const char * GScriptObject::getName() const
{
	return this->name.c_str();
}

void GScriptObject::setName(const std::string & newName)
{
	this->name = newName;
}

IMetaClass * GScriptObject::cloneMetaClass(IMetaClass * metaClass)
{
	IMetaClass * newMetaClass = gdynamic_cast<IMetaClass *>(metaClass->clone());

	this->context->getClassData(metaClass);
	this->context->newClassData(newMetaClass);

	return newMetaClass;
}


} // namespace cpgf


