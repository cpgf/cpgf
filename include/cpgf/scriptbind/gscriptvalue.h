#ifndef __GSCRIPTVALUE_H
#define __GSCRIPTVALUE_H

#include "cpgf/gvariant.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gstdint.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gflags.h"

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
	IMetaItem * metaItem;
	uint32_t flags;
};
#pragma pack(pop)


GMAKE_FINAL(GScriptValue)

class GScriptValue : GFINAL_BASE(GScriptValue)
{
private:
	enum ValueFlags {
		vfTransferOwnership
	};

public:
	enum Type {
		typeNull = 0,
		typeFundamental = 1, typeString = 2,
		typeClass = 3, typeObject = 4,
		typeMethod = 5, typeOverridedMethods = 6,
		typeEnum = 7,
		typeRaw = 8,
		typeAccessible = 9,

		typeScriptObject = 10, typeScriptFunction = 11,
	};
	
private:
	GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem, bool transferOwnership);
	GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem);
	GScriptValue(Type type, const GVariant & value);
	
public:
	GScriptValue();
	explicit GScriptValue(const GScriptValueData & data);
	GScriptValue(const GScriptValue & other);
	GScriptValue & operator = (const GScriptValue & other);

	GScriptValueData getData() const;
	GScriptValueData takeData();

	Type getType() const { return this->type; }
	const GVariant & getValue() const { return this->value; }
	GVariant & getValue() { return this->value; }

	static GScriptValue fromNull();
	static GScriptValue fromFundamental(const GVariant & fundamental);
	static GScriptValue fromString(const char * s);
	static GScriptValue fromClass(IMetaClass * metaClass);
	static GScriptValue fromObject(const GVariant & instance, IMetaClass * metaClass, bool transferOwnership); // instance can be a void * or a shadow object
	static GScriptValue fromMethod(void * instance, IMetaMethod * method);
	static GScriptValue fromOverridedMethods(IMetaList * methods);
	static GScriptValue fromEnum(IMetaEnum * metaEnum);
	static GScriptValue fromRaw(const GVariant & raw);
	static GScriptValue fromAccessible(void * instance, IMetaAccessible * accessible);
	static GScriptValue fromScriptObject(IScriptObject * scriptObject);
	static GScriptValue fromScriptFunction(IScriptFunction * scriptFunction);

	void * toNull() const;
	GVariant toFundamental() const;
	std::string toString() const;
	IMetaClass * toClass() const;
	GVariant toObject(IMetaClass ** outMetaClass, bool * outTransferOwnership) const;
	void * toObjectAddress(IMetaClass ** outMetaClass, bool * outTransferOwnership) const;
	IMetaMethod * toMethod(void ** outInstance) const;
	IMetaList * toOverridedMethods() const;
	IMetaEnum * toEnum() const;
	GVariant toRaw() const;
	IMetaAccessible * toAccessible(void ** outInstance) const;
	IScriptObject * toScriptObject() const;
	IScriptFunction * toScriptFunction() const;
	
	bool isNull() const { return this->type == typeNull; }
	bool isFundamental() const { return this->type == typeFundamental; }
	bool isString() const { return this->type == typeString; }
	bool isClass() const { return this->type == typeClass; }
	bool isObject() const { return this->type == typeObject; }
	bool isMethod() const { return this->type == typeMethod; }
	bool isOverridedMethods() const { return this->type == typeOverridedMethods; }
	bool isEnum() const { return this->type == typeEnum; }
	bool isRaw() const { return this->type == typeRaw; }
	bool isAccessible() const { return this->type == typeAccessible; }
	bool isScriptObject() const { return this->type == typeScriptObject; }
	bool isScriptFunction() const { return this->type == typeScriptFunction; }
	
private:
	Type type;
	GVariant value;
	GSharedInterface<IMetaItem> metaItem;
	GFlags<ValueFlags> flags;
};

IMetaTypedItem * getTypedItemFromScriptValue(const GScriptValue & value);

} // namespace cpgf


#endif
