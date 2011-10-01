#ifndef __TEST_LUA_COMMON_H
#define __TEST_LUA_COMMON_H



#include "cpgf/gmetaapi.h"
#include "cpgf/scriptbind/gluabind.h"

#include "../testscriptbind.h"

#define LINE(...) # __VA_ARGS__ "\n"


namespace testscript {

class TestLuaContext
{
public:
	TestLuaContext() {
		this->luaState = luaL_newstate();
		luaL_openlibs(this->luaState);
		this->service.reset(cpgf::createMetaService());
		this->binding.reset(new cpgf::GLuaScriptObject(this->service.get(), this->luaState, cpgf::GScriptConfig()));
		testscript::bindBasicData(this->binding.get(), this->service.get());
	}

	~TestLuaContext() {
		lua_close(this->luaState);
	}

	cpgf::IMetaService * getService() const {
		return this->service.get();
	}

	lua_State * getLua() const {
		return this->luaState;
	}

	cpgf::GLuaScriptObject * getBinding() const {
		return this->binding.get();
	}

	bool doString(const char * code) const {
		luaL_loadstring(this->luaState, code);
		lua_call(this->luaState, 0, LUA_MULTRET);

		return true;
	}

private:
	lua_State * luaState;
	cpgf::GApiScopedPointer<cpgf::IMetaService> service;
	cpgf::GScopedPointer<cpgf::GLuaScriptObject> binding;
};

inline TestLuaContext * getLuaContext()
{
	static TestLuaContext context;
	
	return &context;
}


}


using namespace cpgf;
using namespace testscript;



#endif
