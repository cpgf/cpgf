#ifndef GBINDGLUEDATA_H
#define GBINDGLUEDATA_H

#include "cpgf/gscopedinterface.h"
#include "cpgf/gvariant.h"
#include "cpgf/gflags.h"
#include "cpgf/gmetaoperatorop.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/scriptbind/gscriptvalue.h"

#include "gbindobject.h"
#include "gbindmetamap.h"

#include <set>
#include <map>
#include <memory>

namespace cpgf {

namespace bind_internal {

/*
A glue data is used as the "user data" in the underlying script engine.
When a value is passed from script to C++, cpgf uses the glue data internally.
*/

class GObjectInstance;

typedef std::shared_ptr<GObjectInstance> GObjectInstancePointer;
typedef std::weak_ptr<GObjectInstance> GWeakObjectInstancePointer;

class GObjectGlueData;

typedef std::shared_ptr<GObjectGlueData> GObjectGlueDataPointer;
typedef std::weak_ptr<GObjectGlueData> GWeakObjectGlueDataPointer;


class GBindingContext;

typedef std::shared_ptr<GBindingContext> GContextPointer;
typedef std::weak_ptr<GBindingContext> GWeakContextPointer;

enum GGlueDataType {
	gdtObject,
	gdtClass,
	gdtEnum,
	gdtAccessible,
	gdtRaw,
	gdtMethod,
	gdtObjectAndMethod, // combination of object and method, only used in Lua binding
	gdtOperator // only used in Lua binding
};

class GScriptDataHolder;
typedef std::shared_ptr<GScriptDataHolder> GScriptDataHolderPointer;

class GScriptDataStorage : public IScriptDataStorage
{
	G_INTERFACE_IMPL_OBJECT

private:
	GScriptDataStorage(const GScriptDataHolderPointer & instanceDataHolder, const GScriptDataHolderPointer & classDataHolder);

public:
	virtual ~GScriptDataStorage();

protected:
	virtual IScriptFunction * G_API_CC getScriptFunction(const char * name);

private:
	GScriptDataHolderPointer instanceDataHolder;
	GScriptDataHolderPointer classDataHolder;

private:
	friend class GBindingContext;
	friend class GObjectInstance;
};


class GScriptDataHolder
{
private:
	struct Item {
		GScriptValue scriptValue;
		/*
			Need to hold the object instance pointer, otherwise, the object
			may be GCed while GScriptDataHolder still hold a pointer to it.
		*/
		GObjectInstancePointer objectInstance;
	};

	typedef std::map<std::string, Item> MapType;

public:
	void setScriptValue(const GContextPointer & context, const char * name, const GScriptValue & value);
	const GScriptValue * findValue(const char * name) const;

private:
	void requireDataMap();

private:
	std::unique_ptr<MapType> dataMap;
};

class GGlueUserData
{
public:
	GGlueUserData() {}
	virtual ~GGlueUserData() {}
};

class GGlueData
{
public:
	void * operator new (const std::size_t size);
	void * operator new (const std::size_t size, void * ptr);
	void operator delete(void * p, size_t size);

	GGlueData(GGlueDataType type, const GContextPointer & context);
	virtual ~GGlueData();

	GGlueDataType getType() const;
	GContextPointer getBindingContext() const;
	bool isValid() const;

	void setUserData(GGlueUserData * newUserData) {
		this->userData = newUserData;
		this->userDataHolder.reset(newUserData);
	}

	void setUserData(GGlueUserData * newUserData, const bool freeIt) {
		this->userData = newUserData;
		this->userDataHolder.reset(freeIt ? newUserData : nullptr);
	}

	GGlueUserData * getUserData() const {
		return this->userData;
	}

	template <typename T>
	T * getUserDataAs() const {
		return static_cast<T *>(this->userData);
	}

	GGlueData(const GGlueData &) = delete;
	GGlueData & operator = (const GGlueData &) = delete;

private:
	GGlueDataType type;
	GWeakContextPointer context;
	std::unique_ptr<GGlueUserData> userDataHolder;
	GGlueUserData * userData;
};

typedef std::shared_ptr<GGlueData> GGlueDataPointer;
typedef std::weak_ptr<GGlueData> GWeakGlueDataPointer;


class GClassGlueData : public GGlueData, public std::enable_shared_from_this<GClassGlueData>
{
private:
	typedef GGlueData super;

protected:
	GClassGlueData(const GContextPointer & context, IMetaClass * metaClass, GMetaMapClass * mapClass);

public:
	~GClassGlueData();

public:
	IMetaClass * getMetaClass() const {
		return this->metaClass.get();
	}

	GMetaMapClass * getClassMap() const {
		return this->mapClass;
	}

	const GScriptDataHolderPointer & getDataHolder() const;

private:
	GSharedInterface<IMetaClass> metaClass;
	GMetaMapClass * mapClass;
	mutable GScriptDataHolderPointer dataHolder;

private:
	friend class GBindingContext;
	friend class GBindingPool;
};

typedef std::shared_ptr<GClassGlueData> GClassGlueDataPointer;
typedef std::weak_ptr<GClassGlueData> GWeakClassGlueDataPointer;


/*
Difference between GObjectInstance and GObjectGlueData

GObjectInstance represents the object entity that is at certain memory address.
The field "classData" is the meta class that created the GObjectInstance originally,
so GObjectInstance will use "classData" to free the underlying C++ object.
"classData" is only used to free the object, it's not the type of GObjectInstance.
Indeed, GObjectInstance is typeless. A GObjectInstance can represent different classes,
such as a base and a derived class, and a GObjectInstance can also represent complete
different types, which happens with pointer aliasing.
Example,
struct D {};
class A
{
	D d;
};
A GObjectInstance of object A may be object A, or object A::d.

GObjectGlueData represents the "logical" object. It contains a meta class as the type,
it also contains the constness.
Assume we have,
A obj;
A * a = &obj;
const A * c = &obj;
Both a and c will have the same GObjectInstance because they point to the same address,
but they have different GObjectGlueData.
*/
class GObjectInstance
{
public:
	static GObjectInstancePointer create(
		const GContextPointer & context,
		const GVariant & instance,
		const GClassGlueDataPointer & classData,
		const bool allowGC
	);

public:
	GObjectInstance(
		const GContextPointer & context,
		const GVariant & instance,
		const GClassGlueDataPointer & classData,
		const bool allowGC
	);
	~GObjectInstance();

	void * getInstanceAddress() const;

	bool isAllowGC() const {
		return this->allowGC;
	}

	void setAllowGC(bool allow) {
		this->allowGC = allow;
	}

	const GVariant & getInstance() const {
		return this->instance;
	}

	GContextPointer getBindingContext() const;

	const GScriptDataHolderPointer & getDataHolder() const;

private:
	GWeakContextPointer context;
	GVariant instance;
	GClassGlueDataPointer classData;
	GSharedInterface<IMetaObjectLifeManager> objectLifeManager;
	bool allowGC;
	bool isSharedPointer;
	GScopedInterface<IScriptDataStorage> dataStorage;
	mutable GScriptDataHolderPointer dataHolder;

private:
	friend class GObjectGlueData;
};


class GObjectGlueData : public GGlueData, public std::enable_shared_from_this<GObjectGlueData>
{
private:
	typedef GGlueData super;

private:
	GObjectGlueData(
		const GContextPointer & context,
		const GClassGlueDataPointer & classGlueData,
		const GObjectInstancePointer & objectInstance,
		const GScriptInstanceCv cv
	);

public:
	~GObjectGlueData();

	GClassGlueDataPointer getClassData() const {
		return this->classGlueData;
	}

	const GVariant & getInstance() const {
		return this->objectInstance->instance;
	}

	void * getInstanceAddress() const {
		if(this->sharedPointerTraits) {
			return this->sharedPointerTraits->getPointer(objectAddressFromVariant(this->getInstance()));
		}
		else {
			return objectAddressFromVariant(this->getInstance());
		}
	}

	bool isAllowGC() const {
		return this->objectInstance->isAllowGC();
	}

	void setAllowGC(bool allow) {
		this->objectInstance->setAllowGC(allow);
	}

	GScriptInstanceCv getCV() const {
		return this->cv;
	}

	void setSharedPointerTraits(IMetaSharedPointerTraits * sharedPointerTraits) {
		this->sharedPointerTraits.reset(sharedPointerTraits);
		this->objectInstance->isSharedPointer = true;
	}

	IMetaSharedPointerTraits * getSharedPointerTraits() const {
		return this->sharedPointerTraits.get();
	}

	const GScriptDataHolderPointer & getDataHolder() const {
		return this->objectInstance->getDataHolder();
	}

	GObjectInstancePointer getObjectInstance() const {
		return this->objectInstance;
	}

private:
	GClassGlueDataPointer classGlueData;
	GScriptInstanceCv cv;
	GObjectInstancePointer objectInstance;
	GSharedInterface<IMetaSharedPointerTraits> sharedPointerTraits;

private:
	friend class GBindingContext;
	friend class GBindingPool;
};


class GMethodGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GMethodGlueData(const GContextPointer & context, const GScriptValue & scriptValue)
		: super(gdtMethod, context), scriptValue(scriptValue) {
	}

public:
	const GScriptValue & getScriptValue() const {
		return this->scriptValue;
	}

private:
	GScriptValue scriptValue;

private:
	friend class GBindingContext;
	friend class GBindingPool;
};

typedef std::shared_ptr<GMethodGlueData> GMethodGlueDataPointer;
typedef std::weak_ptr<GMethodGlueData> GWeakMethodGlueDataPointer;


class GEnumGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GEnumGlueData(const GContextPointer & context, IMetaEnum * metaEnum)
		: super(gdtEnum, context), metaEnum(metaEnum) {
	}

public:
	IMetaEnum * getMetaEnum() const {
		return this->metaEnum.get();
	}

private:
	GSharedInterface<IMetaEnum> metaEnum;

private:
	friend class GBindingContext;
	friend class GBindingPool;
};

typedef std::shared_ptr<GEnumGlueData> GEnumGlueDataPointer;
typedef std::weak_ptr<GEnumGlueData> GWeakEnumGlueDataPointer;


class GAccessibleGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GAccessibleGlueData(const GContextPointer & context, const GVariant & instance, IMetaAccessible * accessible)
		: super(gdtAccessible, context), instance(instance), accessible(accessible) {
	}

public:
	const GVariant & getInstance() const {
		return this->instance;
	}

	void * getInstanceAddress() const {
		return objectAddressFromVariant(this->instance);
	}

	IMetaAccessible * getAccessible() const {
		return this->accessible.get();
	}

private:
	GVariant instance;
	GSharedInterface<IMetaAccessible> accessible;

private:
	friend class GBindingContext;
	friend class GBindingPool;
};

typedef std::shared_ptr<GAccessibleGlueData> GAccessibleGlueDataPointer;
typedef std::weak_ptr<GAccessibleGlueData> GWeakAccessibleGlueDataPointer;


class GRawGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GRawGlueData(const GContextPointer & context, const GVariant & data)
		: super(gdtRaw, context), data(data)
	{
	}

public:
	const GVariant & getData() const {
		return this->data;
	}

	void setSharedPointerTraits(IMetaSharedPointerTraits * sharedPointerTraits) {
		this->sharedPointerTraits.reset(sharedPointerTraits);
	}

	IMetaSharedPointerTraits * getSharedPointerTraits() const {
		return this->sharedPointerTraits.get();
	}

private:
	GVariant data;
	GSharedInterface<IMetaSharedPointerTraits> sharedPointerTraits;

private:
	friend class GBindingContext;
	friend class GBindingPool;
};

typedef std::shared_ptr<GRawGlueData> GRawGlueDataPointer;
typedef std::weak_ptr<GRawGlueData> GWeakRawGlueDataPointer;


// Used by Lua and Python binding
class GObjectAndMethodGlueData : public GGlueData
{
private:
	typedef GGlueData super;

private:
	GObjectAndMethodGlueData(
			const GContextPointer & context,
			const GObjectGlueDataPointer & objectData,
			const GMethodGlueDataPointer & methodData
		)
		:
			super(gdtObjectAndMethod, context),
			objectData(objectData),
			methodData(methodData)
	{
	}

public:
	GObjectGlueDataPointer getObjectData() const {
		return this->objectData;
	}

	GMethodGlueDataPointer getMethodData() const {
		return this->methodData;
	}

private:
	GObjectGlueDataPointer objectData;
	GMethodGlueDataPointer methodData;

private:
	friend class GBindingContext;
	friend class GBindingPool;
};

typedef std::shared_ptr<GObjectAndMethodGlueData> GObjectAndMethodGlueDataPointer;
typedef std::weak_ptr<GObjectAndMethodGlueData> GWeakObjectAndMethodGlueDataPointer;


// Used by Lua and Python binding
class GOperatorGlueData : public GGlueData
{
private:
	typedef GGlueData super;

public:
	GOperatorGlueData(
		const GContextPointer & context,
		const GObjectGlueDataPointer & objectData,
		IMetaClass * metaClass,
		GMetaOpType op
	)
		: super(gdtOperator, context), objectData(objectData), metaClass(metaClass), op(op) {
	}

	const GObjectGlueDataPointer & getObjectData() const {
		return this->objectData;
	}

	void * getInstanceAddress() const {
		return this->objectData->getInstanceAddress();
	}

	IMetaClass * getMetaClass() const {
		return this->metaClass.get();
	}

	GMetaOpType getOp() const {
		return this->op;
	}

private:
	GObjectGlueDataPointer objectData;
	GSharedInterface<IMetaClass> metaClass;
	GMetaOpType op;
};

typedef std::shared_ptr<GOperatorGlueData> GOperatorGlueDataPointer;
typedef std::weak_ptr<GOperatorGlueData> GWeakOperatorGlueDataPointer;


class GGlueDataWrapper
{
public:
	virtual ~GGlueDataWrapper() {
	}

	virtual GGlueDataPointer getData() const = 0;

	template <typename T>
	std::shared_ptr<T> getAs() const {
		return std::static_pointer_cast<T>(this->getData());
	}
};

template <typename T>
class GGlueDataWrapperImplement : public GGlueDataWrapper
{
public:
	explicit GGlueDataWrapperImplement(const T & p)
		: dataPointer(p)
	{
		GASSERT((bool)p);

		p->getBindingContext()->getBindingPool()->glueDataAdded(p);
	}

	virtual ~GGlueDataWrapperImplement()
	{
		if(this->dataPointer->isValid()) {
			this->dataPointer->getBindingContext()->getBindingPool()->glueDataRemoved(this->dataPointer);
		}
	}

	virtual GGlueDataPointer getData() const {
		return std::static_pointer_cast<GGlueData>(this->dataPointer);
	}

private:
	T dataPointer;
};


/*
This class is used to hold all data wrappers which are embeded in script engine,
to avoid memory leak by the underlying script engine.
*/
class GGlueDataWrapperPool
{
private:
	typedef std::set<GGlueDataWrapper *> SetType;

public:
	GGlueDataWrapperPool();
	~GGlueDataWrapperPool();

	void dataWrapperCreated(GGlueDataWrapper * dataWrapper);
	void dataWrapperDestroyed(GGlueDataWrapper * dataWrapper);

	void clear();

private:
	bool active;
	SetType wrapperSet;
};

template <typename T>
GGlueDataWrapper * newGlueDataWrapper(const T & p)
{
	return new GGlueDataWrapperImplement<T>(p);
}

template <typename T>
GGlueDataWrapper * newGlueDataWrapper(const T & p, GGlueDataWrapperPool * pool)
{
	GGlueDataWrapper * wrapper = newGlueDataWrapper(p);
	if(pool != nullptr) {
		pool->dataWrapperCreated(wrapper);
	}
	return wrapper;
}

template <typename T>
GGlueDataWrapper * newGlueDataWrapper(void * address, const T & p)
{
	return new (address) GGlueDataWrapperImplement<T>(p);
}

template <typename T>
size_t getGlueDataWrapperSize()
{
	return sizeof(GGlueDataWrapperImplement<std::shared_ptr<T> >);
}

inline void destroyGlueDataWrapper(GGlueDataWrapper * p)
{
	p->~GGlueDataWrapper();
}

inline void freeGlueDataWrapper(GGlueDataWrapper * p)
{
	delete p;
}

inline void freeGlueDataWrapper(GGlueDataWrapper * p, GGlueDataWrapperPool * pool)
{
	if(pool != nullptr) {
		pool->dataWrapperDestroyed(p);
	}
	freeGlueDataWrapper(p);
}


} //namespace bind_internal

} //namespace cpgf


#endif
