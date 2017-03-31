#ifndef GBINDOBJECT_H
#define GBINDOBJECT_H

#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gvariant.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gsharedptr.h"

namespace cpgf {

class GScriptFunction
{
public:
	GScriptFunction() {}
	virtual ~GScriptFunction() {}

	virtual GScriptValue invoke(const GVariant * params, size_t paramCount) = 0;
	virtual GScriptValue invokeIndirectly(GVariant const * const * params, size_t paramCount) = 0;
	virtual GScriptValue invokeIndirectlyOnObject(GVariant const * const * params, size_t paramCount) { return invokeIndirectly(params, paramCount); };

	// internal use
	virtual void weaken() = 0;

	GMAKE_NONCOPYABLE(GScriptFunction);
};


class GScriptArray
{
public:
	GScriptArray() {}
	virtual ~GScriptArray() {}

	virtual size_t getLength() = 0;
	virtual GScriptValue getValue(size_t index) = 0;
	virtual void setValue(size_t index, const GScriptValue & value) = 0;

	virtual bool maybeIsScriptArray(size_t index) = 0;
	virtual GScriptValue getAsScriptArray(size_t index) = 0;
	virtual GScriptValue createScriptArray(size_t index) = 0;

	GMAKE_NONCOPYABLE(GScriptArray);
};


namespace bind_internal {

enum ObjectPointerCV {
	opcvNone,
	opcvConst,
	opcvVolatile,
	opcvConstVolatile,

	opcvCount
};

class GBindingContext;

typedef GSharedPointer<GBindingContext> GContextPointer;
typedef GWeakPointer<GBindingContext> GWeakContextPointer;

class GScriptObjectBase : public GScriptObject
{
private:
	typedef GScriptObject super;

public:
	GScriptObjectBase(const GContextPointer & context, const GScriptConfig & config);
	GScriptObjectBase(const GScriptObjectBase & other);
	virtual ~GScriptObjectBase();

	IMetaClass * cloneMetaClass(IMetaClass * metaClass);

	IMetaService * getMetaService();

	virtual IScriptContext * getContext() const;

protected:
	const GContextPointer & getBindingContext() const {
		return this->context;
	}

	virtual void doBindCoreService(const char * name, IScriptLibraryLoader * libraryLoader);

private:
	GContextPointer context;
};


class GScriptFunctionBase : public GScriptFunction
{
private:
	typedef GScriptFunction super;

public:
	explicit GScriptFunctionBase(const GContextPointer & context);
	~GScriptFunctionBase();

	virtual void weaken();

protected:
	GContextPointer getBindingContext();

private:
	// Here we must use strong shared pointer,
	// otherwise the context may be freed by the script object
	// while the script function is still live.
	// But there is cyclic shared pointer chain
	// IScriptFunction->GBindingContext->GClassPool->GClassGlueData->GScriptDataHolder->IScriptFunction
	// Now the cyclic chain is broken by calling weaken when setting IScriptFunction to GScriptDataHolder
	// The solution is super ugly, but I can't find better solution for now.

	GContextPointer context;
	GWeakContextPointer weakContext;
};


class GScriptArrayBase : public GScriptArray
{
private:
	typedef GScriptArray super;

public:
	explicit GScriptArrayBase(const GContextPointer & context);
	~GScriptArrayBase();

protected:
	GContextPointer getBindingContext();

private:
	// Here we must use strong shared pointer,
	// otherwise the context may be freed by the script object
	// while the script array is still live.
	GContextPointer context;
};


} //namespace bind_internal

} //namespace cpgf


#endif
