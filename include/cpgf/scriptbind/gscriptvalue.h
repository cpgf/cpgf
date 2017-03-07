#ifndef CPGF_GSCRIPTVALUE_H
#define CPGF_GSCRIPTVALUE_H

#include "cpgf/gvariant.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gflags.h"
#include "cpgf/gcallback.h"

#include <string>
#include <cstdint>

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
struct IScriptArray;

struct IScriptValueBindApi : public IObject
{
	virtual void G_API_CC discardOwnership() = 0;
	virtual bool G_API_CC isOwnershipTransferred() = 0;
};

#pragma pack(push, 1)
#pragma pack(1)
struct GScriptValueData
{
	uint32_t type;
	GVariantData value;
	IMetaItem * metaItem;
	IScriptValueBindApi * bindApi;
};
#pragma pack(pop)

GMAKE_FINAL(GScriptValue)

class GScriptValue : GFINAL_BASE(GScriptValue)
{
public:
	enum Type {
		typeNull = 0,
		typeFundamental = 1, typeString = 2,
		typeClass = 3, typeObject = 4,
		typeMethod = 5, typeOverloadedMethods = 6,
		typeEnum = 7,
		typeRaw = 8,
		typeAccessible = 9,

		typeScriptObject = 10,
		typeScriptFunction = 11,
		typeScriptArray = 12
	};

private:
	GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem, IScriptValueBindApi * bindApi);
	GScriptValue(Type type, const GVariant & value, IMetaItem * metaItem);
	GScriptValue(Type type, const GVariant & value);
	explicit GScriptValue(const GScriptValueData & data);

public:
	GScriptValue();
	GScriptValue(const GScriptValue & other);
	GScriptValue & operator = (const GScriptValue & other);

	Type getType() const { return this->type; }

	static GScriptValue fromNull();
	static GScriptValue fromFundamental(const GVariant & fundamental);
	static GScriptValue fromString(const char * s); // reference to s and s should not be freed
	static GScriptValue fromAndCopyString(const char * s); // duplicate s and s can be freed
	static GScriptValue fromClass(IMetaClass * metaClass);
	static GScriptValue fromObject(const GVariant & instance, IMetaClass * metaClass, bool transferOwnership); // instance can be a void * or a shadow object
	static GScriptValue fromObject(const GVariant & instance, IMetaClass * metaClass, IScriptValueBindApi * bindApi); // instance can be a void * or a shadow object
	static GScriptValue fromMethod(void * instance, IMetaMethod * method);
	static GScriptValue fromOverloadedMethods(IMetaList * methods);
	static GScriptValue fromEnum(IMetaEnum * metaEnum);
	static GScriptValue fromRaw(const GVariant & raw);
	static GScriptValue fromAccessible(void * instance, IMetaAccessible * accessible);
	static GScriptValue fromScriptObject(IScriptObject * scriptObject);
	static GScriptValue fromScriptFunction(IScriptFunction * scriptFunction);
	static GScriptValue fromScriptArray(IScriptArray * scriptArray);

	void * toNull() const;
	GVariant toFundamental() const;
	std::string toString() const;
	IMetaClass * toClass() const;
	GVariant toObject(IMetaClass ** outMetaClass, bool * outTransferOwnership) const;
	void * toObjectAddress(IMetaClass ** outMetaClass, bool * outTransferOwnership) const;
	IMetaMethod * toMethod(void ** outInstance) const;
	IMetaList * toOverloadedMethods() const;
	IMetaEnum * toEnum() const;
	GVariant toRaw() const;
	IMetaAccessible * toAccessible(void ** outInstance) const;
	IScriptObject * toScriptObject() const;
	IScriptFunction * toScriptFunction() const;
	IScriptArray * toScriptArray() const;

	bool isNull() const { return this->type == typeNull; }
	bool isFundamental() const { return this->type == typeFundamental; }
	bool isString() const { return this->type == typeString; }
	bool isClass() const { return this->type == typeClass; }
	bool isObject() const { return this->type == typeObject; }
	bool isMethod() const { return this->type == typeMethod; }
	bool isOverloadedMethods() const { return this->type == typeOverloadedMethods; }
	bool isEnum() const { return this->type == typeEnum; }
	bool isRaw() const { return this->type == typeRaw; }
	bool isAccessible() const { return this->type == typeAccessible; }
	bool isScriptObject() const { return this->type == typeScriptObject; }
	bool isScriptFunction() const { return this->type == typeScriptFunction; }
	bool isScriptArray() const { return this->type == typeScriptArray; }

	// for internal use
	const GVariant & getValue() const { return this->value; }
	GVariant & getValue() { return this->value; }
	GScriptValueData takeData();
	// The result must be passed to another GScriptValue or GScriptValueDataScopedGuard to avoid memory leak
	GScriptValueData getData() const;

	void discardOwnership();
private:
	Type type;
	GVariant value;
	GSharedInterface<IMetaItem> metaItem;
	GSharedInterface<IScriptValueBindApi> bindApi;

private:
	friend GScriptValue createScriptValueFromData(const GScriptValueData & data);
	friend class GScriptValueDataScopedGuard;
};

class GScriptValueDataScopedGuard : public GNoncopyable
{
public:
	explicit GScriptValueDataScopedGuard(const GScriptValueData & data);
	~GScriptValueDataScopedGuard();

private:
	GScriptValueData data;
};

IMetaTypedItem * getTypedItemFromScriptValue(const GScriptValue & value);

} // namespace cpgf


#endif
