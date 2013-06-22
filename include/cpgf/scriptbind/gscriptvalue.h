#ifndef __GSCRIPTVALUE_H
#define __GSCRIPTVALUE_H

#include "cpgf/gvariant.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gstdint.h"
#include "cpgf/gsharedinterface.h"

#include <string>


namespace cpgf {

struct IMetaItem;
struct IMetaClass;
struct IMetaMethod;
struct IMetaList;
struct IMetaEnum;
struct IMetaAccessible;
struct IMetaTypedItem;
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
		typeEmpty = 0,
		typeNull = 1,
		typeFundamental = 2, typeString = 3,
		typeMetaClass = 4, typeObject = 5,
		typeMethod = 6, typeOverridedMethods = 7,
		typeEnum = 8,
		typeRaw = 9,
		typeAccessible = 10,

		typeScriptObject = 11, typeScriptMethod = 12,
	};
	
private:
	GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem);
	GScriptValue(Type type, const GVariant & value);
	
public:
	GScriptValue();
	GScriptValue(const GScriptValue & other);
	GScriptValue & operator = (const GScriptValue & other);

	Type getType() const { return this->type; }
	const GVariant & getValue() const { return this->value; }
	GVariant & getValue() { return this->value; }

	static GScriptValue fromNull();
	static GScriptValue fromFundamental(const GVariant & fundamental);
	static GScriptValue fromString(const char * s);
	static GScriptValue fromMetaClass(IMetaClass * metaClass);
	static GScriptValue fromObject(const GVariant & instance, IMetaClass * metaClass); // instance can be a void * or a shadow object
	static GScriptValue fromMethod(void * instance, IMetaMethod * method);
	static GScriptValue fromOverridedMethods(IMetaList * methods);
	static GScriptValue fromEnum(IMetaEnum * metaEnum);
	static GScriptValue fromRaw(const GVariant & raw);
	static GScriptValue fromAccessible(void * instance, IMetaAccessible * accessible);
	static GScriptValue fromScriptObject(IScriptObject * scriptObject);
	static GScriptValue fromScriptMethod(IScriptFunction * scriptFunction);

	void * toNull() const;
	GVariant toFundamental() const;
	std::string toString() const;
	IMetaClass * toMetaClass() const;
	GVariant toObject(IMetaClass ** metaClass) const;
	void * toObjectAddress(IMetaClass ** metaClass) const;
	IMetaMethod * toMethod(void ** outInstance) const;
	IMetaList * toOverridedMethods() const;
	IMetaEnum * toEnum() const;
	GVariant toRaw() const;
	IMetaAccessible * toAccessible(void ** outInstance) const;
	IScriptObject * toScriptObject() const;
	IScriptFunction * toScriptMethod() const;
	
	bool isEmpty() const { return this->type == typeEmpty; }
	bool isNull() const { return this->type == typeNull; }
	bool isFundamental() const { return this->type == typeFundamental; }
	bool isString() const { return this->type == typeString; }
	bool isMetaClass() const { return this->type == typeMetaClass; }
	bool isObject() const { return this->type == typeObject; }
	bool isMethod() const { return this->type == typeMethod; }
	bool isOverridedMethods() const { return this->type == typeOverridedMethods; }
	bool isEnum() const { return this->type == typeEnum; }
	bool isRaw() const { return this->type == typeRaw; }
	bool isAccessible() const { return this->type == typeAccessible; }
	bool isScriptObject() const { return this->type == typeScriptObject; }
	bool isScriptMethod() const { return this->type == typeScriptMethod; }
	
private:
	Type type;
	GVariant value;
	GSharedInterface<IMetaItem> metaItem;
};

IMetaTypedItem * getTypedItemFromScriptValue(const GScriptValue & value);

} // namespace cpgf


#endif
