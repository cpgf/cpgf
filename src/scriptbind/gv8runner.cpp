#include "cpgf/scriptbind/gscriptrunner.h"
#include "cpgf/private/gscriptrunner_p.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gv8bind.h"
#include "cpgf/gmetaapi.h"

#include <stdexcept>
#include <string>

using namespace v8;
using namespace std;

namespace cpgf {

static v8::Isolate *cpgf_isolate = NULL;

namespace {

class GV8ScriptRunnerImplement : public GScriptRunnerImplement
{
private:
	typedef GScriptRunnerImplement super;

public:
	GV8ScriptRunnerImplement(IMetaService * service);
	GV8ScriptRunnerImplement(IMetaService * service, Handle<Context> ctx);
	~GV8ScriptRunnerImplement();

	virtual void executeString(const char * code);

private:
	bool executeJsString(const char * source);
	void error(const char * message) const;
	void init();

private:
	HandleScope handleScope;
	Persistent<Context> context;
	Context::Scope * contextScope;
};


GV8ScriptRunnerImplement::GV8ScriptRunnerImplement(IMetaService * service)
	: super(service), handleScope(cpgf_isolate), context(cpgf_isolate, Context::New())
{
	init();
}

GV8ScriptRunnerImplement::GV8ScriptRunnerImplement(IMetaService * service, Handle<Context> ctx)
	: super(service), handleScope(cpgf_isolate), context(cpgf_isolate, ctx)
{
	init();
}

void GV8ScriptRunnerImplement::init()
{
	contextScope = new Context::Scope(cpgf_isolate, context);
	Local<Context> ctx = Local<Context>::New(cpgf_isolate, context);
	Local<Object> global = ctx->Global();

	GScopedInterface<IMetaService> metaService(getService());
	GScopedInterface<IScriptObject> scriptObject(createV8ScriptInterface(metaService.get(), global, GScriptConfig()));
	setScripeObject(scriptObject.get());
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

	Local<Context> ctx = Local<Context>::New(cpgf_isolate, context);
	ctx->Enter();
	v8::HandleScope handle_scope(cpgf_isolate);
	v8::TryCatch v8TryCatch;
	v8::Handle<v8::Script> script = v8::Script::Compile(String::New(source), String::New("cpgf"));
	if(! script.IsEmpty()) {
		v8::Handle<v8::Value> result = script->Run();
		ctx->Exit();
		if(! result.IsEmpty()) {
			return true;
		}
	}
	v8::String::AsciiValue stackTrace(v8TryCatch.StackTrace());
	this->error(*stackTrace);
	return false;
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

GScriptRunner * createV8ScriptRunner(IMetaService * service, Handle<Context> ctx)
{
	return GScriptRunnerImplement::createScriptRunner(new GV8ScriptRunnerImplement(service, ctx));
}


} // namespace cpgf
