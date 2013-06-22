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

void GScriptObject::setValue(const char * name, const GScriptValue & value)
{
	switch(value.getType()) {
		case GScriptValue::typeNull:
			this->doBindNull(name);
			break;

		case GScriptValue::typeFundamental:
			this->doBindFundamental(name, value.toFundamental());
			break;

		case GScriptValue::typeString:
			this->doBindString(name, value.toString().c_str());
			break;

		case GScriptValue::typeMetaClass: {
			GScopedInterface<IMetaClass> metaClass(value.toMetaClass());
			this->doBindClass(name, metaClass.get());
			break;
		}

		case GScriptValue::typeObject: {
			GScopedInterface<IMetaClass> metaClassGuard;
			IMetaClass * metaClass;
			bool transferOwnership;
			void * instance = objectAddressFromVariant(value.toObject(&metaClass, &transferOwnership));
			this->doBindObject(name, instance, metaClass, transferOwnership);
			break;
		}

		case GScriptValue::typeMethod: {
			void * instance;
			GScopedInterface<IMetaMethod> method(value.toMethod(&instance));
			this->doBindMethod(name, instance, method.get());
			break;
		}

		case GScriptValue::typeOverridedMethods: {
			GScopedInterface<IMetaList> methodList(value.toOverridedMethods());
			this->doBindMethodList(name, methodList.get());
			break;
		}

		case GScriptValue::typeEnum: {
			GScopedInterface<IMetaEnum> metaEnum(value.toEnum());
			this->doBindEnum(name, metaEnum.get());
			break;
		}

		case GScriptValue::typeRaw:
			this->doBindRaw(name, value.toRaw());
			break;

		case GScriptValue::typeAccessible: {
			void * instance;
			GScopedInterface<IMetaAccessible> accessible(value.toAccessible(&instance));
			this->doBindAccessible(name, instance, accessible.get());
			break;
		}

		//case GScriptValue::typeScriptObject: {
		//	GScopedInterface<IScriptObject> scriptObject(value.toScriptObject());
		//	this->doBindScriptObject(name, scriptObject.get());
		//	break;
		//}

		//case GScriptValue::typeScriptMethod: {
		//	GScopedInterface<IScriptFunction> scriptMethod(value.toScriptMethod());
		//	this->doBindScriptFunction(name, scriptMethod.get());
		//	break;
		//}

	}
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
	return this->getValue(objectName).toObjectAddress(NULL, NULL);
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

GScriptDataType GScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
{
	GScriptValue value(this->getValue(name));
	if(outMetaTypeItem != NULL) {
		*outMetaTypeItem = getTypedItemFromScriptValue(value);
	}
	return (GScriptDataType)value.getType();
}

bool GScriptObject::valueIsNull(const char * name)
{
	return this->getType(name, NULL) == GScriptValue::typeNull;
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


