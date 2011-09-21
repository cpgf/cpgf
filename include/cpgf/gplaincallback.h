#ifndef __GPLAINCALLBACK_H
#define __GPLAINCALLBACK_H

#include "cpgf/gcallback.h"


#define CB_FUNC_TRAITS_ARGS(N, P) GPP_COMMA_IF(N) typename FunctionTraits::ArgList::Arg ## N p ## N

#define CB_DEF_AGENT_N(N, P) \
	template<typename Signature, typename Functor, const Functor & func > \
	class GPlainCallbackAgent { \
	private: \
		typedef GFunctionTraits<Signature> FunctionTraits; \
	public: \
		static typename FunctionTraits::ResultType invoke(GPP_REPEAT(N, CB_FUNC_TRAITS_ARGS, GPP_EMPTY)) { return func(GPP_REPEAT_PARAMS(N, p)); } \
		static typename FunctionTraits::ResultType __stdcall stdcallInvoke(GPP_REPEAT(N, CB_FUNC_TRAITS_ARGS, GPP_EMPTY)) { return func(GPP_REPEAT_PARAMS(N, p)); } \
		static typename FunctionTraits::ResultType __fastcall fastcallInvoke(GPP_REPEAT(N, CB_FUNC_TRAITS_ARGS, GPP_EMPTY)) { return func(GPP_REPEAT_PARAMS(N, p)); } \
	};

#define CB_DEF_FUNC_TRAITS(N, P) \
	template <> \
	struct GPlainCallbackFunctionTraits <N> { \
		CB_DEF_AGENT_N(N, GPP_EMPTY) \
	};


namespace cpgf {

namespace callback_internal {

template <int arity>
class GPlainCallbackFunctionTraits;

GPP_REPEAT_2(CB_MAX_ARITY, CB_DEF_FUNC_TRAITS, GPP_EMPTY)

} // namespace callback_internal


template <typename Signature, typename Functor, const Functor & func>
class GPlainFunction : public callback_internal::GPlainCallbackFunctionTraits<GFunctionTraits<Signature>::Arity>::template GPlainCallbackAgent<Signature, Functor, func>
{
};

template <typename Signature, const GCallback<Signature> & cb>
class GPlainCallback : public GPlainFunction<Signature, GCallback<Signature>, cb>
{
};


} // namespace cpgf


#undef CB_DEF_FUNC_TRAITS
#undef CB_DEF_AGENT_N
#undef CB_FUNC_TRAITS_ARGS



#endif
