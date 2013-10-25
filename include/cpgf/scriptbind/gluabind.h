#ifndef CPGF_GLUABIND_H
#define CPGF_GLUABIND_H

#include "cpgf/scriptbind/gscriptbind.h"


#include "lua.hpp"


namespace cpgf {

GScriptObject * createLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config);
IScriptObject * createLuaScriptInterface(IMetaService * service, lua_State * L, const GScriptConfig & config);



} // namespace cpgf



#endif
