#ifndef CPGF_GCALLBACK_P_H
#define CPGF_GCALLBACK_P_H

#include "cpgf/gcompiler.h"
#include "cpgf/gpp.h"
#include "cpgf/gconfig.h"
#include "cpgf/greference.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/genableif.h"

#include <algorithm>
#include <utility>
#include <stdexcept>
#include <assert.h>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4267)
#endif


#if !defined(CB_MAX_ARITY)
	#define CB_MAX_ARITY G_MAX_ARITY
#endif

#define CB_PARAM_TYPEVALUE(N, P)		GPP_COMMA_IF(N) typename GArgumentTraits<P ## N>::Result  p ## N
#define CB_PARAM_PASSVALUE(N, P)		GPP_COMMA_IF(N) callback_internal::ForwardValue<typename GArgumentTraits<P ## N>::Result, typename GArgumentTraits<P ## N>::Result>::forward(p ## N)


#define CB_DEF_MEMBER(N) \
	template <typename InnerOT, typename InnerFT> \
	struct GCallbackMember { \
    	typedef GCallbackMember_ ## N <InnerOT, InnerFT, RT GPP_REPEAT_TAIL_PARAMS(N, PT)> Type; \
	};

#define CB_DEF_GLOBAL(N) \
	template <typename InnerFT> \
	struct GCallbackGlobal { \
    	typedef GCallbackGlobal_ ## N <InnerFT, RT GPP_REPEAT_TAIL_PARAMS(N, PT)> Type; \
	};

#define CB_DEF_MEMBER_N(N) \
	template <typename InnerOT, typename InnerFT, typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename PT) > \
	class GCallbackMember_ ## N : public GCallbackMemberBase<GCallbackMember_ ## N <InnerOT, InnerFT, RT GPP_REPEAT_TAIL_PARAMS(N, PT)>, InnerOT, InnerFT, RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> { \
	private: \
		typedef GCallbackMember_ ## N <InnerOT, InnerFT, RT GPP_REPEAT_TAIL_PARAMS(N, PT)> ThisType; \
		typedef GCallbackMemberBase<ThisType, InnerOT, InnerFT, RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> super; \
	public: \
		GCallbackMember_ ## N(InnerOT * instance, const InnerFT & func) : super(instance, func) {} \
		static RT virtualInvoke(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) { return (static_cast<ThisType *>(self)->instance->*(*&(static_cast<ThisType *>(self)->func)))(GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } \
	};

#define CB_DEF_GLOBAL_N(N) \
	template <typename InnerFT, typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename PT) > \
	class GCallbackGlobal_ ## N : public GCallbackGlobalBase<GCallbackGlobal_ ## N <InnerFT, RT GPP_REPEAT_TAIL_PARAMS(N, PT)>, InnerFT, RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> { \
	private: \
		typedef GCallbackGlobal_ ## N <InnerFT, RT GPP_REPEAT_TAIL_PARAMS(N, PT)> ThisType; \
		typedef GCallbackGlobalBase<ThisType, InnerFT, RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> super; \
	public: \
		GCallbackGlobal_ ## N(const InnerFT & func) : super(func) {} \
		static RT virtualInvoke(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) { return (*&(static_cast<ThisType *>(self)->func))(GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } \
	};


#define CB_FUNC_TRAITS_ARGS(N, P) GPP_COMMA_IF(N) typename FunctionTraits::ArgList::Arg ## N

#define CB_DEF_FUNC_TRAITS(N, P) \
	template <typename FT> \
	struct GCallbackFunctionTraits <N, FT> { \
		typedef GFunctionTraits<FT> FunctionTraits; \
		typedef GCallbackAgent_ ## N <typename FunctionTraits::ResultType GPP_COMMA_IF(N) GPP_REPEAT(N, CB_FUNC_TRAITS_ARGS, GPP_EMPTY)> CallbackAgentType; \
	};

#define CB_DEF_AGENT_N(N, P) \
	CB_DEF_MEMBER_N(N) \
	CB_DEF_GLOBAL_N(N) \
	template<typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename PT) > \
	class GCallbackAgent_ ## N : public GCallbackBase<RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> { \
	protected: \
		typedef GCallbackAgent_ ## N < RT GPP_REPEAT_TAIL_PARAMS(N, PT) > ThisType; \
		typedef GCallbackBase<RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> super; \
		typedef typename super::BaseType BaseType; \
	protected: \
		typedef RT FunctionType(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)); \
		typedef FunctionType * FunctionPointer; \
		CB_DEF_MEMBER(N) \
		CB_DEF_GLOBAL(N) \
		template <typename RR> int doInvoke(GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT) GPP_COMMA_IF(N) typename GEnableIfResult<IsSameType<RR, void> >::Result * = 0) const { if(this->getBase()) { ((FunctionPointer)(this->getBase()->getInvoke()))(this->getBase() GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } else { invokeEmptyCallback<int>(); } return 0; } \
		template <typename RR> RT doInvoke(GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT) GPP_COMMA_IF(N) typename GDisableIfResult<IsSameType<RR, void> >::Result * = 0) const { if(this->getBase()) { return ((FunctionPointer)(this->getBase()->getInvoke()))(this->getBase() GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } else { return invokeEmptyCallback<RT>(); } } \
		template<typename OT, typename FT>	void init(OT * instance, const FT & func) { \
			this->setBase(this->allocator.template newObject<typename GCallbackMember<OT, FT>::Type >(instance, func)); \
		} \
		template<typename Derived, typename FT> void init(const FT & func) { \
			this->setBase(callback_internal::ThisTypeTrait<BaseType, GCallbackGlobal, Derived, FT>::createBase(func, &this->allocator)); \
		} \
	public: \
		typename cpgf::callback_internal::ReturnType<RT>::Result invoke(GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) const { return doInvoke<RT>(GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } \
		typename cpgf::callback_internal::ReturnType<RT>::Result operator () (GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) const { return this->doInvoke<RT>(GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } \
	};

namespace cpgf {


namespace callback_internal {

template <typename To, typename From>
struct ForwardValue
{
	static typename AddReference<To>::Result forward(typename AddReference<From>::Result value) {
		return value;
	}
};

#if G_SUPPORT_RVALUE_REFERENCE
template <typename To, typename From>
struct ForwardValue <To &&, From>
{
	static To && forward(From & value) {
		return static_cast<To &&>(value);
	}
};
#endif


template <typename RT>
struct ReturnType
{
	typedef RT Result;
};

template <>
struct ReturnType <void>
{
	typedef int Result;
};

template <typename T>
inline T invokeEmptyCallback() {
	throw std:: runtime_error("Invoking uninitialized callback.");
}

template <bool tooBig>
struct CBInplaceMeasure;

template <>
struct CBInplaceMeasure<true> {
	template <typename T, typename PT1, typename PT2>
	static T * newObject(void * /*buffer*/, const PT1 & p1, const PT2 & p2) {
		return new T(p1, p2);
	}

	template <typename T, typename PT1>
	static T * newObject(void * /*buffer*/, const PT1 & p1) {
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
		// We can't call CBInplaceMeasure::deleteObject since T here maybe a base class
		// and p points to a derived object.
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
	template <typename InvokeType>
	friend class GCallbackBase;
};


template <typename BT, template<typename>class GT, typename MyCT, typename FT>
struct ThisTypeTrait {
	static BT * createBase(const FT & func, CBAllocator * allocator) {
		return allocator->newObject<typename GT<FT>::Type >(func);
	}
};

template <typename BT, template<typename>class GT, typename MyCT, typename FT>
struct ThisTypeTrait<BT, GT, MyCT, const FT> {
	static BT * createBase(const FT & func, CBAllocator * allocator) {
		return allocator->newObject<typename GT<const FT>::Type >(func);
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


template <typename InvokeType>
struct GCallbackVirtual
{
	void (*destructObject)(void * self);
	void * (*setOrGetObject)(const void * self, void * o, bool set);
	void * (*clone)(const void * self, CBAllocator * allocator);
	bool (*isSameCallback)(const void * self, const void * other);
	InvokeType invoke;
};

template <typename InvokeType>
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

	InvokeType getInvoke() const {
		return this->virtualFunctions->invoke;
	}

protected:
	GCallbackVirtual<InvokeType> * virtualFunctions;
};

template <typename DerivedT, typename InnerOT, typename InnerFT, typename InvokeType>
class GCallbackMemberBase : public GCallbackFunctorBase <InvokeType> {
protected:
	typedef GCallbackMemberBase<DerivedT, InnerOT, InnerFT, InvokeType> ThisType;
	typedef GCallbackFunctorBase<InvokeType> BaseType;
	typedef DerivedT DerivedType;

	static void virtualDestructObject(void * self) {
		(void)self; // VC will issue unused parameter without this line?
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
		static GCallbackVirtual<InvokeType> thisFunctions = {
			&virtualDestructObject, &virtualSetOrGetObject,
			&virtualClone, &virtualIsSameCallback, &DerivedType::virtualInvoke };

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

template <typename DerivedT, typename InnerFT, typename InvokeType>
class GCallbackGlobalBase : public GCallbackFunctorBase <InvokeType> {
protected:
	typedef GCallbackGlobalBase<DerivedT, InnerFT, InvokeType> ThisType;
	typedef DerivedT DerivedType;
	typedef GCallbackFunctorBase<InvokeType> BaseType;

	static void virtualDestructObject(void * self) {
		(void)self; // 'self" is unused, weird
		static_cast<ThisType *>(self)->~GCallbackGlobalBase();
	}

	static void * virtualSetOrGetObject(const void * /*self*/, void * /*o*/, bool /*set*/) {
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
		static GCallbackVirtual<InvokeType> thisFunctions = {
			&virtualDestructObject, &virtualSetOrGetObject,
			&virtualClone, &virtualIsSameCallback, &DerivedType::virtualInvoke };

		this->virtualFunctions = &thisFunctions;
	}

protected:
	InnerFT func;
};


template <typename InvokeType>
class GCallbackBase
{
public:
	typedef GCallbackFunctorBase<InvokeType> BaseType;

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

#ifndef G_NO_MEMBER_TEMPLATE_FRIENDS
protected:
#else
public:
#endif
	CBAllocator allocator;

#ifndef G_NO_MEMBER_TEMPLATE_FRIENDS
private:
	template <typename CallbackType, template<typename> class ConnectionT, template<typename T, typename = std::allocator<T> > class ListType>
	friend class GCallbackListBase;

	template <typename CallbackType>
	friend class GCallbackConnection;

	template <typename BT, template<typename>class GT, typename MyCT, typename CT>
	friend struct ThisTypeTrait;
#endif
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
	G_STATIC_CONSTANT(bool,
		Result = (
			! IsFundamental<BaseType>::Result
			&& ! IsSameType<BaseType, void>::Result
			&& (IsFunction<BaseType>::Result
				|| IsClass<typename RemoveConstVolatile<typename RemoveReference<FT>::Result>::Result>::Result
                )
        )
	);
};


} // namespace callback_internal



} // namespace cpgf


#undef CB_DEF_AGENT_N
#undef CB_DEF_FUNC_TRAITS
#undef CB_DEF_GLOBAL
#undef CB_DEF_MEMBER
#undef CB_FUNC_TRAITS_ARGS
#undef CB_PARAM_TYPEVALUE


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif

