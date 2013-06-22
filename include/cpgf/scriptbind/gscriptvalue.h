#ifndef __GSCRIPTVALUE_H
#define __GSCRIPTVALUE_H

#include "cpgf/gvariant.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gstdint.h"
#include "cpgf/gsharedinterface.h"

#include <string>


namespace cpgf {

struct IMetaClass;
struct IMetaMethod;
struct IMetaList;
struct IMetaEnum;
struct IScriptObject;
struct IScriptFunction;

#pragma pack(push, 1)
#pragma pack(1)
struct GScriptValueData
{
	uint32_t type;
	GVariantData value;
	IMetaClass * metaClass;
};
#pragma pack(pop)


GMAKE_FINAL(GScriptValue)

class GScriptValue : GFINAL_BASE(GScriptValue)
{
public:
	enum Type {
		typeUnknown = 0,
		typeNull = 1,
		typeFundamental = 2, typeString = 3,
		typeMetaClass = 4, typeObject = 5,
		typeMethod = 6, typeOverridedMethods = 7,
		typeEnum = 8,
		typeRaw = 9,

		typeScriptObject = 10, typeScriptMethod = 11,
	};
	
private:
	GScriptValue(Type type, const GVariant & value, IMetaClass * metaClass);
	GScriptValue(Type type, const GVariant & value);
	
public:
	GScriptValue();

	Type getType() const { return this->type; }

	static GScriptValue fromNull();
	static GScriptValue fromFundamental(const GVariant & fundamental);
	static GScriptValue fromString(const char * s);
	static GScriptValue fromMetaClass(IMetaClass * metaClass);
	static GScriptValue fromObject(void * instance, IMetaClass * metaClass);
	static GScriptValue fromMethod(IMetaMethod * method);
	static GScriptValue fromOverridedMethods(IMetaList * methods);
	static GScriptValue fromEnum(IMetaEnum * metaEnum);
	static GScriptValue fromRaw(const GVariant & raw);
	static GScriptValue fromScriptObject(IScriptObject * scriptObject);
	static GScriptValue fromScriptMethod(IScriptFunction * scriptFunction);

	void * toNull() const;
	GVariant toFundamental() const;
	std::string toString() const;
	IMetaClass * toMetaClass() const;
	void * toObject(IMetaClass ** metaClass) const;
	IMetaMethod * toMethod() const;
	IMetaList * toOverridedMethods() const;
	IMetaEnum * toEnum() const;
	GVariant toRaw() const;
	IScriptObject * toScriptObject() const;
	IScriptFunction * toScriptMethod() const;
	
	bool isUnknown() const { return this->type == typeUnknown; }
	bool isNull() const { return this->type == typeNull; }
	bool isFundamental() const { return this->type == typeFundamental; }
	bool isString() const { return this->type == typeString; }
	bool isMetaClass() const { return this->type == typeMetaClass; }
	bool isObject() const { return this->type == typeObject; }
	bool isMethod() const { return this->type == typeMethod; }
	bool isOverridedMethods() const { return this->type == typeOverridedMethods; }
	bool isEnum() const { return this->type == typeEnum; }
	bool isRaw() const { return this->type == typeRaw; }
	bool isScriptObject() const { return this->type == typeScriptObject; }
	bool isScriptMethod() const { return this->type == typeScriptMethod; }
	
private:
	Type type;
	GVariant value;
	GSharedInterface<IMetaClass> objectMetaClass;
};

} // namespace cpgf


#endif
