#ifndef CPGF_GCALLBACK_H
#define CPGF_GCALLBACK_H

#include "cpgf/gfunctiontraits.h"
#include "cpgf/gtypetraits.h"

#include <type_traits>
#include <functional>
#include <memory>

namespace cpgf {

template <typename T>
T * callbackGetPointer(T * p)
{
	return p;
}

template <typename T>
T * callbackGetPointer(const std::shared_ptr<T> & p)
{
	return p.get();
}

template <typename T>
T * callbackGetPointer(const std::unique_ptr<T> & p)
{
	return p.get();
}

template <typename T>
void callbackSetPointer(T * & instance, void * p)
{
	instance = static_cast<T *>(p);
}

template <typename T>
void callbackSetPointer(std::shared_ptr<T> & instance, void * p)
{
	instance.reset(static_cast<T *>(p));
}

template <typename T>
void callbackSetPointer(std::unique_ptr<T> & instance, void * p)
{
	instance.reset(static_cast<T *>(p));
}


namespace callback_internal {

struct SizeOfCallbackBase {
	void * p;
};

struct SizeOfCallbackSon : public SizeOfCallbackBase {
	virtual void a(int) { (void)a(0); }
};

constexpr auto BufferSize = sizeof(&SizeOfCallbackSon::a) + sizeof(SizeOfCallbackBase);

template <typename BaseType>
typename std::enable_if<(sizeof(BaseType) <= BufferSize), BaseType *>::type
 allocateMemory(void * buffer)
{
	return reinterpret_cast<BaseType *>(buffer);
}

template <typename BaseType>
typename std::enable_if<(sizeof(BaseType) > BufferSize), BaseType *>::type
 allocateMemory(void * /*buffer*/)
{
	return reinterpret_cast<BaseType *>(new char[sizeof(BaseType)]);
}

template <typename T, typename... Parameters>
T * allocateBase(void * buffer, Parameters && ... parameters)
{
	T * base = allocateMemory<T>(buffer);
	new(base) T(std::forward<Parameters>(parameters)...);
	return base;
}

template <typename T, typename U>
std::false_type operator == (const T &, const U &);

template <typename T, typename U>
struct EqualOperatorExists
{
	static constexpr bool value = ! std::is_same<decltype(*& *(T*)(0) == *& *(U*)(0)), std::false_type>::value;
};

template <typename T, typename U>
bool doCheckEqual(const T & a, const U & b, typename std::enable_if<EqualOperatorExists<T, U>::value>::type * = 0)
{
	// Use * & to deal with makeReference in cpgf or reference_wrapper.
	return *&a == *&b;
}

template <typename T, typename U>
bool doCheckEqual(const T & a, const U & b, typename std::enable_if<! EqualOperatorExists<T, U>::value>::type * = 0)
{
	return (void *)&a == (void *)&b;
}

template <typename T, typename U>
bool callbackCheckEqual(const T & a, const U & b)
{
	return doCheckEqual(a, b);
}

template <typename T, typename U>
bool callbackCheckEqual(const std::reference_wrapper<T> & a, const std::reference_wrapper<U> & b)
{
	return doCheckEqual(a.get(), b.get());
}


template <typename Expect, typename Actural>
struct GCheckReturnType
{
	static constexpr bool value = std::is_convertible<Actural, Expect>::value;
};

template <typename Actural>
struct GCheckReturnType <void, Actural>
{
	static constexpr bool value = true;
};

template <typename T>
struct GReturnEmptyValue
{
	static T invoke() {
		return T();
	}
};

template <typename T>
struct GReturnEmptyValue <T &>
{
	static T & invoke() {
		return *(T *)0;
	}
};

template <typename RT, typename... Parameters>
struct GCallbackBase
{
private:
	typedef GCallbackBase <RT, Parameters...> ThisType;

public:
	typedef void (*Destroy)(ThisType * self);
	typedef ThisType * (*Clone)(ThisType * self, void * buffer);
	typedef RT (*Invoker)(ThisType * self, Parameters && ... parameters);
	typedef void * (*GetObject)(ThisType * self);
	typedef void (*SetObject)(ThisType * self, void * object);
	typedef bool (*IsSame)(ThisType * self, void * other);
	
	struct CallbackVirtuals {
		Destroy destroy;
		Clone clone;
		Invoker invoker;
		GetObject getObject;
		SetObject setObject;
		// when isSame is called, "other" is guaranteed to be the same type of "self".
		IsSame isSame;
	};
	
	explicit GCallbackBase(const CallbackVirtuals * virtuals)
		: virtuals(virtuals)
	{
	}
	
	const CallbackVirtuals * virtuals;
};

template <typename Func, typename RT, typename... Parameters>
struct GCallbackBaseFunc : public GCallbackBase <RT, Parameters...>
{
private:
	typedef GCallbackBase <RT, Parameters...> BaseType;
	typedef BaseType super;
	typedef GCallbackBaseFunc <Func, RT, Parameters...> ThisType;
	
	static void destroy(BaseType * self)
	{
		static_cast<ThisType *>(self)->~ThisType();
	}
	
	static BaseType * clone(BaseType * self, void * buffer)
	{
		return allocateBase<ThisType>(buffer, *static_cast<ThisType *>(self));
	}
	
	static RT invoker(BaseType * self, Parameters && ... parameters)
	{
		return (RT)((*&(static_cast<ThisType *>(self)->func))(std::forward<Parameters>(parameters)...));
	}
	
	static void * getObject(BaseType * /*self*/)
	{
		return nullptr;
	}
	
	static void setObject(BaseType * /*self*/, void * /*object*/)
	{
	}
	
	static bool isSame(BaseType * self, void * other)
	{
		using namespace callback_internal;

		return callbackCheckEqual(static_cast<ThisType *>(self)->func, static_cast<ThisType *>(other)->func);
	}
	
	static typename super::CallbackVirtuals * doGetVirtuals() {
		static typename super::CallbackVirtuals callbackVirtuals = {
			&destroy,
			&clone,
			&invoker,
			&getObject,
			&setObject,
			&isSame
		};
		
		return &callbackVirtuals;
	}

public:
	GCallbackBaseFunc(const Func & func)
		: super(doGetVirtuals()), func(func)
	{
	}

private:
	Func func;
};

template <typename Instance, typename OT, typename Func, typename RT, typename... Parameters>
struct GCallbackBaseMember : public GCallbackBase <RT, Parameters...>
{
private:
	typedef GCallbackBase <RT, Parameters...> BaseType;
	typedef BaseType super;
	typedef GCallbackBaseMember <Instance, OT, Func, RT, Parameters...> ThisType;
	
	static void destroy(BaseType * self)
	{
		static_cast<ThisType *>(self)->~ThisType();
	}
	
	static BaseType * clone(BaseType * self, void * buffer)
	{
		return allocateBase<ThisType>(buffer, *static_cast<ThisType *>(self));
	}
	
	static RT invoker(BaseType * self, Parameters && ... parameters)
	{
		return (RT)(
			(callbackGetPointer(static_cast<ThisType *>(self)->instance)
				->*(*&(static_cast<ThisType *>(self)->func))
			)(std::forward<Parameters>(parameters)...)
		);
	}
	
	static void * getObject(BaseType * self)
	{
		return (void *)callbackGetPointer(static_cast<ThisType *>(self)->instance);
	}
	
	static void setObject(BaseType * self, void * object)
	{
		callbackSetPointer(static_cast<ThisType *>(self)->instance, object);
	}
	
	static bool isSame(BaseType * self, void * other)
	{
		using namespace callback_internal;

		return callbackCheckEqual(static_cast<ThisType *>(self)->instance, static_cast<ThisType *>(other)->instance)
			&& callbackCheckEqual(static_cast<ThisType *>(self)->func, static_cast<ThisType *>(other)->func);
	}
	
	static typename super::CallbackVirtuals * doGetVirtuals() {
		static typename super::CallbackVirtuals callbackVirtuals = {
			&destroy,
			&clone,
			&invoker,
			&getObject,
			&setObject,
			&isSame
		};
		
		return &callbackVirtuals;
	}

public:
	GCallbackBaseMember(const Instance & instance, const Func & func)
		: super(doGetVirtuals()), instance(instance), func(func)
	{
	}

private:
	Instance instance;
	Func func;
};


} //namespace callback_internal


template <typename Signature>
class GCallback
{
};

template <typename RT, typename... Parameters>
class GCallback <RT (*)(Parameters...)>
{
private:
	typedef callback_internal::GCallbackBase <RT, Parameters...> BaseType;
	typedef GCallback <RT (*)(Parameters...)> ThisType;

public:
	typedef RT FunctionType(Parameters...);
	typedef FunctionType * FunctionPointer;
	typedef cpgf::GFunctionTraits<FunctionType> TraitsType;

public:
	GCallback() : base(nullptr)
	{
	}
	
	~GCallback()
	{
		this->doFreeBase();
	}

	template <typename FT>
	GCallback(
		const FT & func,
		typename std::enable_if<! GFunctionTraits<FT>::IsMember && IsCallable<FT, Parameters...>::Result>::type * = 0
	)
		:
			base(
				callback_internal::allocateBase<
					callback_internal::GCallbackBaseFunc<
						typename std::conditional<GFunctionTraits<FT>::IsFunction, typename GFunctionTraits<FT>::FunctionPointer, FT>::type,
						RT, Parameters...>
				>((void *)this->buffer, func)
			)
	{
	}

	template <typename FT>
	GCallback(
		const FT & func,
		typename std::enable_if<GFunctionTraits<FT>::IsMember>::type * = 0
	)
		:
			base(
				callback_internal::allocateBase<
					callback_internal::GCallbackBaseMember<typename GFunctionTraits<FT>::ObjectType *, typename GFunctionTraits<FT>::ObjectType, FT, RT,Parameters...>
				>((void *)this->buffer, (typename GFunctionTraits<FT>::ObjectType *)nullptr, func)
			)
	{
	}

	template <typename Instance, typename FT>
	GCallback(Instance instance, const FT & func)
		:
			base(
				callback_internal::allocateBase<
					callback_internal::GCallbackBaseMember<Instance, typename cpgf::GFunctionTraits<FT>::ObjectType, FT, RT, Parameters...>
				>((void *)this->buffer, instance, func)
			)
	{
	}

	GCallback(const ThisType & other)
		: base(other.base != nullptr ? other.base->virtuals->clone(other.base, (void *)this->buffer) : nullptr)
	{
	}
	
	GCallback(ThisType && other)
		: base(other.base != nullptr ? other.base->virtuals->clone(other.base, (void *)this->buffer) : nullptr)
	{
	}
	
	GCallback & operator = (const ThisType & other)
	{
		if(this != &other) {
			this->doFreeBase();
			this->base = (other.base != nullptr ? other.base->virtuals->clone(other.base, (void *)this->buffer) : nullptr);
		}
		
		return *this;
	}

	RT invoke(Parameters... args) const
	{
		if(this->base != nullptr) {
			return this->base->virtuals->invoker(this->base, std::forward<Parameters>(args)...);
		}
		else {
			return callback_internal::GReturnEmptyValue<RT>::invoke();
		}
	}
	
	RT operator() (Parameters... args) const
	{
		if(this->base != nullptr) {
			return this->base->virtuals->invoker(this->base, std::forward<Parameters>(args)...);
		}
		else {
			return callback_internal::GReturnEmptyValue<RT>::invoke();
		}
	}
	
	void * getObject() const
	{
		if(this->base != nullptr) {
			return this->base->virtuals->getObject(this->base);
		}
		else {
			return nullptr;
		}
	}

	void setObject(void * object) const
	{
		if(this->base != nullptr) {
			this->base->virtuals->setObject(this->base, object);
		}
	}
	
	void clear() {
		this->doFreeBase();
	}

	bool empty() const {
		return this->base == nullptr;
	}

	operator bool() const {
		return ! this->empty();
	}
	
	bool operator == (const ThisType & other) const
	{
		if(this->base == other.base) {
			return true;
		}
	
		if(this->base != nullptr && other && other.base != nullptr) {
			return this->base->virtuals->isSame(this->base, other.base);
		}
		else {
			return false;
		}
	}

	bool operator != (const ThisType & other) const
	{
		return ! this-> operator == (other);
	}
	
private:
	void doFreeBase()
	{
		if(this->base != nullptr) {
			this->base->virtuals->destroy(this->base);

			if(this->base != (void *)this->buffer) {
				delete reinterpret_cast<char *>(this->base);
			}

			this->base = nullptr;
		}
	}

private:
	BaseType * base;
	char buffer[callback_internal::BufferSize];
};

template <typename RT, typename ...Parameters>
class GCallback <RT (Parameters...)> : public GCallback <RT (*)(Parameters...)>
{
private:
	typedef GCallback <RT (*)(Parameters...)> super;

public:
	GCallback() : super() {}

	template <typename FT>
	GCallback(
		const FT & func,
		typename std::enable_if<! GFunctionTraits<FT>::IsMember && IsCallable<FT, Parameters...>::Result>::type * = 0
	)
		: super(func)
	{
	}

	template <typename FT>
	GCallback(
		const FT & func,
		typename std::enable_if<GFunctionTraits<FT>::IsMember>::type * = 0
	)
		: super(func)
	{
	}

	template <typename Instance, typename FT>
	GCallback(Instance instance, const FT & func)
		: super(instance, func)
	{
	}
};

template <typename FT>
struct FunctionCallbackType
{
	typedef GCallback<typename cpgf::GFunctionTraits<FT>::FunctionType> Result;
};

template <typename Signature>
struct FunctionCallbackType <GCallback<Signature> >
{
	typedef GCallback<Signature> Result;
};

template <typename OT, typename FT>
typename FunctionCallbackType<FT>::Result
makeCallback(OT * instance, const FT & func) {
	return GCallback<typename cpgf::GFunctionTraits<FT>::FunctionType>(instance, func);
}

template <typename FT>
typename FunctionCallbackType<FT>::Result
makeCallback(FT func) {
	return GCallback<typename cpgf::GFunctionTraits<FT>::FunctionType>(func);
}


} //namespace cpgf

#endif
