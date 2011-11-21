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
		this->printError = true;

		this->service.reset(cpgf::createDefaultMetaService());

		this->luaStateLib = luaL_newstate();
		luaL_openlibs(this->luaStateLib);

		this->bindingLib.reset(new cpgf::GLuaScriptObject(this->service.get(), this->luaStateLib, cpgf::GScriptConfig()));
		testscript::bindBasicData(this->bindingLib.get(), this->service.get());

		this->luaStateApi = luaL_newstate();
		luaL_openlibs(this->luaStateApi);

		this->bindingApi.reset(cpgf::createLuaScriptObject(this->service.get(), this->luaStateApi, cpgf::GScriptConfig()));
		testscript::bindBasicData(this->bindingApi.get(), this->service.get());
	}

	~TestLuaContext() {
		lua_close(this->luaStateLib);
		lua_close(this->luaStateApi);
	}

	cpgf::IMetaService * getService() const {
		return this->service.get();
	}

	lua_State * getLuaLib() const {
		return this->luaStateLib;
	}

	lua_State * getLuaApi() const {
		return this->luaStateApi;
	}

	cpgf::GLuaScriptObject * getBindingLib() const {
		return this->bindingLib.get();
	}

	cpgf::IScriptObject * getBindingApi() const {
		this->bindingApi->addReference();
		return this->bindingApi.get();
	}

	bool doLib(const char * code) const {
		luaL_loadstring(this->luaStateLib, code);
		return checkError(lua_pcall(this->luaStateLib, 0, LUA_MULTRET, 0), this->luaStateLib);
	}

	bool doApi(const char * code) const {
		luaL_loadstring(this->luaStateApi, code);
		return checkError(lua_pcall(this->luaStateApi, 0, LUA_MULTRET, 0), this->luaStateApi);
	}

	bool doString(const char * code) const {
		bool ok;

		this->printError = true;

		ok = this->doLib(code);
		ok = this->doApi(code) && ok;

		return ok;
	}

	bool doError(const char * code) const {
		bool ok;

		this->printError = false;

		ok = this->doLib(code);
		ok = this->doApi(code) && ok;

		return !ok;
	}

private:
	bool checkError(int errorCode, lua_State * L) const
	{
		if(this->printError && errorCode != 0) {
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
		}

		return errorCode == 0;
	}

private:
	cpgf::GScopedInterface<cpgf::IMetaService> service;
	lua_State * luaStateLib;
	lua_State * luaStateApi;
	cpgf::GScopedPointer<cpgf::GLuaScriptObject> bindingLib;
	cpgf::GScopedInterface<cpgf::IScriptObject> bindingApi;
	mutable bool printError;
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
