#include "cpgf/gmetaannotation.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetafield.h"
#include "cpgf/gmetamethod.h"
#include "cpgf/gmetaoperator.h"
#include "cpgf/gmetaproperty.h"
#include "cpgf/gmetafundamental.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gmetamodule.h"
#include "cpgf/gassert.h"
#include "cpgf/gexception.h"
#include "cpgf/gapiutil.h"
#include "cpgf/gmemorypool.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gsharedinterface.h"

#include <string>


#ifdef G_COMPILER_CPPBUILDER
#pragma warn -8104 //Local Static with constructor dangerous for multi-threaded apps
#endif


#define ENTER_META_API() \
	this->ginterface_implExtendObject.clearError(); \
	try {

#define LEAVE_META_API(...) \
	} \
	catch(const GException & e) { this->ginterface_implExtendObject.handleError(e.getCode(), e.getMessage()); __VA_ARGS__; }
	

#define IMPL_ITEM \
protected: \
	virtual IMetaItem * G_API_CC clone() { return this->doClone(); } \
	virtual const char * G_API_CC getName() { return this->doGetName(); } \
	virtual const char * G_API_CC getQualifiedName() { return this->doGetQualifiedName(); } \
	virtual IMetaItem * G_API_CC getOwnerItem() { return this->doGetOwnerItem(); } \
	virtual void G_API_CC getItemType(GMetaTypeData * outType) { this->doGetItemType(outType); } \
	virtual void G_API_CC getItemExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags) { this->doGetItemExtendType(outExtendType, flags); } \
	virtual uint32_t G_API_CC getCategory() { return this->doGetCategory(); } \
	virtual IMetaAnnotation * G_API_CC getAnnotation(const char * name) { return this->doGetAnnotation(name); } \
	virtual uint32_t G_API_CC getAnnotationCount() { return this->doGetAnnotationCount(); } \
	virtual IMetaAnnotation * G_API_CC getAnnotationAt(uint32_t index) { return this->doGetAnnotationAt(index); } \
	virtual gapi_bool G_API_CC equals(IMetaItem * other) { return this->doEquals(other); } \
	virtual gapi_bool G_API_CC isStatic() { return this->doIsStatic(); } \
	const GMetaItem * getItem() const {	return this->doGetItem(); }

#define IMPL_ALL \
	G_INTERFACE_IMPL_OBJECT_DERIVED \
	G_INTERFACE_IMPL_EXTENDOBJECT_DERIVED \
	IMPL_ITEM

#define IMPL_TYPEDITEM \
protected: \
	virtual void G_API_CC getMetaType(GMetaTypeData * outType) { this->doGetMetaType(outType); } \
	virtual uint32_t G_API_CC getTypeSize() { return this->doGetTypeSize(); } \
	virtual void * G_API_CC createInstance() { return this->doCreateInstance(); } \
	virtual void * G_API_CC createInplace(void * placement) { return this->doCreateInplace(placement); } \
	virtual void * G_API_CC cloneInstance(const void * instance) { return this->doCloneInstance(instance); } \
	virtual void * G_API_CC cloneInplace(const void * instance, void * placement) { return this->doCloneInplace(instance, placement); } \
	virtual void G_API_CC destroyInstance(void * instance) { this->doDestroyInstance(instance); } \
	virtual void G_API_CC destroyInplace(void * instance) { this->doDestroyInplace(instance); }

#define IMPL_CALLABLE \
protected: \
	virtual void G_API_CC getParamType(GMetaTypeData * outType, uint32_t index) { this->doGetParamType(outType, index); } \
	virtual uint32_t G_API_CC getParamCount() { return this->doGetParamCount(); } \
	virtual uint32_t G_API_CC getDefaultParamCount() { return this->doGetDefaultParamCount(); } \
	virtual gapi_bool G_API_CC hasResult() { return this->doHasResult(); } \
	virtual void G_API_CC getResultType(GMetaTypeData * outType) { this->doGetResultType(outType); } \
	virtual void G_API_CC getResultExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags) { this->doGetResultExtendType(outExtendType, flags); } \
	virtual void G_API_CC getParamExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags, uint32_t index) { this->doGetParamExtendType(outExtendType, flags, index); } \
	virtual gapi_bool G_API_CC isVariadic() { return this->doIsVariadic(); } \
	virtual gapi_bool G_API_CC isExplicitThis() { return this->doIsExplicitThis(); } \
	virtual gapi_bool G_API_CC checkParam(const GVariantData * param, uint32_t paramIndex) { return this->doCheckParam(param, paramIndex); } \
	virtual gapi_bool G_API_CC isParamTransferOwnership(uint32_t paramIndex) { return this->doIsParamTransferOwnership(paramIndex); } \
	virtual gapi_bool G_API_CC isResultTransferOwnership() { return this->doIsResultTransferOwnership(); }


#define IMPL_ACCESSIBLE \
protected: \
	virtual gapi_bool G_API_CC canGet() { return this->doCanGet(); } \
	virtual gapi_bool G_API_CC canSet() { return this->doCanSet(); } \
	virtual void G_API_CC get(GVariantData * outResult, const void * instance) { this->doGet(outResult, instance); } \
	virtual void G_API_CC set(void * instance, const GVariantData * value) { this->doSet(instance, value); } \
	virtual void * G_API_CC getAddress(const void * instance) { return this->doGetAddress(instance); } \
	virtual uint32_t G_API_CC getSize() { return this->doGetSize(); }

#define USE_POOL(cls) \
	void * operator new(size_t /*size*/) { \
		return GMemoryPool::getInstance()->allocate(sizeof(cls)); \
	} \
	void operator delete(void * p, size_t size) { \
		GMemoryPool::getInstance()->free(p, size); \
	}



namespace cpgf {


class ImplMetaItem
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

public:
	ImplMetaItem(const GMetaItem * item, bool freeItem);
	virtual ~ImplMetaItem();

public:
	const GMetaItem * doGetItem() const {
		return this->item;
	}

protected:
	IMetaItem * doClone();
	const char * doGetName();
	const char * doGetQualifiedName();
	IMetaItem * doGetOwnerItem();
	void doGetItemType(GMetaTypeData * outType);
	void doGetItemExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags);
	uint32_t doGetCategory();
	IMetaAnnotation * doGetAnnotation(const char * name);
	uint32_t doGetAnnotationCount();
	IMetaAnnotation * doGetAnnotationAt(uint32_t index);
	gapi_bool doEquals(IMetaItem * other);
	
	gapi_bool doIsStatic();
	gapi_bool doIsField();
	gapi_bool doIsProperty();
	gapi_bool doIsMethod();
	gapi_bool doIsEnum();
	gapi_bool doIsOperator();
	gapi_bool doIsConstructor();
	gapi_bool doIsClass();
	gapi_bool doIsAnnotation();
	gapi_bool doIsFundamental();

private:
	const GMetaItem * item;
	bool freeItem;
};


class ImplMetaTypedItem : public ImplMetaItem
{
private:
	typedef ImplMetaItem super;

public:
	ImplMetaTypedItem(const GMetaTypedItem * typedItem, bool freeItem);

protected:
	void doGetMetaType(GMetaTypeData * outType);
	uint32_t doGetTypeSize();

	void * doCreateInstance();
	void * doCreateInplace(void * placement);
	void * doCloneInstance(const void * instance);
	void * doCloneInplace(const void * instance, void * placement);

	void doDestroyInstance(void * instance);
	void doDestroyInplace(void * instance);

private:
	const GMetaTypedItem * getTypedItem() const {
		return static_cast<const GMetaTypedItem *>(this->doGetItem());
	}
};


class ImplMetaList : public IMetaList
{
private:
	typedef GSharedInterface<IMetaItem> ItemType;
	typedef std::vector<ItemType> ListType;

public:
	USE_POOL(ImplMetaList)

	ImplMetaList();
	ImplMetaList(GMetaList * metaList);
	virtual ~ImplMetaList();
	
	void load(GMetaList * metaList);

	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

protected:
	virtual void G_API_CC add(IMetaItem * item, void * instance);
	virtual uint32_t G_API_CC getCount();
	virtual IMetaItem * G_API_CC getAt(uint32_t index);
	virtual void * G_API_CC getInstanceAt(uint32_t index);
	virtual void G_API_CC clear();

private:
	ImplMetaList(const ImplMetaList &);
	ImplMetaList & operator = (const ImplMetaList &);

private:
	ListType metaList;
	std::vector<void *> instanceList;
};


class ImplMetaCallable : public ImplMetaItem
{
private:
	typedef ImplMetaItem super;

public:
	ImplMetaCallable(const GMetaCallable * callable, bool freeItem = false);

protected:
	void doGetParamType(GMetaTypeData * outType, uint32_t index);
	uint32_t doGetParamCount();
	uint32_t doGetDefaultParamCount();
	gapi_bool doHasResult();
	void doGetResultType(GMetaTypeData * outType);
	void doGetResultExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags);
	void doGetParamExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags, uint32_t index);
	gapi_bool doIsVariadic();
	gapi_bool doIsExplicitThis();
	gapi_bool doCheckParam(const GVariantData * param, uint32_t paramIndex);
	gapi_bool doIsParamTransferOwnership(uint32_t paramIndex);
	gapi_bool doIsResultTransferOwnership();
	IMetaConverter * doCreateResultConverter();

private:
	const GMetaCallable * getCallable() const {
		return static_cast<const GMetaCallable * >(this->doGetItem());
	}
};

class ImplMetaAccessible : public ImplMetaItem
{
private:
	typedef ImplMetaItem super;

public:
	ImplMetaAccessible(const GMetaAccessible * accessible, bool freeItem = false);

protected:
	gapi_bool doCanGet();
	gapi_bool doCanSet();
	void doGet(GVariantData * outResult, const void * instance);
	void doSet(void * instance, const GVariantData * value);
	void * doGetAddress(const void * instance);
	uint32_t doGetSize();

private:
	const GMetaAccessible * getAccessible() const {
		return static_cast<const GMetaAccessible * >(this->doGetItem());
	}
};

class ImplMetaField : public ImplMetaAccessible, public IMetaField
{
private:
	typedef ImplMetaAccessible super;

public:
	USE_POOL(ImplMetaField)

	ImplMetaField(const GMetaField * field, bool freeItem);

	IMPL_ALL


	IMPL_ACCESSIBLE

private:
	const GMetaField * getField() const {
		return static_cast<const GMetaField *>(this->doGetItem());
	}
};


class ImplMetaProperty : public ImplMetaAccessible, public IMetaProperty
{
private:
	typedef ImplMetaAccessible super;

public:
	USE_POOL(ImplMetaProperty)

	ImplMetaProperty(const GMetaProperty * prop, bool freeItem);

	IMPL_ALL

	IMPL_ACCESSIBLE

protected:

private:
	const GMetaProperty * getProperty() const {
		return static_cast<const GMetaProperty *>(this->doGetItem());
	}
};


class ImplMetaMethod : public ImplMetaCallable, public IMetaMethod
{
private:
	typedef ImplMetaCallable super;

public:
	USE_POOL(ImplMetaMethod)

	ImplMetaMethod(const GMetaMethod * method, bool freeItem);

	IMPL_ALL

	IMPL_CALLABLE

protected:
	virtual void G_API_CC execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC executeIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount);
	virtual void G_API_CC invoke(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC invokeIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount);

private:
	const GMetaMethod * getMethod() const {
		return static_cast<const GMetaMethod *>(this->doGetItem());
	}
};


class ImplMetaConstructor : public ImplMetaCallable, public IMetaConstructor
{
private:
	typedef ImplMetaCallable super;

	IMPL_ALL

	IMPL_CALLABLE

public:
	USE_POOL(ImplMetaConstructor)

	ImplMetaConstructor(const GMetaConstructor * constructor, bool freeItem);

protected:
	virtual void G_API_CC execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC executeIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount);
	virtual void * G_API_CC invoke(const GVariantData * params, uint32_t paramCount);
	virtual void * G_API_CC invokeIndirectly(GVariantData const * const * params, uint32_t paramCount);

private:
	const GMetaConstructor * getConstructor() const {
		return static_cast<const GMetaConstructor *>(this->doGetItem());
	}
};



class ImplMetaOperator : public ImplMetaCallable, public IMetaOperator
{
private:
	typedef ImplMetaCallable super;

	IMPL_ALL

	IMPL_CALLABLE

public:
	USE_POOL(ImplMetaOperator)

	ImplMetaOperator(const GMetaOperator * op, bool freeItem);

protected:
	virtual int32_t G_API_CC getOperator();
	virtual void G_API_CC execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC executeIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount);
	virtual void G_API_CC invokeUnary(GVariantData * outResult, const GVariantData * p0);
	virtual void G_API_CC invokeBinary(GVariantData * outResult, const GVariantData * p0, const GVariantData * p1);
	virtual void G_API_CC invokeFunctor(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount);
	virtual void G_API_CC invokeFunctorIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount);

private:
	const GMetaOperator * getOperatorItem() const {
		return static_cast<const GMetaOperator *>(this->doGetItem());
	}
};



class ImplMetaFundamental : public ImplMetaTypedItem, public IMetaFundamental
{
private:
	typedef ImplMetaTypedItem super;

	IMPL_ALL
	IMPL_TYPEDITEM

public:
	USE_POOL(ImplMetaFundamental)

	ImplMetaFundamental(const GMetaFundamental * fundamental, bool freeItem);

protected:
	virtual void G_API_CC getValue(GVariantData * outResult, const void * instance);

private:
	const GMetaFundamental * getFundamental() const {
		return static_cast<const GMetaFundamental *>(this->doGetItem());
	}
};


class ImplMetaEnum : public ImplMetaTypedItem, public IMetaEnum
{
private:
	typedef ImplMetaTypedItem super;

	IMPL_ALL
	IMPL_TYPEDITEM

public:
	USE_POOL(ImplMetaEnum)

	ImplMetaEnum(const GMetaEnum * en, bool freeItem);

protected:
	virtual uint32_t G_API_CC getCount();
	virtual const char * G_API_CC getKey(uint32_t index);
	virtual void G_API_CC getValue(GVariantData * outResult, uint32_t index);
	virtual int32_t G_API_CC findKey(const char * key);

private:
	const GMetaEnum * getEnum() const {
		return static_cast<const GMetaEnum *>(this->doGetItem());
	}
};


class ImplMetaAnnotationValue : public IMetaAnnotationValue
{
public:
	USE_POOL(ImplMetaAnnotationValue)

	ImplMetaAnnotationValue(const GAnnotationValue * value, bool);
	virtual ~ImplMetaAnnotationValue();

	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

protected:
	virtual void G_API_CC getVariant(GVariantData * outVariant);
	virtual gapi_bool G_API_CC canToString();
	virtual gapi_bool G_API_CC canToWideString();
	virtual gapi_bool G_API_CC canToInt();
	virtual const char * G_API_CC toString();
	virtual const wchar_t * G_API_CC toWideString();
	virtual int32_t G_API_CC toInt32();
	virtual gapi_bool G_API_CC toBoolean();

private:
	const GAnnotationValue * value;
};


class ImplMetaAnnotation : public ImplMetaItem, public IMetaAnnotation
{
private:
	typedef ImplMetaItem super;

public:
	USE_POOL(ImplMetaAnnotation)

	ImplMetaAnnotation(const GMetaAnnotation * annotation, bool freeItem = false);

	IMPL_ALL

protected:
	virtual IMetaItem * G_API_CC getMetaItem();
	virtual IMetaAnnotationValue * G_API_CC getValue(const char * name);
	virtual uint32_t G_API_CC getCount();
	virtual const char * G_API_CC getNameAt(uint32_t index);
	virtual IMetaAnnotationValue * G_API_CC getValueAt(uint32_t index);

private:
	const GMetaAnnotation * getAnnotation() const {
		return static_cast<const GMetaAnnotation *>(this->doGetItem());
	}
};



class ImplMetaClass : public ImplMetaTypedItem, public IMetaClass
{
private:
	typedef ImplMetaTypedItem super;

public:
	USE_POOL(ImplMetaClass)

	ImplMetaClass(const GMetaClass * cls, bool freeItem);

	IMPL_ALL
	IMPL_TYPEDITEM

protected:
	virtual IMetaConstructor * G_API_CC getConstructorByParamCount(uint32_t paramCount);
	virtual uint32_t G_API_CC getConstructorCount();
	virtual IMetaConstructor * G_API_CC getConstructorAt(uint32_t index);

	virtual IMetaField * G_API_CC getFieldInHierarchy(const char * name, void ** instance);
	virtual IMetaField * G_API_CC getField(const char * name);
	virtual uint32_t G_API_CC getFieldCount();
	virtual IMetaField * G_API_CC getFieldAt(uint32_t index);

	virtual IMetaProperty * G_API_CC getPropertyInHierarchy(const char * name, void ** instance);
	virtual IMetaProperty * G_API_CC getProperty(const char * name);
	virtual uint32_t G_API_CC getPropertyCount();
	virtual IMetaProperty * G_API_CC getPropertyAt(uint32_t index);

	virtual IMetaMethod * G_API_CC getMethodInHierarchy(const char * name, void ** instance);
	virtual IMetaMethod * G_API_CC getMethod(const char * name);
	virtual uint32_t G_API_CC getMethodCount();
	virtual IMetaMethod * G_API_CC getMethodAt(uint32_t index);

	virtual IMetaOperator * G_API_CC getOperatorInHierarchy(uint32_t op, void ** instance);
	virtual IMetaOperator * G_API_CC getOperator(uint32_t op);
	virtual uint32_t G_API_CC getOperatorCount();
	virtual IMetaOperator * G_API_CC getOperatorAt(uint32_t index);

	virtual IMetaEnum * G_API_CC getEnumInHierarchy(const char * name, void ** instance);
	virtual IMetaEnum * G_API_CC getEnum(const char * name);
	virtual uint32_t G_API_CC getEnumCount();
	virtual IMetaEnum * G_API_CC getEnumAt(uint32_t index);

	virtual IMetaClass * G_API_CC getClassInHierarchy(const char * name, void ** instance);
	virtual IMetaClass * G_API_CC getClass(const char * name);
	virtual uint32_t G_API_CC getClassCount();
	virtual IMetaClass * G_API_CC getClassAt(uint32_t index);

	virtual uint32_t G_API_CC getMetaCount();
	virtual IMetaItem * G_API_CC getMetaAt(uint32_t index);

	virtual gapi_bool G_API_CC isGlobal();
	virtual gapi_bool G_API_CC isAbstract();
	virtual gapi_bool G_API_CC isPolymorphic();
	virtual gapi_bool G_API_CC canCreateInstance();
	virtual gapi_bool G_API_CC canCopyInstance();

	virtual IMetaClass * G_API_CC getBaseClass(uint32_t baseIndex);
	virtual uint32_t G_API_CC getBaseCount();

	virtual IMetaClass * G_API_CC getDerivedClass(uint32_t baseIndex);
	virtual uint32_t G_API_CC getDerivedCount();
	
	virtual gapi_bool G_API_CC isInheritedFrom(IMetaClass * ancient);

	virtual void * G_API_CC castFromBase(const void * base, uint32_t baseIndex);
	virtual void * G_API_CC castToBase(const void * self, uint32_t baseIndex);

	virtual void * G_API_CC castFromDerived(const void * derived, uint32_t derivedIndex);
	virtual void * G_API_CC castToDerived(const void * self, uint32_t derivedIndex);

private:
	const GMetaClass * getClass() const {
		return static_cast<const GMetaClass *>(this->doGetItem());
	}
};



class ImplMetaModule : public IMetaModule
{
public:
	USE_POOL(ImplMetaModule)

	ImplMetaModule(GMetaModule * module, GMetaClass * metaClass);
	virtual ~ImplMetaModule();

	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

protected:
	virtual IMetaClass * G_API_CC getGlobalMetaClass();
	
	virtual IMetaTypedItem * G_API_CC findTypedItemByName(const char * name);
	virtual IMetaClass * G_API_CC findClassByName(const char * name);

private:
	GMetaModule * module;
	GMetaClass * metaClass;
};



class ImplMetaService : public IMetaService
{
private:
	typedef GSharedInterface<IMetaModule> ItemType;
	typedef std::vector<ItemType> ListType;

public:
	USE_POOL(ImplMetaService)

	ImplMetaService();
	virtual ~ImplMetaService();

	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

protected:
	virtual void G_API_CC addModule(IMetaModule * module);
	virtual uint32_t G_API_CC getModuleCount();
	virtual IMetaModule * G_API_CC getModuleAt(uint32_t index);

	virtual IMemoryAllocator * G_API_CC getAllocator();

	virtual IMetaTypedItem * G_API_CC findTypedItemByName(const char * name);
	virtual IMetaClass * G_API_CC findClassByName(const char * name);

private:
	void clear();

private:
	ListType moduleList;
	GScopedInterface<IMemoryAllocator> allocator;
};



template <typename T, typename P>
T * doCreateItem(P * p)
{
	if(p) {
		return new T(p, false);
	}
	else {
		return nullptr;
	}
}

void loadMetaList(IMetaList * metaList, GMetaList * rawMetaList)
{
	size_t count = rawMetaList->getCount();
	for(size_t i = 0; i < count; ++i) {
		IMetaItem * item = metaItemToInterface(rawMetaList->getAt(i));
		metaList->add(item, rawMetaList->getInstanceAt(i));
		item->releaseReference();
	}
}

// This function is defined in gvariant.cpp, for internal use.
GVariant createVariantFromData(const GVariantData & data);


ImplMetaItem::ImplMetaItem(const GMetaItem * item, bool freeItem)
	: item(item), freeItem(freeItem)
{
}

ImplMetaItem::~ImplMetaItem()
{
	if(this->freeItem) {
		delete this->item;
	}
}

IMetaItem * ImplMetaItem::doClone()
{
	return metaItemToInterface(this->item);
}

const char * ImplMetaItem::doGetName()
{
	ENTER_META_API()

	return this->item->getName().c_str();

	LEAVE_META_API(return nullptr)
}

const char * ImplMetaItem::doGetQualifiedName()
{
	ENTER_META_API()

	return this->item->getQualifiedName().c_str();

	LEAVE_META_API(return nullptr)
}

IMetaItem * ImplMetaItem::doGetOwnerItem()
{
	ENTER_META_API()

	return metaItemToInterface(this->item->getOwnerItem());

	LEAVE_META_API(return nullptr)
}

void ImplMetaItem::doGetItemType(GMetaTypeData * outType)
{
	ENTER_META_API()
	
	initializeMetaType(outType);

	GMetaType type = this->item->getItemType();

	fixupMetaType(&type, this->item);

	*outType = type.refData();

	LEAVE_META_API()
}

void ImplMetaItem::doGetItemExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags)
{
	ENTER_META_API()

	GMetaExtendType extendType = this->item->getItemExtendType(flags);

	*outExtendType = extendType.takeData();

	LEAVE_META_API()
}

uint32_t ImplMetaItem::doGetCategory()
{
// Don't try/catch to avoid "unreachable code" warning. It's safe.
//	ENTER_META_API()

	return this->item->getCategory();

//	LEAVE_META_API(return 0)
}

IMetaAnnotation * ImplMetaItem::doGetAnnotation(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaAnnotation>(this->doGetItem()->getAnnotation(name));

	LEAVE_META_API(return nullptr)
}

uint32_t ImplMetaItem::doGetAnnotationCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->doGetItem()->getAnnotationCount());

	LEAVE_META_API(return 0)
}

IMetaAnnotation * ImplMetaItem::doGetAnnotationAt(uint32_t index)
{
	ENTER_META_API()

	return new ImplMetaAnnotation(this->doGetItem()->getAnnotationAt(index));

	LEAVE_META_API(return nullptr)
}

gapi_bool ImplMetaItem::doEquals(IMetaItem * other)
{
	ENTER_META_API()

	return strcmp(this->doGetQualifiedName(), other->getQualifiedName()) == 0;

	LEAVE_META_API(return false)
}

gapi_bool ImplMetaItem::doIsStatic()
{
// Don't try/catch to avoid "unreachable code" warning. It's safe.
//	ENTER_META_API()

	return this->doGetItem()->isStatic();

//	LEAVE_META_API(return false)
}



ImplMetaTypedItem::ImplMetaTypedItem(const GMetaTypedItem * typedItem, bool freeItem)
	: super(typedItem, freeItem)
{
}

void ImplMetaTypedItem::doGetMetaType(GMetaTypeData * outType)
{
	ENTER_META_API()

	initializeMetaType(outType);

	GMetaType type = this->getTypedItem()->getMetaType();
	
	fixupMetaType(&type, this->getTypedItem());

	*outType = type.refData();

	LEAVE_META_API()
}

uint32_t ImplMetaTypedItem::doGetTypeSize()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getTypedItem()->getTypeSize());

	LEAVE_META_API(return 0)
}

void * ImplMetaTypedItem::doCreateInstance()
{
	ENTER_META_API()

	return this->getTypedItem()->createInstance();

	LEAVE_META_API(return nullptr)
}

void * ImplMetaTypedItem::doCreateInplace(void * placement)
{
	ENTER_META_API()

	return this->getTypedItem()->createInplace(placement);

	LEAVE_META_API(return nullptr)
}

void * ImplMetaTypedItem::doCloneInstance(const void * instance)
{
	ENTER_META_API()

	return this->getTypedItem()->cloneInstance(instance);

	LEAVE_META_API(return nullptr)
}

void * ImplMetaTypedItem::doCloneInplace(const void * instance, void * placement)
{
	ENTER_META_API()

	return this->getTypedItem()->cloneInplace(instance, placement);

	LEAVE_META_API(return nullptr)
}

void ImplMetaTypedItem::doDestroyInstance(void * instance)
{
	ENTER_META_API()

	this->getTypedItem()->destroyInstance(instance);

	LEAVE_META_API()
}

void ImplMetaTypedItem::doDestroyInplace(void * instance)
{
	ENTER_META_API()

	this->getTypedItem()->destroyInplace(instance);

	LEAVE_META_API()
}



ImplMetaCallable::ImplMetaCallable(const GMetaCallable * callable, bool freeItem)
	: super(callable, freeItem)
{
}

void ImplMetaCallable::doGetParamType(GMetaTypeData * outType, uint32_t index)
{
	ENTER_META_API()

	initializeMetaType(outType);

	GMetaType type = this->getCallable()->getParamType(index);

	fixupMetaType(&type, this->getCallable());

	*outType = type.refData();

	LEAVE_META_API()
}

uint32_t ImplMetaCallable::doGetParamCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getCallable()->getParamCount());

	LEAVE_META_API(return 0)
}

uint32_t ImplMetaCallable::doGetDefaultParamCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getCallable()->getDefaultParamCount());

	LEAVE_META_API(return 0)
}

gapi_bool ImplMetaCallable::doHasResult()
{
	ENTER_META_API()

	return this->getCallable()->hasResult();

	LEAVE_META_API(return false)
}

void ImplMetaCallable::doGetResultType(GMetaTypeData * outType)
{
	ENTER_META_API()

	initializeMetaType(outType);

	GMetaType type = this->getCallable()->getResultType();

	fixupMetaType(&type, this->getCallable());

	*outType = type.refData();

	LEAVE_META_API()
}

void ImplMetaCallable::doGetResultExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags)
{
	ENTER_META_API()

	GMetaExtendType extendType = this->getCallable()->getResultExtendType(flags);

	*outExtendType = extendType.takeData();

	LEAVE_META_API()
}

void ImplMetaCallable::doGetParamExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags, uint32_t index)
{
	ENTER_META_API()

	GMetaExtendType extendType = this->getCallable()->getParamExtendType(flags, index);

	*outExtendType = extendType.takeData();

	LEAVE_META_API()
}

gapi_bool ImplMetaCallable::doIsVariadic()
{
	ENTER_META_API()

	return this->getCallable()->isVariadic();

	LEAVE_META_API(return false)
}

gapi_bool ImplMetaCallable::doIsExplicitThis()
{
	ENTER_META_API()

	return this->getCallable()->isExplicitThis();

	LEAVE_META_API(return false)
}

gapi_bool ImplMetaCallable::doCheckParam(const GVariantData * param, uint32_t paramIndex)
{
	ENTER_META_API()

	return this->getCallable()->checkParam(createVariantFromData(*param), paramIndex);

	LEAVE_META_API(return false)
}

gapi_bool ImplMetaCallable::doIsParamTransferOwnership(uint32_t paramIndex)
{
	ENTER_META_API()

	return this->getCallable()->isParamTransferOwnership(paramIndex);

	LEAVE_META_API(return false)
}

gapi_bool ImplMetaCallable::doIsResultTransferOwnership()
{
	ENTER_META_API()

	return this->getCallable()->isResultTransferOwnership();

	LEAVE_META_API(return false)
}


ImplMetaAccessible::ImplMetaAccessible(const GMetaAccessible * accessible, bool freeItem)
	: super(accessible, freeItem)
{
}

gapi_bool ImplMetaAccessible::doCanGet()
{
	ENTER_META_API()

	return this->getAccessible()->canGet();

	LEAVE_META_API(return false)
}

gapi_bool ImplMetaAccessible::doCanSet()

{
	ENTER_META_API()

	return this->getAccessible()->canSet();

	LEAVE_META_API(return false)
}

void ImplMetaAccessible::doGet(GVariantData * outResult, const void * instance)
{
	ENTER_META_API()
	
	*outResult = this->getAccessible()->get(instance).takeData();

	LEAVE_META_API()
}

void ImplMetaAccessible::doSet(void * instance, const GVariantData * value)
{
	ENTER_META_API()

	this->getAccessible()->set(instance, createVariantFromData(*value));

	LEAVE_META_API()
}

void * ImplMetaAccessible::doGetAddress(const void * instance)
{
	ENTER_META_API()

	return this->getAccessible()->getAddress(instance);

	LEAVE_META_API(return 0)
}

uint32_t ImplMetaAccessible::doGetSize()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getAccessible()->getSize());

	LEAVE_META_API(return 0)
}


ImplMetaList::ImplMetaList()
{
}

ImplMetaList::ImplMetaList(GMetaList * metaList)
{
	loadMetaList(this, metaList);
}

ImplMetaList::~ImplMetaList()
{
	this->clear();
}

void G_API_CC ImplMetaList::add(IMetaItem * item, void * instance)
{
	this->metaList.push_back(ItemType(item));
	this->instanceList.push_back(instance);
}

uint32_t G_API_CC ImplMetaList::getCount()
{
	return static_cast<uint32_t>(this->metaList.size());
}

IMetaItem * G_API_CC ImplMetaList::getAt(uint32_t index)
{
	IMetaItem * item = this->metaList.at(index).get();
	item->addReference();
	return item;
}

void * G_API_CC ImplMetaList::getInstanceAt(uint32_t index)
{
	return this->instanceList.at(index);
}

void G_API_CC ImplMetaList::clear()
{
	this->metaList.clear();
	this->instanceList.clear();
}


ImplMetaField::ImplMetaField(const GMetaField * field, bool freeItem)
	: super(field, freeItem)
{
}


ImplMetaProperty::ImplMetaProperty(const GMetaProperty * prop, bool freeItem)
	: super(prop, freeItem)
{
}


ImplMetaMethod::ImplMetaMethod(const GMetaMethod * method, bool freeItem)
	: super(method, freeItem)
{
}

void G_API_CC ImplMetaMethod::execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount)
{
	this->invoke(outResult, instance, params, paramCount);
}

void G_API_CC ImplMetaMethod::executeIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount)
{
	this->invokeIndirectly(outResult, instance, params, paramCount);
}

struct VariantParameterBuffer
{
	VariantParameterBuffer(GVariantData const * const * params, uint32_t paramCount) noexcept
		: paramCount(paramCount), variants((GVariant *)variantsBuffer)
	{
		GASSERT(paramCount <= REF_MAX_ARITY);

		for(uint32_t i = 0; i < this->paramCount; ++i) {
			new (&this->variants[i]) GVariant(createVariantFromData(*params[i]));
		}
	}

	VariantParameterBuffer(const GVariantData * params, uint32_t paramCount) noexcept
		: paramCount(paramCount), variants((GVariant *)variantsBuffer)
	{
		GASSERT(paramCount <= REF_MAX_ARITY);

		for(uint32_t i = 0; i < this->paramCount; ++i) {
			new (&this->variants[i]) GVariant(createVariantFromData(params[i]));
		}
	}

	~VariantParameterBuffer()
	{
		for(uint32_t i = 0; i < this->paramCount; ++i) {
			this->variants[i].~GVariant();
		}
	}

	uint32_t paramCount;
	GVariant * variants;
	char variantsBuffer[sizeof(GVariant) * REF_MAX_ARITY];
};

void G_API_CC ImplMetaMethod::invoke(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount)
{
	ENTER_META_API()

	VariantParameterBuffer variantsBuffer(params, paramCount);

	if(outResult == nullptr) {
		this->getMethod()->execute(instance, variantsBuffer.variants, paramCount);
	}
	else {
		*outResult = this->getMethod()->execute(instance, variantsBuffer.variants, paramCount).takeData();
	}

	LEAVE_META_API()
}

void G_API_CC ImplMetaMethod::invokeIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount)
{
	ENTER_META_API()
/*
	VariantParameterBuffer variantsBuffer(params, paramCount);

	if(outResult == nullptr) {
		this->getMethod()->execute(instance, variantsBuffer.variants, paramCount).takeData();
	}
	else {
		*outResult = this->getMethod()->execute(instance, variantsBuffer.variants, paramCount).takeData();
	}
return;
*/
	if(outResult == nullptr) {
		this->getMethod()->executeByData(instance, (const GVariantData **)params, paramCount);
	}
	else {
		*outResult = this->getMethod()->executeByData(instance, (const GVariantData **)params, paramCount).takeData();
	}

	LEAVE_META_API()
}



ImplMetaConstructor::ImplMetaConstructor(const GMetaConstructor * constructor, bool freeItem)
	: super(constructor, freeItem)
{
}

void G_API_CC ImplMetaConstructor::execute(GVariantData * outResult, void * /*instance*/, const GVariantData * params, uint32_t paramCount)
{
	if(outResult != nullptr) {
		void * newObj = this->invoke(params, paramCount);
		*outResult = GVariant(newObj).takeData();
	}
}

void G_API_CC ImplMetaConstructor::executeIndirectly(GVariantData * outResult, void * /*instance*/, GVariantData const * const * params, uint32_t paramCount)
{
	if(outResult != nullptr) {
		void * newObj = this->invokeIndirectly(params, paramCount);
		*outResult = GVariant(newObj).takeData();
	}
}

void * G_API_CC ImplMetaConstructor::invoke(const GVariantData * params, uint32_t paramCount)
{
	ENTER_META_API()

	VariantParameterBuffer variantsBuffer(params, paramCount);

	return objectAddressFromVariant(this->getConstructor()->execute(nullptr, variantsBuffer.variants, paramCount));

	LEAVE_META_API(return nullptr)
}

void * G_API_CC ImplMetaConstructor::invokeIndirectly(GVariantData const * const * params, uint32_t paramCount)
{
	ENTER_META_API()

	VariantParameterBuffer variantsBuffer(params, paramCount);

	return objectAddressFromVariant(this->getConstructor()->execute(nullptr, variantsBuffer.variants, paramCount));

	LEAVE_META_API(return nullptr)
}



ImplMetaOperator::ImplMetaOperator(const GMetaOperator * op, bool freeItem)
	: super(op, freeItem)
{
}

int32_t G_API_CC ImplMetaOperator::getOperator()
{
	ENTER_META_API()

	return this->getOperatorItem()->getOperator();

	LEAVE_META_API(return 0)
}

void G_API_CC ImplMetaOperator::execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount)
{
	this->invokeFunctor(outResult, instance, params, paramCount);
}

void G_API_CC ImplMetaOperator::executeIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount)
{
	this->invokeFunctorIndirectly(outResult, instance, params, paramCount);
}

void G_API_CC ImplMetaOperator::invokeUnary(GVariantData * outResult, const GVariantData * p0)
{
	ENTER_META_API()

	if(outResult == nullptr) {
		this->getOperatorItem()->invokeUnary(createVariantFromData(*p0));
	}
	else {
		*outResult = this->getOperatorItem()->invokeUnary(createVariantFromData(*p0)).takeData();
	}

	LEAVE_META_API()
}

void G_API_CC ImplMetaOperator::invokeBinary(GVariantData * outResult, const GVariantData * p0, const GVariantData * p1)
{
	ENTER_META_API()

	if(outResult == nullptr) {
		this->getOperatorItem()->invokeBinary(createVariantFromData(*p0), createVariantFromData(*p1));
	}
	else {
		*outResult = this->getOperatorItem()->invokeBinary(createVariantFromData(*p0), createVariantFromData(*p1)).takeData();
	}

	LEAVE_META_API()
}

void G_API_CC ImplMetaOperator::invokeFunctor(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount)
{
	ENTER_META_API()

	VariantParameterBuffer variantsBuffer(params, paramCount);

	if(outResult == nullptr) {
		this->getOperatorItem()->execute(instance, variantsBuffer.variants, paramCount);
	}
	else {
		*outResult = this->getOperatorItem()->execute(instance, variantsBuffer.variants, paramCount).takeData();
	}

	LEAVE_META_API()
}

void G_API_CC ImplMetaOperator::invokeFunctorIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount)
{
	ENTER_META_API()

	VariantParameterBuffer variantsBuffer(params, paramCount);

	if(outResult == nullptr) {
		this->getOperatorItem()->execute(instance, variantsBuffer.variants, paramCount);
	}
	else {
		*outResult = this->getOperatorItem()->execute(instance, variantsBuffer.variants, paramCount).takeData();
	}

	LEAVE_META_API()
}



ImplMetaFundamental::ImplMetaFundamental(const GMetaFundamental * fundamental, bool freeItem)
	: super(fundamental, freeItem)
{
}

void G_API_CC ImplMetaFundamental::getValue(GVariantData * outResult, const void * instance)
{
	ENTER_META_API()

	*outResult = this->getFundamental()->getValue(instance).takeData();

	LEAVE_META_API()
}


ImplMetaEnum::ImplMetaEnum(const GMetaEnum * en, bool freeItem)
	: super(en, freeItem)
{
}

uint32_t G_API_CC ImplMetaEnum::getCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getEnum()->getCount());

	LEAVE_META_API(return 0)
}

const char * G_API_CC ImplMetaEnum::getKey(uint32_t index)
{
	ENTER_META_API()

	return this->getEnum()->getKey(index);

	LEAVE_META_API(return nullptr)
}

void G_API_CC ImplMetaEnum::getValue(GVariantData * outResult, uint32_t index)
{
	ENTER_META_API()

	*outResult = this->getEnum()->getValue(index).takeData();

	LEAVE_META_API()
}

int32_t G_API_CC ImplMetaEnum::findKey(const char * key)
{
	ENTER_META_API()

	return this->getEnum()->findKey(key);

	LEAVE_META_API(return -1)
}


ImplMetaAnnotationValue::ImplMetaAnnotationValue(const GAnnotationValue * value, bool)
	: value(value)
{
}

ImplMetaAnnotationValue::~ImplMetaAnnotationValue()
{
}

void G_API_CC ImplMetaAnnotationValue::getVariant(GVariantData * outVariant)
{
	ENTER_META_API()

	*outVariant = GVariant(*this->value->getVariant()).takeData();

	LEAVE_META_API()
}

gapi_bool G_API_CC ImplMetaAnnotationValue::canToString()
{
	ENTER_META_API()

	return this->value->canToString();

	LEAVE_META_API(return false)
}

gapi_bool G_API_CC ImplMetaAnnotationValue::canToWideString()
{
	ENTER_META_API()

	return this->value->canToWideString();

	LEAVE_META_API(return false)
}

gapi_bool G_API_CC ImplMetaAnnotationValue::canToInt()
{
	ENTER_META_API()

	return this->value->canToInt();

	LEAVE_META_API(return false)
}

const char * G_API_CC ImplMetaAnnotationValue::toString()
{
	ENTER_META_API()

	return this->value->toString();

	LEAVE_META_API(return nullptr)
}

const wchar_t * G_API_CC ImplMetaAnnotationValue::toWideString()
{
	ENTER_META_API()

	return this->value->toWideString();

	LEAVE_META_API(return nullptr)
}

int32_t G_API_CC ImplMetaAnnotationValue::toInt32()
{
	ENTER_META_API()

	return this->value->toInt();

	LEAVE_META_API(return 0)
}

gapi_bool G_API_CC ImplMetaAnnotationValue::toBoolean()
{
	ENTER_META_API()

	return this->value->toBoolean();

	LEAVE_META_API(return 0)
}


ImplMetaAnnotation::ImplMetaAnnotation(const GMetaAnnotation * annotation, bool freeItem)
	: super(annotation, freeItem)
{
}

IMetaItem * G_API_CC ImplMetaAnnotation::getMetaItem()
{
	ENTER_META_API()

	return metaItemToInterface(this->getAnnotation()->getMetaItem());

	LEAVE_META_API(return nullptr)
}

IMetaAnnotationValue * G_API_CC ImplMetaAnnotation::getValue(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaAnnotationValue>(this->getAnnotation()->getValue(name));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaAnnotation::getCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getAnnotation()->getCount());

	LEAVE_META_API(return 0)
}

const char * G_API_CC ImplMetaAnnotation::getNameAt(uint32_t index)
{
	ENTER_META_API()

	return this->getAnnotation()->getNameAt(index);

	LEAVE_META_API(return nullptr)
}

IMetaAnnotationValue * G_API_CC ImplMetaAnnotation::getValueAt(uint32_t index)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaAnnotationValue>(this->getAnnotation()->getValueAt(index));

	LEAVE_META_API(return nullptr)
}




ImplMetaClass::ImplMetaClass(const GMetaClass * cls, bool freeItem)
	: super(cls, freeItem)
{
}

IMetaConstructor * G_API_CC ImplMetaClass::getConstructorByParamCount(uint32_t paramCount)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaConstructor>(this->getClass()->getConstructorByParamCount(paramCount));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaClass::getConstructorCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getConstructorCount());

	LEAVE_META_API(return 0)
}

IMetaConstructor * G_API_CC ImplMetaClass::getConstructorAt(uint32_t index)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaConstructor>(this->getClass()->getConstructorAt(index));

	LEAVE_META_API(return nullptr)
}

IMetaField * G_API_CC ImplMetaClass::getFieldInHierarchy(const char * name, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaField>(this->getClass()->getFieldInHierarchy(name, instance));

	LEAVE_META_API(return nullptr)
}

IMetaField * G_API_CC ImplMetaClass::getField(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaField>(this->getClass()->getField(name));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaClass::getFieldCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getFieldCount());

	LEAVE_META_API(return 0)
}

IMetaField * G_API_CC ImplMetaClass::getFieldAt(uint32_t index)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaField>(this->getClass()->getFieldAt(index));

	LEAVE_META_API(return nullptr)
}

IMetaProperty * G_API_CC ImplMetaClass::getPropertyInHierarchy(const char * name, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaProperty>(this->getClass()->getPropertyInHierarchy(name, instance));

	LEAVE_META_API(return nullptr)
}

IMetaProperty * G_API_CC ImplMetaClass::getProperty(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaProperty>(this->getClass()->getProperty(name));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaClass::getPropertyCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getPropertyCount());

	LEAVE_META_API(return 0)
}

IMetaProperty * G_API_CC ImplMetaClass::getPropertyAt(uint32_t index)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaProperty>(this->getClass()->getPropertyAt(index));

	LEAVE_META_API(return nullptr)
}

IMetaMethod * G_API_CC ImplMetaClass::getMethodInHierarchy(const char * name, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaMethod>(this->getClass()->getMethodInHierarchy(name, instance));

	LEAVE_META_API(return nullptr)
}

IMetaMethod * G_API_CC ImplMetaClass::getMethod(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaMethod>(this->getClass()->getMethod(name));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaClass::getMethodCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getMethodCount());

	LEAVE_META_API(return 0)
}

IMetaMethod * G_API_CC ImplMetaClass::getMethodAt(uint32_t index)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaMethod>(this->getClass()->getMethodAt(index));

	LEAVE_META_API(return nullptr)
}

IMetaOperator * G_API_CC ImplMetaClass::getOperatorInHierarchy(uint32_t op, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaOperator>(this->getClass()->getOperatorInHierarchy(static_cast<GMetaOpType>(op), instance));

	LEAVE_META_API(return nullptr)
}

IMetaOperator * G_API_CC ImplMetaClass::getOperator(uint32_t op)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaOperator>(this->getClass()->getOperator(static_cast<GMetaOpType>(op)));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaClass::getOperatorCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getOperatorCount());

	LEAVE_META_API(return 0)
}

IMetaOperator * G_API_CC ImplMetaClass::getOperatorAt(uint32_t index)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaOperator>(this->getClass()->getOperatorAt(index));

	LEAVE_META_API(return nullptr)
}

IMetaEnum * G_API_CC ImplMetaClass::getEnumInHierarchy(const char * name, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaEnum>(this->getClass()->getEnumInHierarchy(name, instance));

	LEAVE_META_API(return nullptr)
}

IMetaEnum * G_API_CC ImplMetaClass::getEnum(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaEnum>(this->getClass()->getEnum(name));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaClass::getEnumCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getEnumCount());

	LEAVE_META_API(return 0)
}

IMetaEnum * G_API_CC ImplMetaClass::getEnumAt(uint32_t index)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaEnum>(this->getClass()->getEnumAt(index));

	LEAVE_META_API(return nullptr)
}

IMetaClass * G_API_CC ImplMetaClass::getClassInHierarchy(const char * name, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaClass>(this->getClass()->getClassInHierarchy(name, instance));

	LEAVE_META_API(return nullptr)
}


IMetaClass * G_API_CC ImplMetaClass::getClass(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaClass>(this->getClass()->getClass(name));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaClass::getClassCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getClassCount());

	LEAVE_META_API(return 0)
}

IMetaClass * G_API_CC ImplMetaClass::getClassAt(uint32_t index)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaClass>(this->getClass()->getClassAt(index));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaClass::getMetaCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getMetaCount());

	LEAVE_META_API(return 0)
}

IMetaItem * G_API_CC ImplMetaClass::getMetaAt(uint32_t index)
{
	ENTER_META_API()

	return metaItemToInterface(this->getClass()->getMetaAt(index));

	LEAVE_META_API(return nullptr)
}

gapi_bool G_API_CC ImplMetaClass::isGlobal()
{
	ENTER_META_API()

	return this->getClass()->isGlobal();

	LEAVE_META_API(return false)
}

gapi_bool G_API_CC ImplMetaClass::isAbstract()
{
	ENTER_META_API()

	return this->getClass()->isAbstract();

	LEAVE_META_API(return false)
}

gapi_bool G_API_CC ImplMetaClass::isPolymorphic()
{
	ENTER_META_API()

	return this->getClass()->isPolymorphic();

	LEAVE_META_API(return false)
}

gapi_bool G_API_CC ImplMetaClass::canCreateInstance()
{
	ENTER_META_API()

	return this->getClass()->canCreateInstance();

	LEAVE_META_API(return false)
}

gapi_bool G_API_CC ImplMetaClass::canCopyInstance()
{
	ENTER_META_API()

	return this->getClass()->canCopyInstance();

	LEAVE_META_API(return false)
}

IMetaClass * G_API_CC ImplMetaClass::getBaseClass(uint32_t baseIndex)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaClass>(this->getClass()->getBaseClass(baseIndex));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaClass::getBaseCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getBaseCount());

	LEAVE_META_API(return 0)
}

IMetaClass * G_API_CC ImplMetaClass::getDerivedClass(uint32_t derivedIndex)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaClass>(this->getClass()->getDerivedClass(derivedIndex));

	LEAVE_META_API(return nullptr)
}

uint32_t G_API_CC ImplMetaClass::getDerivedCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getDerivedCount());

	LEAVE_META_API(return 0)
}

gapi_bool G_API_CC ImplMetaClass::isInheritedFrom(IMetaClass * ancient)
{
	ENTER_META_API()

	GScopedInterface<IMetaClass> item;

	for(uint32_t i = 0; i < this->getBaseCount(); ++i) {
		item.reset(this->getBaseClass(i));
		if(item) {
			if(item->equals(ancient)) {
				return true;
			}
			if(item->isInheritedFrom(ancient)) {
				return true;
			}
		}
	}

	return false;

	LEAVE_META_API(return false)
}

void * G_API_CC ImplMetaClass::castFromBase(const void * base, uint32_t baseIndex)
{
	ENTER_META_API()

	return this->getClass()->castFromBase(base, baseIndex);

	LEAVE_META_API(return nullptr)
}

void * G_API_CC ImplMetaClass::castToBase(const void * self, uint32_t baseIndex)
{
	ENTER_META_API()

	return this->getClass()->castToBase(self, baseIndex);

	LEAVE_META_API(return nullptr)
}

void * G_API_CC ImplMetaClass::castFromDerived(const void * derived, uint32_t derivedIndex)
{
	ENTER_META_API()

	return this->getClass()->castFromDerived(derived, derivedIndex);

	LEAVE_META_API(return nullptr)
}

void * G_API_CC ImplMetaClass::castToDerived(const void * self, uint32_t derivedIndex)
{
	ENTER_META_API()

	return this->getClass()->castToDerived(self, derivedIndex);

	LEAVE_META_API(return nullptr)
}



ImplMetaModule::ImplMetaModule(GMetaModule * module, GMetaClass * metaClass)
	: module(module), metaClass(metaClass)
{
	if(this->metaClass != nullptr) {
		this->metaClass->setModule(this->module);
	}
}

ImplMetaModule::~ImplMetaModule()
{
}

IMetaClass * G_API_CC ImplMetaModule::getGlobalMetaClass()
{
	ENTER_META_API()

	return new ImplMetaClass(this->metaClass, false);

	LEAVE_META_API(return nullptr)
}

IMetaTypedItem * G_API_CC ImplMetaModule::findTypedItemByName(const char * name)
{
	ENTER_META_API()

	const GMetaTypedItem * typedItem = this->metaClass->getModule()->findItemByName(name);

	return static_cast<IMetaTypedItem *>(metaItemToInterface(typedItem));

	LEAVE_META_API(return nullptr)
}

IMetaClass * G_API_CC ImplMetaModule::findClassByName(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaClass>(this->metaClass->getModule()->findClassByName(name));

	LEAVE_META_API(return nullptr)
}



ImplMetaService::ImplMetaService()
{
}

ImplMetaService::~ImplMetaService()
{
	this->clear();
}

void G_API_CC ImplMetaService::addModule(IMetaModule * module)
{
	this->moduleList.push_back(ItemType(module));
}

uint32_t G_API_CC ImplMetaService::getModuleCount()
{
	return static_cast<uint32_t>(this->moduleList.size());
}

IMetaModule * G_API_CC ImplMetaService::getModuleAt(uint32_t index)
{
	IMetaModule * module = this->moduleList.at(index).get();
	module->addReference();
	return module;
}

void ImplMetaService::clear()
{
	this->moduleList.clear();
}

IMemoryAllocator * G_API_CC ImplMetaService::getAllocator()
{
	if(!this->allocator) {
		this->allocator.reset(new GImplMemoryAllocator);
	}
	
	this->allocator->addReference();

	return this->allocator.get();
}

IMetaTypedItem * G_API_CC ImplMetaService::findTypedItemByName(const char * name)
{
	ENTER_META_API()

	if(name == nullptr) {
		return nullptr;
	}

	for(ListType::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		GScopedInterface<IMetaTypedItem> item((*it)->findTypedItemByName(name));
		if(item) {
			return item.take();
		}
	}

	return nullptr;

	LEAVE_META_API(return nullptr)
}

IMetaClass * G_API_CC ImplMetaService::findClassByName(const char * name)
{
	ENTER_META_API()

	if(name == nullptr) {
		return nullptr;
	}

	for(ListType::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		GScopedInterface<IMetaClass> item((*it)->findClassByName(name));
		if(item) {
			return item.take();
		}
	}

	return nullptr;

	LEAVE_META_API(return nullptr)
}


IMetaModule * createMetaModule(GMetaModule * module, GMetaClass * metaClass)
{
	return new ImplMetaModule(module, metaClass);
}

IMetaService * createMetaService(IMetaModule * primaryModule)
{
	IMetaService * service = new ImplMetaService;
	service->addModule(primaryModule);
	return service;
}

IMetaService * createDefaultMetaService()
{
	GScopedInterface<IMetaModule> module(new ImplMetaModule(getGlobalMetaClass()->getModule(), getGlobalMetaClass()));
	return createMetaService(module.get());
}


IMetaList * createMetaList()
{
	return new ImplMetaList;
}

IMetaItem * metaItemToInterface(const GMetaItem * item, bool freeItem)
{
	if(item == nullptr) {
		return nullptr;
	}

	switch(item->getCategory()) {
		case mcatFundamental:
			return new ImplMetaFundamental(static_cast<const GMetaFundamental *>(item), freeItem);

		case mcatField:
			return new ImplMetaField(static_cast<const GMetaField *>(item), freeItem);

		case mcatProperty:
			return new ImplMetaProperty(static_cast<const GMetaProperty *>(item), freeItem);

		case mcatMethod:
			return new ImplMetaMethod(static_cast<const GMetaMethod *>(item), freeItem);

		case mcatEnum:
			return new ImplMetaEnum(static_cast<const GMetaEnum *>(item), freeItem);

		case mcatOperator:
			return new ImplMetaOperator(static_cast<const GMetaOperator *>(item), freeItem);

		case mcatConstructor:
			return new ImplMetaConstructor(static_cast<const GMetaConstructor *>(item), freeItem);

		case mcatClass:
			return new ImplMetaClass(static_cast<const GMetaClass *>(item), freeItem);

		case mcatAnnotation:
			return new ImplMetaAnnotation(static_cast<const GMetaAnnotation *>(item), freeItem);

		default:
			break;

	}

	return nullptr;
}

IMetaItem * metaItemToInterface(const GMetaItem * item)
{
	return metaItemToInterface(item, false);
}



} // namespace cpgf



