#ifndef __GCALLBACK_H
#define __GCALLBACK_H

#include "cpgf/private/gcallback_p.h"

namespace cpgf {


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

public:
	typedef typename super::FunctionType FunctionType;
	typedef FunctionType * FunctionPointer;
	typedef typename CallbackTraitsType::FunctionTraits TraitsType;

public:
	GCallback() : super() {
	}

	template<typename OT, typename FT>
	GCallback(OT * instance, const FT & func) {
		this->template init<OT, FT>(instance, func);
	}

	template<typename FT>
	GCallback(const FT & func, typename GEnableIfResult<callback_internal::TypeMaybeFunctor<FT> >::Result * = 0) {
		assert(callback_debug::isCompleteCallback(func));

		this->template init<ThisType, FT>(func);
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

	operator bool() const {
		return ! this->empty();
	}

};


// auxiliary global functions and templates

#define CB_DEF_CALLBACK_N(N, unused) \
	template<typename RT GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, typename PT) > \
	class GPP_CONCAT(GCallback, N) { \
	public: \
		typedef GCallback <RT (GPP_REPEAT_PARAMS(N, PT)) > type; \
	};

GPP_REPEAT_2(CB_MAX_ARITY, CB_DEF_CALLBACK_N, GPP_EMPTY)

#undef CB_DEF_CALLBACK_N

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

GPP_REPEAT_2(CB_MAX_ARITY, CB_ORD_OBJ_MAKE,		GPP_EMPTY)
GPP_REPEAT_2(CB_MAX_ARITY, CB_ORD_GLOBAL_MAKE,	GPP_EMPTY)

#undef CB_ORD_OBJ_MAKE
#undef CB_GLOBAL_FUNC_HELPER
#undef CB_OBJ_FUNC_HELPER


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
makeCallback(FT func) {
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


#undef CB_GLOBAL_FUNC
#undef CB_OBJ_FUNC
#undef CB_ORD_GLOBAL_MAKE


#endif
