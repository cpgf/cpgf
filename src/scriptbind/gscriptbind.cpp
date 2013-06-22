#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gglobal.h"

#include "../pinclude/gbindcommon.h"


namespace cpgf {


GScriptFunction::GScriptFunction()
{
}

GScriptFunction::~GScriptFunction()
{
}


GScriptObject::GScriptObject(const GScriptConfig & config)
	: config(config), owner(NULL)
{
}

GScriptObject::GScriptObject(const GScriptObject & other)
	: config(other.config), owner(NULL)
{
}

GScriptObject::~GScriptObject()
{
}

GScriptValue GScriptObject::getValue(const char * name)
{
	return this->doGetValue(name);
}

void GScriptObject::bindClass(const char * name, IMetaClass * metaClass)
{
	this->doBindClass(name, metaClass);
}

void GScriptObject::bindEnum(const char * name, IMetaEnum * metaEnum)
{
	this->doBindEnum(name, metaEnum);
}

void GScriptObject::bindFundamental(const char * name, const GVariant & value)
{
	this->doBindFundamental(name, value);
}

void GScriptObject::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doBindAccessible(name, instance, accessible);
}

void GScriptObject::bindString(const char * stringName, const char * s)
{
	this->doBindString(stringName, s);
}

void GScriptObject::bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	this->doBindObject(objectName, instance, type, transferOwnership);
}

void GScriptObject::bindRaw(const char * name, const GVariant & value)
{
	this->doBindRaw(name, value);
}

void GScriptObject::bindMethod(const char * name, void * instance, IMetaMethod * method)
{
	this->doBindMethod(name, instance, method);
}

void GScriptObject::bindMethodList(const char * name, IMetaList * methodList)
{
	this->doBindMethodList(name, methodList);
}

void GScriptObject::bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader)
{
	this->doBindCoreService(name, libraryLoader);
}

IMetaClass * GScriptObject::getClass(const char * className)
{
	return this->getValue(className).toMetaClass();
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
	return this->getValue(objectName).toObjectAddress(NULL);
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
	return this->getValue(methodName).toOverridedMethods();
}

//GScriptDataType GScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
//{
//	GScriptValue value(this->getValue(name));
//	if(outMetaTypeItem != NULL) {
//		*outMetaTypeItem = getTypedItemFromScriptValue(value);
//	}
//	return (GScriptDataType)value.getType();
//}

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
	return this->owner == NULL;
}

const char * GScriptObject::getName() const
{
	return this->name.c_str();
}

void GScriptObject::setName(const std::string & newName)
{
	this->name = newName;
}

GScriptObject * GScriptObject::createScriptObject(const char * name)
{
	const int delimiter = '.';
	if(strchr(name, delimiter) == NULL) {
		return this->doCreateScriptObject(name);
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
			if(next != NULL) {
				*next = '\0';
			}
			GScriptObject * obj = scriptObject.get();
			if(obj == NULL) {
				obj = this;
			}
			scriptObject.reset(obj->doCreateScriptObject(head));
			if(! scriptObject) {
				break;
			}
			if(next == NULL) {
				break;
			}
			++next;
			head = next;
		}
		return scriptObject.take();
	}
}

void GScriptObject::holdObject(IObject * object)
{
	this->objectHolder.push_back(GSharedInterface<IObject>(object));
}



} // namespace cpgf


