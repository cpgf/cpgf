#ifndef CPGF_GV8BIND_H
#define CPGF_GV8BIND_H


#include "cpgf/scriptbind/gscriptbind.h"

#include <memory>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4100 4127)
#endif

#include "v8.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif


namespace cpgf {

v8::Isolate *getV8Isolate();

class v8RuntimeException : public std::runtime_error
{
public:
	v8RuntimeException(v8::Local<v8::Value> error, v8::Local<v8::Message> message)
		:
		std::runtime_error(*v8::String::Utf8Value(error)),
		error(new v8::Persistent<v8::Value>(getV8Isolate(), error)),
		message(new v8::Persistent<v8::Message>(getV8Isolate(), message))
	{
	}

	v8::Local<v8::Value> G_API_CC getV8Error() const
	{
		return v8::Local<v8::Value>::New(getV8Isolate(), *error.get());
	}

	v8::Local<v8::Message> G_API_CC getV8Message() const
	{
		return v8::Local<v8::Message>::New(getV8Isolate(), *message.get());
	}

private:
	std::shared_ptr<v8::Persistent<v8::Value> > error;
	std::shared_ptr<v8::Persistent<v8::Message> > message;
};


GScriptObject * createV8ScriptObject(IMetaService * service, v8::Local<v8::Object> object);
IScriptObject * createV8ScriptInterface(IMetaService * service, v8::Local<v8::Object> object);

void clearV8DataPool();

GScriptValue convertV8ObjectToScriptValue(v8::Local<v8::Object> object);

} // namespace cpgf




#endif


