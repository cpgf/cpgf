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
#include "cpgf/gifelse.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif


namespace cpgf {

const GMetaClass * findMetaClass(const GMetaType & type);

namespace meta_internal {


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
	G_STATIC_CONSTANT(bool, IsGlobal = IsSameType<OT, void>::Result);
	G_STATIC_CONSTANT(bool, IsAbstract = IsAbstractClass<OT>::Result);
	G_STATIC_CONSTANT(bool, NoDefaultConstructor = PolicyHasRule<Policy, GMetaRuleDefaultConstructorAbsent>::Result);
	G_STATIC_CONSTANT(bool, NoCopyConstructor = PolicyHasRule<Policy, GMetaRuleCopyConstructorAbsent>::Result);
	G_STATIC_CONSTANT(bool, CanDefaultConstruct = !IsGlobal && !IsAbstract && !NoDefaultConstructor);
	G_STATIC_CONSTANT(bool, CanCopyConstruct = !IsGlobal && !IsAbstract && !NoCopyConstructor);

private:
	static void errorAbstract() {
		raiseCoreException(Error_Meta_CannotInitAbstractClass);
	}

	static void errorNoDefault() {
		raiseCoreException(Error_Meta_NoDefaultConstructor);
	}

	static void errorNoCopy() {
		raiseCoreException(Error_Meta_NoCopyConstructor);
	}

public:
	virtual bool canCreateInstance() const {
		return CanDefaultConstruct;
	}

	virtual bool canCopyInstance() const {
		return CanCopyConstruct;
	}

	virtual void * createInstance() const {
		return this->doCreateInstance<typename GIfElse<CanDefaultConstruct, void, int>::Result >();
	}

	virtual void * createInplace(void * placement) const {
		return this->doCreateInplace<typename GIfElse<CanDefaultConstruct, void, int>::Result >(placement);
	}

	virtual void * cloneInstance(void * instance) const {
		return this->doCloneInstance<typename GIfElse<CanCopyConstruct, void, int>::Result >(instance);
	}

	virtual void * cloneInplace(void * instance, void * placement) const {
		return this->doCloneInplace<typename GIfElse<CanCopyConstruct, void, int>::Result >(instance, placement);
	}

	virtual size_t getObjectSize() const {
		return this->doGetObjectSize<typename GIfElse<IsGlobal, void, OT>::Result >();
	}

	virtual bool isAbstract() const {
		return this->doIsAbstract<typename GIfElse<IsGlobal, void, OT>::Result >();
	}

private:
	template <typename T>
	void * doCreateInstance(typename GEnableIfResult<IsVoid<T>, T>::Result * = 0) const {
		return new OT;
	}

	template <typename T>
	void * doCreateInstance(typename GDisableIfResult<IsVoid<T>, T>::Result * = 0) const {
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
	void * doCreateInplace(typename GEnableIfResult<IsVoid<T>, void>::Result * placement) const {
		return new (placement) OT;
	}

	template <typename T>
	void * doCreateInplace(typename GDisableIfResult<IsVoid<T>, void>::Result * placement) const {
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
	void * doCloneInstance(typename GEnableIfResult<IsVoid<T>, void>::Result * instance) const {
		return new OT (*static_cast<OT *>(instance));
	}

	template <typename T>
	void * doCloneInstance(typename GDisableIfResult<IsVoid<T>, void>::Result * instance) const {
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
	void * doCloneInplace(typename GEnableIfResult<IsVoid<T>, void>::Result * instance, void * placement) const {
		return new (placement) OT (*static_cast<OT *>(instance));
	}

	template <typename T>
	void * doCloneInplace(typename GDisableIfResult<IsVoid<T>, void>::Result * instance, void * placement) const {
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
	size_t doGetObjectSize(typename GDisableIfResult<IsVoid<T>, void>::Result * = 0) const {
		return sizeof(T);
	}

	template <typename T>
	size_t doGetObjectSize(typename GEnableIfResult<IsVoid<T>, void>::Result * = 0) const {
		return 0;
	}

	template <typename T>
	bool doIsAbstract(typename GDisableIfResult<IsVoid<T>, void>::Result * = 0) const {
		return IsAbstractClass<OT>::Result;
	}

	template <typename T>
	bool doIsAbstract(typename GEnableIfResult<IsVoid<T>, void>::Result * = 0) const {
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
	static void * downCast(void * base, typename GEnableIfResult<IsVirtualBase<D, B> >::Result * = 0) {
		(void)base;

		return NULL;
	}

	template <typename D, typename B>
	static void * downCast(void * base, typename GDisableIfResult<IsVirtualBase<D, B> >::Result * = 0) {
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
