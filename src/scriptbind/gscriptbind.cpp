#include "cpgf/scriptbind/gscriptbind.h"

namespace cpgf {


GScriptNameData::GScriptNameData()
	: referenceCount(1)
{
}

GScriptNameData::~GScriptNameData()
{
}

void GScriptNameData::retain()
{
	++this->referenceCount;
}

void GScriptNameData::release()
{
	--this->referenceCount;
	if(this->referenceCount <= 0) {
		delete this;
	}
}

const char * GScriptNameData::getName(const char * candidate) const
{
	return candidate;
}

bool GScriptNameData::isAvailable() const
{
	return false;
}



GScriptName::GScriptName(const char * name)
	: name(name), data(NULL)
{
}

GScriptName::GScriptName(const char * name, GScriptNameData * data)
	: name(name), data(data)
{
	if(this->data != NULL) {
		this->data->retain();
	}
}

GScriptName::GScriptName(const GScriptName & other)
	: name(other.name), data(other.data)
{
	if(this->data != NULL) {
		this->data->retain();
	}
}

GScriptName::~GScriptName()
{
	if(this->data != NULL) {
		this->data->release();
	}
}

GScriptName & GScriptName::operator = (GScriptName other)
{
	this->swap(other);

	return *this;
}

void GScriptName::uncache()
{
	if(this->data != NULL) {
		this->data->release();
		this->data = NULL;
	}
}

void GScriptName::swap(GScriptName & other)
{
	using std::swap;

	swap(this->name, other.name);
	swap(this->data, other.data);
}

const char * GScriptName::getName() const
{
	if(this->data != NULL) {
		return this->data->getName(this->name);
	}
	else {
		return this->name;
	}
}

GScriptNameData * GScriptName::getData() const
{
	return this->data;
}

bool GScriptName::hasData() const
{
	return this->data != NULL && this->data->isAvailable();
}



GScriptObject::GScriptObject(const GScriptConfig & config)
	: config(config), owner(NULL)
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

bool GScriptObject::isGlobal() const
{
	return this->owner == NULL;
}

const char * GScriptObject::getName() const
{
	return this->name.c_str();
}


} // namespace cpgf


