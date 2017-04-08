#ifndef CPGF_GSCRIPTBINDAPI_H
#define CPGF_GSCRIPTBINDAPI_H

#include "cpgf/gmetaapi.h"

namespace cpgf {

struct IScriptLibraryLoader;
struct GScriptValueData;
struct IScriptContext;

struct IScriptFunction : public IExtendObject
{
public:
	virtual void G_API_CC invoke(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeIndirectly(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeOnObject(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeIndirectlyOnObject(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount) = 0;
};


struct IScriptArray : public IExtendObject
{
public:
	virtual uint32_t G_API_CC getLength() = 0;
	virtual void G_API_CC getValue(GScriptValueData * outResult, uint32_t index) = 0;
	virtual void G_API_CC setValue(uint32_t index, const GScriptValueData * value) = 0;

	virtual gapi_bool G_API_CC maybeIsScriptArray(uint32_t index) = 0;
	virtual void G_API_CC getAsScriptArray(GScriptValueData * outResult, uint32_t index) = 0;
	virtual void G_API_CC createScriptArray(GScriptValueData * outResult, uint32_t index) = 0;
};


struct IScriptObject : public IExtendObject
{
	virtual IScriptContext * G_API_CC getContext() = 0;

	virtual IScriptObject * G_API_CC getOwner() = 0;
	virtual gapi_bool G_API_CC isGlobal() = 0;

	virtual void G_API_CC getValue(GScriptValueData * outResult, const char * name) = 0;
	virtual void G_API_CC setValue(const char * name, const GScriptValueData * value) = 0;

	virtual void G_API_CC createScriptObject(GScriptValueData * outResult, const char * name) = 0;

	virtual void G_API_CC invoke(GScriptValueData * outResult, const char * name, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeIndirectly(GScriptValueData * outResult, const char * name, GVariantData const * const * params, uint32_t paramCount) = 0;

	virtual void G_API_CC assignValue(const char * fromName, const char * toName) = 0;

	virtual void G_API_CC bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader) = 0;
	virtual void G_API_CC holdObject(IObject * object) = 0;

	virtual gapi_bool G_API_CC maybeIsScriptArray(const char * name) = 0;
	virtual void G_API_CC getAsScriptArray(GScriptValueData * outResult, const char * name) = 0;
	virtual void G_API_CC createScriptArray(GScriptValueData * outResult, const char * name) = 0;

};



} // namespace cpgf




#endif
