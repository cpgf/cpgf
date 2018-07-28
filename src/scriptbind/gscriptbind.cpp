#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gglobal.h"

#include "gbindcommon.h"
#include "gbindapiimpl.h"


namespace cpgf {


GScriptObject::GScriptObject()
	:
		owner(nullptr),
		name(),
		objectHolder()
{
}

GScriptObject::GScriptObject(const GScriptObject & /*other*/)
	:
		owner(nullptr),
		name(),
		objectHolder()
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
	this->doSetValue(name, value);
}

void GScriptObject::bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader)
{
	this->doBindCoreService(name, libraryLoader);
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
		std::vector<char> tempName(len + 1);
		memmove(tempName.data(), name, len + 1);
		char * next;
		char * head = tempName.data();
		std::unique_ptr<GScriptObject> scriptObject;
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
		object = scriptObject.release();
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


