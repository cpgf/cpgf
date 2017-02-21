#include "../testscriptbind.h"

#include "../testscriptbindmetadata2.h"

#include "cpgf/scriptbind/gv8bind.h"
#include "cpgf/scriptbind/gv8runner.h"
#include "cpgf/gvariant.h"

#include <iostream>
namespace {

void rawV8Method(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
	v8::Local<v8::Object> v8obj = args[0].As<v8::Object>();
	
	GScriptValue scriptValue = cpgf::convertV8ObjectToScriptValue(v8obj);
	testscript::TestObject * testObj = cpgf::fromVariant<testscript::TestObject *>(scriptValue.getValue());
	args.GetReturnValue().Set(testObj == testObj->self());
}


void test__getScriptValue__(TestScriptContext * context)
{
	using namespace v8;

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	HandleScope handle_scope(isolate);
	Local<Context> ctx = isolate->GetCurrentContext();
	Local<Object> global( ctx->Global() );
	v8::Local<v8::FunctionTemplate> t = v8::FunctionTemplate::New(getV8Isolate(), rawV8Method);
	v8::Local<v8::Function> fn = t->GetFunction();
	v8::Local<v8::String> fn_name = v8::String::NewFromUtf8(isolate, "__call_v8_raw_method_for_getScriptValue");
	fn->SetName(fn_name);
	global->Set(fn_name, fn);
	
	QNEWOBJ(a, TestObject(3))
	QASSERT(__call_v8_raw_method_for_getScriptValue(a))
}

#define CASE test__getScriptValue__
#include "../testcase_v8.h"


}
