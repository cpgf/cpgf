#ifndef CPGF_GMETACLASS_H
#define CPGF_GMETACLASS_H

#include "cpgf/private/gmetaclass_p.h"

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetapolicy.h"

#include "cpgf/gmetaapi.h"

#include <vector>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif



namespace cpgf {


class GMetaModule;

class GMetaClassImplement;

GMAKE_FINAL(GMetaClass)

class GMetaClass : public GMetaTypedItem, GFINAL_BASE(GMetaClass)
{
private:
	typedef GMetaTypedItem super;
	
	typedef std::vector<const GMetaClass *> DerivedListType;

public:
	template <typename ClassT, typename Policy>
	GMetaClass(ClassT *, meta_internal::GMetaSuperList * superList, const char * name, void (*metaRegister)(GMetaClass *), const Policy &)
		:	super(name, createMetaType<ClassT>(), mcatClass),
			intialized(false),
			metaRegister(metaRegister),
			superList(superList),
			baseData(new meta_internal::GMetaClassData<ClassT, Policy>()) {

		GASSERT_STATIC(IsClass<ClassT>::Result || (IsSameType<ClassT, void>::Result) || IsFundamental<ClassT>::Result);

		this->initialize();
	}

	virtual ~GMetaClass();

public:
	virtual void * createInstance() const;
	virtual void * createInplace(void * placement) const;
	virtual void * cloneInstance(const void * instance) const;
	virtual void * cloneInplace(const void * instance, void * placement) const;

	virtual void destroyInstance(void * instance) const;
	virtual void destroyInplace(void * instance) const;

	virtual size_t getTypeSize() const;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const;

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

	void setModule(GMetaModule * module) const;
	GMetaModule * getModule() const;

public:
	bool isGlobal() const;
	bool isAbstract() const;
	bool isPolymorphic() const;

	bool canCreateInstance() const;
	bool canCopyInstance() const;

	bool equals(const GMetaClass * other) const;

	const GMetaClass * getBaseClass(size_t baseIndex) const;
	size_t getBaseCount() const;

	const GMetaClass * getDerivedClass(size_t derivedIndex) const;
	size_t getDerivedCount() const;

	bool isInheritedFrom(const GMetaClass * ancient) const;

	// return a pointer points to this class
	void * castFromBase(const void * base, size_t baseIndex) const;

	// return a pointer points to base class
	void * castToBase(const void * self, size_t baseIndex) const;

	// return a pointer points to this class
	void * castFromDerived(const void * derived, size_t derivedIndex) const;

	// return a pointer points to derived class
	void * castToDerived(const void * self, size_t derivedIndex) const;

public:
	template <typename ClassType, typename BaseType>
	void addBaseClass() {
		GMetaClass * baseClass = const_cast<GMetaClass *>(this->superList->add<ClassType, BaseType>()->getBaseClass());
		if(baseClass != nullptr) {
			baseClass->addDerivedClass(this);
		}
	}
	
	void addDerivedClass(const GMetaClass * derived);

	GMetaConstructor * addConstructor(GMetaConstructor * constructor);
	GMetaField * addField(GMetaField * field);
	GMetaProperty * addProperty(GMetaProperty * prop);
	GMetaMethod * addMethod(GMetaMethod * method);
	GMetaOperator * addOperator(GMetaOperator * metaOperator);
	GMetaEnum * addEnum(GMetaEnum * en);
	GMetaClass * addClass(const GMetaClass * cls);

	void extractTo(GMetaClass * master);

	const GMetaClass * doGetClass(const char * name) const;

private:
	void addItem(GMetaCategory listIndex, GMetaItem * item);
	size_t getItemCount(GMetaCategory listIndex) const;
	size_t doGetItemCount(GMetaCategory listIndex) const; // not trigger module initialization
	const GMetaItem * getItemAt(GMetaCategory listIndex, size_t index) const;
	const GMetaItem * doGetItemAt(GMetaCategory listIndex, size_t index) const; // not trigger module initialization
	const GMetaItem * getItemByName(GMetaCategory listIndex, const char * name, bool findSuper, void ** outInstance) const;
	const GMetaItem * doGetItemByName(GMetaCategory listIndex, const char * name, bool findSuper, void ** outInstance) const;  // not trigger module initialization

private:
	void ensureRegistered() const;
	void setupItemLists();

	const GMetaOperator * doGetOperator(GMetaOpType op, bool findSuper, void ** outInstance) const;

	void initialize();

private:
	void rebindName(const char * name);
	bool fixupHierarchy();

private:
	mutable bool intialized;
	void (*metaRegister)(GMetaClass * metaClass);

	GScopedPointer<meta_internal::GMetaSuperList> superList;
	
	GScopedPointer<meta_internal::GMetaClassDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaClassDataBase> >
		baseData;
		
	GScopedPointer<DerivedListType> derivedList;

	mutable GMetaModule * module;
	
	GMetaClassImplement * implement;

private:
	friend class GDefineMetaGlobal;
	friend class GMetaModule;
};


const GMetaClass * findMetaClass(const GMetaType & type);
const GMetaClass * findMetaClass(const char * name);

GMetaClass * getGlobalMetaClass();


} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
