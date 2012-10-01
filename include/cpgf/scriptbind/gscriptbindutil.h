#ifndef __GSCRIPTBINDUTIL_H
#define __GSCRIPTBINDUTIL_H


#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"

namespace cpgf {


#define DECLARE_CALL_HELPER(N, unused) \
	GVariant invokeScriptFunction(GScriptObject * scriptObject, const char * functionName GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)); \
	GVariant invokeScriptFunction(IScriptObject * scriptObject, const char * functionName GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)); \
	GVariant invokeScriptFunction(GScriptFunction * scriptFunction GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)); \
	GVariant invokeScriptFunction(IScriptFunction * scriptFunction GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p));

GPP_REPEAT_2(REF_MAX_ARITY, DECLARE_CALL_HELPER, GPP_EMPTY())

#undef DECLARE_CALL_HELPER

GVariant scriptGetFundamental(GScriptObject * scriptObject, const char * name);
GVariant scriptGetFundamental(IScriptObject * scriptObject, const char * name);

IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject, bool freeObject);
IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject);

void injectObjectToScript(IScriptObject * scriptObject, IMetaClass * metaClass, void * instance);


} // namespace cpgf



#endif

