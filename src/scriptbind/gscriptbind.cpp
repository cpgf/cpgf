#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gglobal.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"


namespace cpgf {


GScriptObject::GScriptObject(const GScriptConfig & config)
	: config(config), owner(nullptr)
{
}

GScriptObject::GScriptObject(const GScriptObject & other)
	: config(other.config), owner(nullptr)
{
}

GScriptObject::~GScriptObject()
{
}

GScriptValue GScriptObject::getValue(const char * name)
{
	return this->doGetValue(name);
}

extern int Error_ScriptBinding_CantSetScriptValue;
void GScriptObject::setValue(const char * name, const GScriptValue & value)
{
	switch(value.getType()) {
		// We can't set any script object back to script engine,
		// otherwise, cross module portability will be broken.
		case GScriptValue::typeScriptObject:
		case GScriptValue::typeScriptFunction:
		case GScriptValue::typeScriptArray:
			raiseCoreException(Error_ScriptBinding_CantSetScriptValue);
			break;

		default:
			this->doSetValue(name, value);
			break;
	}
}

void GScriptObject::bindClass(const char * name, IMetaClass * metaClass)
{
	this->setValue(name, GScriptValue::fromClass(metaClass));
}

void GScriptObject::bindEnum(const char * name, IMetaEnum * metaEnum)
{
	this->setValue(name, GScriptValue::fromEnum(metaEnum));
}

void GScriptObject::bindFundamental(const char * name, const GVariant & value)
{
	this->setValue(name, GScriptValue::fromFundamental(value));
}

void GScriptObject::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->setValue(name, GScriptValue::fromAccessible(instance, accessible));
}

void GScriptObject::bindString(const char * stringName, const char * s)
{
	this->setValue(stringName, GScriptValue::fromString(s));
}

void GScriptObject::bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	this->setValue(objectName, GScriptValue::fromObject(instance, type, transferOwnership));
}

void GScriptObject::bindRaw(const char * name, const GVariant & value)
{
	this->setValue(name, GScriptValue::fromRaw(value));
}

void GScriptObject::bindMethod(const char * name, void * instance, IMetaMethod * method)
{
	this->setValue(name, GScriptValue::fromMethod(instance, method));
}

void GScriptObject::bindMethodList(const char * name, IMetaList * methodList)
{
	this->setValue(name, GScriptValue::fromOverloadedMethods(methodList));
}

void GScriptObject::bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader)
{
	this->doBindCoreService(name, libraryLoader);
}

IMetaClass * GScriptObject::getClass(const char * className)
{
	return this->getValue(className).toClass();
}

IMetaEnum * GScriptObject::getEnum(const char * enumName)
{
	return this->getValue(enumName).toEnum();
}

GVariant GScriptObject::getFundamental(const char * name)
{
	return this->getValue(name).toFundamental();
}

std::string GScriptObject::getString(const char * stringName)
{
	return this->getValue(stringName).toString();
}

void * GScriptObject::getObject(const char * objectName)
{
	return this->getValue(objectName).toObjectAddress(nullptr, nullptr);
}

GVariant GScriptObject::getRaw(const char * name)
{
	return this->getValue(name).toRaw();
}

IMetaMethod * GScriptObject::getMethod(const char * methodName, void ** outInstance)
{
	return this->getValue(methodName).toMethod(outInstance);
}

IMetaList * GScriptObject::getMethodList(const char * methodName)
{
	return this->getValue(methodName).toOverloadedMethods();
}

GScriptValue::Type GScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
{
	GScriptValue value(this->getValue(name));
	if(outMetaTypeItem != nullptr) {
		*outMetaTypeItem = getTypedItemFromScriptValue(value);
	}
	return value.getType();
}

bool GScriptObject::valueIsNull(const char * name)
{
	return this->getValue(name).isNull();
}

void GScriptObject::nullifyValue(const char * name)
{
	this->setValue(name, GScriptValue::fromNull());
}

const GScriptConfig & GScriptObject::getConfig() const
{
	return this->config;
}

GScriptObject * GScriptObject::getOwner() const
{
	return this->owner;
}

void GScriptObject::setOwner(GScriptObject * newOwner)
{
	this->owner = newOwner;
}

bool GScriptObject::isGlobal() const
{
	return this->owner == nullptr;
}

const char * GScriptObject::getName() const
{
	return this->name.c_str();
}

void GScriptObject::setName(const std::string & newName)
{
	this->name = newName;
}

GScriptValue GScriptObject::createScriptObject(const char * name)
{
	GScriptObject * object = nullptr;
	const int delimiter = '.';
	if(strchr(name, delimiter) == nullptr) {
		object = this->doCreateScriptObject(name);
	}
	else {
		size_t len = strlen(name);
		GScopedArray<char> tempName(new char[len + 1]);
		memmove(tempName.get(), name, len + 1);
		char * next;
		char * head = tempName.get();
		GScopedPointer<GScriptObject> scriptObject;
		for(;;) {
			next = strchr(head, delimiter);
			if(next != nullptr) {
				*next = '\0';
			}
			GScriptObject * obj = scriptObject.get();
			if(obj == nullptr) {
				obj = this;
			}
			scriptObject.reset(obj->doCreateScriptObject(head));
			if(! scriptObject) {
				break;
			}
			if(next == nullptr) {
				break;
			}
			++next;
			head = next;
		}
		object = scriptObject.take();
	}
	if(object == nullptr) {
		return GScriptValue();
	}
	else {
		GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(object, true));
		return GScriptValue::fromScriptObject(scriptObject.get());
	}
}

void GScriptObject::holdObject(IObject * object)
{
	this->objectHolder.push_back(GSharedInterface<IObject>(object));
}



} // namespace cpgf


