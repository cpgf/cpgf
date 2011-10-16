#ifndef __GMETACLASS_H
#define __GMETACLASS_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetaannotation.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetafield.h"
#include "cpgf/gmetamethod.h"
#include "cpgf/gmetaoperator.h"
#include "cpgf/gmetaproperty.h"
#include "cpgf/gmetapolicy.h"

#include "cpgf/gmetaapi.h"
#include "cpgf/gflags.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif


#define I_GMETA_DO_COMMON_CLASS \
	public: \
		static const cpgf::GMetaClass * getMetaClass() { \
			return getMetaClassObject(); \
		} \
	private: \
		static void destroyMetaObject(void * o) { delete static_cast<MetaCurrentClassType *>(o); } \
		template <typename i_Reflect_Signature> static void reflectConstructor() { getMetaClassObject()->template addConstructor<MetaCurrentClassType, i_Reflect_Signature>(cpgf::GMetaPolicyDefault()); } \
		template <typename i_Reflect_Signature, typename i_Reflect_Policy> static void reflectConstructor(const i_Reflect_Policy & policy) { getMetaClassObject()->template addConstructor<MetaCurrentClassType, i_Reflect_Signature>(policy); } \
		template <typename i_Reflect_FT> static void reflectMethod(const char * name, i_Reflect_FT func) { getMetaClassObject()->addMethod(cpgf::GMetaMethod::newMethod<MetaCurrentClassType>(name, func, cpgf::GMetaPolicyDefault())); } \
		template <typename i_Reflect_FT, typename i_Reflect_Policy> static void reflectMethod(const char * name, i_Reflect_FT func, const i_Reflect_Policy & policy) { getMetaClassObject()->addMethod(cpgf::GMetaMethod::newMethod<MetaCurrentClassType>(name, func, policy)); } \
		template <typename i_Reflect_FT> static void reflectField(const char * name, i_Reflect_FT field) { getMetaClassObject()->addField(new cpgf::GMetaField(name, field, cpgf::GMetaPolicyDefault())); } \
		template <typename i_Reflect_FT, typename i_Reflect_Policy> static void reflectField(const char * name, i_Reflect_FT field, const i_Reflect_Policy & policy) { getMetaClassObject()->addField(new cpgf::GMetaField(name, field, policy)); } \
		template <typename i_Reflect_Signature, typename i_Reflect_Creator> static void reflectOperator(const i_Reflect_Creator & creator) { getMetaClassObject()->addOperator(new cpgf::GMetaOperator(creator.template create<MetaCurrentClassType, i_Reflect_Signature>(cpgf::GMetaPolicyDefault()))); } \
		template <typename i_Reflect_Signature, typename i_Reflect_Creator, typename i_Reflect_Policy> static void reflectOperator(const i_Reflect_Creator & creator, const i_Reflect_Policy & policy) { getMetaClassObject()->addOperator(new cpgf::GMetaOperator(creator.template create<MetaCurrentClassType, i_Reflect_Signature>(policy))); } \
		template <typename i_Reflect_Getter, typename i_Reflect_Setter> static void reflectProperty(const char * name, const i_Reflect_Getter & getter, const i_Reflect_Setter & setter) { getMetaClassObject()->addProperty(new cpgf::GMetaProperty(name, getter, setter, cpgf::GMetaPolicyDefault())); } \
		template <typename i_Reflect_Getter, typename i_Reflect_Setter, typename i_Reflect_Policy> static void reflectProperty(const char * name, const i_Reflect_Getter & getter, const i_Reflect_Setter & setter, const i_Reflect_Policy & policy) { getMetaClassObject()->addProperty(new cpgf::GMetaProperty(name, getter, setter, policy)); } \
		template <typename i_Reflect_T> static cpgf::GMetaEnum & reflectEnum(const char * name) { return getMetaClassObject()->addEnum(new cpgf::GMetaEnum(name, cpgf::createMetaType<i_Reflect_T>(), new cpgf::meta_internal::GMetaEnumData(NULL, sizeof(i_Reflect_T)))); } \
		static void reflectClass(const cpgf::GMetaClass * metaClass) { getMetaClassObject()->addClass(metaClass); } \
		static cpgf::GMetaAnnotation & reflectAnnotation(const char * name) { return getMetaClassObject()->addAnnotation(new cpgf::GMetaAnnotation(name)); }; \
		static void flushAnnotation() { getMetaClassObject()->flushAnnotation(); } \
		inline static void registerMetaInformation()

#define I_GMETA_DO_DEFINE_CLASS(cls, clsname, reg, policy, ...) \
	private: \
		typedef cls MetaCurrentClassType; \
		static cpgf::GMetaClass * getMetaClassObject() { \
			static cpgf::GMetaClass metaClass((MetaCurrentClassType *)0, cpgf::meta_internal::MakeSuperList<cpgf::TypeList_Make<__VA_ARGS__>::Result, MetaCurrentClassType >(), clsname, &destroyMetaObject, &reg, policy); \
			metaClass.addModifier(cpgf::metaModifierNoFree); \
			return &metaClass; \
		} \
		I_GMETA_DO_COMMON_CLASS

#define GMETA_INTRUSIVE_CLASS(cls, clsname, ...) I_GMETA_DO_DEFINE_CLASS(cls, clsname, cls::registerMetaInformation, cpgf::GMetaPolicyDefault(), __VA_ARGS__)
#define GMETA_INTRUSIVE_POLICIED_CLASS(cls, clsname, policy, ...) I_GMETA_DO_DEFINE_CLASS(cls, clsname, cls::registerMetaInformation, policy, __VA_ARGS__)

#define I_GMETA_DEFINE_CLASS(cls, alias, clsname, policy, ...) \
	class GMETA_WRAP_CLASS(alias) { \
		I_GMETA_DO_DEFINE_CLASS(cls, clsname, GMETA_WRAP_CLASS(alias)::registerMetaInformation, policy, __VA_ARGS__); \
	}; \
	static cpgf::meta_internal::GMetaClassRegister GPP_CONCAT(i_offClass_, __LINE__) (GMETA_WRAP_CLASS(alias)::getMetaClass()); \
	inline void GMETA_WRAP_CLASS(alias)::registerMetaInformation()

#define GMETA_DEFINE_CLASS(cls, alias, clsname, ...) I_GMETA_DEFINE_CLASS(cls, alias, clsname, cpgf::GMetaPolicyDefault(), __VA_ARGS__)
#define GMETA_DEFINE_POLICIED_CLASS(cls, alias, clsname, policy, ...) I_GMETA_DEFINE_CLASS(cls, alias, clsname, policy, __VA_ARGS__)

// This macro is almost same as GMETA_DEFINE_POLICIED_TEMPLATE except the policy
// but we can't call GMETA_DEFINE_POLICIED_TEMPLATE in this macro
// because GPP_COMMA can't be used in nested macro call in GCC.
#define GMETA_DEFINE_TEMPLATE(cls, alias, types, params, clsname, ...) \
	template <types> \
	class GMETA_WRAP_CLASS(alias) { \
	private: \
		typedef cls < params > MetaCurrentClassType; \
		static cpgf::GMetaClass * getMetaClassObject() { \
			static cpgf::GMetaClass metaClass((MetaCurrentClassType *)0, cpgf::meta_internal::MakeSuperList<typename cpgf::TypeList_Make<__VA_ARGS__>::Result, MetaCurrentClassType >(), clsname, &destroyMetaObject, &GMETA_WRAP_CLASS(alias)::registerMetaInformation, cpgf::GMetaPolicyDefault()); \
			metaClass.addModifier(cpgf::metaModifierNoFree); \
			return &metaClass; \
		} \
		I_GMETA_DO_COMMON_CLASS; \
	public: \
		static const cpgf::GMetaClass * instantiate(const char * name) { getMetaClassObject()->rebindName(name); return getMetaClass(); } \
	}; \
	template <types> \
	inline void GMETA_WRAP_CLASS(alias) < params >::registerMetaInformation()

#define GMETA_DEFINE_POLICIED_TEMPLATE(cls, alias, types, params, clsname, policy, ...) \
	template <types> \
	class GMETA_WRAP_CLASS(alias) { \
	private: \
		typedef cls < params > MetaCurrentClassType; \
		static cpgf::GMetaClass * getMetaClassObject() { \
			static cpgf::GMetaClass metaClass((MetaCurrentClassType *)0, cpgf::meta_internal::MakeSuperList<typename cpgf::TypeList_Make<__VA_ARGS__>::Result, MetaCurrentClassType >(), clsname, &destroyMetaObject, &GMETA_WRAP_CLASS(alias)::registerMetaInformation, policy); \
			metaClass.addModifier(cpgf::metaModifierNoFree); \
			return &metaClass; \
		} \
		I_GMETA_DO_COMMON_CLASS; \
	public: \
		static const cpgf::GMetaClass * instantiate(const char * name) { getMetaClassObject()->rebindName(name); return getMetaClass(); } \
	}; \
	template <types> \
	inline void GMETA_WRAP_CLASS(alias) < params >::registerMetaInformation()

#define GMETA_INSTANTIATE_TEMPLATE(name, alias, params) \
	static cpgf::meta_internal::GMetaClassRegister GPP_CONCAT(i_offClass_, __LINE__) (GMETA_WRAP_CLASS(alias) < params >::instantiate(name))



#define GMETA_WRAP_CLASS(cls) cls ## _oFf_mEta_

#define GMETA_FRIEND(cls) friend class GMETA_WRAP_CLASS(cls)


#define GMETA_CLASS(cls) \
	using namespace cpgf; \
	reflectClass(findMetaClass<MetaCurrentClassType::cls>())

#define GMETA_QUALIFIED_CLASS(cls) \
	using namespace cpgf; \
	reflectClass(findMetaClass<cls>())

#define GMETA_DEFINE_GLOBAL() \
	struct GPP_CONCAT(GMetaGlobalRegister_, __LINE__) { \
		static cpgf::GMetaClass * getMetaClass() { return cpgf::getGlobalMetaClass(); } \
		static cpgf::GMetaClass * getMetaClassObject() { return cpgf::getGlobalMetaClass(); } \
		inline static void registerGlobalMetaInformation(); \
		GPP_CONCAT(GMetaGlobalRegister_, __LINE__)() { \
			static bool initialized = false; \
			if(!initialized) { registerGlobalMetaInformation(); initialized = true; } \
		} \
	}; \
	static GPP_CONCAT(GMetaGlobalRegister_, __LINE__) GPP_CONCAT(mgr_i_, __LINE__); \
	inline void GPP_CONCAT(GMetaGlobalRegister_, __LINE__)::registerGlobalMetaInformation()




namespace cpgf {

inline void reflectClass(const GMetaClass * metaClass);

const GMetaClass * findMetaClass(const GMetaType & type);
const GMetaClass * findMetaClass(const char * name);


template <typename ClassType>
inline const GMetaClass * findMetaClass();

GMetaClass * getGlobalMetaClass();

class GMetaField;
class GMetaProperty;
class GMetaMethod;
class GMetaConstructor;
class GMetaOperator;
class GMetaEnum;
class GMetaAnnotation;

namespace meta_internal {


template <typename cls>
struct GCheckExistMetaClassSelector
{
private:
	typedef char YesType;
	struct NoType { char a[8]; };
	template <typename T, T> struct TestFunc {};
	template <typename FT> static YesType test(TestFunc<const GMetaClass * (*)(), &FT::getMetaClass> *);
	template <typename FT> static NoType test(...);
public:
	enum { Result = sizeof(test<cls>(0)) == sizeof(YesType) };
};

template <typename cls>
struct GCheckExistMetaClass
{
	enum { Result = GCheckExistMetaClassSelector<cls>::Result };
};


class GMetaClassDataBase
{
public:
	GMetaClassDataBase() {
	}

	virtual ~GMetaClassDataBase() {
	}

	virtual bool canCreateInstance() const = 0;
	virtual bool canCopyInstance() const = 0;
	virtual void * createInstance() const = 0;
	virtual void * createInplace(void * placement) const = 0;
	virtual void * cloneInstance(void * instance) const = 0;
	virtual void * cloneInplace(void * instance, void * placement) const = 0;
	virtual size_t getObjectSize() const = 0;
	virtual bool isAbstract() const = 0;
};

template <typename OT, typename Policy>
class GMetaClassData : public GMetaClassDataBase
{
private:
	enum {
		IsGlobal = IsSameType<OT, void>::Result,
		IsAbstract = IsAbstractClass<OT>::Result,
		NoDefaultConstructor = PolicyHasRule<Policy, GMetaRuleDefaultConstructorAbsent>::Result,
		NoCopyConstructor = PolicyHasRule<Policy, GMetaRuleCopyConstructorAbsent>::Result,
		CanDefaultConstruct = !IsGlobal && !IsAbstract && !NoDefaultConstructor,
		CanCopyConstruct = !IsGlobal && !IsAbstract && !NoCopyConstructor
	};

private:
	static void errorAbstract() {
		raiseException(Error_Meta_CannotInitAbstractClass, "Can't instantiate abstract class.");
	}

	static void errorNoDefault() {
		raiseException(Error_Meta_CannotInitAbstractClass, "Can't instantiate object that has no default constructor.");
	}

	static void errorNoCopy() {
		raiseException(Error_Meta_CannotInitAbstractClass, "Can't copy object that has no copy constructor.");
	}

public:
	virtual bool canCreateInstance() const {
		return CanDefaultConstruct;
	}

	virtual bool canCopyInstance() const {
		return CanCopyConstruct;
	}

	virtual void * createInstance() const {
		return this->doCreateInstance<void>();
	}

	virtual void * createInplace(void * placement) const {
		return this->doCreateInplace<void>(placement);
	}

	virtual void * cloneInstance(void * instance) const {
		return this->doCloneInstance<void>(instance);
	}

	virtual void * cloneInplace(void * instance, void * placement) const {
		return this->doCloneInplace<void>(instance, placement);
	}

	virtual size_t getObjectSize() const {
		return this->doGetObjectSize<void>();
	}

	virtual bool isAbstract() const {
		return this->doIsAbstract<void>();
	}

private:
	template <typename T>
	void * doCreateInstance(typename GEnableIf<CanDefaultConstruct, T>::Result * = 0) const {
		return new OT;
	}

	template <typename T>
	void * doCreateInstance(typename GDisableIf<CanDefaultConstruct, T>::Result * = 0) const {
		if(IsAbstract) {
			errorAbstract();
		}
		else {
			if(NoDefaultConstructor) {
				errorNoDefault();
			}
		}

		return NULL;
	}

	template <typename T>
	void * doCreateInplace(typename GEnableIf<CanDefaultConstruct, T>::Result * placement) const {
		return new (placement) OT;
	}

	template <typename T>
	void * doCreateInplace(typename GDisableIf<CanDefaultConstruct, T>::Result * placement) const {
		(void)placement;

		if(IsAbstract) {
			errorAbstract();
		}
		else {
			if(NoDefaultConstructor) {
				errorNoDefault();
			}
		}

		return NULL;
	}

	template <typename T>
	void * doCloneInstance(typename GEnableIf<CanCopyConstruct, T>::Result * instance) const {
		return new OT (*static_cast<OT *>(instance));
	}

	template <typename T>
	void * doCloneInstance(typename GDisableIf<CanCopyConstruct, T>::Result * instance) const {
		(void)instance;

		if(IsAbstract) {
			errorAbstract();
		}
		else {
			if(NoCopyConstructor) {
				errorNoCopy();
			}
		}

		return NULL;
	}

	template <typename T>
	void * doCloneInplace(typename GEnableIf<CanCopyConstruct, T>::Result * instance, void * placement) const {
		return new (placement) OT (*static_cast<OT *>(instance));
	}

	template <typename T>
	void * doCloneInplace(typename GDisableIf<CanCopyConstruct, T>::Result * instance, void * placement) const {
		(void)instance;
		(void)placement;

		if(IsAbstract) {
			errorAbstract();
		}
		else {
			if(NoCopyConstructor) {
				errorNoCopy();
			}
		}

		return NULL;
	}

	template <typename T>
	size_t doGetObjectSize(typename GDisableIf<IsGlobal, T>::Result * = 0) const {
		return sizeof(OT);
	}

	template <typename T>
	size_t doGetObjectSize(typename GEnableIf<IsGlobal, T>::Result * = 0) const {
		return 0;
	}

	template <typename T>
	bool doIsAbstract(typename GDisableIf<IsGlobal, T>::Result * = 0) const {
		return IsAbstractClass<OT>::Result;
	}

	template <typename T>
	bool doIsAbstract(typename GEnableIf<IsGlobal, T>::Result * = 0) const {
		return true;
	}

};


class GMetaClassCasterBase
{
public:
	virtual ~GMetaClassCasterBase() {
	}

	virtual GMetaClassCasterBase * clone() const = 0;
	virtual void * downCast(void * base) const = 0;
	virtual void * upCast(void * derived) const = 0;
};

template <bool IsPolymorphic>
struct GMetaClassCasterSelector
{
	template <typename D, typename B>
	static typename GEnableIf<IsVirtualBase<D, B>::Result>::Result * downCast(void * base) {
		return NULL;
	}

	template <typename D, typename B>
	static typename GEnableIf<!IsVirtualBase<D, B>::Result>::Result * downCast(void * base) {
		return static_cast<D *>(static_cast<B *>(base));
	}

};

template <>
struct GMetaClassCasterSelector <true>
{
	template <typename D, typename B>
	static void * downCast(void * base) {
		return dynamic_cast<D *>(static_cast<B *>(base));
	}
};

template <typename Derived, typename Base>
class GMetaClassCaster : public GMetaClassCasterBase
{
public:
	virtual GMetaClassCasterBase * clone() const {
		return new GMetaClassCaster<Derived, Base>;
	}

	virtual void * downCast(void * base) const {
		return GMetaClassCasterSelector<IsPolymorphic<Base>::Result>::template downCast<Derived, Base>(base);
	}

	virtual void * upCast(void * derived) const {
		return static_cast<Base *>(static_cast<Derived *>(derived));
	}
};

template <typename Derived>
class GMetaClassCaster <Derived, void>: public GMetaClassCasterBase
{
public:
	virtual GMetaClassCasterBase * clone() const {
		return new GMetaClassCaster<Derived, void>;
	}

	virtual void * downCast(void * base) const {
		return base;
	}

	virtual void * upCast(void * derived) const {
		return derived;
	}
};

class GMetaSuperListItem
{
public:
	GMetaSuperListItem(const GMetaClass * superClass, const GMetaType & type, GMetaClassCasterBase * caster)
		: superClass(superClass), type(type), caster(caster) {
	}

	void freeList() {
		delete this->caster;
	}

	const GMetaClass * getBaseClass() const {
		if(this->superClass == NULL) {
			this->superClass = findMetaClass(this->type);
		}

		return this->superClass;
	}

	const GMetaClassCasterBase * getCaster() const {
		return this->caster;
	}

private:
	mutable const GMetaClass * superClass;
	GMetaType type;
	GMetaClassCasterBase * caster;
};

class GMetaSuperListImplement;

class GMetaSuperList
{
public:
	GMetaSuperList();
	~GMetaSuperList();

	size_t getCount() const;
	const GMetaClass * getSuper(size_t index) const;
	const GMetaClassCasterBase * getCaster(size_t index) const;

	template <typename ClassType, typename BaseType>
	void add() {
		if(this->isMetaRoot<BaseType>()) {
			return;
		}

		this->doAdd(GMetaSuperListItem(NULL, createMetaType<BaseType>(), new GMetaClassCaster<ClassType, BaseType>()));
	}

private:
	template <typename T>
	bool isMetaRoot() const {
		return IsSameType<T, void>::Result;
	}

	void doAdd(const GMetaSuperListItem & item);

private:
	GScopedPointer<GMetaSuperListImplement> implement;
};

class GMetaItemListImplement;


} // namespace meta_internal



class GMetaClassImplement;

class GMetaClass : public GMetaTypedItem
{
private:
	typedef GMetaTypedItem super;

public:
	template <typename ClassT, typename Policy>
	GMetaClass(ClassT *, meta_internal::GMetaSuperList * superList, const char * name, void (*destroy)(void *), void (*metaRegister)(), const Policy &)
		:	super(name, createMetaType<ClassT>(), mcatClass),
			intialized(false),
			destroy(destroy),
			metaRegister(metaRegister),
			previousAddedItem(NULL),
			superList(superList),
			baseData(new meta_internal::GMetaClassData<ClassT, Policy>()) {

		GASSERT_STATIC(IsClass<ClassT>::Result || (IsSameType<ClassT, void>::Result) || IsFundamental<ClassT>::Result);

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
	void addConstructor(const Policy & policy) {
		if(this->isGlobal()) {
			return;
		}
		this->addItem(mcatConstructor, new GMetaConstructor(
				GCallback<Signature>(
					meta_internal::GMetaConstructorInvoker<GFunctionTraits<Signature>::Arity, OT, typename GFunctionTraits<Signature>::ArgList>()
				),
				policy
			)
			)
			;
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
	size_t getMethodList(GMetaList * metaList, const char * name, const GFlags<GMetaFilters> & filters) const;
	size_t getMethodListInHierarchy(GMetaList * metaList, const char * name, const GFlags<GMetaFilters> & filters, void * instance) const;

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
	void addField(GMetaField * field);
	void addProperty(GMetaProperty * field);
	void addMethod(GMetaMethod * method);
	void addOperator(GMetaOperator * metaOperator);
	GMetaEnum & addEnum(GMetaEnum * en);
	void addClass(const GMetaClass * cls);
	GMetaAnnotation & addAnnotation(GMetaAnnotation * annotation);
	void flushAnnotation();

private:
	void addItem(GMetaCategory listIndex, GMetaItem * item);
	size_t getItemCount(GMetaCategory listIndex) const;
	const GMetaItem * getItemAt(GMetaCategory listIndex, size_t index) const;
	const GMetaItem * getItemByName(GMetaCategory listIndex, const char * name, bool findSuper, void ** outInstance) const;
	size_t getItemListByName(GMetaList * metaList, GMetaCategory listIndex, const char * name, bool findSuper, const GFlags<GMetaFilters> & filters, void * instance) const;

public:
	// internal use
	void rebindName(const char * name);

private:
	void ensureRegistered() const;
	void setupItemLists();

	void itemAdded(GMetaItem * metaItem);
	void refreshAnnotation(GMetaItem * metaItem);

	const GMetaOperator * doGetOperator(GMetaOpType op, bool findSuper, void ** outInstance) const;

	void intializeImplement();

private:
	mutable bool intialized;
	void (*destroy)(void *);
	void (*metaRegister)();

	GMetaItem * previousAddedItem;

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


#define CASE_SUPERLIST_ARG(N, unused) case N: superList->add<ClassType, typename TypeList_GetWithDefault<TList, N, void>::Result>(); break;

template <typename TList, typename ClassType>
GMetaSuperList * MakeSuperList() {
	GMetaSuperList * superList = new GMetaSuperList;

	for(int i = 0; i < 20; ++i) {
		if(i >= static_cast<int>(TypeList_Length<TList>::Result)) {
			break;
		}

		switch(i) {
			GPP_REPEAT(19, CASE_SUPERLIST_ARG, GPP_EMPTY())

			default:
				break;
		}
	}

	return superList;
}

#undef CASE_SUPERLIST_ARG


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


inline void reflectClass(const GMetaClass * metaClass)
{
	getGlobalMetaClass()->addClass(metaClass);
}


} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
