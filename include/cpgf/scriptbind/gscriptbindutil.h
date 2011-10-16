#ifndef __GSCRIPTBINDUTIL_H
#define __GSCRIPTBINDUTIL_H


#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"

namespace cpgf {


#define DEF_LOAD_PARAM_HELPER(N, unused) params[N] = &GPP_CONCAT(p, N);
#define DEF_LOAD_PARAM(N) \
	const GMetaVariant * params[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER, GPP_EMPTY())

#define DEF_LOAD_PARAM_HELPER_API(N, unused) params[N] = GPP_CONCAT(p, N).getData();
#define DEF_LOAD_PARAM_API(N) \
	GMetaVarData params[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER_API, GPP_EMPTY())

#define DEF_CALL_HELPER(N, unused) \
	GMetaVariant invokeScriptFunction(GScriptObject * scriptObject, const GScriptName & name GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GMetaVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		return scriptObject->invokeIndirectly(name, params, N); \
	} \
	GMetaVariant invokeScriptFunction(IScriptObject * scriptObject, const GScriptName & name GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GMetaVariant & p)) { \
		DEF_LOAD_PARAM_API(N) \
		GMetaVarData result; \
		GScopedInterface<IScriptName> scriptName(scriptObject->createName(name.getName())); \
		scriptObject->invoke(scriptName.get(), &result, params, N); \
		return GMetaVariant(result); \
	}

GPP_REPEAT_2(REF_MAX_ARITY, DEF_CALL_HELPER, GPP_EMPTY())

#undef DEF_CALL_HELPER
#undef DEF_LOAD_PARAM
#undef DEF_LOAD_PARAM_HELPER
#undef DEF_LOAD_PARAM_API
#undef DEF_LOAD_PARAM_HELPER_API


} // namespace cpgf





#endif
