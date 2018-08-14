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
	G_INTERFACE_IMPL_OBJECT_OVERRIDE
	G_INTERFACE_IMPL_EXTENDOBJECT_OVERRIDE

	virtual void G_API_CC invoke(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount) override;
	virtual void G_API_CC invokeIndirectly(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount) override;
	virtual void G_API_CC invokeOnObject(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount) override;
	virtual void G_API_CC invokeIndirectlyOnObject(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount) override;

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
	G_INTERFACE_IMPL_OBJECT_OVERRIDE
	G_INTERFACE_IMPL_EXTENDOBJECT_OVERRIDE

	virtual uint32_t G_API_CC getLength() override;
	virtual void G_API_CC getValue(GScriptValueData * outResult, uint32_t index) override;
	virtual void G_API_CC setValue(uint32_t index, const GScriptValueData * value) override;

	virtual gapi_bool G_API_CC maybeIsScriptArray(uint32_t index) override;
	virtual void G_API_CC getAsScriptArray(GScriptValueData * outResult, uint32_t index) override;
	virtual void G_API_CC createScriptArray(GScriptValueData * outResult, uint32_t index) override;

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
	G_INTERFACE_IMPL_OBJECT_OVERRIDE
	G_INTERFACE_IMPL_EXTENDOBJECT_OVERRIDE

	virtual IScriptContext * G_API_CC getContext() override;

	virtual IScriptObject * G_API_CC getOwner() override;
	virtual gapi_bool G_API_CC isGlobal() override;

	virtual void G_API_CC getValue(GScriptValueData * outResult, const char * name) override;
	virtual void G_API_CC setValue(const char * name, const GScriptValueData * value) override;

	virtual void G_API_CC createScriptObject(GScriptValueData * outResult, const char * name) override;

	virtual void G_API_CC invoke(GScriptValueData * outResult, const char * name, const GVariantData * params, uint32_t paramCount) override;
	virtual void G_API_CC invokeIndirectly(GScriptValueData * outResult, const char * name, GVariantData const * const * params, uint32_t paramCount) override;

	virtual void G_API_CC assignValue(const char * fromName, const char * toName) override;

	virtual void G_API_CC bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader) override;
	virtual void G_API_CC holdObject(IObject * object) override;

	virtual gapi_bool G_API_CC maybeIsScriptArray(const char * name) override;
	virtual void G_API_CC getAsScriptArray(GScriptValueData * outResult, const char * name) override;
	virtual void G_API_CC createScriptArray(GScriptValueData * outResult, const char * name) override;

private:
	GScriptObject * scriptObject;
	bool freeObject;
};



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
