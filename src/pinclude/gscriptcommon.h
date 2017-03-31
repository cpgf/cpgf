#ifndef GSCRIPTCOMMON_H
#define GSCRIPTCOMMON_H

#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/gvariant.h"
#include "cpgf/gmetaapi.h"

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

} //namespace bind_internal

} //namespace cpgf


#endif
