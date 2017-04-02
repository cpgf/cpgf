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

class GScriptValue final
{
public:
	enum Type {
		typeNull = 0,
		typePrimary = 1,
		typeClass = 2, typeObject = 3,
		typeMethod = 4, typeOverloadedMethods = 5,
		typeEnum = 6,
		typeRaw = 7,
		typeAccessible = 8,

		typeScriptObject = 9,
		typeScriptFunction = 10,
		typeScriptArray = 11
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
	// primary is boolean, integer, float point, string, wide string
	static GScriptValue fromPrimary(const GVariant & primary);
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
	GVariant toPrimary() const;
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
	bool isPrimary() const { return this->type == typePrimary; }
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
