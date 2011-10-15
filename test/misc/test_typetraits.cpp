#include "test_misc_common.h"

#include "cpgf/gtypetraits.h"


#define CLASS TestClass_TypeTraits

using namespace std;
using namespace cpgf;

namespace Test_TypeTraits { namespace {

class CLASS
{
public:
	CLASS() {}
	CLASS(int) {}
};

GTEST(TestTypeTraits_PointerDimension)
{
	GEQUAL(PointerDimension<CLASS>::Result, 0);
	GEQUAL(PointerDimension<CLASS &>::Result, 0);
	GEQUAL(PointerDimension<const CLASS &>::Result, 0);
	GEQUAL(PointerDimension<const CLASS * &>::Result, 0);
	GEQUAL(PointerDimension<CLASS const * volatile * &>::Result, 0);

	GEQUAL(PointerDimension<int>::Result, 0);
	GEQUAL(PointerDimension<int &>::Result, 0);
	GEQUAL(PointerDimension<const int &>::Result, 0);
	GEQUAL(PointerDimension<const int * &>::Result, 0);
	GEQUAL(PointerDimension<int const * volatile * &>::Result, 0);

	GEQUAL(PointerDimension<CLASS *>::Result, 1);
	GEQUAL(PointerDimension<CLASS const *>::Result, 1);
	GEQUAL(PointerDimension<CLASS volatile * const volatile>::Result, 1);

	GEQUAL(PointerDimension<double *>::Result, 1);
	GEQUAL(PointerDimension<double const *>::Result, 1);
	GEQUAL(PointerDimension<double volatile * const volatile>::Result, 1);

	GEQUAL(PointerDimension<CLASS **>::Result, 2);
	GEQUAL(PointerDimension<CLASS const **>::Result, 2);
	GEQUAL(PointerDimension<CLASS const * volatile *>::Result, 2);
	GEQUAL(PointerDimension<CLASS const volatile * volatile *>::Result, 2);
	GEQUAL(PointerDimension<CLASS const * volatile * volatile>::Result, 2);

	GEQUAL(PointerDimension<char **>::Result, 2);
	GEQUAL(PointerDimension<char const **>::Result, 2);
	GEQUAL(PointerDimension<char const * volatile *>::Result, 2);
	GEQUAL(PointerDimension<char const volatile * volatile *>::Result, 2);
	GEQUAL(PointerDimension<char const * volatile * volatile>::Result, 2);

	GEQUAL(PointerDimension<CLASS ***>::Result, 3);
	GEQUAL(PointerDimension<CLASS * const **>::Result, 3);
	GEQUAL(PointerDimension<CLASS * volatile * const *>::Result, 3);
	GEQUAL(PointerDimension<CLASS const volatile * const * const volatile *>::Result, 3);
	GEQUAL(PointerDimension<CLASS const volatile * const * const volatile * const volatile>::Result, 3);

	GEQUAL(PointerDimension<CLASS ***************>::Result, 15);
	GEQUAL(PointerDimension<CLASS const ***************>::Result, 15);
	GEQUAL(PointerDimension<CLASS * volatile **************>::Result, 15);
	GEQUAL(PointerDimension<CLASS **** const ****** volatile *****>::Result, 15);
	GEQUAL(PointerDimension<CLASS ***** const ****** volatile ****>::Result, 15);
	GEQUAL(PointerDimension<CLASS *************** const volatile>::Result, 15);
	GEQUAL(PointerDimension<CLASS ******** volatile * const *** const volatile *** const>::Result, 15);
	GEQUAL(PointerDimension<CLASS *** volatile **** const volatile ********>::Result, 15);
	GEQUAL(PointerDimension<CLASS **** volatile **** const volatile ******* const volatile>::Result, 15);
}


GTEST(TestTypeTraits_IsPointer)
{
	GCHECK(! IsPointer<CLASS>::Result);
	GCHECK(! IsPointer<CLASS &>::Result);
	GCHECK(! IsPointer<const CLASS &>::Result);
	GCHECK(! IsPointer<const CLASS * &>::Result);
	GCHECK(! IsPointer<CLASS const * volatile * &>::Result);

	GCHECK(! IsPointer<int>::Result);
	GCHECK(! IsPointer<int &>::Result);
	GCHECK(! IsPointer<const int &>::Result);
	GCHECK(! IsPointer<const int * &>::Result);
	GCHECK(! IsPointer<int const * volatile * &>::Result);

	GCHECK(IsPointer<CLASS *>::Result);
	GCHECK(IsPointer<CLASS const *>::Result);
	GCHECK(IsPointer<CLASS volatile * const volatile>::Result);

	GCHECK(IsPointer<double *>::Result);
	GCHECK(IsPointer<double const *>::Result);
	GCHECK(IsPointer<double volatile * const volatile>::Result);

	GCHECK(IsPointer<CLASS **>::Result);
	GCHECK(IsPointer<CLASS const **>::Result);
	GCHECK(IsPointer<CLASS const * volatile *>::Result);
	GCHECK(IsPointer<CLASS const volatile * volatile *>::Result);
	GCHECK(IsPointer<CLASS const * volatile * volatile>::Result);

	GCHECK(IsPointer<char **>::Result);
	GCHECK(IsPointer<char const **>::Result);
	GCHECK(IsPointer<char const * volatile *>::Result);
	GCHECK(IsPointer<char const volatile * volatile *>::Result);
	GCHECK(IsPointer<char const * volatile * volatile>::Result);

	GCHECK(IsPointer<CLASS ***>::Result);
	GCHECK(IsPointer<CLASS * const **>::Result);
	GCHECK(IsPointer<CLASS * volatile * const *>::Result);
	GCHECK(IsPointer<CLASS const volatile * const * const volatile *>::Result);
	GCHECK(IsPointer<CLASS const volatile * const * const volatile * const volatile>::Result);

	GCHECK(IsPointer<CLASS ***************>::Result);
	GCHECK(IsPointer<CLASS const ***************>::Result);
	GCHECK(IsPointer<CLASS * volatile **************>::Result);
	GCHECK(IsPointer<CLASS **** const ****** volatile *****>::Result);
	GCHECK(IsPointer<CLASS ***** const ****** volatile ****>::Result);
	GCHECK(IsPointer<CLASS *************** const volatile>::Result);
	GCHECK(IsPointer<CLASS ******** volatile * const *** const volatile *** const>::Result);
	GCHECK(IsPointer<CLASS *** volatile **** const volatile ********>::Result);
	GCHECK(IsPointer<CLASS **** volatile **** const volatile ******* const volatile>::Result);
}

GTEST(TestTypeTraits_IsReference)
{
	GCHECK(! IsReference<CLASS>::Result);
	GCHECK(! IsReference<const CLASS>::Result);
	GCHECK(! IsReference<CLASS *>::Result);

	GCHECK(IsReference<CLASS &>::Result);
	GCHECK(IsReference<CLASS * &>::Result);
	GCHECK(IsReference<const CLASS &>::Result);
	GCHECK(IsReference<const CLASS * const * &>::Result);
	GCHECK(IsReference<volatile CLASS * const * &>::Result);
	GCHECK(IsReference<volatile CLASS &>::Result);
}

GTEST(TestTypeTraits_IsArray)
{
	GCHECK(! IsArray<CLASS>::Result);
	GCHECK(! IsArray<const CLASS>::Result);
	GCHECK(! IsArray<CLASS *>::Result);
	GCHECK(! IsArray<const CLASS &>::Result);

	GCHECK(IsArray<CLASS[]>::Result);
	GCHECK(IsArray<const CLASS[]>::Result);
	GCHECK(IsArray<volatile CLASS[]>::Result);
	GCHECK(IsArray<const volatile CLASS[]>::Result);

	GCHECK(IsArray<CLASS[5]>::Result);
	GCHECK(IsArray<const CLASS[8]>::Result);
	GCHECK(IsArray<volatile CLASS[6]>::Result);
	GCHECK(IsArray<const volatile CLASS[1]>::Result);
}

GTEST(TestTypeTraits_IsConst)
{
	GCHECK(! IsConst<CLASS>::Result);
	GCHECK(! IsConst<CLASS &>::Result);
	GCHECK(! IsConst<const CLASS &>::Result);
	GCHECK(! IsConst<const CLASS *>::Result);
	GCHECK(! IsConst<CLASS const * const *>::Result);

	GCHECK(IsConst<const CLASS>::Result);
	GCHECK(IsConst<CLASS * const>::Result);
}

GTEST(TestTypeTraits_IsVolatile)
{
	GCHECK(! IsVolatile<CLASS>::Result);
	GCHECK(! IsVolatile<CLASS &>::Result);
	GCHECK(! IsVolatile<volatile CLASS &>::Result);
	GCHECK(! IsVolatile<volatile CLASS *>::Result);
	GCHECK(! IsVolatile<CLASS volatile * volatile *>::Result);

	GCHECK(IsVolatile<volatile CLASS>::Result);
	GCHECK(IsVolatile<CLASS * volatile>::Result);
}

GTEST(TestTypeTraits_IsConstVolatile)
{
	GCHECK(! IsConstVolatile<CLASS>::Result);
	GCHECK(! IsConstVolatile<CLASS &>::Result);
	GCHECK(! IsConstVolatile<const volatile CLASS &>::Result);
	GCHECK(! IsConstVolatile<const volatile CLASS *>::Result);
	GCHECK(! IsConstVolatile<CLASS const volatile * const volatile *>::Result);

	GCHECK(IsConstVolatile<const volatile CLASS>::Result);
	GCHECK(IsConstVolatile<CLASS * const volatile>::Result);
}

GTEST(TestTypeTraits_IsSameType)
{
	GCHECK(! IsSameType<CLASS, int>::Result);
	GCHECK(! IsSameType<CLASS, CLASS *>::Result);
	GCHECK(! IsSameType<CLASS, const CLASS>::Result);
	GCHECK(! IsSameType<const CLASS, volatile CLASS>::Result);
	GCHECK(! IsSameType<CLASS, CLASS &>::Result);

	GCHECK(IsSameType<CLASS, CLASS>::Result);
}

GTEST(TestTypeTraits_IsVoid)
{
	GCHECK(! IsVoid<CLASS>::Result);
	GCHECK(! IsVoid<void *>::Result);
	GCHECK(! IsVoid<const void *>::Result);

	GCHECK(IsVoid<void>::Result);
	GCHECK(IsVoid<const void>::Result);
	GCHECK(IsVoid<volatile void>::Result);
	GCHECK(IsVoid<const volatile void>::Result);
}

GTEST(TestTypeTraits_IsFloat)
{
	GCHECK(! IsFloat<CLASS>::Result);
	GCHECK(! IsFloat<int>::Result);
	GCHECK(! IsFloat<float *>::Result);
	GCHECK(! IsFloat<float &>::Result);

	GCHECK(IsFloat<float>::Result);
	GCHECK(IsFloat<double>::Result);
	GCHECK(IsFloat<long double>::Result);
}

GTEST(TestTypeTraits_IsSigned)
{
	GCHECK(! IsSigned<CLASS>::Result);
	GCHECK(! IsSigned<unsigned int>::Result);
	GCHECK(! IsSigned<int &>::Result);
	GCHECK(! IsSigned<char>::Result);
	GCHECK(! IsSigned<double>::Result);
	
	GCHECK(IsSigned<int>::Result);
	GCHECK(IsSigned<signed char>::Result);
}

GTEST(TestTypeTraits_IsUnsigned)
{
	GCHECK(! IsUnsigned<CLASS>::Result);
	GCHECK(! IsUnsigned<int>::Result);
	GCHECK(! IsUnsigned<unsigned int &>::Result);
	GCHECK(! IsUnsigned<char>::Result);
	GCHECK(! IsUnsigned<double>::Result);
	
	GCHECK(IsUnsigned<unsigned int>::Result);
	GCHECK(IsUnsigned<unsigned char>::Result);
}

GTEST(TestTypeTraits_IsUnknownSign)
{
	GCHECK(! IsUnknownSign<CLASS>::Result);
	GCHECK(! IsUnknownSign<int>::Result);
	GCHECK(! IsUnknownSign<unsigned int &>::Result);
	GCHECK(! IsUnknownSign<unsigned char>::Result);
	GCHECK(! IsUnknownSign<double>::Result);
	
	GCHECK(IsUnknownSign<bool>::Result);
	GCHECK(IsUnknownSign<char>::Result);
	GCHECK(IsUnknownSign<wchar_t>::Result);
}




} }
