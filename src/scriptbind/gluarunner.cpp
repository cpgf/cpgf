#include "cpgf/scriptbind/gluarunner.h"
#include "cpgf/private/gscriptrunner_p.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gluabind.h"
#include "cpgf/gmetaapi.h"

#include "lua.hpp"

#include <stdexcept>
#include <stdio.h>

namespace cpgf {


namespace {

class GLuaScriptRunnerImplement : public GScriptRunnerImplement
{
private:
	typedef GScriptRunnerImplement super;

public:
	GLuaScriptRunnerImplement(IMetaService * service);
	~GLuaScriptRunnerImplement();

	virtual void executeString(const char * code);

private:
	void checkError(int errorCode) const;

private:
	lua_State * luaState;
};


GLuaScriptRunnerImplement::GLuaScriptRunnerImplement(IMetaService * service)
	: super(service), luaState(luaL_newstate())
{
	luaL_openlibs(this->luaState);

	GScopedInterface<IMetaService> metaService(this->getService());
	GScopedInterface<IScriptObject> scriptObject(createLuaScriptInterface(metaService.get(), this->luaState, GScriptConfig()));
	this->setScripeObject(scriptObject.get());
}

GLuaScriptRunnerImplement::~GLuaScriptRunnerImplement()
{
	lua_close(this->luaState);
}

void GLuaScriptRunnerImplement::executeString(const char * code)
{
	luaL_loadstring(this->luaState, code);
	this->checkError(lua_pcall(this->luaState, 0, LUA_MULTRET, 0));
}

void GLuaScriptRunnerImplement::checkError(int errorCode) const
{
	if(errorCode != 0) {
		const char * message = lua_tostring(this->luaState, -1);
		fprintf(stderr, "Lua error: %s\n", message);
		throw std::runtime_error(message);
	}
}


} // unnamed namespace


GScriptRunner * createLuaScriptRunner(IMetaService * service)
{
	return GScriptRunnerImplement::createScriptRunner(new GLuaScriptRunnerImplement(service));
}



} // namespace cpgf
