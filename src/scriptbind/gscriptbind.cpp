#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gglobal.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"


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

extern int Error_ScriptBinding_CantSetScriptValue;
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

		case GScriptValue::typeClass: {
			GScopedInterface<IMetaClass> metaClass(value.toClass());
			this->doBindClass(name, metaClass.get());
			break;
		}

		case GScriptValue::typeObject: {
			IMetaClass * metaClass;
			bool transferOwnership;
			void * instance = objectAddressFromVariant(value.toObject(&metaClass, &transferOwnership));
			GScopedInterface<IMetaClass> metaClassGuard(metaClass);
			this->doBindObject(name, instance, metaClass, transferOwnership);
			break;
		}

		case GScriptValue::typeMethod: {
			void * instance;
			GScopedInterface<IMetaMethod> method(value.toMethod(&instance));
			this->doBindMethod(name, instance, method.get());
			break;
		}

		case GScriptValue::typeOverloadedMethods: {
			GScopedInterface<IMetaList> methodList(value.toOverloadedMethods());
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

		// We can't set any script object back to script engine,
		// otherwise, cross module portability will be broken.
		//case GScriptValue::typeScriptObject:
		//case case GScriptValue::typeScriptMethod:
		default:
			raiseCoreException(Error_ScriptBinding_CantSetScriptValue);
			break;

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
	return this->getValue(methodName).toOverloadedMethods();
}

GScriptValue::Type GScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
{
	GScriptValue value(this->getValue(name));
	if(outMetaTypeItem != NULL) {
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

GScriptValue GScriptObject::createScriptObject(const char * name)
{
	GScriptObject * object = NULL;
	const int delimiter = '.';
	if(strchr(name, delimiter) == NULL) {
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
		object = scriptObject.take();
	}
	if(object == NULL) {
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


