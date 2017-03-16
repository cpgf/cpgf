#ifndef CPGF_GSCRIPTBINDUTIL_H
#define CPGF_GSCRIPTBINDUTIL_H


#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"

namespace cpgf {

template <typename T>
GVariant createTypedVariant(const T & value)
{
	return createTypedVariant(GVariant(value), createMetaType<T>());
}
class GTypedVariant
{
public:
	template <typename T>
	GTypedVariant(const T & value) : value(createTypedVariant(value)) {}

	operator const GVariant & () const { return this->value; }
	operator GVariant & () { return this->value; }
	operator const GVariant * () const { return &this->value; }
	operator GVariant * () { return &this->value; }
	const GVariant * operator & () const { return & this->value; }
	GVariant * operator & () { return & this->value; }

	const GVariant & getValue() const { return this->value; }
	GVariant & getValue() { return this->value; }

private:
	GVariant value;
};


#define DECLARE_CALL_HELPER(N, unused) \
	GScriptValue invokeScriptFunction(GScriptObject * scriptObject, const char * functionName GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)); \
	GScriptValue invokeScriptFunction(IScriptObject * scriptObject, const char * functionName GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)); \
	GScriptValue invokeScriptFunction(IScriptFunction * scriptFunction GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)); \
	GScriptValue invokeScriptFunctionOnObject(IScriptFunction * scriptFunction GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p));

GPP_REPEAT_2(REF_MAX_ARITY, DECLARE_CALL_HELPER, GPP_EMPTY())

#undef DECLARE_CALL_HELPER

GScriptValue scriptGetValue(GScriptObject * scriptObject, const char * name);
GScriptValue scriptGetValue(IScriptObject * scriptObject, const char * name);
void scriptSetValue(GScriptObject * scriptObject, const char * name, const GScriptValue & value);
void scriptSetValue(IScriptObject * scriptObject, const char * name, const GScriptValue & value);

GScriptValue scriptCreateScriptObject(GScriptObject * scriptObject, const char * name);
GScriptValue scriptCreateScriptObject(IScriptObject * scriptObject, const char * name);

GScriptValue scriptGetScriptFunction(GScriptObject * scriptObject, const char * name);
GScriptValue scriptGetScriptFunction(IScriptObject * scriptObject, const char * name);

GScriptValue scriptGetAsScriptArray(GScriptObject * scriptObject, const char * name);
GScriptValue scriptGetAsScriptArray(IScriptObject * scriptObject, const char * name);

GScriptValue scriptCreateScriptArray(GScriptObject * scriptObject, const char * name);
GScriptValue scriptCreateScriptArray(IScriptObject * scriptObject, const char * name);

GScriptValue scriptGetScriptArrayValue(IScriptArray * scriptArray, size_t index);
void scriptSetScriptArrayValue(IScriptArray * scriptArray, size_t index, const GScriptValue & value);
GScriptValue scriptGetAsScriptArray(IScriptArray * scriptArray, size_t index);
GScriptValue scriptCreateScriptArray(IScriptArray * scriptArray, size_t index);

IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject, bool freeObject);
IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject);

void injectObjectToScript(IScriptObject * scriptObject, IMetaClass * metaClass, void * instance, const char * namespaceName = nullptr);
void injectObjectToScript(GScriptObject * scriptObject, IMetaClass * metaClass, void * instance, const char * namespaceName = nullptr);
void injectObjectToScript(IScriptObject * scriptObject, GMetaClass * metaClass, void * instance, const char * namespaceName = nullptr);
void injectObjectToScript(GScriptObject * scriptObject, GMetaClass * metaClass, void * instance, const char * namespaceName = nullptr);

// We can't do this for GScriptObject because if namespaces is nullptr, we can't return owner of GScriptObject (we can't share the ownership)
IScriptObject * createScriptObject(IScriptObject * owner, const char * namespaces);


} // namespace cpgf



#endif

