#ifndef __GMETACLASS_H
#define __GMETACLASS_H

#include "cpgf/private/gmetaclass_p.h"

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetapolicy.h"

#include "cpgf/gmetaapi.h"
#include "cpgf/gflags.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif



namespace cpgf {


GMetaClass * getGlobalMetaClass();

class GMetaClassImplement;

GMAKE_FINAL(GMetaClass)

class GMetaClass : public GMetaTypedItem, GFINAL_BASE(GMetaClass)
{
private:
	typedef GMetaTypedItem super;

public:
	template <typename ClassT, typename Policy>
	GMetaClass(ClassT *, meta_internal::GMetaSuperList * superList, const char * name, void (*destroy)(void *), void (*metaRegister)(GMetaClass *), const Policy &)
		:	super(name, createMetaType<ClassT>(), mcatClass),
			intialized(false),
			destroy(destroy),
			metaRegister(metaRegister),
			superList(superList),
			baseData(new meta_internal::GMetaClassData<ClassT, Policy>()) {

		GASSERT_STATIC(IsClass<ClassT>::Result || (IsSameType<ClassT, void>::Result) || IsFundamental<ClassT>::Result);

		this->addModifier(metaModifierStatic);

		this->intializeImplement();
		this->setupItemLists();
	}

	virtual ~GMetaClass();

public:
	virtual void * createInstance() const;
	virtual void * createInplace(void * placement) const;
	virtual void * cloneInstance(void * instance) const;
	virtual void * cloneInplace(void * instance, void * placement) const;

	virtual void destroyInstance(void * instance) const;

	virtual size_t getTypeSize() const;

	template <typename OT, typename Signature, typename Policy>
	GMetaConstructor * addConstructor(const Policy & policy) {
		if(this->isGlobal()) {
			return NULL;
		}
		GMetaConstructor * metaConstructor = new GMetaConstructor(
				GCallback<Signature>(
					meta_internal::GMetaConstructorInvoker<GFunctionTraits<Signature>::Arity, OT, typename GFunctionTraits<Signature>::ArgList>()
				),
				policy
			);
		this->addItem(mcatConstructor, metaConstructor);

		return metaConstructor;
	}

	const GMetaConstructor * getConstructorByParamCount(size_t paramCount) const;
	size_t getConstructorCount() const;
	const GMetaConstructor * getConstructorAt(size_t index) const;

	const GMetaField * getFieldInHierarchy(const char * name, void ** outInstance) const;
	const GMetaField * getField(const char * name) const;
	size_t getFieldCount() const;
	const GMetaField * getFieldAt(size_t index) const;

	const GMetaProperty * getPropertyInHierarchy(const char * name, void ** outInstance) const;
	const GMetaProperty * getProperty(const char * name) const;
	size_t getPropertyCount() const;
	const GMetaProperty * getPropertyAt(size_t index) const;

	const GMetaMethod * getMethodInHierarchy(const char * name, void ** outInstance) const;
	const GMetaMethod * getMethod(const char * name) const;
	size_t getMethodCount() const;
	const GMetaMethod * getMethodAt(size_t index) const;

	const GMetaOperator * getOperatorInHierarchy(GMetaOpType op, void ** outInstance) const;
	const GMetaOperator * getOperator(GMetaOpType op) const;
	size_t getOperatorCount() const;
	const GMetaOperator * getOperatorAt(size_t index) const;

	const GMetaEnum * getEnumInHierarchy(const char * name, void ** outInstance) const;
	const GMetaEnum * getEnum(const char * name) const;
	size_t getEnumCount() const;
	const GMetaEnum * getEnumAt(size_t index) const;

	const GMetaClass * getClassInHierarchy(const char * name, void ** outInstance) const;
	const GMetaClass * getClass(const char * name) const;
	size_t getClassCount() const;
	const GMetaClass * getClassAt(size_t index) const;

	size_t getMetaCount() const;
	const GMetaItem * getMetaAt(size_t index) const;

public:
	bool isGlobal() const;
	bool isAbstract() const;

	bool canCreateInstance() const;
	bool canCopyInstance() const;

	bool isSameClass(const GMetaClass * other) const;

	const GMetaClass * getBaseClass(size_t baseIndex) const;
	size_t getBaseCount() const;

	bool isInheritedFrom(const GMetaClass * ancient) const;

	// return a pointer points to this class
	void * castFromBase(void * base, size_t baseIndex) const;

	// return a pointer points to base class
	void * castToBase(void * self, size_t baseIndex) const;

public:
	GMetaField * addField(GMetaField * field);
	GMetaProperty * addProperty(GMetaProperty * prop);
	GMetaMethod * addMethod(GMetaMethod * method);
	GMetaOperator * addOperator(GMetaOperator * metaOperator);
	GMetaEnum & addEnum(GMetaEnum * en);
	GMetaClass * addClass(const GMetaClass * cls);

	template <typename ClassType, typename BaseType>
	void addBaseClass() {
		this->superList->add<ClassType, BaseType>();
	}

private:
	void addItem(GMetaCategory listIndex, GMetaItem * item);
	size_t getItemCount(GMetaCategory listIndex) const;
	const GMetaItem * getItemAt(GMetaCategory listIndex, size_t index) const;
	const GMetaItem * getItemByName(GMetaCategory listIndex, const char * name, bool findSuper, void ** outInstance) const;

public:
	// internal use
	void rebindName(const char * name);

private:
	void ensureRegistered() const;
	void setupItemLists();

	const GMetaOperator * doGetOperator(GMetaOpType op, bool findSuper, void ** outInstance) const;

	void intializeImplement();

private:
	mutable bool intialized;
	void (*destroy)(void *);
	void (*metaRegister)(GMetaClass * metaClass);

	GScopedPointer<meta_internal::GMetaSuperList> superList;
	GScopedPointer<meta_internal::GMetaClassDataBase> baseData;
	
	GMetaClassImplement * implement;
};



namespace meta_internal {

template <typename cls, bool inCls>
struct GGetMetaClass
{
	static const GMetaClass * get() {
		return cls::getMetaClass();
	}
};

template <typename cls>
struct GGetMetaClass <cls, false>
{
	static const GMetaClass * get() {
		return NULL;
	}
};


struct GMetaClassRegister
{
	GMetaClassRegister(const GMetaClass * metaClass) {
		getGlobalMetaClass()->addClass(metaClass);
	}
};


} // namespace meta_internal


template <typename ClassType>
inline const GMetaClass * findMetaClass()
{
	const GMetaClass * metaClass = meta_internal::GGetMetaClass<ClassType, cpgf::meta_internal::GCheckExistMetaClass<ClassType>::Result>::get();

	if(metaClass == NULL) {
		metaClass = findMetaClass(createMetaType<ClassType>());
	}

	return metaClass;
}

const GMetaClass * findMetaClass(const GMetaType & type);
const GMetaClass * findMetaClass(const char * name);


} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
