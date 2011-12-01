#ifndef __GSCRIPTBIND_H
#define __GSCRIPTBIND_H


#include "cpgf/scriptbind/gscriptbindapi.h"

#include "cpgf/gmetaapiutil.h"
#include "cpgf/gmetavariant.h"
#include "cpgf/gclassutil.h"

#include <string>
#include <algorithm>


namespace cpgf {


enum GScriptDataType {
	sdtUnknown = 0,
	sdtNull = 1,
	sdtFundamental = 2, sdtString = 3,
	sdtClass = 4, sdtObject = 5,
	sdtMethod = 6, sdtMethodList = 7,
	sdtEnum = 8,

	sdtScriptObject = 10, sdtScriptMethod = 11,
};


GMAKE_FINAL(GScriptConfig)

class GScriptConfig : GFINAL_BASE(GScriptConfig)
{
private:
	enum {
		scAccessStaticDataViaInstance = 1 << 0,
		scAccessStaticMethodViaInstance = 1 << 1,
		scAccessEnumTypeViaInstance = 1 << 2,
		scAccessEnumValueViaInstance = 1 << 3,
		scAccessClassViaInstance = 1 << 4
	};

	enum {
		DefaultFlags =
				scAccessStaticDataViaInstance
				| scAccessStaticMethodViaInstance
				| scAccessEnumTypeViaInstance
				| scAccessEnumValueViaInstance
				| scAccessClassViaInstance
	};

public:
	GScriptConfig() : flags(DefaultFlags) {
	}

	GScriptConfig(const GScriptConfig & other) : flags(other.flags) {
	}

	GScriptConfig & operator = (const GScriptConfig & other) {
		this->flags = other.flags;

		return *this;
	}

	void setAccessStaticDataViaInstance(bool set) {
		this->setFlag(scAccessStaticDataViaInstance, set);
	}

	bool allowAccessStaticDataViaInstance() const {
		return this->hasFlag(scAccessStaticDataViaInstance);
	}

	void setAccessStaticMethodViaInstance(bool set) {
		this->setFlag(scAccessStaticMethodViaInstance, set);
	}

	bool allowAccessStaticMethodViaInstance() const {
		return this->hasFlag(scAccessStaticMethodViaInstance);
	}

	void setAccessEnumTypeViaInstance(bool set) {
		this->setFlag(scAccessEnumTypeViaInstance, set);
	}

	bool allowAccessEnumTypeViaInstance() const {
		return this->hasFlag(scAccessEnumTypeViaInstance);
	}

	void setAccessEnumValueViaInstance(bool set) {
		this->setFlag(scAccessEnumValueViaInstance, set);
	}

	bool allowAccessEnumValueViaInstance() const {
		return this->hasFlag(scAccessEnumValueViaInstance);
	}

	void setAccessClassViaInstance(bool set) {
		this->setFlag(scAccessClassViaInstance, set);
	}

	bool allowAccessClassViaInstance() const {
		return this->hasFlag(scAccessClassViaInstance);
	}

private:
	bool hasFlag(unsigned int flag) const {
		return (this->flags & flag) != 0;
	}

	void setFlag(unsigned int flag, bool set) {
		if(set) {
			this->setFlag(flag);
		}
		else {
			this->clearFlag(flag);
		}
	}

	void setFlag(unsigned int flag) {
		this->flags |= flag;
	}

	void clearFlag(unsigned int flag) {
		this->flags &= ~flag;
	}

private:
	unsigned int flags;
};


class GScriptObject
{
public:
	GScriptObject(const GScriptConfig & config);
	virtual ~GScriptObject();

	const GScriptConfig & getConfig() const;

	GScriptObject * getOwner() const;

	bool isGlobal() const;

public:	
	virtual void bindClass(const char * name, IMetaClass * metaClass) = 0;
	virtual void bindEnum(const char * name, IMetaEnum * metaEnum) = 0;

	virtual void bindFundamental(const char * name, const GVariant & value) = 0;
	virtual void bindString(const char * stringName, const char * s) = 0;
	virtual void bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership) = 0;
	virtual void bindMethod(const char * name, void * instance, IMetaMethod * method) = 0;
	virtual void bindMethodList(const char * name, IMetaList * methodList) = 0;

	virtual IMetaClass * getClass(const char * className) = 0;
	virtual IMetaEnum * getEnum(const char * enumName) = 0;
	
	virtual GVariant getFundamental(const char * name) = 0;
	virtual std::string getString(const char * stringName) = 0;
	virtual void * getObject(const char * objectName) = 0;
	virtual IMetaMethod * getMethod(const char * methodName, void ** outInstance) = 0;
	virtual IMetaList * getMethodList(const char * methodName) = 0;

	virtual GScriptDataType getType(const char * name, IMetaTypedItem ** outMetaTypeItem) = 0;

	virtual GScriptObject * createScriptObject(const char * name) = 0;
	virtual GScriptObject * getScriptObject(const char * name) = 0;
	
	virtual GMetaVariant invoke(const char * name, const GMetaVariant * params, size_t paramCount) = 0;
	virtual GMetaVariant invokeIndirectly(const char * name, GMetaVariant const * const * params, size_t paramCount) = 0;

	virtual void assignValue(const char * fromName, const char * toName) = 0;
	virtual bool valueIsNull(const char * name) = 0;
	virtual void nullifyValue(const char * name) = 0;

protected:
	const char * getName() const;

private:
	GScriptObject(const GScriptObject &);
	GScriptObject & operator = (const GScriptObject &);

protected:
	GScriptConfig config;
	GScriptObject * owner;
	std::string name;
};



} // namespace cpgf





#endif
