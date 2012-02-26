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

struct GMetaClassDataVirtual
{
	void (*deleteObject)(void * self);
	bool (*canCreateInstance)(const void * self);
	bool (*canCopyInstance)(const void * self);
	void * (*createInstance)(const void * self);
	void * (*createInplace)(const void * self, void * placement);
	void * (*cloneInstance)(const void * self, void * instance);
	void * (*cloneInplace)(const void * self, void * instance, void * placement);
	size_t (*getObjectSize)(const void * self);
	bool (*isAbstract)(const void * self);
};

class GMetaClassDataBase
{
public:
	void deleteObject();
	
	bool canCreateInstance() const;
	bool canCopyInstance() const;
	
	void * createInstance() const;
	void * createInplace(void * placement) const;
	void * cloneInstance(void * instance) const;
	void * cloneInplace(void * instance, void * placement) const;

	size_t getObjectSize() const;

	bool isAbstract() const;

protected:
	GMetaClassDataVirtual * virtualFunctions;
};

template <typename OT, typename Policy>
class GMetaClassData : public GMetaClassDataBase
{
private:
	G_STATIC_CONSTANT(bool, IsGlobal = (IsSameType<OT, void>::Result));
	G_STATIC_CONSTANT(bool, IsAbstract = (IsAbstractClass<OT>::Result));
	G_STATIC_CONSTANT(bool, NoDefaultConstructor = (PolicyHasRule<Policy, GMetaRuleDefaultConstructorAbsent>::Result));
	G_STATIC_CONSTANT(bool, NoCopyConstructor = (PolicyHasRule<Policy, GMetaRuleCopyConstructorAbsent>::Result));
	G_STATIC_CONSTANT(bool, CanDefaultConstruct = (!IsGlobal && !IsAbstract && !NoDefaultConstructor));
	G_STATIC_CONSTANT(bool, CanCopyConstruct = (!IsGlobal && !IsAbstract && !NoCopyConstructor));

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

private:
	static bool virtualCanCreateInstance(const void * self) {
		(void)self;

		return CanDefaultConstruct;
	}

	static bool virtualCanCopyInstance(const void * self) {
		(void)self;

		return CanCopyConstruct;
	}

	static void * virtualCreateInstance(const void * self) {
		return static_cast<const GMetaClassData *>(self)->doCreateInstance<typename GIfElse<CanDefaultConstruct, void, int>::Result >();
	}

	static void * virtualCreateInplace(const void * self, void * placement) {
		return static_cast<const GMetaClassData *>(self)->doCreateInplace<typename GIfElse<CanDefaultConstruct, void, int>::Result >(placement);
	}

	static void * virtualCloneInstance(const void * self, void * instance) {
		return static_cast<const GMetaClassData *>(self)->doCloneInstance<typename GIfElse<CanCopyConstruct, void, int>::Result >(instance);
	}

	static void * virtualCloneInplace(const void * self, void * instance, void * placement) {
		return static_cast<const GMetaClassData *>(self)->doCloneInplace<typename GIfElse<CanCopyConstruct, void, int>::Result >(instance, placement);
	}

	static size_t virtualGetObjectSize(const void * self) {
		return static_cast<const GMetaClassData *>(self)->doGetObjectSize<typename GIfElse<IsGlobal, void, OT>::Result >();
	}

	static bool virtualIsAbstract(const void * self) {
		return static_cast<const GMetaClassData *>(self)->doIsAbstract<typename GIfElse<IsGlobal, void, OT>::Result >();
	}

public:
	GMetaClassData() {
		static GMetaClassDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<GMetaClassData>,
			&virtualCanCreateInstance,
			&virtualCanCopyInstance,
			&virtualCreateInstance,
			&virtualCreateInplace,
			&virtualCloneInstance,
			&virtualCloneInplace,
			&virtualGetObjectSize,
			&virtualIsAbstract
		};

		this->virtualFunctions = &thisFunctions;
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


class GMetaClassCasterBase;

struct GMetaClassCasterVirtual
{
	GMetaClassCasterBase * (*clone)();
	void * (*downCast)(void * base);
	void * (*upCast)(void * derived);
};

class GMetaClassCasterBase
{
public:
	GMetaClassCasterBase * clone() const;

	void * downCast(void * base) const;
	void * upCast(void * derived) const;

protected:
	GMetaClassCasterVirtual * virtualFunctions;
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
private:
	static GMetaClassCasterBase * virtualClone() {
		return new GMetaClassCaster<Derived, Base>;
	}

	static void * virtualDownCast(void * base) {
		return GMetaClassCasterSelector<IsPolymorphic<Base>::Result>::template downCast<Derived, Base>(base);
	}

	static void * virtualUpCast(void * derived) {
		return static_cast<Base *>(static_cast<Derived *>(derived));
	}

public:
	GMetaClassCaster() {
		static GMetaClassCasterVirtual thisFunctions = {
			&virtualClone,
			&virtualDownCast,
			&virtualUpCast
		};

		this->virtualFunctions = &thisFunctions;
	}
};

template <typename Derived>
class GMetaClassCaster <Derived, void>: public GMetaClassCasterBase
{
private:
	static GMetaClassCasterBase * virtualClone() {
		return new GMetaClassCaster<Derived, void>;
	}

	static void * virtualDownCast(void * base) {
		return base;
	}

	static void * virtualUpCast(void * derived) {
		return derived;
	}

public:
	GMetaClassCaster() {
		static GMetaClassCasterVirtual thisFunctions = {
			&virtualClone,
			&virtualDownCast,
			&virtualUpCast
		};

		this->virtualFunctions = &thisFunctions;
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
