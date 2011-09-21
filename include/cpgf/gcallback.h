#ifndef __GCALLBACK_H
#define __GCALLBACK_H

#include "cpgf/gpp.h"
#include "cpgf/gconfig.h"
#include "cpgf/greference.h"
#include "cpgf/gcontainer.h"
#include "cpgf/gtypetraits.h"

#include <algorithm>
#include <utility>
#include <assert.h>

#if !defined(CB_MAX_ARITY)
	#define CB_MAX_ARITY G_MAX_ARITY
#endif

#include "gfunctiontraits.h"


// namespace is cpgf


#define CB_PARAM_TYPEVALUE(N, P)		GPP_COMMA_IF(N) typename GArgumentTraits<P ## N>::Result  p ## N


#define CB_DEF_MEMBER(N) \
	template <typename InnerOT, typename InnerFT> \
	class GCallbackMember : public callback_internal::GCallbackMemberBase<GCallbackMember<InnerOT, InnerFT>, InnerOT, InnerFT> { \
	private: \
		typedef GCallbackMember <InnerOT, InnerFT> ThisType; \
		typedef callback_internal::GCallbackMemberBase<ThisType, InnerOT, InnerFT> super; \
	public: \
		GCallbackMember(InnerOT * instance, const InnerFT & func) : super(instance, func) {} \
		static RT virtualInvoke(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) { return (static_cast<ThisType *>(self)->instance->*(*&(static_cast<ThisType *>(self)->func)))(GPP_REPEAT_PARAMS(N, p)); } \
	};

#define CB_DEF_GLOBAL(N) \
	template <typename InnerFT> \
	class GCallbackGlobal : public callback_internal::GCallbackGlobalBase<GCallbackGlobal<InnerFT>, InnerFT> { \
	private: \
		typedef GCallbackGlobal <InnerFT> ThisType; \
		typedef callback_internal::GCallbackGlobalBase<ThisType, InnerFT> super; \
	public: \
		GCallbackGlobal (const InnerFT & func) : super(func) {} \
		static RT virtualInvoke(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) { return (*&(static_cast<ThisType *>(self)->func))(GPP_REPEAT_PARAMS(N, p)); } \
	};

#define CB_DEF_CALLBACK_N(N, unused) \
	template<typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename PT) > \
	class GPP_CONCAT(GCallback, N) { \
	public: \
		typedef GCallback <RT (GPP_REPEAT_PARAMS(N, PT)) > type; \
	};


#define CB_FUNC_TRAITS_ARGS(N, P) GPP_COMMA_IF(N) typename FunctionTraits::ArgList::Arg ## N

#define CB_DEF_FUNC_TRAITS(N, P) \
	template <typename FT> \
	struct GCallbackFunctionTraits <N, FT> { \
		typedef GFunctionTraits<FT> FunctionTraits; \
		typedef GCallbackAgent_ ## N <typename FunctionTraits::ResultType GPP_COMMA_IF(N) GPP_REPEAT(N, CB_FUNC_TRAITS_ARGS, GPP_EMPTY)> CallbackAgentType; \
	};

#define CB_DEF_AGENT_N(N, P) \
	template<typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename PT) > \
	class GCallbackAgent_ ## N : public GCallbackBase { \
	private: \
		typedef GCallbackAgent_ ## N < RT GPP_REPEAT_TAIL_PARAMS(N, PT) > ThisType; \
	protected: \
		typedef RT FunctionType(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)); \
		typedef FunctionType * FunctionPointer; \
		CB_DEF_MEMBER(N) \
		CB_DEF_GLOBAL(N) \
	public: \
		RT invoke(GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) const { if(this->getBase()) { return reinterpret_cast<FunctionPointer>(this->getBase()->getInvoke())(this->getBase() GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, p)); } else { return callback_internal::ConstructDefault<RT>::construct(); } } \
		RT operator () (GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) const { return this->invoke(GPP_REPEAT_PARAMS(N, p)); } \
	};

#define CB_OBJ_FUNC_HELPER(name, N, qualify) \
	template <typename RT, typename OT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename PT) > \
	inline GCallback <RT (GPP_REPEAT_PARAMS(N, PT)) > \
	name(qualify OT * instance, RT (OT::*func)(GPP_REPEAT_PARAMS(N, PT)) qualify) \
	{ \
		return GCallback <RT (GPP_REPEAT_PARAMS(N, PT)) >(instance, func); \
	}

#define CB_OBJ_FUNC(name, N) \
	CB_OBJ_FUNC_HELPER(name, N, GPP_EMPTY()) \
	CB_OBJ_FUNC_HELPER(name, N, const) \
	CB_OBJ_FUNC_HELPER(name, N, volatile) \
	CB_OBJ_FUNC_HELPER(name, N, const volatile)

#define CB_GLOBAL_FUNC_HELPER(name, N, qualify) \
	template <typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename PT) > \
	inline GCallback <RT (GPP_REPEAT_PARAMS(N, PT)) > \
	name(RT (* qualify func)(GPP_REPEAT_PARAMS(N, PT))) \
	{ \
		return GCallback <RT (GPP_REPEAT_PARAMS(N, PT)) >(func); \
	}

#define CB_GLOBAL_FUNC(name, N) \
	CB_GLOBAL_FUNC_HELPER(name, N, GPP_EMPTY())


#define CB_ORD_OBJ_MAKE(N, P) CB_OBJ_FUNC(GPP_CONCAT(makeCallback, N), N)
#define CB_ORD_GLOBAL_MAKE(N, P) CB_GLOBAL_FUNC(GPP_CONCAT(makeCallback, N), N)


namespace cpgf {


namespace callback_internal {

// traits to construct default return value
// and avoid "invalid value-initialization of reference types" error in GCC when return type is reference
template <typename T>
struct ConstructDefault
{
	static T construct() {
		return typename RemoveConstVolatile<T>::Result();
	}
};

template <typename T> struct ConstructDefault <T &> { static T & construct() { return *static_cast<T *>(0); } };

template <bool tooBig>
struct CBInplaceMeasure;

template <>
struct CBInplaceMeasure<true> {
	template <typename T, typename PT1, typename PT2>
	static T * newObject(void * buffer, const PT1 & p1, const PT2 & p2) {
		(void)buffer;

		return new T(p1, p2);
	}

	template <typename T, typename PT1>
	static T * newObject(void * buffer, const PT1 & p1) {
		(void)buffer;

		return new T(p1);
	}

	template <typename T>
	static void deleteObject(T * p) {
		if(p) {
			p->deleteFunctor();
			delete p;
		}
	}
};

template <>
struct CBInplaceMeasure<false> {
	template <typename T, typename PT1, typename PT2>
	static T * newObject(void * buffer, const PT1 & p1, const PT2 & p2) {
		return new(buffer) T(p1, p2);
	}

	template <typename T, typename PT1>
	static T * newObject(void * buffer, const PT1 & p1) {
		return new(buffer) T(p1);
	}

	template <typename T>
	static void deleteObject(T * p) {
		p->deleteFunctor(); // p can never be NULL
	}
};

struct SizeOfCallbackBase {
	void * p;
};

struct SizeOfCallbackSon : public SizeOfCallbackBase {
	virtual void a(int) { (void)a(0); }
};

class CBAllocator
{
private:
	typedef CBAllocator ThisType;

	enum { BufferSize = sizeof(&SizeOfCallbackSon::a) + sizeof(SizeOfCallbackBase) };

public:
	template <typename T, typename PT1, typename PT2>
	T * newObject(const PT1 & p1, const PT2 & p2) {
		return CBInplaceMeasure<(sizeof(T) > BufferSize)>::template newObject<T>(this->buffer, p1, p2);
	}

	template <typename T, typename PT1>
	T * newObject(const PT1 & p1) {
		return CBInplaceMeasure<(sizeof(T) > BufferSize)>::template newObject<T>(this->buffer, p1);
	}

	template <typename T>
	void deleteObject(T * p) const {
		if(p) {
			p->deleteFunctor();

			if(const_cast<const char *>(reinterpret_cast<char *>(p)) != this->buffer) {
				delete p;
			}
		}
	}

	template <typename T>
	T * take(T * p, CBAllocator * allocator) const {
		if(const_cast<const char *>(reinterpret_cast<char *>(p)) == this->buffer) {
			T * instance = p->clone(allocator);
			p->~T();
			return instance;
		}
		else {
			return p;
		}
	}

private:
	void * base;
	char buffer[BufferSize];

private:
	friend class GCallbackBase;
};


template <typename BT, template<typename>class GT, typename MyCT, typename FT>
struct ThisTypeTrait {
	static BT * createBase(const FT & func, CBAllocator * allocator) {
		return allocator->newObject<GT<FT> >(func);
	}
};

template <typename BT, template<typename>class GT, typename MyCT, typename FT>
struct ThisTypeTrait<BT, GT, MyCT, const FT> {
	static BT * createBase(const FT & func, CBAllocator * allocator) {
		return allocator->newObject<GT<const FT> >(func);
	}
};

template <typename BT, template<typename>class GT, typename FT>
struct ThisTypeTrait<BT, GT, FT, FT> {
	static BT * createBase(const FT & func, CBAllocator * allocator) {
		return func.getBase() ? func.getBase()->clone(allocator) : NULL;
	}
};

template <typename BT, template<typename>class GT, typename MyCT>
struct ThisTypeTrait<BT, GT, MyCT, GReference<MyCT> > {
	static BT * createBase(const GReference<MyCT> & func, CBAllocator * allocator) {
		return ThisTypeTrait<BT, GT, MyCT, MyCT>::createBase(func, allocator);
	}
};

template <typename BT, template<typename>class GT, typename MyCT>
struct ThisTypeTrait<BT, GT, MyCT, GReference<const MyCT> > {
	static BT * createBase(const GReference<const MyCT> & func, CBAllocator * allocator) {
		return ThisTypeTrait<BT, GT, MyCT, const MyCT>::createBase(func, allocator);
	}
};

template <typename BT, template<typename>class GT, typename MyCT>
struct ThisTypeTrait<BT, GT, MyCT, const GReference<MyCT> > {
	static BT * createBase(const GReference<MyCT> & func, CBAllocator * allocator) {
		return ThisTypeTrait<BT, GT, MyCT, MyCT>::createBase(func, allocator);
	}
};

template <typename BT, template<typename>class GT, typename MyCT>
struct ThisTypeTrait<BT, GT, MyCT, const GReference<const MyCT> > {
	static BT * createBase(const GReference<const MyCT> & func, CBAllocator * allocator) {
		return ThisTypeTrait<BT, GT, MyCT, const MyCT>::createBase(func, allocator);
	}
};


struct GCallbackVirtual
{
	void (*destructObject)(void * self);
	void * (*setOrGetObject)(const void * self, void * o, bool set);
	void * (*clone)(const void * self, CBAllocator * allocator);
	bool (*isSameCallback)(const void * self, const void * other);
	void * invoke;
};

class GCallbackFunctorBase
{
public:
	void deleteFunctor() {
		this->virtualFunctions->destructObject(this);
	}

	void * getObject() {
		return this->virtualFunctions->setOrGetObject(this, NULL, false);
	};

	const void * getObject() const {
		return this->virtualFunctions->setOrGetObject(this, NULL, false);
	};

	void setObject(void * instance) const {
		this->virtualFunctions->setOrGetObject(this, instance, true);
	}

	GCallbackFunctorBase * clone(CBAllocator * allocator) const {
		return static_cast<GCallbackFunctorBase *>(this->virtualFunctions->clone(this, allocator));
	}

	bool isSameCallback(const GCallbackFunctorBase * other) const {
		return this->virtualFunctions->isSameCallback(this, other);
	}

	void * getInvoke() const {
		return this->virtualFunctions->invoke;
	}

protected:
	GCallbackVirtual * virtualFunctions;
};

template <typename DerivedT, typename InnerOT, typename InnerFT>
class GCallbackMemberBase : public GCallbackFunctorBase {
private:
	typedef GCallbackMemberBase <DerivedT, InnerOT, InnerFT> ThisType;
	typedef GCallbackFunctorBase BaseType;
	typedef DerivedT DerivedType;

	static void virtualDestructObject(void * self) {
		(void)self;

		static_cast<ThisType *>(self)->~GCallbackMemberBase();
	}

	static void * virtualSetOrGetObject(const void * self, void * o, bool set) {
		if(set) {
			static_cast<const ThisType *>(self)->instance = reinterpret_cast<InnerOT *>(o);
			return NULL;
		}
		else {
			return reinterpret_cast<void *>(
				const_cast<char *>(
				reinterpret_cast<const volatile char *>(static_cast<const ThisType *>(self)->instance)));
		}
	}

	static void * virtualClone(const void * self, callback_internal::CBAllocator * allocator) {
		return allocator->newObject<DerivedType>(static_cast<const DerivedType *>(self)->instance, static_cast<const DerivedType *>(self)->func);
	}

	static bool virtualIsSameCallback(const void * self, const void * other) {
		return static_cast<const ThisType *>(self)->instance == static_cast<const ThisType *>(other)->instance && static_cast<const ThisType *>(self)->func == static_cast<const ThisType *>(other)->func;
	}

public:
	GCallbackMemberBase(InnerOT * instance, const InnerFT & func) : instance(instance), func(func) {
		static GCallbackVirtual thisFunctions = {
			&virtualDestructObject, &virtualSetOrGetObject,
			&virtualClone, &virtualIsSameCallback, reinterpret_cast<void *>(&DerivedType::virtualInvoke) };

		this->virtualFunctions = &thisFunctions;
	}

protected:
	mutable InnerOT * instance;
	InnerFT func;
};

namespace _test_equal {

// Go here if T has no operator ==
// But if T has a private == or declared but undefined ==, compiler or linker will raise error.
template <typename T>
inline bool operator == (const T & a, const T & b) {
	return &a == &b;
}

template <typename T>
inline bool testEqual(const T & a, const T & b) {
	return *&a == *&b;
}

} // namespace _test_equal

template <typename DerivedT, typename InnerFT>
class GCallbackGlobalBase : public GCallbackFunctorBase {
private:
	typedef GCallbackGlobalBase <DerivedT, InnerFT> ThisType;
	typedef DerivedT DerivedType;
	typedef GCallbackFunctorBase BaseType;

	static void virtualDestructObject(void * self) {
		(void)self;

		static_cast<ThisType *>(self)->~GCallbackGlobalBase();
	}

	static void * virtualSetOrGetObject(const void * self, void * o, bool set) {
		(void)self; (void)o; (void)set;

		return NULL;
	}

	static void * virtualClone(const void * self, callback_internal::CBAllocator * allocator) {
		return allocator->newObject<DerivedType>(static_cast<const DerivedType *>(self)->func);
	}

	static bool virtualIsSameCallback(const void * self, const void * other) {
		return _test_equal::testEqual<InnerFT>(static_cast<const ThisType *>(self)->func, static_cast<const ThisType *>(other)->func);
	}

public:
	GCallbackGlobalBase(const InnerFT & func) : func(func) {
		static GCallbackVirtual thisFunctions = {
			&virtualDestructObject, &virtualSetOrGetObject,
			&virtualClone, &virtualIsSameCallback, reinterpret_cast<void *>(&DerivedType::virtualInvoke) };

		this->virtualFunctions = &thisFunctions;
	}

protected:
	InnerFT func;
};


class GCallbackBase
{
public:
	typedef GCallbackFunctorBase BaseType;

public:
	GCallbackBase() {
		this->allocator.base = NULL;
	}

    GCallbackBase(const GCallbackBase & other) {
		this->allocator.base = other.getBase() ? other.getBase()->clone(&this->allocator) : NULL;
	}

	~GCallbackBase() {
		this->allocator.deleteObject(this->getBase());
	}

    GCallbackBase & operator = (const GCallbackBase & other) {
		if(this != &other) {
			void * newBase = other.getBase() ? other.getBase()->clone(&this->allocator) : NULL;
			this->allocator.deleteObject(this->getBase());
			this->allocator.base = newBase;
		}
		return *this;
	}

	bool operator == (const GCallbackBase & other) const {
		return this->getBase() ? this->getBase()->isSameCallback(other.getBase()) : (! other.getBase());
	}

	bool operator != (const GCallbackBase & other) const {
		return ! this->operator == (other);
	}

	void takeOver(GCallbackBase & other) {
		this->allocator.deleteObject(this->getBase());
		this->setBase(other.allocator.take(other.getBase(), &this->allocator));
		other.setBase(NULL);
	}

public:
	BaseType * getBase() const {
		return reinterpret_cast<BaseType * >(this->allocator.base);
	}

	void setBase(BaseType * base) {
		this->allocator.base = base;
	}

protected:
	CBAllocator allocator;

private:
	template <typename CallbackType, template<typename> class ConnectionT, template<typename T, typename = std::allocator<T> > class ListType>
	friend class GCallbackListBase;

	template <typename CallbackType>
	friend class GCallbackConnection;

	template <typename BT, template<typename>class GT, typename MyCT, typename CT>
	friend struct ThisTypeTrait;
};


template <int arity, typename FT>
class GCallbackFunctionTraits;

GPP_REPEAT_2(CB_MAX_ARITY, CB_DEF_AGENT_N, GPP_EMPTY)

GPP_REPEAT_2(CB_MAX_ARITY, CB_DEF_FUNC_TRAITS, GPP_EMPTY)


template <typename FT>
struct TypeMaybeFunctor
{
private:
	typedef typename RemoveConstVolatile<typename RemovePointer<typename RemoveConstVolatile<typename RemoveReference<FT>::Result>::Result>::Result>::Result BaseType;

public:
	enum {
		Result = 
			! IsFundamental<BaseType>::Result
			&& ! IsSameType<BaseType, void>::Result
			&& (IsFunction<BaseType>::Result
				|| IsClass<typename RemoveConstVolatile<typename RemoveReference<FT>::Result>::Result>::Result)
	};
};


} // namespace callback_internal

namespace callback_debug {

/*	This function checks if a potential callback is completed.
	It returns:
		If func is a callback for member function, return true if the target object is not NULL, return false if it's NULL.
		Otherwise, return true.
	How can this function be useful?
		It's useful to detect a very hard to track bug that passing a callback for member function with object as NULL to construct another callback,
			under this circumstance, calling the new constructed callback will always cause memory access error if the destination function reference to "this".
*/
template <typename FT>
bool isCompleteCallback(const FT & func);
} // namespace callback_debug


template<typename Signature>
class GCallback : public callback_internal::GCallbackFunctionTraits<GFunctionTraits<Signature>::Arity, Signature>::CallbackAgentType {
private:
	typedef GCallback <Signature> ThisType;
	typedef callback_internal::GCallbackFunctionTraits<GFunctionTraits<Signature>::Arity, Signature> CallbackTraitsType;
	typedef typename CallbackTraitsType::CallbackAgentType super;
	typedef typename super::BaseType BaseType;

public:
	typedef typename super::FunctionType FunctionType;
	typedef FunctionType * FunctionPointer;
	typedef typename CallbackTraitsType::FunctionTraits TraitsType;

public:
	GCallback() : super() {
	}

	template<typename OT, typename FT>
	GCallback(OT * instance, const FT & func) {
		this->setBase(this->allocator.template newObject<typename super::template GCallbackMember<OT, FT> >(instance, func));
	}

	template<typename FT>
	GCallback(const FT & func, typename GEnableIf<callback_internal::TypeMaybeFunctor<FT>::Result>::Result * = 0) {
		assert(callback_debug::isCompleteCallback(func));

		this->setBase(callback_internal::ThisTypeTrait<BaseType, super::template GCallbackGlobal, ThisType, FT>::createBase(func, &this->allocator));
	}

    GCallback(const ThisType & other) : super(other) {
	}

	GCallback & operator = (const ThisType & other) {
		super::operator = (other);
		return *this;
	}

	bool operator == (const ThisType & other) const {
		return super::operator == (other);
	}

	template <typename T>
	bool operator == (const T & other) const {
		return super::operator == (other);
	}

	bool operator != (const ThisType & other) const {
		return super::operator != (other);
	}

	template <typename T>
	bool operator != (const T & other) const {
		return super::operator != (other);
	}

	void * getObject() {
		return this->getBase() ? this->getBase()->getObject() : NULL;
	}

	const void * getObject() const {
		return this->getBase() ? this->getBase()->getObject() : NULL;
	}

	void setObject(void * instance) const {
		if(this->getBase()) {
			this->getBase()->setObject(instance);
		}
	}

	bool empty() const {
		return this->getBase() == NULL;
	}

};


GPP_REPEAT_2(CB_MAX_ARITY, CB_DEF_CALLBACK_N, GPP_EMPTY)


// auxiliary global functions and templates

template <typename FT>
struct FunctionCallbackType
{
	typedef GCallback<typename GFunctionTraits<FT>::FunctionType> Result;
};

template <typename OT, typename FT>
typename FunctionCallbackType<FT>::Result
makeCallback(OT * instance, const FT & func) {
	return GCallback<typename GFunctionTraits<FT>::FunctionType>(instance, func);
}

namespace callback_internal {

template <typename OT>
struct GlobalCallbackSelector
{
	template <typename FT>
	static typename FunctionCallbackType<FT>::Result
	makeCallback(const FT & func) {
		return GCallback<typename GFunctionTraits<FT>::FunctionType>((OT *)NULL, func);
	}
};

template <>
struct GlobalCallbackSelector <GFunctionTraitNullType>
{
	template <typename FT>
	static typename FunctionCallbackType<FT>::Result
	makeCallback(const FT & func) {
		return GCallback<typename GFunctionTraits<FT>::FunctionType>(func);
	}
};

} // namespace callback_internal

template <typename FT>
typename FunctionCallbackType<FT>::Result
makeCallback(const FT & func) {
//	return GCallback<typename GFunctionTraits<FT>::FunctionType>(func);
	return callback_internal::GlobalCallbackSelector<typename GFunctionTraits<FT>::ObjectType>::makeCallback(func);
}


template<typename Signature>
bool isMemberCallback(const GCallback<Signature> & cb) {
	const GCallback<Signature> * p = &cb;
	void * instance = p->getObject();
	p->setObject(p);
	bool isMember = (p->getObject() == p);
	p->setObject(instance);
	return isMember;
}

GPP_REPEAT_2(CB_MAX_ARITY, CB_ORD_OBJ_MAKE,		GPP_EMPTY)
GPP_REPEAT_2(CB_MAX_ARITY, CB_ORD_GLOBAL_MAKE,	GPP_EMPTY)


namespace callback_debug {

// for debug only

template <typename FT>
struct CompleteCallbackHelper {
	static bool isCompleted(const FT & /*func*/) {
		return true;
	}
};

template <typename FT>
struct CompleteCallbackHelper <GCallback<FT> > {
	static bool isCompleted(const GCallback<FT> & func) {
		return !isMemberCallback(func) || func.getObject() != NULL;
	}
};

template <typename FT>
bool isCompleteCallback(const FT & func) {
	return CompleteCallbackHelper<FT>::isCompleted(func);
}

} // namespace callback_debug


} // namespace cpgf


#undef CB_DEF_GLOBAL
#undef CB_DEF_MEMBER
#undef CB_GLOBAL_FUNC
#undef CB_OBJ_FUNC
#undef CB_ORD_GLOBAL_MAKE
#undef CB_ORD_OBJ_MAKE
#undef CB_PARAM_TYPEVALUE
#undef CB_GLOBAL_FUNC_HELPER
#undef CB_OBJ_FUNC_HELPER
#undef CB_FUNC_TRAITS_ARGS
#undef CB_DEF_FUNC_TRAITS
#undef CB_DEF_AGENT_N
#undef CB_DEF_CALLBACK_N



#endif
