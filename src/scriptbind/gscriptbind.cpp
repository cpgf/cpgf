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
		while(true) {
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


