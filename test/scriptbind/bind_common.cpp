#include "bind_common.h"

#if ENABLE_LUA
#include "cpgf/scriptbind/gluabind.h"
#endif

#if ENABLE_V8
#include "cpgf/scriptbind/gv8bind.h"
#include "cpgf/scriptbind/gv8runner.h"
#endif

#if ENABLE_PYTHON
#include "cpgf/scriptbind/gpythonbind.h"
#endif

#if ENABLE_SPIDERMONKEY
#include "cpgf/scriptbind/gspidermonkeybind.h"
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

std::string TestScriptCoder::getVarPrefix()
{
	return "";
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

	binding->bindCoreService("cpgf", NULL);
}

void TestScriptContext::setBinding(cpgf::IScriptObject * binding)
{
	this->bindingApi.reset(binding);

	binding->bindCoreService("cpgf", NULL);
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

	v8::HandleScope handle_scope(getV8Isolate());
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
		: super(new TestScriptCoderV8), handleScope(getV8Isolate()), context(getV8Isolate(), Context::New(getV8Isolate()))//, contextScope(context)
	{
		this->contextScope = new Context::Scope(getV8Isolate(), this->context);
	    Local<Context> ctx = Local<Context>::New(getV8Isolate(), context);
		Local<Object> global = ctx->Global();

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


#if ENABLE_PYTHON

class TestScriptCoderPython : public TestScriptCoder
{
public:
	virtual std::string getNew() {
		return "";
	}
};

class TestScriptContextPython : public TestScriptContext
{
private:
	typedef TestScriptContext super;

public:
	TestScriptContextPython(TestScriptApi api)
		: super(new TestScriptCoderPython), moduleMain(NULL), mainDict(NULL)
	{
		Py_InitializeEx(0);

		this->moduleMain = PyImport_ImportModule("__main__");
		this->mainDict = PyModule_GetDict(this->moduleMain);

		Py_XINCREF(this->mainDict);

		if(api == tsaLib) {
			this->setBinding(cpgf::createPythonScriptObject(this->getService(), this->moduleMain, cpgf::GScriptConfig()));
		}

		if(api == tsaApi) {
			this->setBinding(cpgf::createPythonScriptInterface(this->getService(), this->moduleMain, cpgf::GScriptConfig()));
		}
	}

	~TestScriptContextPython() {
		Py_XDECREF(this->mainDict);
		Py_XDECREF(this->moduleMain);

		Py_Finalize();
	}

	virtual bool isPython() const {
		return true;
	}

protected:
	virtual bool doLib(const char * code) const {
		return this->doCode(code);
	}

	virtual bool doApi(const char * code) const {
		return this->doCode(code);
	}
	
private:
	bool doCode(const char * code) const {
		return PyRun_SimpleString(code) == 0;
		
		//GPythonScopedPointer codeObject(Py_CompileString(code, "", Py_single_input));
		//if(! codeObject) {
		//	return false;
		//}
		//GPythonScopedPointer result(PyEval_EvalCode((PyCodeObject *)(codeObject.get()), this->mainDict, this->mainDict));
		//return (bool)result;

//		GPythonScopedPointer result(PyRun_String(code, Py_single_input, this->mainDict, this->mainDict));
//		return (bool)result;
	}

	bool checkError(int error) const
	{
		if(this->canPrintError() && error < 0) {
			fprintf(stderr, "Error\n");
		}

		return error >=0;
	}

private:
	PyObject * moduleMain;
	PyObject * mainDict;
};

#endif


#if ENABLE_SPIDERMONKEY

using namespace JS;

template <int Arity, typename F>
struct JSNewRuntimeCaller
{
};

template <typename F>
struct JSNewRuntimeCaller <1, F>
{
	static JSRuntime * call(F * f) {
		return f(128L * 1024L * 1024L);
	}
};

template <typename F>
struct JSNewRuntimeCaller <2, F>
{
	static JSRuntime * call(F * f) {
		return f(512L * 1024L * 1024L, JS_NO_HELPER_THREADS);
	}
};

template <typename F>
JSRuntime * callJSNewRuntime(F * f)
{
	return JSNewRuntimeCaller<GFunctionTraits<F>::Arity, F>::call(f);
}

class TestScriptCoderSpiderMonkey : public TestScriptCoder
{
public:
	virtual std::string getNew() {
		return " new ";
	}
	
	virtual std::string getVarPrefix() {
		return "var ";
	}
};

class SpiderMonkeyRuntime
{
public:
	SpiderMonkeyRuntime()
		: jsRuntime(callJSNewRuntime(&JS_NewRuntime))
	{
	}

	~SpiderMonkeyRuntime() {
		JS_DestroyRuntime(this->jsRuntime);
		JS_ShutDown();
	}

public:
	JSRuntime * jsRuntime;
};
static GScopedPointer<SpiderMonkeyRuntime> spiderMonkeyRuntime;

void reportError(JSContext *cx, const char *message, JSErrorReport *report)
{
//	fprintf(stderr, "SpiderMonkey error: %s\n", message);
}

class SpiderMonkeyEnv
{
public:
	SpiderMonkeyEnv()
	{
		if(!spiderMonkeyRuntime)
		{
			spiderMonkeyRuntime.reset();
			spiderMonkeyRuntime.reset(new SpiderMonkeyRuntime);
		}
		this->jsContext = JS_NewContext(spiderMonkeyRuntime->jsRuntime, 8192);
//		JS_SetOptions(this->jsContext, JSOPTION_VAROBJFIX | JSOPTION_METHODJIT);
		JS_SetOptions(this->jsContext, JSOPTION_METHODJIT);
		JS_SetVersion(this->jsContext, JSVERSION_LATEST);
		JS_SetErrorReporter(this->jsContext, &reportError);
		this->jsGlobal = createSpiderMonkeyGlobaObject(this->jsContext);
		JS_InitStandardClasses(this->jsContext, this->jsGlobal);
	}

	~SpiderMonkeyEnv() {
//		JS_DestroyContext(this->jsContext);
//		JS_DestroyContextNoGC(this->jsContext);
	}

	void resetContext() {
//		JS_ClearNonGlobalObject(this->jsContext, this->jsGlobal);
	}

public:
	JSContext * jsContext;
	JSObject  * jsGlobal;
};

static GScopedPointer<SpiderMonkeyEnv> spiderMonkeyEnv;

const char * const SpiderMonkeyNullObjects[] = {
	"nso", "methodConst", "incStaticValue", "add", "Inner"
};

class TestScriptContextSpiderMonkey : public TestScriptContext
{
private:
	typedef TestScriptContext super;

public:
	TestScriptContextSpiderMonkey(TestScriptApi api)
		: super(new TestScriptCoderSpiderMonkey)
	{
		if(! spiderMonkeyEnv)
		{
			spiderMonkeyEnv.reset();
			spiderMonkeyEnv.reset(new SpiderMonkeyEnv());
		}
		spiderMonkeyEnv->resetContext();

		if(api == tsaLib) {
			this->setBinding(cpgf::createSpiderMonkeyScriptObject(this->getService(), spiderMonkeyEnv->jsContext, spiderMonkeyEnv->jsGlobal, GScriptConfig()));
			
			this->nullObjects();
		}

		if(api == tsaApi) {
			this->setBinding(cpgf::createSpiderMonkeyScriptInterface(this->getService(), spiderMonkeyEnv->jsContext, spiderMonkeyEnv->jsGlobal, cpgf::GScriptConfig()));

			this->nullObjects();
		}
	}

	~TestScriptContextSpiderMonkey() {
	}

	virtual bool isSpiderMonkey() const {
		return true;
	}

protected:
	virtual bool doLib(const char * code) const {
		return this->executeString(code, this->canPrintError());
	}

	virtual bool doApi(const char * code) const {
		return this->executeString(code, this->canPrintError());
	}

	bool executeString(const char * source, bool printError) const {
		jsval result;
		JSBool success = JS_EvaluateScript(spiderMonkeyEnv->jsContext, spiderMonkeyEnv->jsGlobal, source, (unsigned int)strlen(source), "script", 1, &result);
		return success == JS_TRUE;
	}

	void nullObjects() {
		for(int i = 0; i < sizeof(SpiderMonkeyNullObjects) / sizeof(SpiderMonkeyNullObjects[0]); ++i) {
			if(this->getBindingLib()) {
				scriptSetValue(this->getBindingLib(), SpiderMonkeyNullObjects[i], GScriptValue::fromNull());
			}
			else {
				scriptSetValue(this->getBindingApi(), SpiderMonkeyNullObjects[i], GScriptValue::fromNull());
			}
		}
	}
};


#endif


TestScriptContext * createTestScriptContext(TestScriptLang lang, TestScriptApi api)
{
	switch(lang) {
	case tslLua:
#if ENABLE_LUA
		return new TestScriptContextLua(api);
#else
		break;
#endif

	case tslV8:
#if ENABLE_V8
		return new TestScriptContextV8(api);
#else
		break;
#endif

	case tslPython:
#if ENABLE_PYTHON
		return new TestScriptContextPython(api);
#else
		break;
#endif

	case tslSpider:
#if ENABLE_SPIDERMONKEY
		return new TestScriptContextSpiderMonkey(api);
#else
		break;
#endif
	}

	return NULL;
}


} // namespace testscript


