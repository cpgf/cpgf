#include "cpgf/scriptbind/gscriptrunner.h"
#include "cpgf/private/gscriptrunner_p.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gv8bind.h"
#include "cpgf/gmetaapi.h"

#include <stdexcept>


using namespace v8;

namespace cpgf {


namespace {

class GV8ScriptRunnerImplement : public GScriptRunnerImplement
{
private:
	typedef GScriptRunnerImplement super;

public:
	GV8ScriptRunnerImplement(IMetaService * service);
	~GV8ScriptRunnerImplement();

	virtual void executeString(const char * code);

private:
	bool executeJsString(const char * source);
	void error(const char * message) const;

private:
	HandleScope handleScope;
	Persistent<Context> context;
	Context::Scope * contextScope;
};


GV8ScriptRunnerImplement::GV8ScriptRunnerImplement(IMetaService * service)
	: super(service), handleScope(), context(Context::New())
{
	this->contextScope = new Context::Scope(this->context);
	Local<Object> global = context->Global();

	GScopedInterface<IMetaService> metaService(this->getService());
	GScopedInterface<IScriptObject> scriptObject(createV8ScriptInterface(metaService.get(), global, GScriptConfig()));
	this->setScripeObject(scriptObject.get());
}

GV8ScriptRunnerImplement::~GV8ScriptRunnerImplement()
{
	delete this->contextScope;

	this->context.Dispose();
	this->context.Clear();
}

bool GV8ScriptRunnerImplement::executeJsString(const char * source)
{
	using namespace v8;

	v8::HandleScope handle_scope;
	v8::TryCatch try_catch;
	v8::Handle<v8::Script> script = v8::Script::Compile(String::New(source), String::New("cpgf"));
	if(script.IsEmpty()) {
		v8::String::AsciiValue error(try_catch.Exception());
		this->error(*error);
		return false;
	}
	else {
		v8::Handle<v8::Value> result = script->Run();
		if(result.IsEmpty()) {
			v8::String::AsciiValue error(try_catch.Exception());
			this->error(*error);
			return false;
		}
		else {
			return true;
		}
	}
}

void GV8ScriptRunnerImplement::executeString(const char * code)
{
	this->executeJsString(code);
}

void GV8ScriptRunnerImplement::error(const char * message) const
{
	throw std::runtime_error(message);
}


} // unnamed namespace


GScriptRunner * createV8ScriptRunner(IMetaService * service)
{
	return GScriptRunnerImplement::createScriptRunner(new GV8ScriptRunnerImplement(service));
}



} // namespace cpgf
