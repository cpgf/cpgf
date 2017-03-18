#ifndef CPGF_GMETACLASS_P_H
#define CPGF_GMETACLASS_P_H

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

extern int Error_Meta_CannotInitAbstractClass;
extern int Error_Meta_NoDefaultConstructor;
extern int Error_Meta_NoCopyConstructor;

const GMetaClass * findMetaClass(const GMetaType & type);

namespace meta_internal {

struct GMetaClassDataVirtual
{
	void (*deleteSelf)(void * self);
	bool (*canCreateInstance)();
	bool (*canCopyInstance)();
	void * (*createInstance)(const void * self);
	void * (*createInplace)(const void * self, void * placement);
	void * (*cloneInstance)(const void * self, const void * instance);
	void * (*cloneInplace)(const void * self, const void * instance, void * placement);
	void (*destroyInstance)(void * instance);
	void (*destroyInplace)(void * instance);
	size_t (*getObjectSize)(const void * self);
	bool (*isAbstract)(const void * self);
	bool (*isPolymorphic)();
	GMetaExtendType (*getItemExtendType)(uint32_t flags, const GMetaItem * metaItem);
};

class GMetaClassDataBase
{
public:
	void deleteSelf();
	
	bool canCreateInstance() const;
	bool canCopyInstance() const;
	
	void * createInstance() const;
	void * createInplace(void * placement) const;
	void * cloneInstance(const void * instance) const;
	void * cloneInplace(const void * instance, void * placement) const;

	void destroyInstance(void * instance) const;
	void destroyInplace(void * instance) const;

	size_t getObjectSize() const;
	
	// must be defined in header to make template function overloading happy.
	GMetaExtendType getItemExtendType(uint32_t flags, const GMetaItem * metaItem) const {
		return this->virtualFunctions->getItemExtendType(flags, metaItem);
	}

	bool isAbstract() const;
	bool isPolymorphic() const;

protected:
	GMetaClassDataVirtual * virtualFunctions;
};

template <typename T, bool CanDelete>
struct ObjectDeleter
{
	static void destroyInstance(void * instance) {
		delete static_cast<T *>(instance);
	}

	static void destroyInplace(void * instance) {
		(void)instance;
		static_cast<T *>(instance)->~T();
	}
};

template <typename T>
struct ObjectDeleter <T, false>
{
	static void destroyInstance(void * /*instance*/) {
	}

	static void destroyInplace(void * /*instance*/) {
	}
};

template <bool CanDelete>
struct ObjectDeleter <void, CanDelete>
{
	static void destroyInstance(void * /*instance*/) {
	}

	static void destroyInplace(void * /*instance*/) {
	}
};

template <typename OT, typename Policy>
class GMetaClassData : public GMetaClassDataBase
{
private:
	G_STATIC_CONSTANT(bool, IsGlobal = (IsSameType<OT, void>::Result));
	G_STATIC_CONSTANT(bool, IsAbstract = (IsAbstractClass<OT>::Result));
	G_STATIC_CONSTANT(bool, NoDefaultConstructor = (PolicyHasRule<Policy, GMetaRuleDefaultConstructorAbsent>::Result));
	G_STATIC_CONSTANT(bool, NoCopyConstructor = (PolicyHasRule<Policy, GMetaRuleCopyConstructorAbsent>::Result));
	G_STATIC_CONSTANT(bool, CanDefaultConstruct = (!IsGlobal && !IsAbstract && !NoDefaultConstructor && std::is_default_constructible<OT>::value));
	G_STATIC_CONSTANT(bool, CanCopyConstruct = (!IsGlobal && !IsAbstract && !NoCopyConstructor && std::is_copy_constructible<OT>::value));

	typedef ObjectDeleter<OT, !PolicyHasRule<Policy, GMetaRuleDestructorAbsent>::Result> Deleter;

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
	static bool virtualCanCreateInstance() {
		return CanDefaultConstruct;
	}

	static bool virtualCanCopyInstance() {
		return CanCopyConstruct;
	}

	static void * virtualCreateInstance(const void * self) {
		return static_cast<const GMetaClassData *>(self)->doCreateInstance<typename GIfElse<CanDefaultConstruct, void, int>::Result >();
	}

	static void * virtualCreateInplace(const void * self, void * placement) {
		return static_cast<const GMetaClassData *>(self)->doCreateInplace<typename GIfElse<CanDefaultConstruct, void, int>::Result >(placement);
	}

	static void * virtualCloneInstance(const void * self, const void * instance) {
		return static_cast<const GMetaClassData *>(self)->doCloneInstance<typename GIfElse<CanCopyConstruct, void, int>::Result >(instance);
	}

	static void * virtualCloneInplace(const void * self, const void * instance, void * placement) {
		return static_cast<const GMetaClassData *>(self)->doCloneInplace<typename GIfElse<CanCopyConstruct, void, int>::Result >(instance, placement);
	}

	static void virtualDestroyInstance(void * instance) {
		Deleter::destroyInstance(instance);
	}

	static void virtualDestroyInplace(void * instance) {
		Deleter::destroyInplace(instance);
	}

	static size_t virtualGetObjectSize(const void * self) {
		return static_cast<const GMetaClassData *>(self)->doGetObjectSize<typename GIfElse<IsGlobal, void, OT>::Result >();
	}

	static GMetaExtendType virtualGetItemExtendType(uint32_t flags, const GMetaItem * metaItem) {
		return createMetaExtendType<OT>(flags, metaItem);
	}

	static bool virtualIsAbstract(const void * self) {
		return static_cast<const GMetaClassData *>(self)->doIsAbstract<typename GIfElse<IsGlobal, void, OT>::Result >();
	}

	static bool virtualPolymorphic() {
		return IsPolymorphic<OT>::Result;
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
			&virtualDestroyInstance,
			&virtualDestroyInplace,
			&virtualGetObjectSize,
			&virtualIsAbstract,
			&virtualPolymorphic,
			&virtualGetItemExtendType
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

		return nullptr;
	}

	template <typename T>
	void * doCreateInplace(typename GEnableIfResult<IsVoid<T>, void>::Result * placement) const {
		return new (placement) OT;
	}

	template <typename T>
	void * doCreateInplace(typename GDisableIfResult<IsVoid<T>, void>::Result * /*placement*/) const {
		if(IsAbstract) {
			errorAbstract();
		}
		else {
			if(NoDefaultConstructor) {
				errorNoDefault();
			}
		}

		return nullptr;
	}

	template <typename T>
	void * doCloneInstance(typename GEnableIfResult<IsVoid<T>, void>::Result const * instance) const {
		return new OT (*static_cast<const OT *>(instance));
	}

	template <typename T>
	void * doCloneInstance(typename GDisableIfResult<IsVoid<T>, void>::Result const * /*instance*/) const {
		if(IsAbstract) {
			errorAbstract();
		}
		else {
			if(NoCopyConstructor) {
				errorNoCopy();
			}
		}

		return nullptr;
	}

	template <typename T>
	void * doCloneInplace(typename GEnableIfResult<IsVoid<T>, void>::Result const * instance, void * placement) const {
		return new (placement) OT (*static_cast<const OT *>(instance));
	}

	template <typename T>
	void * doCloneInplace(typename GDisableIfResult<IsVoid<T>, void>::Result const * /*instance*/, void * /*placement*/) const {
		if(IsAbstract) {
			errorAbstract();
		}
		else {
			if(NoCopyConstructor) {
				errorNoCopy();
			}
		}

		return nullptr;
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

	void * downCast(const void * base) const;
	void * upCast(const void * derived) const;

protected:
	GMetaClassCasterVirtual * virtualFunctions;
};

template <bool IsPolymorphic>
struct GMetaClassCasterSelector
{
	template <typename D, typename B>
	static void * downCast(void * /*base*/, typename GEnableIfResult<IsVirtualBase<D, B> >::Result * = 0) {
		return nullptr;
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
	GMetaSuperListItem(GMetaClass * superClass, const GMetaType & type, GMetaClassCasterBase * caster)
		: superClass(superClass), type(type), caster(caster), addedDerived(false) {
	}

	void freeList() {
		delete this->caster;
	}

	const GMetaClass * getBaseClass() const {
		if(this->superClass == nullptr) {
			this->superClass = findMetaClass(this->type);
		}

		return this->superClass;
	}

	const GMetaClassCasterBase * getCaster() const {
		return this->caster;
	}

	bool hasAddedDerived() const {
		return this->addedDerived;
	}

	void setAddedDerived(bool addedDerived) {
		this->addedDerived = addedDerived;
	}

private:
	mutable const GMetaClass * superClass;
	GMetaType type;
	GMetaClassCasterBase * caster;
	bool addedDerived;
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
	bool hasAddedDerived(size_t index) const;
	void setAddedDerived(size_t index, bool addedDerived);

	template <typename ClassType, typename BaseType>
	GMetaSuperListItem * add() {
		if(this->isMetaRoot<BaseType>()) {
			return nullptr;
		}

		return this->doAdd(GMetaSuperListItem(nullptr, createMetaType<BaseType>(), new GMetaClassCaster<ClassType, BaseType>()));
	}

private:
	template <typename T>
	bool isMetaRoot() const {
		return IsSameType<T, void>::Result;
	}

	GMetaSuperListItem * doAdd(const GMetaSuperListItem & item);

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
