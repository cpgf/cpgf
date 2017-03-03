#ifndef CPGF_GTYPEUTIL_H
#define CPGF_GTYPEUTIL_H

#include "cpgf/gcallback.h"


namespace cpgf {

typedef GCallback<void * (void *)> GTypeConverterCallback;

template <typename From, typename To>
struct GTypeConverter
{
	void * operator() (void * p) const {
		return static_cast<To>(static_cast<From>(p));
	}
};


typedef GCallback<void (void *)> GTypeDeleterCallback;

template <typename T>
struct GTypeDeleter_Delete
{
	void operator() (void * p) const {
		delete static_cast<T *>(p);
	}
};


/*
GCallableApplier can be used on any callable object, not only GCallback.

Usage:
std::tuple is not a must, it can be replaced by other types, such as a type list.

template <unsigned int N>
struct Transformer
{
	template <typename Tuple>
	typename std::tuple_element<N, Tuple>::type operator()(const Tuple & tuple)
	{
		return std::get<N>(tuple);
	}
};

int func(int a, float b, double c, const std::string & s)
{
	return a + (int)b + (int)c + (int)s.size();
}

void doTest()
{
	auto cb = makeCallback(func);
	auto arg = std::make_tuple(1, 2.0f, 3.0, "abc");
	const int result = GCallableApplier<decltype(cb)::TraitsType::Arity>::apply<decltype(cb)::TraitsType::ResultType, Transformer>(cb, arg);
	// here result is 9
}
*/

template <unsigned int N>
struct GCallableApplier
{
	template <
		typename ResultType,
		template <unsigned int Index> class Transformer,
		typename Callable,
		typename TransformerParameter,
		typename... Parameters
	>
	static ResultType apply(const Callable & callable, const TransformerParameter & arg, Parameters && ... parameters)
	{
		return (ResultType)GCallableApplier<N - 1>::template apply<ResultType, Transformer>(
			callable, arg, Transformer<N - 1>()(arg), std::forward<Parameters>(parameters)...
		);
	}
};

template <>
struct GCallableApplier <0>
{
	template <
		typename ResultType,
		template <unsigned int Index> class Transformer,
		typename Callable,
		typename TransformerParameter,
		typename... Parameters
	>
	static ResultType apply(const Callable & callable, const TransformerParameter & /*arg*/, Parameters && ... parameters)
	{
		return (ResultType)callable(std::forward<Parameters>(parameters)...);
	}
};


/*
Usage:

template <unsigned int N>
struct Selector
{
	template <typename Tuple>
	typename std::tuple_element<N, Tuple>::type operator()(const Tuple & tuple)
	{
		return std::get<N>(tuple);
	}
};

void doTest()
{
	auto args = std::make_tuple(1, "a", 2, "b");
	constexpr unsigned int index = 1;
	auto result = GTypeSelector<4>::select<typename std::tuple_element<index, decltype(args)>::type, Selector>(index, args);
	cout << result << endl; // output a
}


*/
template <unsigned int Count>
struct GTypeSelector
{
	template<typename ResultType, template <unsigned int Index> class Selector, typename... SelectorParameters>
	static ResultType select(const unsigned int index, SelectorParameters && ... selectorParameters)
	{
		if(index == Count - 1) {
			return (ResultType)Selector<Count - 1>()(selectorParameters...);
		}
		else {
			return GTypeSelector<Count - 1>::template select<ResultType, Selector>(index, std::forward<SelectorParameters>(selectorParameters)...);
		}
	}
};

template <>
struct GTypeSelector <0>
{
	template<typename ResultType, template <unsigned int Index> class Selector, typename... SelectorParameters>
	static ResultType select(const unsigned int /*index*/, SelectorParameters && ... /*selectorParameters*/)
	{
		return ResultType();
	}
};


/*
Enumerate through 0..Count - 1

Usage:
template <unsigned int N>
struct Func
{
	template <typename Tuple>
	void operator()(const Tuple & tuple)
	{
		cout << std::get<N>(tuple) << endl;
	}
};

void test()
{
	auto args = std::make_tuple(1, "a", 2, "b");
	GTypeForEach<4>::forEach<Func>(args);
	// Output
	// 1
	// a
	// 2
	// b
};
*/
template <unsigned int Count, unsigned int Index = 0>
struct GTypeForEach
{
	template<template <unsigned int> class Func, typename... FuncParameters>
	static void forEach(FuncParameters && ... funcParameters)
	{
		Func<Index>()(funcParameters...);
		GTypeForEach<Count, Index + 1>::template forEach<Func>(std::forward<FuncParameters>(funcParameters)...);
	}
};

template <unsigned int Count>
struct GTypeForEach <Count, Count>
{
	template<template <unsigned int> class Func, typename... FuncParameters>
	static void forEach(FuncParameters && ... /*funcParameters*/)
	{
	}
};


} // namespace cpgf



#endif
