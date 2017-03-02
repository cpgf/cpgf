#include "test_callback_common.h"

#include "cpgf/gtypeutil.h"
#include <tuple>
#include <string>
#include <iostream>
using namespace std;

using namespace cpgf;

namespace {


template <unsigned int N>
struct Selector
{
	template <typename Tuple>
	typename std::tuple_element<N, Tuple>::type operator()(const Tuple & tuple)
	{
		return std::get<N>(tuple);
	}
};

void doTestApplier()
{
	auto args = std::make_tuple(1, "a", 2, "b");
	constexpr unsigned int index = 1;
	auto result = GTypeSelector<4>::select<typename std::tuple_element<index, decltype(args)>::type, Selector>(index, args);
	cout << result << endl;
}

GTEST(Callback_Functor_constness)
{
	doTestApplier();
};


} //unnamed namespace
