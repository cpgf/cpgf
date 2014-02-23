#ifndef CPGF_GSCRIPTBINDUTIL_H
#define CPGF_GSCRIPTBINDUTIL_H


#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"

namespace cpgf {


#define DECLARE_CALL_HELPER(N, unused) \
	GVariant invokeScriptFunction(GScriptObject * scriptObject, const char * functionName GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)); \
	GVariant invokeScriptFunction(IScriptObject * scriptObject, const char * functionName GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)); \
	GVariant invokeScriptFunction(IScriptFunction * scriptFunction GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p));

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

void injectObjectToScript(IScriptObject * scriptObject, IMetaClass * metaClass, void * instance, const char * namespaceName = NULL);
void injectObjectToScript(GScriptObject * scriptObject, IMetaClass * metaClass, void * instance, const char * namespaceName = NULL);
void injectObjectToScript(IScriptObject * scriptObject, GMetaClass * metaClass, void * instance, const char * namespaceName = NULL);
void injectObjectToScript(GScriptObject * scriptObject, GMetaClass * metaClass, void * instance, const char * namespaceName = NULL);

// We can't do this for GScriptObject because if namespaces is NULL, we can't return owner of GScriptObject (we can't share the ownership)
IScriptObject * createScriptObject(IScriptObject * owner, const char * namespaces);


} // namespace cpgf



#endif

