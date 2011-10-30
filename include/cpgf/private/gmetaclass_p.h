#ifndef __GMETACLASS_P_H
#define __GMETACLASS_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetaannotation.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetafield.h"
#include "cpgf/gmetamethod.h"
#include "cpgf/gmetaoperator.h"
#include "cpgf/gmetaproperty.h"
#include "cpgf/gmetapolicy.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif



namespace cpgf {

const GMetaClass * findMetaClass(const GMetaType & type);

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

} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
