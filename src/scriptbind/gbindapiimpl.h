#ifndef CPGF_GBINDAPIIMPL_H
#define CPGF_GBINDAPIIMPL_H


#include "cpgf/gmetaapi.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gexception.h"

#include "gbindcommon.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


#define ENTER_BINDING_API() \
	this->ginterface_implExtendObject.clearError(); \
	try {

#define LEAVE_BINDING_API(...) \
	} \
	catch(const GException & e) { this->ginterface_implExtendObject.handleError(e.getCode(), e.getMessage()); __VA_ARGS__; }


namespace cpgf {

class ImplScriptFunction : public IScriptFunction
{
public:
	ImplScriptFunction(GScriptFunction * scriptFunction, bool freeFunction);
	virtual ~ImplScriptFunction();

	// for internal use!
	GScriptFunction * getUnderlying() const { return this->scriptFunction; }

protected:
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

	virtual void G_API_CC invoke(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC invokeIndirectly(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount);
	virtual void G_API_CC invokeOnObject(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC invokeIndirectlyOnObject(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount);
	// Internal use only!!!
	virtual void G_API_CC weaken();

private:
	GScriptFunction * scriptFunction;
	bool freeFunction;
};


class ImplScriptArray : public IScriptArray
{
public:
	ImplScriptArray(GScriptArray * scriptArray, bool freeArray);
	virtual ~ImplScriptArray();

	// for internal use!
	GScriptArray * getUnderlying() const { return this->scriptArray; }

protected:
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

	virtual uint32_t G_API_CC getLength();
	virtual void G_API_CC getValue(GScriptValueData * outResult, uint32_t index);
	virtual void G_API_CC setValue(uint32_t index, const GScriptValueData * value);

	virtual gapi_bool G_API_CC maybeIsScriptArray(uint32_t index);
	virtual void G_API_CC getAsScriptArray(GScriptValueData * outResult, uint32_t index);
	virtual void G_API_CC createScriptArray(GScriptValueData * outResult, uint32_t index);

private:
	GScriptArray * scriptArray;
	bool freeArray;
};


class ImplScriptObject : public IScriptObject
{
public:
	ImplScriptObject(GScriptObject * scriptObject, bool freeObject);
	virtual ~ImplScriptObject();

	// for internal use!
	GScriptObject * getUnderlying() const { return this->scriptObject; }

protected:
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

	virtual IScriptContext * G_API_CC getContext();

	virtual IScriptObject * G_API_CC getOwner();
	virtual gapi_bool G_API_CC isGlobal();

	virtual void G_API_CC getValue(GScriptValueData * outResult, const char * name);
	virtual void G_API_CC setValue(const char * name, const GScriptValueData * value);

	virtual void G_API_CC createScriptObject(GScriptValueData * outResult, const char * name);

	virtual void G_API_CC invoke(GScriptValueData * outResult, const char * name, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC invokeIndirectly(GScriptValueData * outResult, const char * name, GVariantData const * const * params, uint32_t paramCount);

	virtual void G_API_CC assignValue(const char * fromName, const char * toName);

	virtual void G_API_CC bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader);
	virtual void G_API_CC holdObject(IObject * object);

	virtual gapi_bool G_API_CC maybeIsScriptArray(const char * name);
	virtual void G_API_CC getAsScriptArray(GScriptValueData * outResult, const char * name);
	virtual void G_API_CC createScriptArray(GScriptValueData * outResult, const char * name);

private:
	GScriptObject * scriptObject;
	bool freeObject;
};



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif

