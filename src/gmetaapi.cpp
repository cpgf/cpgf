#include "cpgf/gmetaclass.h"
#include "cpgf/gmetafundamental.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gassert.h"

#include "pinclude/gmetatypereg.h"
#include "pinclude/gapiimpl.h"

#include <string>


#if defined(_MSC_VER)
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif



#define ENTER_META_API() \
	this->clearError(); \
	try {

#define LEAVE_META_API(...) \
	} \
	catch(const GException & e) { this->handleError(e.getCode(), e.getMessage()); __VA_ARGS__; }
	

#define IMPL_ITEM \
protected: \
	virtual const char * G_API_CC getName() { return this->doGetName(); } \
	virtual const char * G_API_CC getQualifiedName() { return this->doGetQualifiedName(); } \
	virtual IMetaItem * G_API_CC getOwnerItem() { return this->doGetOwnerItem(); } \
	virtual void G_API_CC getItemType(GMetaTypeData * outType) { this->doGetItemType(outType); } \
	virtual uint32_t G_API_CC getCategory() { return this->doGetCategory(); } \
	virtual IMetaAnnotation * G_API_CC getAnnotation(const char * name) { return this->doGetAnnotation(name); } \
	virtual uint32_t G_API_CC getAnnotationCount() { return this->doGetAnnotationCount(); } \
	virtual IMetaAnnotation * G_API_CC getAnnotationAt(uint32_t index) { return this->doGetAnnotationAt(index); } \
	virtual gapi_bool G_API_CC equals(IMetaItem * other) { return this->doEquals(other); } \
	virtual gapi_bool G_API_CC isStatic() { return this->doIsStatic(); } \
	const GMetaItem * getItem() const {	return this->doGetItem(); }

#define IMPL_ALL \
	IMPL_OBJECT \
	IMPL_EXTENDOBJECT \
	IMPL_ITEM

#define IMPL_TYPEDITEM \
protected: \
	virtual void G_API_CC getMetaType(GMetaTypeData * outType) { this->doGetMetaType(outType); } \
	virtual uint32_t G_API_CC getTypeSize() { return this->doGetTypeSize(); } \
	virtual const char * G_API_CC getTypeName() { return this->doGetTypeName(); } \
	virtual void * G_API_CC createInstance() { return this->doCreateInstance(); } \
	virtual void * G_API_CC createInplace(void * placement) { return this->doCreateInplace(placement); } \
	virtual void * G_API_CC cloneInstance(void * instance) { return this->doCloneInstance(instance); } \
	virtual void * G_API_CC cloneInplace(void * instance, void * placement) { return this->doCloneInplace(instance, placement); } \
	virtual void G_API_CC destroyInstance(void * instance) { this->doDestroyInstance(instance); }

#define IMPL_CALLABLE \
protected: \
	virtual void G_API_CC getParamType(GMetaTypeData * outType, uint32_t index) { this->doGetParamType(outType, index); } \
	virtual uint32_t G_API_CC getParamCount() { return this->doGetParamCount(); } \
	virtual gapi_bool G_API_CC hasResult() { return this->doHasResult(); } \
	virtual void G_API_CC getResultType(GMetaTypeData * outType) { this->doGetResultType(outType); } \
	virtual gapi_bool G_API_CC isVariadic() { return this->doIsVariadic(); } \
	virtual gapi_bool G_API_CC checkParam(const GVariantData * param, uint32_t paramIndex) { return this->doCheckParam(param, paramIndex); } \
	virtual gapi_bool G_API_CC isParamTransferOwnership(uint32_t paramIndex) { return this->doIsParamTransferOwnership(paramIndex); } \
	virtual gapi_bool G_API_CC isResultTransferOwnership() { return this->doIsResultTransferOwnership(); } \
	virtual IMetaConverter * G_API_CC createResultConverter() { return this->doCreateResultConverter(); }


#define IMPL_ACCESSIBLE \
protected: \
	virtual gapi_bool G_API_CC canGet() { return this->doCanGet(); } \
	virtual gapi_bool G_API_CC canSet() { return this->doCanSet(); } \
	virtual void G_API_CC get(GVariantData * outResult, void * instance) { this->doGet(outResult, instance); } \
	virtual void G_API_CC set(void * instance, const GVariantData * value) { this->doSet(instance, value); } \
	virtual uint32_t G_API_CC getSize() { return this->doGetSize(); } \
	virtual IMetaConverter * G_API_CC createConverter() { return this->doCreateConverter(); }

namespace cpgf {


class ImplMetaItem : public ImplExtendObject
{
public:
	ImplMetaItem(const GMetaItem * item);

public:
	const GMetaItem * doGetItem() const {
		return this->item;
	}

protected:
	const char * doGetName();
	const char * doGetQualifiedName();
	IMetaItem * doGetOwnerItem();
	void doGetItemType(GMetaTypeData * outType);
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
};


class ImplMetaTypedItem : public ImplMetaItem
{
private:
	typedef ImplMetaItem super;

public:
	ImplMetaTypedItem(const GMetaTypedItem * typedItem);

protected:
	void doGetMetaType(GMetaTypeData * outType);
	uint32_t doGetTypeSize();

	const char * doGetTypeName();

	void * doCreateInstance();
	void * doCreateInplace(void * placement);
	void * doCloneInstance(void * instance);
	void * doCloneInplace(void * instance, void * placement);

	void doDestroyInstance(void * instance);

private:
	const GMetaTypedItem * getTypedItem() const {
		return static_cast<const GMetaTypedItem *>(this->doGetItem());
	}
};


class ImplMetaList : public ImplExtendObject, public IMetaList
{
private:
	typedef std::vector<IMetaItem *> ListType;

public:
	ImplMetaList();
	ImplMetaList(GMetaList * metaList);
	virtual ~ImplMetaList();
	
	void load(GMetaList * metaList);

	IMPL_OBJECT
	IMPL_EXTENDOBJECT

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


class ImplMetaConverter : public ImplExtendObject, public IMetaConverter
{
public:
	ImplMetaConverter(GMetaConverter * metaConverter);
	virtual ~ImplMetaConverter();

	IMPL_OBJECT
	IMPL_EXTENDOBJECT

protected:
	virtual gapi_bool G_API_CC canToCString();
	virtual const char * G_API_CC toCString(const void * instance, gapi_bool * needFree, IMemoryAllocator * allocator);

private:
	GScopedPointer<GMetaConverter> metaConverter;
};


class ImplMetaCallable : public ImplMetaItem
{
private:
	typedef ImplMetaItem super;

public:
	ImplMetaCallable(const GMetaCallable * callable);

protected:
	void doGetParamType(GMetaTypeData * outType, uint32_t index);
	uint32_t doGetParamCount();
	gapi_bool doHasResult();
	void doGetResultType(GMetaTypeData * outType);
	gapi_bool doIsVariadic();
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
	ImplMetaAccessible(const GMetaAccessible * accessible);

protected:
	gapi_bool doCanGet();
	gapi_bool doCanSet();
	void doGet(GVariantData * outResult, void * instance);
	void doSet(void * instance, const GVariantData * value);
	uint32_t doGetSize();
	IMetaConverter * doCreateConverter();

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
	ImplMetaField(const GMetaField * field);

	IMPL_ALL

	IMPL_ACCESSIBLE

protected:
	virtual void * G_API_CC getAddress(void * instance);

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
	ImplMetaProperty(const GMetaProperty * prop);

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
	ImplMetaMethod(const GMetaMethod * method);

	IMPL_ALL

	IMPL_CALLABLE

protected:
	virtual void G_API_CC execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount);
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
	ImplMetaConstructor(const GMetaConstructor * constructor);

protected:
	virtual void G_API_CC execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount);
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
	ImplMetaOperator(const GMetaOperator * op);

protected:
	virtual int32_t G_API_CC getOperator();
	virtual void G_API_CC execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount);
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
	ImplMetaFundamental(const GMetaFundamental * fundamental);

protected:
	virtual void G_API_CC getValue(GVariantData * outResult, void * instance);

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
	ImplMetaEnum(const GMetaEnum * en);

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


class ImplMetaAnnotationValue : public ImplExtendObject, public IMetaAnnotationValue
{
private:
	typedef ImplExtendObject super;

public:
	ImplMetaAnnotationValue(const GAnnotationValue * value);

	IMPL_OBJECT
	IMPL_EXTENDOBJECT

protected:
	virtual void G_API_CC getVariant(GVariantData * outVariant);
	virtual gapi_bool G_API_CC canToString();
	virtual gapi_bool G_API_CC canToWideString();
	virtual gapi_bool G_API_CC canToInt();
	virtual const char * G_API_CC toString();
	virtual const wchar_t * G_API_CC toWideString();
	virtual int32_t G_API_CC toInt32();

private:
	const GAnnotationValue * value;
};


class ImplMetaAnnotation : public ImplMetaItem, public IMetaAnnotation
{
private:
	typedef ImplMetaItem super;

public:
	ImplMetaAnnotation(const GMetaAnnotation * annotation);

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
	ImplMetaClass(const GMetaClass * cls);

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
	virtual void G_API_CC getMethodList(IMetaList * methodList, const char * name, uint32_t filters);
	virtual void G_API_CC getMethodListInHierarchy(IMetaList * methodList, const char * name, uint32_t filters, void * instance);

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
	virtual gapi_bool G_API_CC canCreateInstance();
	virtual gapi_bool G_API_CC canCopyInstance();

	virtual IMetaClass * G_API_CC getBaseClass(uint32_t baseIndex);
	virtual uint32_t G_API_CC getBaseCount();

	virtual gapi_bool G_API_CC isInheritedFrom(IMetaClass * ancient);

	virtual void * G_API_CC castFromBase(void * base, uint32_t baseIndex);
	virtual void * G_API_CC castToBase(void * self, uint32_t baseIndex);

private:
	const GMetaClass * getClass() const {
		return static_cast<const GMetaClass *>(this->doGetItem());
	}
};



class ImplMetaModule : public ImplExtendObject, public IMetaModule
{
public:
	ImplMetaModule();
	~ImplMetaModule();

	IMPL_OBJECT
	IMPL_EXTENDOBJECT

protected:
	virtual IMetaClass * G_API_CC getGlobalMetaClass();
	
	virtual IMetaTypedItem * G_API_CC findTypedItemByName(const char * name);
	virtual IMetaFundamental * G_API_CC findFundamental(GVariantType vt);
	virtual IMetaClass * G_API_CC findClassByName(const char * name);
};



class ImplMetaService : public ImplExtendObject, public IMetaService
{
private:
	typedef std::vector<IMetaModule *> ListType;
public:
	ImplMetaService();
	~ImplMetaService();

	IMPL_OBJECT
	IMPL_EXTENDOBJECT

protected:
	virtual void G_API_CC addModule(IMetaModule * module);
	virtual uint32_t G_API_CC getModuleCount();
	virtual IMetaModule * G_API_CC getModuleAt(uint32_t index);

	virtual IMetaList * G_API_CC createMetaList();

	virtual IMemoryAllocator * G_API_CC getAllocator();

	virtual IMetaTypedItem * G_API_CC findTypedItemByName(const char * name);
	virtual IMetaFundamental * G_API_CC findFundamental(GVariantType vt);
	virtual IMetaClass * G_API_CC findClassByName(const char * name);

private:
	void clear();

private:
	ListType moduleList;
	GScopedInterface<IMemoryAllocator> allocator;
};



IMetaItem * createMetaItem(const GMetaItem * item)
{
	if(item == NULL) {
		return NULL;
	}

	switch(item->getCategory()) {
		case mcatFundamental:
			return new ImplMetaFundamental(static_cast<const GMetaFundamental *>(item));

		case mcatField:
			return new ImplMetaField(static_cast<const GMetaField *>(item));

		case mcatProperty:
			return new ImplMetaProperty(static_cast<const GMetaProperty *>(item));

		case mcatMethod:
			return new ImplMetaMethod(static_cast<const GMetaMethod *>(item));

		case mcatEnum:
			return new ImplMetaEnum(static_cast<const GMetaEnum *>(item));

		case mcatOperator:
			return new ImplMetaOperator(static_cast<const GMetaOperator *>(item));

		case mcatConstructor:
			return new ImplMetaConstructor(static_cast<const GMetaConstructor *>(item));

		case mcatClass:
			return new ImplMetaClass(static_cast<const GMetaClass *>(item));

		case mcatAnnotation:
			return new ImplMetaAnnotation(static_cast<const GMetaAnnotation *>(item));

		default:
			break;

	}

	return NULL;
}

template <typename T, typename P>
T * doCreateItem(P * p)
{
	if(p) {
		return new T(p);
	}
	else {
		return NULL;
	}
}

void loadMetaList(IMetaList * metaList, GMetaList * rawMetaList)
{
	size_t count = rawMetaList->getCount();
	for(size_t i = 0; i < count; ++i) {
		IMetaItem * item = createMetaItem(rawMetaList->getAt(i));
		metaList->add(item, rawMetaList->getInstanceAt(i));
		item->releaseReference();
	}
}


ImplMetaItem::ImplMetaItem(const GMetaItem * item)
	: item(item)
{
}

const char * ImplMetaItem::doGetName()
{
	ENTER_META_API()

	return this->item->getName().c_str();

	LEAVE_META_API(return NULL)
}

const char * ImplMetaItem::doGetQualifiedName()
{
	ENTER_META_API()

	return this->item->getQualifiedName().c_str();

	LEAVE_META_API(return NULL)
}

IMetaItem * ImplMetaItem::doGetOwnerItem()
{
	ENTER_META_API()

	return createMetaItem(this->item->getOwnerItem());

	LEAVE_META_API(return NULL)
}

void ImplMetaItem::doGetItemType(GMetaTypeData * outType)
{
	ENTER_META_API()
	
	initializeMetaType(outType);

	GMetaType type = this->item->getItemType();

	fixupMetaType(&type);

	*outType = type.getData();

	LEAVE_META_API()
}

uint32_t ImplMetaItem::doGetCategory()
{
	ENTER_META_API()

	return this->item->getCategory();

	LEAVE_META_API(return 0)
}

IMetaAnnotation * ImplMetaItem::doGetAnnotation(const char * name)
{
	ENTER_META_API()

	return new ImplMetaAnnotation(this->doGetItem()->getAnnotation(name));

	LEAVE_META_API(return NULL)
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

	LEAVE_META_API(return NULL)
}

gapi_bool ImplMetaItem::doEquals(IMetaItem * other)
{
	ENTER_META_API()

	return strcmp(this->doGetQualifiedName(), other->getQualifiedName()) == 0;

	LEAVE_META_API(return false)
}

gapi_bool ImplMetaItem::doIsStatic()
{
	ENTER_META_API()

	return this->doGetItem()->isStatic();

	LEAVE_META_API(return false)
}



ImplMetaTypedItem::ImplMetaTypedItem(const GMetaTypedItem * typedItem)
	: super(typedItem)
{
}

void ImplMetaTypedItem::doGetMetaType(GMetaTypeData * outType)
{
	ENTER_META_API()

	initializeMetaType(outType);

	GMetaType type = this->getTypedItem()->getMetaType();
	
	fixupMetaType(&type);

	*outType = type.getData();

	LEAVE_META_API()
}

uint32_t ImplMetaTypedItem::doGetTypeSize()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getTypedItem()->getTypeSize());

	LEAVE_META_API(return 0)
}

const char * ImplMetaTypedItem::doGetTypeName()
{
	ENTER_META_API()

	return this->getTypedItem()->getTypeName().c_str();

	LEAVE_META_API(return NULL)
}

void * ImplMetaTypedItem::doCreateInstance()
{
	ENTER_META_API()

	return this->getTypedItem()->createInstance();

	LEAVE_META_API(return NULL)
}

void * ImplMetaTypedItem::doCreateInplace(void * placement)
{
	ENTER_META_API()

	return this->getTypedItem()->createInplace(placement);

	LEAVE_META_API(return NULL)
}

void * ImplMetaTypedItem::doCloneInstance(void * instance)
{
	ENTER_META_API()

	return this->getTypedItem()->cloneInstance(instance);

	LEAVE_META_API(return NULL)
}

void * ImplMetaTypedItem::doCloneInplace(void * instance, void * placement)
{
	ENTER_META_API()

	return this->getTypedItem()->cloneInplace(instance, placement);

	LEAVE_META_API(return NULL)
}

void ImplMetaTypedItem::doDestroyInstance(void * instance)
{
	ENTER_META_API()

	this->getTypedItem()->destroyInstance(instance);

	LEAVE_META_API()
}



ImplMetaCallable::ImplMetaCallable(const GMetaCallable * callable)
	: super(callable)
{
}

void ImplMetaCallable::doGetParamType(GMetaTypeData * outType, uint32_t index)
{
	ENTER_META_API()

	initializeMetaType(outType);

	GMetaType type = this->getCallable()->getParamType(index);

	fixupMetaType(&type);

	*outType = type.getData();

	LEAVE_META_API()
}

uint32_t ImplMetaCallable::doGetParamCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getCallable()->getParamCount());

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

	fixupMetaType(&type);

	*outType = type.getData();

	LEAVE_META_API()
}

gapi_bool ImplMetaCallable::doIsVariadic()
{
	ENTER_META_API()

	return this->getCallable()->isVariadic();

	LEAVE_META_API(return false)
}

gapi_bool ImplMetaCallable::doCheckParam(const GVariantData * param, uint32_t paramIndex)
{
	ENTER_META_API()

	return this->getCallable()->checkParam(GVariant(*param), paramIndex);

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

IMetaConverter * ImplMetaCallable::doCreateResultConverter()
{
	ENTER_META_API()

	return doCreateItem<ImplMetaConverter>(this->getCallable()->createResultConverter());

	LEAVE_META_API(return NULL)
}


ImplMetaAccessible::ImplMetaAccessible(const GMetaAccessible * accessible)
	: super(accessible)
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

void ImplMetaAccessible::doGet(GVariantData * outResult, void * instance)
{
	ENTER_META_API()
	
	initializeVarData(outResult);

	*outResult = this->getAccessible()->get(instance).takeData();

	LEAVE_META_API()
}

void ImplMetaAccessible::doSet(void * instance, const GVariantData * value)
{
	ENTER_META_API()

	this->getAccessible()->set(instance, GVariant(*value));

	LEAVE_META_API()
}

uint32_t ImplMetaAccessible::doGetSize()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getAccessible()->getSize());

	LEAVE_META_API(return 0)
}

IMetaConverter * ImplMetaAccessible::doCreateConverter()
{
	ENTER_META_API()

	return doCreateItem<ImplMetaConverter>(this->getAccessible()->createConverter());

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
	this->metaList.push_back(item);
	this->instanceList.push_back(instance);
	item->addReference();
}

uint32_t G_API_CC ImplMetaList::getCount()
{
	return static_cast<uint32_t>(this->metaList.size());
}

IMetaItem * G_API_CC ImplMetaList::getAt(uint32_t index)
{
	IMetaItem * item = this->metaList.at(index);
	item->addReference();
	return item;
}

void * G_API_CC ImplMetaList::getInstanceAt(uint32_t index)
{
	return this->instanceList.at(index);
}

void G_API_CC ImplMetaList::clear()
{
	for(ListType::iterator it = this->metaList.begin(); it != this->metaList.end(); ++it) {
		(*it)->releaseReference();
	}

	this->metaList.clear();
	this->instanceList.clear();
}


ImplMetaConverter::ImplMetaConverter(GMetaConverter * metaConverter)
	: metaConverter(metaConverter)
{
}

ImplMetaConverter::~ImplMetaConverter()
{
}

gapi_bool G_API_CC ImplMetaConverter::canToCString()
{
	return this->metaConverter->canToCString();
}

const char * G_API_CC ImplMetaConverter::toCString(const void * instance, gapi_bool * needFree, IMemoryAllocator * allocator)
{
	int free;
	
	const char * s = this->metaConverter->toCString(instance, &free, allocator);
	
	*needFree = free;

	return s;
}

ImplMetaField::ImplMetaField(const GMetaField * field)
	: super(field)
{
}

void * G_API_CC ImplMetaField::getAddress(void * instance)
{
	ENTER_META_API()

	return this->getField()->getAddress(instance);

	LEAVE_META_API(return NULL)
}



ImplMetaProperty::ImplMetaProperty(const GMetaProperty * prop)
	: super(prop)
{
}


ImplMetaMethod::ImplMetaMethod(const GMetaMethod * method)
	: super(method)
{
}

void G_API_CC ImplMetaMethod::execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount)
{
	this->invoke(outResult, instance, params, paramCount);
}

void G_API_CC ImplMetaMethod::invoke(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount)
{
	GASSERT(paramCount <= REF_MAX_ARITY);

	ENTER_META_API()

	GVariant variants[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		variants[i] = GVariant(params[i]);
	}

	if(outResult == NULL) {
		this->getMethod()->execute(instance, variants, paramCount);
	}
	else {
		*outResult = this->getMethod()->execute(instance, variants, paramCount).takeData();
	}

	LEAVE_META_API()
}

void G_API_CC ImplMetaMethod::invokeIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount)
{
	GASSERT(paramCount <= REF_MAX_ARITY);

	ENTER_META_API()

	GVariant variants[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		variants[i] = GVariant(*params[i]);
	}

	if(outResult == NULL) {
		this->getMethod()->execute(instance, variants, paramCount).takeData();
	}
	else {
		*outResult = this->getMethod()->execute(instance, variants, paramCount).takeData();
	}

	LEAVE_META_API()
}



ImplMetaConstructor::ImplMetaConstructor(const GMetaConstructor * constructor)
	: super(constructor)
{
}

void G_API_CC ImplMetaConstructor::execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount)
{
	(void)instance;
	if(outResult != NULL) {
		void * newObj = this->invoke(params, paramCount);
		*outResult = GVariant(newObj).takeData();
	}
}

void * G_API_CC ImplMetaConstructor::invoke(const GVariantData * params, uint32_t paramCount)
{
	GASSERT(paramCount <= REF_MAX_ARITY);

	ENTER_META_API()

	GVariant variants[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		variants[i] = GVariant(params[i]);
	}

	return fromVariant<void *>(this->getConstructor()->execute(NULL, variants, paramCount));

	LEAVE_META_API(return NULL)
}

void * G_API_CC ImplMetaConstructor::invokeIndirectly(GVariantData const * const * params, uint32_t paramCount)
{
	GASSERT(paramCount <= REF_MAX_ARITY);

	ENTER_META_API()

	GVariant variants[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		variants[i] = GVariant(*params[i]);
	}

	return fromVariant<void *>(this->getConstructor()->execute(NULL, variants, paramCount));

	LEAVE_META_API(return NULL)
}



ImplMetaOperator::ImplMetaOperator(const GMetaOperator * op)
	: super(op)
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

void G_API_CC ImplMetaOperator::invokeUnary(GVariantData * outResult, const GVariantData * p0)
{
	ENTER_META_API()

	if(outResult == NULL) {
		this->getOperatorItem()->invokeUnary(GVariant(*p0));
	}
	else {
		*outResult = this->getOperatorItem()->invokeUnary(GVariant(*p0)).takeData();
	}

	LEAVE_META_API()
}

void G_API_CC ImplMetaOperator::invokeBinary(GVariantData * outResult, const GVariantData * p0, const GVariantData * p1)
{
	ENTER_META_API()

	if(outResult == NULL) {
		this->getOperatorItem()->invokeBinary(GVariant(*p0), GVariant(*p1));
	}
	else {
		*outResult = this->getOperatorItem()->invokeBinary(GVariant(*p0), GVariant(*p1)).takeData();
	}

	LEAVE_META_API()
}

void G_API_CC ImplMetaOperator::invokeFunctor(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount)
{
	GASSERT(paramCount <= REF_MAX_ARITY);

	ENTER_META_API()

	GVariant variants[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		variants[i] = GVariant(params[i]);
	}

	if(outResult == NULL) {
		this->getOperatorItem()->execute(instance, variants, paramCount);
	}
	else {
		*outResult = this->getOperatorItem()->execute(instance, variants, paramCount).takeData();
	}

	LEAVE_META_API()
}

void G_API_CC ImplMetaOperator::invokeFunctorIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount)
{
	GASSERT(paramCount <= REF_MAX_ARITY);

	ENTER_META_API()

	GVariant variants[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		variants[i] = GVariant(*params[i]);
	}

	if(outResult == NULL) {
		this->getOperatorItem()->execute(instance, variants, paramCount);
	}
	else {
		*outResult = this->getOperatorItem()->execute(instance, variants, paramCount).takeData();
	}

	LEAVE_META_API()
}



ImplMetaFundamental::ImplMetaFundamental(const GMetaFundamental * fundamental)
	: super(fundamental)
{
}

void G_API_CC ImplMetaFundamental::getValue(GVariantData * outResult, void * instance)
{
	ENTER_META_API()

	initializeVarData(outResult);

	*outResult = this->getFundamental()->getValue(instance).takeData();

	LEAVE_META_API()
}


ImplMetaEnum::ImplMetaEnum(const GMetaEnum * en)
	: super(en)
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

	LEAVE_META_API(return NULL)
}

void G_API_CC ImplMetaEnum::getValue(GVariantData * outResult, uint32_t index)
{
	ENTER_META_API()

	initializeVarData(outResult);

	*outResult = this->getEnum()->getValue(index).takeData();

	LEAVE_META_API()
}

int32_t G_API_CC ImplMetaEnum::findKey(const char * key)
{
	ENTER_META_API()

	return this->getEnum()->findKey(key);

	LEAVE_META_API(return -1)
}


ImplMetaAnnotationValue::ImplMetaAnnotationValue(const GAnnotationValue * value)
	: value(value)
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

	LEAVE_META_API(return NULL)
}

const wchar_t * G_API_CC ImplMetaAnnotationValue::toWideString()
{
	ENTER_META_API()

	return this->value->toWideString();

	LEAVE_META_API(return NULL)
}

int32_t G_API_CC ImplMetaAnnotationValue::toInt32()
{
	ENTER_META_API()

	return this->value->toInt();

	LEAVE_META_API(return 0)
}


ImplMetaAnnotation::ImplMetaAnnotation(const GMetaAnnotation * annotation)
	: super(annotation)
{
}

IMetaItem * G_API_CC ImplMetaAnnotation::getMetaItem()
{
	ENTER_META_API()

	return createMetaItem(this->getAnnotation()->getMetaItem());

	LEAVE_META_API(return NULL)
}

IMetaAnnotationValue * G_API_CC ImplMetaAnnotation::getValue(const char * name)
{
	ENTER_META_API()

	return new ImplMetaAnnotationValue(this->getAnnotation()->getValue(name));

	LEAVE_META_API(return NULL)
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

	LEAVE_META_API(return NULL)
}

IMetaAnnotationValue * G_API_CC ImplMetaAnnotation::getValueAt(uint32_t index)
{
	ENTER_META_API()

	return new ImplMetaAnnotationValue(this->getAnnotation()->getValueAt(index));

	LEAVE_META_API(return NULL)
}




ImplMetaClass::ImplMetaClass(const GMetaClass * cls)
	: super(cls)
{
}

IMetaConstructor * G_API_CC ImplMetaClass::getConstructorByParamCount(uint32_t paramCount)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaConstructor>(this->getClass()->getConstructorByParamCount(paramCount));

	LEAVE_META_API(return NULL)
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

	LEAVE_META_API(return NULL)
}

IMetaField * G_API_CC ImplMetaClass::getFieldInHierarchy(const char * name, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaField>(this->getClass()->getFieldInHierarchy(name, instance));

	LEAVE_META_API(return NULL)
}

IMetaField * G_API_CC ImplMetaClass::getField(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaField>(this->getClass()->getField(name));

	LEAVE_META_API(return NULL)
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

	LEAVE_META_API(return NULL)
}

IMetaProperty * G_API_CC ImplMetaClass::getPropertyInHierarchy(const char * name, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaProperty>(this->getClass()->getPropertyInHierarchy(name, instance));

	LEAVE_META_API(return NULL)
}

IMetaProperty * G_API_CC ImplMetaClass::getProperty(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaProperty>(this->getClass()->getProperty(name));

	LEAVE_META_API(return NULL)
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

	LEAVE_META_API(return NULL)
}

IMetaMethod * G_API_CC ImplMetaClass::getMethodInHierarchy(const char * name, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaMethod>(this->getClass()->getMethodInHierarchy(name, instance));

	LEAVE_META_API(return NULL)
}

IMetaMethod * G_API_CC ImplMetaClass::getMethod(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaMethod>(this->getClass()->getMethod(name));

	LEAVE_META_API(return NULL)
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

	LEAVE_META_API(return NULL)
}

void G_API_CC ImplMetaClass::getMethodList(IMetaList * methodList, const char * name, uint32_t filters)
{
	GScopedPointer<GMetaList> metaList(new GMetaList);

	this->getClass()->getMethodList(metaList.get(), name, filters);

	loadMetaList(methodList, metaList.get());
}

void G_API_CC ImplMetaClass::getMethodListInHierarchy(IMetaList * methodList, const char * name, uint32_t filters, void * instance)
{
	GScopedPointer<GMetaList> metaList(new GMetaList);

	this->getClass()->getMethodListInHierarchy(metaList.get(), name, filters, instance);

	loadMetaList(methodList, metaList.get());
}

IMetaOperator * G_API_CC ImplMetaClass::getOperatorInHierarchy(uint32_t op, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaOperator>(this->getClass()->getOperatorInHierarchy(static_cast<GMetaOpType>(op), instance));

	LEAVE_META_API(return NULL)
}

IMetaOperator * G_API_CC ImplMetaClass::getOperator(uint32_t op)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaOperator>(this->getClass()->getOperator(static_cast<GMetaOpType>(op)));

	LEAVE_META_API(return NULL)
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

	LEAVE_META_API(return NULL)
}

IMetaEnum * G_API_CC ImplMetaClass::getEnumInHierarchy(const char * name, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaEnum>(this->getClass()->getEnumInHierarchy(name, instance));

	LEAVE_META_API(return NULL)
}

IMetaEnum * G_API_CC ImplMetaClass::getEnum(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaEnum>(this->getClass()->getEnum(name));

	LEAVE_META_API(return NULL)
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

	LEAVE_META_API(return NULL)
}

IMetaClass * G_API_CC ImplMetaClass::getClassInHierarchy(const char * name, void ** instance)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaClass>(this->getClass()->getClassInHierarchy(name, instance));

	LEAVE_META_API(return NULL)
}

IMetaClass * G_API_CC ImplMetaClass::getClass(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaClass>(this->getClass()->getClass(name));

	LEAVE_META_API(return NULL)
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

	LEAVE_META_API(return NULL)
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

	return createMetaItem(this->getClass()->getMetaAt(index));

	LEAVE_META_API(return NULL)
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

	LEAVE_META_API(return NULL)
}

uint32_t G_API_CC ImplMetaClass::getBaseCount()
{
	ENTER_META_API()

	return static_cast<uint32_t>(this->getClass()->getBaseCount());

	LEAVE_META_API(return 0)
}

gapi_bool G_API_CC ImplMetaClass::isInheritedFrom(IMetaClass * ancient)
{
	ENTER_META_API()

	GScopedInterface<IMetaClass> item;

	for(uint32_t i = 0; i < this->getBaseCount(); ++i) {
		item.reset(this->getBaseClass(i));
		if(item && ancient->equals(item.get())) {
			return true;
		}
	}

	return false;

	LEAVE_META_API(return false)
}

void * G_API_CC ImplMetaClass::castFromBase(void * base, uint32_t baseIndex)
{
	ENTER_META_API()

	return this->getClass()->castFromBase(base, baseIndex);

	LEAVE_META_API(return NULL)
}

void * G_API_CC ImplMetaClass::castToBase(void * self, uint32_t baseIndex)
{
	ENTER_META_API()

	return this->getClass()->castToBase(self, baseIndex);

	LEAVE_META_API(return NULL)
}



ImplMetaModule::ImplMetaModule()
{
}

ImplMetaModule::~ImplMetaModule()
{
}

IMetaClass * G_API_CC ImplMetaModule::getGlobalMetaClass()
{
	ENTER_META_API()

	return new ImplMetaClass(cpgf::getGlobalMetaClass());

	LEAVE_META_API(return NULL)
}

IMetaTypedItem * G_API_CC ImplMetaModule::findTypedItemByName(const char * name)
{
	ENTER_META_API()

	const GMetaTypedItem * typedItem = findMetaType(name);

	return static_cast<IMetaTypedItem *>(createMetaItem(typedItem));

	LEAVE_META_API(return NULL)
}

IMetaFundamental * G_API_CC ImplMetaModule::findFundamental(GVariantType vt)
{
	GASSERT_MSG(vtIsFundamental(vt), "Type must be fundamental");

	ENTER_META_API()

	return doCreateItem<ImplMetaFundamental>(meta_internal::findRegisteredMetaFundamental(vt));

	LEAVE_META_API(return NULL)
}

IMetaClass * G_API_CC ImplMetaModule::findClassByName(const char * name)
{
	ENTER_META_API()

	return doCreateItem<ImplMetaClass>(findMetaClass(name));

	LEAVE_META_API(return NULL)
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
	this->moduleList.push_back(module);
	module->addReference();
}

uint32_t G_API_CC ImplMetaService::getModuleCount()
{
	return this->moduleList.size();
}

IMetaModule * G_API_CC ImplMetaService::getModuleAt(uint32_t index)
{
	IMetaModule * module = this->moduleList.at(index);
	module->addReference();
	return module;
}

void ImplMetaService::clear()
{
	for(ListType::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		(*it)->releaseReference();
	}

	this->moduleList.clear();
}

IMetaList * G_API_CC ImplMetaService::createMetaList()
{
	return new ImplMetaList;
}

IMemoryAllocator * G_API_CC ImplMetaService::getAllocator()
{
	if(!this->allocator) {
		this->allocator.reset(new ImplMemoryAllocator);
	}
	
	this->allocator->addReference();

	return this->allocator.get();
}

IMetaTypedItem * G_API_CC ImplMetaService::findTypedItemByName(const char * name)
{
	ENTER_META_API()

	if(name == NULL) {
		return NULL;
	}

	for(ListType::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		GScopedInterface<IMetaTypedItem> item((*it)->findTypedItemByName(name));
		if(item) {
			return item.take();
		}
	}

	return NULL;

	LEAVE_META_API(return NULL)
}

IMetaFundamental * G_API_CC ImplMetaService::findFundamental(GVariantType vt)
{
	GASSERT_MSG(vtIsFundamental(vt), "Type must be fundamental");

	ENTER_META_API()

	for(ListType::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		GScopedInterface<IMetaFundamental> item((*it)->findFundamental(vt));
		if(item) {
			return item.take();
		}
	}

	return NULL;

	LEAVE_META_API(return NULL)
}

IMetaClass * G_API_CC ImplMetaService::findClassByName(const char * name)
{
	ENTER_META_API()

	if(name == NULL) {
		return NULL;
	}

	for(ListType::iterator it = this->moduleList.begin(); it != this->moduleList.end(); ++it) {
		GScopedInterface<IMetaClass> item((*it)->findClassByName(name));
		if(item) {
			return item.take();
		}
	}

	return NULL;

	LEAVE_META_API(return NULL)
}


IMetaModule * getMetaModule()
{
	static GScopedInterface<ImplMetaModule> module;

	if(!module) {
		module.reset(new ImplMetaModule);
	}

	module->addReference();

	return module.get();
}

IMetaService * createMetaService(IMetaModule * primaryModule)
{
	IMetaService * service = new ImplMetaService;
	service->addModule(primaryModule);
	return service;
}

IMetaService * createDefaultMetaService()
{
	GScopedInterface<IMetaModule> module(getMetaModule());
	return createMetaService(module.get());
}



} // namespace cpgf



