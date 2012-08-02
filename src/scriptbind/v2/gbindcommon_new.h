#ifndef __GBINDCOMMON_NEW_H
#define __GBINDCOMMON_NEW_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gglobal.h"
#include "cpgf/gmetaoperatorop.h"
#include "cpgf/gsharedptr.h"


namespace cpgf {

namespace _bind_internal {

class GBindingContext
{
public:
	GBindingContext(IMetaService * service, const GScriptConfig & config);
	virtual ~GBindingContext();

	IMetaService * getService() const {
		return this->service.get();
	}

	const GScriptConfig & getConfig() const {
		return this->config;
	}

private:
	GSharedInterface<IMetaService> service;
	GScriptConfig config;
};

typedef GSharedPointer<GBindingContext> GContextPointer;
typedef GWeakPointer<GBindingContext> GWeakContextPointer;


enum GUserDataType {
	udtObject,
	udtClass,
	udtExtendMethod,
	udtEnum,
	udtOperator,
	udtAccessible,
	udtRaw,
	udtMethod
};

class GUserData : public GNoncopyable
{
public:
	GUserData(GUserDataType type, const GContextPointer & context)
		: type(type), context(GWeakContextPointer(context))
	{
	}

	virtual ~GUserData() {
	}

	GUserDataType getType() const {
		return this->type;
	}

	GContextPointer getContext() const {
		return GContextPointer(this->context);
	}

private:
	GUserDataType type;
	GWeakContextPointer context;
};

typedef GSharedPointer<GUserData> GUserDataPointer;


class GClassUserData : public GUserData
{
private:
	typedef GUserData super;

public:
	GClassUserData(const GContextPointer & context, IMetaClass * metaClass)
		: super(udtClass, context), metaClass(metaClass)
	{
	}

	IMetaClass * getMetaClass() const {
		return this->metaClass.get();
	}

private:
	GSharedInterface<IMetaClass> metaClass;
};




} // namespace _bind_internal


} // namespace cpgf



#endif
