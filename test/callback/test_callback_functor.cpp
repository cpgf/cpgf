#include "test_callback_common.h"

#include "cpgf/gtypeutil.h"
#include <iostream>
#include <tuple>
using namespace std;

using namespace cpgf;

namespace {

template <unsigned int N>
struct Func
{
	template <typename Tuple>
	void operator()(const Tuple & tuple)
	{
		cout << std::get<N>(tuple) << endl;
	}
};


GTEST(Callback_Functor_constness)
{
	auto args = std::make_tuple(1, "a", 2, "b");
	GTypeForEach<4>::forEach<Func>(args);
};


} //unnamed namespace
