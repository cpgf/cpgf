#ifndef __GCALLBACK_P_H
#define __GCALLBACK_P_H

#include "cpgf/gcompiler.h"
#include "cpgf/gpp.h"
#include "cpgf/gconfig.h"
#include "cpgf/greference.h"
#include "cpgf/gcontainer.h"
#include "cpgf/gtypetraits.h"
#include "cpgf/genableif.h"

#include <algorithm>
#include <utility>
#include <assert.h>

#if !defined(CB_MAX_ARITY)
	#define CB_MAX_ARITY G_MAX_ARITY
#endif

#if G_SUPPORT_RVALUE_REFERENCE
	template <typename T>
	T && forwardRValue(T & value) {
		return static_cast<T &&>(value);
	}

#else
	template <typename T>
	T & forwardRValue(T & value) {
		return value;
	}
#endif


#define CB_PARAM_TYPEVALUE(N, P)		GPP_COMMA_IF(N) typename GArgumentTraits<P ## N>::Result  p ## N
#define CB_PARAM_PASSVALUE(N, P)		GPP_COMMA_IF(N) forwardRValue(p ## N)


#define CB_DEF_MEMBER(N) \
	template <typename InnerOT, typename InnerFT> \
	class GCallbackMember : public callback_internal::GCallbackMemberBase<GCallbackMember<InnerOT, InnerFT>, InnerOT, InnerFT, RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> { \
	private: \
		typedef GCallbackMember <InnerOT, InnerFT> ThisType; \
		typedef callback_internal::GCallbackMemberBase<ThisType, InnerOT, InnerFT, RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> super; \
	public: \
		GCallbackMember(InnerOT * instance, const InnerFT & func) : super(instance, func) {} \
		static RT virtualInvoke(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) { return (static_cast<ThisType *>(self)->instance->*(*&(static_cast<ThisType *>(self)->func)))(GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } \
	};

#define CB_DEF_GLOBAL(N) \
	template <typename InnerFT> \
	class GCallbackGlobal : public callback_internal::GCallbackGlobalBase<GCallbackGlobal<InnerFT>, InnerFT, RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> { \
	private: \
		typedef GCallbackGlobal <InnerFT> ThisType; \
		typedef callback_internal::GCallbackGlobalBase<ThisType, InnerFT, RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> super; \
	public: \
		GCallbackGlobal (const InnerFT & func) : super(func) {} \
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
	template<typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename PT) > \
	class GCallbackAgent_ ## N : public GCallbackBase<RT (*)(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT))> { \
	private: \
		typedef GCallbackAgent_ ## N < RT GPP_REPEAT_TAIL_PARAMS(N, PT) > ThisType; \
	protected: \
		typedef RT FunctionType(void * self GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)); \
		typedef FunctionType * FunctionPointer; \
		CB_DEF_MEMBER(N) \
		CB_DEF_GLOBAL(N) \
		template <typename RR> int doInvoke(GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT) GPP_COMMA_IF(N) typename GEnableIf<IsSameType<RR, void>::Result>::Result * = 0) const { if(this->getBase()) { ((FunctionPointer)(this->getBase()->getInvoke()))(this->getBase() GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } return 0; } \
		template <typename RR> RT doInvoke(GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT) GPP_COMMA_IF(N) typename GEnableIf<!IsSameType<RR, void>::Result>::Result * = 0) const { if(this->getBase()) { return ((FunctionPointer)(this->getBase()->getInvoke()))(this->getBase() GPP_COMMA_IF(N) GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } else { return callback_internal::ConstructDefault<RT>::construct(); } } \
	public: \
		typename cpgf::callback_internal::ReturnType<RT>::Result invoke(GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) const { return doInvoke<RT>(GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } \
		typename cpgf::callback_internal::ReturnType<RT>::Result operator () (GPP_REPEAT(N, CB_PARAM_TYPEVALUE, PT)) const { return this->doInvoke<RT>(GPP_REPEAT(N, CB_PARAM_PASSVALUE, PT)); } \
	};

namespace cpgf {


namespace callback_internal {

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
	template <typename InvokeType>
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
private:
	typedef GCallbackMemberBase<DerivedT, InnerOT, InnerFT, InvokeType> ThisType;
	typedef GCallbackFunctorBase<InvokeType> BaseType;
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
private:
	typedef GCallbackGlobalBase<DerivedT, InnerFT, InvokeType> ThisType;
	typedef DerivedT DerivedType;
	typedef GCallbackFunctorBase<InvokeType> BaseType;

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



} // namespace cpgf


#undef CB_DEF_AGENT_N
#undef CB_DEF_FUNC_TRAITS
#undef CB_DEF_GLOBAL
#undef CB_DEF_MEMBER
#undef CB_FUNC_TRAITS_ARGS
#undef CB_PARAM_TYPEVALUE



#endif
