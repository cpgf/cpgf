#include "cpgf/scriptbind/gscriptbindutil.h"

#include "../pinclude/gscriptbindapiimpl.h"

#include <string>


using namespace std;
using namespace cpgf::bind_internal;


namespace cpgf {


#define DEF_LOAD_PARAM_HELPER(N, unused) params[N] = &GPP_CONCAT(p, N);
#define DEF_LOAD_PARAM(N) \
	const GVariant * params[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER, GPP_EMPTY())

#define DEF_LOAD_PARAM_HELPER_API(N, unused) params[N] = GPP_CONCAT(p, N).getValue().refData();
#define DEF_LOAD_PARAM_API(N) \
	GVariantData params[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER_API, GPP_EMPTY())

#define DEF_CALL_HELPER(N, unused) \
	GVariant invokeScriptFunction(GScriptObject * scriptObject, const char * functionName GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		return scriptObject->invokeIndirectly(functionName, params, N); \
	} \
	GVariant invokeScriptFunction(IScriptObject * scriptObject, const char * functionName GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)) { \
		DEF_LOAD_PARAM_API(N) \
		GVariant result; \
		GSharedInterface<IScriptObject> holder(scriptObject); /* Hold the object so metaCheckError won't crash if scriptObject is freed in invoke */ \
		scriptObject->invoke(&result.refData(), functionName, params, N); \
		metaCheckError(scriptObject); \
		return result; \
	} \
	GVariant invokeScriptFunction(GScriptFunction * scriptFunction GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		return scriptFunction->invokeIndirectly(params, N); \
	} \
	GVariant invokeScriptFunction(IScriptFunction * scriptFunction GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GTypedVariant & p)) { \
		DEF_LOAD_PARAM_API(N) \
		GVariant result; \
		GSharedInterface<IScriptFunction> holder(scriptFunction); /* Hold the function so metaCheckError won't crash if scriptFunction is freed in invoke */ \
		scriptFunction->invoke(&result.refData(), params, N); \
		metaCheckError(scriptFunction); \
		return result; \
	}

GPP_REPEAT_2(REF_MAX_ARITY, DEF_CALL_HELPER, GPP_EMPTY())


inline bool isCSymbol(unsigned char c) {
    return isalpha(c) || c == '_' || isdigit(c);
}

std::string normalizeReflectName(const char * name)
{
	size_t length = strlen(name);

	if(length == 0) {
		return "";
	}

	const char * p = name + (length - 1);
	const char * end = p;

	while(!isCSymbol(*end) && end > name) {
		--end;
	}
	if(isCSymbol(*end)) {
		++end;
	}

	while(isCSymbol(*p) && p > name) {
		--p;
	}

	if(!isCSymbol(*p)) {
		++p;
	}

	return std::string(p, end - p);
}


GVariant scriptGetFundamental(GScriptObject * scriptObject, const char * name)
{
	return scriptObject->getFundamental(name);
}

GVariant scriptGetFundamental(IScriptObject * scriptObject, const char * name)
{
	GVariant v;
	scriptObject->getFundamental(&v.refData(), name);
	return v;
}


IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject, bool freeObject)
{
	return new ImplScriptObject(scriptObject, freeObject);
}

IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject)
{
	return scriptObjectToInterface(scriptObject, false);
}

GScriptObject * createOrGetScriptObject(GScriptObject * scriptObject, const char * name)
{
	GScriptObject * object = scriptObject->gainScriptObject(name);
	if(object == NULL) {
		object = scriptObject->createScriptObject(name);
	}
	return object;
}

IScriptObject * createOrGetScriptObject(IScriptObject * scriptObject, const char * name)
{
	IScriptObject * object = scriptObject->gainScriptObject(name);
	if(object == NULL) {
		object = scriptObject->createScriptObject(name);
	}
	return object;
}

void injectObjectToScript(IScriptObject * scriptObject, IMetaClass * metaClass, void * instance, const char * namespaceName)
{
	GScopedInterface<IObject> metaObject;
	
	GMetaMapClass mapClass(metaClass);
	
	GScopedInterface<IScriptObject> namespaceHolder;
	if(namespaceName != NULL && *namespaceName) {
		namespaceHolder.reset(createOrGetScriptObject(scriptObject, namespaceName));
		scriptObject = namespaceHolder.get();
	}
	
	const GMetaMapClass::MapType * mapData = mapClass.getMap();
	for(GMetaMapClass::MapType::const_iterator it = mapData->begin(); it != mapData->end(); ++it) {
		const char * name = it->first;
		const GMetaMapItem * item = &it->second;
		metaObject.reset(item->getItem());

		switch(item->getType()) {
			case mmitMethod:
				scriptObject->bindMethod(normalizeReflectName(name).c_str(), instance, gdynamic_cast<IMetaMethod *>(metaObject.get()));
				break;

			case mmitMethodList:
				scriptObject->bindMethodList(normalizeReflectName(name).c_str(), gdynamic_cast<IMetaList *>(metaObject.get()));
				break;

			case mmitProperty:
			case mmitField:
				scriptObject->bindAccessible(normalizeReflectName(name).c_str(), instance, gdynamic_cast<IMetaAccessible *>(metaObject.get()));
				break;

			case mmitEnum:
				scriptObject->bindEnum(normalizeReflectName(name).c_str(), gdynamic_cast<IMetaEnum *>(metaObject.get()));
				break;

			case mmitEnumValue:
				break;

			case mmitClass:
				scriptObject->bindClass(normalizeReflectName(name).c_str(), gdynamic_cast<IMetaClass *>(metaObject.get()));
				break;

			default:
				break;
		}
	}
	
}

void injectObjectToScript(GScriptObject * scriptObject, IMetaClass * metaClass, void * instance, const char * namespaceName)
{
	GScopedInterface<IScriptObject> scriptObjectInterface(scriptObjectToInterface(scriptObject));
	injectObjectToScript(scriptObjectInterface.get(), metaClass, instance, namespaceName);
}

void injectObjectToScript(IScriptObject * scriptObject, GMetaClass * metaClass, void * instance, const char * namespaceName)
{
	GScopedInterface<IMetaClass> metaClassInterface(static_cast<IMetaClass *>(metaItemToInterface(metaClass)));
	injectObjectToScript(scriptObject, metaClassInterface.get(), instance, namespaceName);
}

void injectObjectToScript(GScriptObject * scriptObject, GMetaClass * metaClass, void * instance, const char * namespaceName)
{
	GScopedInterface<IMetaClass> metaClassInterface(static_cast<IMetaClass *>(metaItemToInterface(metaClass)));
	injectObjectToScript(scriptObject, metaClassInterface.get(), instance, namespaceName);
}



} // namespace cpgf

