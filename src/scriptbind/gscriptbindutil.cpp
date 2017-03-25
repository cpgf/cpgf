#include "cpgf/scriptbind/gscriptbindutil.h"
#include "../pinclude/gscriptbindapiimpl.h"

#include <string>

using namespace std;
using namespace cpgf::bind_internal;

namespace cpgf {

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


GScriptValue scriptGetValue(GScriptObject * scriptObject, const char * name)
{
	return scriptObject->getValue(name);
}

GScriptValue scriptGetValue(IScriptObject * scriptObject, const char * name)
{
	GScriptValueData data;
	scriptObject->getValue(&data, name);
	return createScriptValueFromData(data);
}

void scriptSetValue(GScriptObject * scriptObject, const char * name, const GScriptValue & value)
{
	scriptObject->setValue(name, value);
}

void scriptSetValue(IScriptObject * scriptObject, const char * name, const GScriptValue & value)
{
	GScriptValueData data(GScriptValue(value).takeData());
	scriptObject->setValue(name, &data);
}

GScriptValue scriptCreateScriptObject(GScriptObject * scriptObject, const char * name)
{
	return scriptObject->createScriptObject(name);
}

GScriptValue scriptCreateScriptObject(IScriptObject * scriptObject, const char * name)
{
	GScriptValueData data;
	scriptObject->createScriptObject(&data, name);
	return createScriptValueFromData(data);
}

GScriptValue scriptGetScriptFunction(GScriptObject * scriptObject, const char * name)
{
	return scriptObject->getScriptFunction(name);
}

GScriptValue scriptGetScriptFunction(IScriptObject * scriptObject, const char * name)
{
	GScriptValueData data;
	scriptObject->getScriptFunction(&data, name);
	return createScriptValueFromData(data);
}

GScriptValue scriptGetAsScriptArray(GScriptObject * scriptObject, const char * name)
{
	return scriptObject->getAsScriptArray(name);
}

GScriptValue scriptGetAsScriptArray(IScriptObject * scriptObject, const char * name)
{
	GScriptValueData data;
	scriptObject->getAsScriptArray(&data, name);
	return createScriptValueFromData(data);
}

GScriptValue scriptCreateScriptArray(GScriptObject * scriptObject, const char * name)
{
	return scriptObject->createScriptArray(name);
}

GScriptValue scriptCreateScriptArray(IScriptObject * scriptObject, const char * name)
{
	GScriptValueData data;
	scriptObject->createScriptArray(&data, name);
	return createScriptValueFromData(data);
}

GScriptValue scriptGetScriptArrayValue(IScriptArray * scriptArray, size_t index)
{
	GScriptValueData data;
	scriptArray->getValue(&data, (uint32_t)index);
	return createScriptValueFromData(data);
}

void scriptSetScriptArrayValue(IScriptArray * scriptArray, size_t index, const GScriptValue & value)
{
	GScriptValueData data(GScriptValue(value).takeData());
	scriptArray->setValue((uint32_t)index, &data);
}

GScriptValue scriptGetAsScriptArray(IScriptArray * scriptArray, size_t index)
{
	GScriptValueData data;
	scriptArray->getAsScriptArray(&data, (uint32_t)index);
	return createScriptValueFromData(data);
}
GScriptValue scriptCreateScriptArray(IScriptArray * scriptArray, size_t index)
{
	GScriptValueData data;
	scriptArray->createScriptArray(&data, (uint32_t)index);
	return createScriptValueFromData(data);
}
IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject, bool freeObject)
{
	return new ImplScriptObject(scriptObject, freeObject);
}

IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject)
{
	return scriptObjectToInterface(scriptObject, false);
}

void injectObjectToScript(IScriptObject * scriptObject, IMetaClass * metaClass, void * instance, const char * namespaceName)
{
	GScopedInterface<IObject> metaObject;

	GMetaMapClass mapClass(metaClass);

	GScopedInterface<IScriptObject> namespaceHolder;
	if(namespaceName != nullptr && *namespaceName) {
		namespaceHolder.reset(scriptCreateScriptObject(scriptObject, namespaceName).toScriptObject());
		scriptObject = namespaceHolder.get();
	}

	const GMetaMapClass::MapType * mapData = mapClass.getMap();
	for(GMetaMapClass::MapType::const_iterator it = mapData->begin(); it != mapData->end(); ++it) {
		const char * name = it->first;
		const GMetaMapItem * item = &it->second;
		metaObject.reset(item->getItem());

		switch(item->getType()) {
			case mmitMethod:
				scriptSetValue(scriptObject, normalizeReflectName(name).c_str(),
					GScriptValue::fromMethod(instance, gdynamic_cast<IMetaMethod *>(metaObject.get())));
				break;

			case mmitMethodList: {
				IMetaList * metaList = gdynamic_cast<IMetaList *>(metaObject.get());
				GScopedInterface<IMetaList> newMetaList(createMetaList());
				for(uint32_t i = 0; i < metaList->getCount(); ++i) {
					GScopedInterface<IMetaItem> metaItem(metaList->getAt(i));
					newMetaList->add(metaItem.get(), instance);
				}
				scriptSetValue(scriptObject, normalizeReflectName(name).c_str(),
					GScriptValue::fromOverloadedMethods(newMetaList.get()));
			}
				break;

			case mmitProperty:
			case mmitField:
				scriptSetValue(scriptObject, normalizeReflectName(name).c_str(),
					GScriptValue::fromAccessible(instance, gdynamic_cast<IMetaAccessible *>(metaObject.get())));
				break;

			case mmitEnum:
				scriptSetValue(scriptObject, normalizeReflectName(name).c_str(),
					GScriptValue::fromEnum(gdynamic_cast<IMetaEnum *>(metaObject.get())));
				break;

			case mmitEnumValue:
				break;

			case mmitClass:
				scriptSetValue(scriptObject, normalizeReflectName(name).c_str(),
					GScriptValue::fromClass(gdynamic_cast<IMetaClass *>(metaObject.get())));
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

IScriptObject * createScriptObject(IScriptObject * owner, const char * namespaces)
{
	if(namespaces == nullptr || *namespaces == 0) {
		owner->addReference();
		return owner;
	}
	else {
		return scriptCreateScriptObject(owner, namespaces).toScriptObject();
	}
}


} // namespace cpgf

