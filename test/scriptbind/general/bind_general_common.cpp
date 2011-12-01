#include "bind_general_common.h"

#if ENABLE_LUA
#include "cpgf/scriptbind/gluabind.h"
#endif

#if ENABLE_V8
#include "cpgf/scriptbind/gv8bind.h"
#endif


using namespace cpgf;
using namespace std;


namespace testscript {


TestScriptCoder::~TestScriptCoder()
{
}

std::string TestScriptCoder::newObject(const std::string & lhs, const std::string & className)
{
	string s = "";

	if(!lhs.empty()) {
		s = lhs + " = ";
	}

	s = s + this->getNew() + className;

	return s;
}



TestScriptContext::TestScriptContext(TestScriptCoder * coder)
	: service(createDefaultMetaService()), coder(coder), printError(true)
{
}

TestScriptContext::~TestScriptContext()
{
}

void TestScriptContext::setBinding(cpgf::GScriptObject * binding)
{
	this->bindingLib.reset(binding);

	if(binding != NULL) {
		testscript::bindBasicData(binding, this->service.get());
	}
}

void TestScriptContext::setBinding(cpgf::IScriptObject * binding)
{
	this->bindingApi.reset(binding);

	if(binding != NULL) {
		testscript::bindBasicData(binding, this->service.get());
	}
}

bool TestScriptContext::doString(const std::string & code) const
{
	this->printError = true;

	bool ok = true;

	if(this->bindingLib) {
		ok = this->doLib(code.c_str());
	}
	
	if(this->bindingApi) {
		ok = this->doApi(code.c_str()) && ok;
	}

	return ok;
}

bool TestScriptContext::doError(const std::string & code) const
{
	this->printError = false;

	bool ok = true;

	if(this->bindingLib) {
		ok = this->doLib(code.c_str());
	}
	
	if(this->bindingApi) {
		ok = this->doApi(code.c_str()) && ok;
	}

	return !ok;
}



#if ENABLE_LUA

class TestScriptCoderLua : public TestScriptCoder
{
public:
	virtual std::string getNew() {
		return "";
	}
};

class TestScriptContextLua : public TestScriptContext
{
private:
	typedef TestScriptContext super;

public:
	TestScriptContextLua(TestScriptApi api)
		: super(new TestScriptCoderLua), luaStateLib(NULL), luaStateApi(NULL)
	{
		if(api == tsaLib) {
			this->luaStateLib = luaL_newstate();
			luaL_openlibs(this->luaStateLib);

			this->setBinding(new cpgf::GLuaScriptObject(this->getService(), this->luaStateLib, cpgf::GScriptConfig()));
		}

		if(api == tsaApi) {
			this->luaStateApi = luaL_newstate();
			luaL_openlibs(this->luaStateApi);

			this->setBinding(cpgf::createLuaScriptObject(this->getService(), this->luaStateApi, cpgf::GScriptConfig()));
		}
	}

	~TestScriptContextLua() {
		if(this->luaStateLib != NULL) {
			lua_close(this->luaStateLib);
		}
		
		if(this->luaStateApi != NULL) {
			lua_close(this->luaStateApi);
		}
	}

protected:
	virtual bool doLib(const char * code) const {
		luaL_loadstring(this->luaStateLib, code);
		return checkError(lua_pcall(this->luaStateLib, 0, LUA_MULTRET, 0), this->luaStateLib);
	}

	virtual bool doApi(const char * code) const {
		luaL_loadstring(this->luaStateApi, code);
		return checkError(lua_pcall(this->luaStateApi, 0, LUA_MULTRET, 0), this->luaStateApi);
	}

private:
	bool checkError(int errorCode, lua_State * L) const
	{
		if(this->canPrintError() && errorCode != 0) {
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
		}

		return errorCode == 0;
	}

private:
	lua_State * luaStateLib;
	lua_State * luaStateApi;
};

#endif


#if ENABLE_V8

class TestScriptCoderV8 : public TestScriptCoder
{
public:
	virtual std::string getNew() {
		return " new ";
	}
};


#endif


TestScriptContext * createTestScriptContext(TestScriptLang lang, TestScriptApi api)
{
	switch(lang) {
	case tslLua:
		return new TestScriptContextLua(api);

	case tslV8:
		return new TestScriptContextLua(api);
	}

	return NULL;
}


} // namespace testscript



