#ifndef CPGF_GSPIDERMONKEYBIND_H
#define CPGF_GSPIDERMONKEYBIND_H

#include "cpgf/scriptbind/gscriptbind.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4127 4100 4800 4512 4480 4267)
#endif

#ifndef UINT32_MAX
#    define UINT32_MAX  ((uint32_t)-1)
#endif

#include "jsapi.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


namespace cpgf {

JSObject * createSpiderMonkeyGlobaObject(JSContext * jsContext);
GScriptObject * createSpiderMonkeyScriptObject(IMetaService * service, JSContext * jsContext, JSObject  * jsObject, const GScriptConfig & config);
IScriptObject * createSpiderMonkeyScriptInterface(IMetaService * service, JSContext *jsContext, JSObject  * jsObject, const GScriptConfig & config);



} // namespace cpgf



#endif
