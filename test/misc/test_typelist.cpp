#include "../unittestbase.h"
#include "cpgf/gtypelist.h"

#include <type_traits>

using namespace std;
using namespace cpgf;


namespace Test_TypeList { namespace {

struct TestType {};

GTEST(TestTypeList_Get)
{
	using typeList = GTypeList<int, void, TestType, const TestType &>;
	
	GCHECK(std::is_same<TypeList_Get<typeList, 0>::Result, int>::value);
	GCHECK(std::is_same<TypeList_Get<typeList, 1>::Result, void>::value);
	GCHECK(std::is_same<TypeList_Get<typeList, 2>::Result, TestType>::value);
	GCHECK(std::is_same<TypeList_Get<typeList, 3>::Result, const TestType &>::value);
}

GTEST(TestTypeList_Length)
{
	GCHECK(TypeList_Length<GTypeList<> >::Result == 0);
	GCHECK(TypeList_Length<GTypeList<int> >::Result == 1);
	GCHECK(TypeList_Length<GTypeList<int, TestType, TestType &> >::Result == 3);
}

GTEST(TestTypeList_IndexOf)
{
	using typeList = GTypeList<int, void, TestType, const TestType &>;
	
	GCHECK(TypeList_IndexOf<typeList, int>::Result == 0);
	GCHECK(TypeList_IndexOf<typeList, void>::Result == 1);
	GCHECK(TypeList_IndexOf<typeList, TestType>::Result == 2);
	GCHECK(TypeList_IndexOf<typeList, const TestType &>::Result == 3);
	GCHECK(TypeList_IndexOf<typeList, const int>::Result == -1);
}

GTEST(TestTypeList_Append)
{
	using typeList = GTypeList<>;

	using typeList1 = TypeList_Append<typeList, int>::Result;
	GCHECK(std::is_same<typeList1, GTypeList<int> >::value);

	using typeList2 = TypeList_Append<typeList1, void>::Result;
	GCHECK(std::is_same<typeList2, GTypeList<int, void> >::value);

	using typeList3 = TypeList_Append<typeList2, const TestType &>::Result;
	GCHECK(std::is_same<typeList3, GTypeList<int, void, const TestType &> >::value);
}


} }
