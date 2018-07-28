#include "gbindobject.h"
#include "gbindcontext.h"

#include "cpgf/scriptbind/gscriptuserconverter.h"
#include "cpgf/gglobal.h"

namespace cpgf {

namespace bind_internal {

GScriptObjectBase::GScriptObjectBase(const GContextPointer & context)
	: super(), context(context)
{
}

GScriptObjectBase::GScriptObjectBase(const GScriptObjectBase & other)
	: super(other), context(other.context)
{
}

GScriptObjectBase::~GScriptObjectBase()
{
}

IMetaClass * GScriptObjectBase::cloneMetaClass(IMetaClass * metaClass)
{
	IMetaClass * newMetaClass = gdynamic_cast<IMetaClass *>(metaClass->clone());

	this->context->getClassData(metaClass);
	this->context->getClassData(newMetaClass);

	return newMetaClass;
}

IMetaService * GScriptObjectBase::getMetaService()
{
	IMetaService * service = this->context->getService();
	service->addReference();
	return service;
}

IScriptContext * GScriptObjectBase::getContext() const
{
	IScriptContext * scriptContext = this->context->borrowScriptContext();
	scriptContext->addReference();
	return scriptContext;
}

void GScriptObjectBase::doBindCoreService(const char * name, IScriptLibraryLoader * libraryLoader)
{
	this->getBindingContext()->bindScriptCoreService(this, name, libraryLoader);
}


GScriptFunctionBase::GScriptFunctionBase(const GContextPointer & context)
	: /*context(context),*/ weakContext(context)
{
}

GScriptFunctionBase::~GScriptFunctionBase()
{
}

GContextPointer GScriptFunctionBase::getBindingContext()
{
	return this->weakContext.lock();
}

GScriptArrayBase::GScriptArrayBase(const GContextPointer & context)
	: context(context)
{
}

GScriptArrayBase::~GScriptArrayBase()
{
}

GContextPointer GScriptArrayBase::getBindingContext()
{
	return this->context;
}


} //namespace bind_internal

} //namespace cpgf

