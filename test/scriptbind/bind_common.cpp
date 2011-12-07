#include "bind_common.h"

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

			this->setBinding(cpgf::createLuaScriptObject(this->getService(), this->luaStateLib, cpgf::GScriptConfig()));
		}

		if(api == tsaApi) {
			this->luaStateApi = luaL_newstate();
			luaL_openlibs(this->luaStateApi);

			this->setBinding(cpgf::createLuaScriptInterface(this->getService(), this->luaStateApi, cpgf::GScriptConfig()));
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

	virtual bool isLua() const {
		return true;
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

using namespace v8;

class TestScriptCoderV8 : public TestScriptCoder
{
public:
	virtual std::string getNew() {
		return " new ";
	}
};

bool executeString(const char * source, bool printError, bool printResult = false)
{
	using namespace v8;

	v8::HandleScope handle_scope;
	v8::TryCatch try_catch;
	v8::Handle<v8::Script> script = v8::Script::Compile(String::New(source), String::New("sample"));
	if(script.IsEmpty()) {
		if(printError) {
			v8::String::AsciiValue error(try_catch.Exception());
			printf("%s\n", *error);
		}
		return false;
	}
	else {
		v8::Handle<v8::Value> result = script->Run();
		if(result.IsEmpty()) {
			if(printError) {
				v8::String::AsciiValue error(try_catch.Exception());
				printf("%s\n", *error);
			}
			return false;
		}
		else {
			if(printResult && !result->IsUndefined()) {
				v8::String::AsciiValue str(result);
				printf("%s\n", *str);
			}
			return true;
		}
	}
}

class TestScriptContextV8 : public TestScriptContext
{
private:
	typedef TestScriptContext super;

public:
	TestScriptContextV8(TestScriptApi api)
		: super(new TestScriptCoderV8), handleScope(), context(Context::New())//, contextScope(context)
	{
		this->contextScope = new Context::Scope(this->context);
		Local<Object> global = context->Global();

		if(api == tsaLib) {
			this->setBinding(cpgf::createV8ScriptObject(this->getService(), global, GScriptConfig()));
		}

		if(api == tsaApi) {
			this->setBinding(cpgf::createV8ScriptInterface(this->getService(), global, cpgf::GScriptConfig()));
		}
	}

	~TestScriptContextV8() {
		delete this->contextScope;

		this->context.Dispose();
		this->context.Clear();
	}

	virtual bool isV8() const {
		return true;
	}
	
protected:
	virtual bool doLib(const char * code) const {
		return executeString(code, this->canPrintError());
	}

	virtual bool doApi(const char * code) const {
		return executeString(code, this->canPrintError());
	}

private:
	HandleScope handleScope;
	Persistent<Context> context;
	Context::Scope * contextScope;
};


#endif


TestScriptContext * createTestScriptContext(TestScriptLang lang, TestScriptApi api)
{
	switch(lang) {
	case tslLua:
		return new TestScriptContextLua(api);

	case tslV8:
		return new TestScriptContextV8(api);
	}

	return NULL;
}


} // namespace testscript



