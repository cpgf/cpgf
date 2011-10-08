#ifndef __GSCRIPTBINDUTIL_H
#define __GSCRIPTBINDUTIL_H


#include "cpgf/scriptbind/gscriptbind.h"

namespace cpgf {


#define DEF_LOAD_PARAM_HELPER(N, unused) params[N] = &GPP_CONCAT(p, N);
#define DEF_LOAD_PARAM(N) \
	const GVariant * params[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER, GPP_EMPTY())

#define DEF_CALL_HELPER(N, unused) \
	GVariant callScriptFunction(GScriptObject * scriptObject, const GScriptName & name GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		return scriptObject->invokeIndirectly(name, params, N); \
	}

GPP_REPEAT_2(REF_MAX_ARITY, DEF_CALL_HELPER, GPP_EMPTY())

#undef DEF_CALL_HELPER
#undef DEF_LOAD_PARAM
#undef DEF_LOAD_PARAM_HELPER


} // namespace cpgf





#endif
