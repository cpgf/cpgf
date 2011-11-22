#include "test_lua_common.h"


namespace {


GTEST(Operator)
{
	TestLuaContext * context = getLuaContext();

	GCHECK(
		context->doString(
			LINE(a = TestOperator(99))
			LINE(b = a + 5)
			LINE(scriptAssert(b.value == 99 + 5))
	
			LINE(b = a + TestOperator(8))
			LINE(scriptAssert(b.value == 99 + 8))
	
			LINE(b = a + TestObject(15))
			LINE(scriptAssert(b.value == 99 + 15))
	
			
			LINE(a = TestOperator(98))
			LINE(b = a - 5)
			LINE(scriptAssert(b.value == 98 - 5))
	
			LINE(b = a - TestOperator(8))
			LINE(scriptAssert(b.value == 98 - 8))
	
			LINE(b = a - TestObject(15))
			LINE(scriptAssert(b.value == 98 - 15))
	
			
			LINE(a = TestOperator(97))
			LINE(b = a * 5)
			LINE(scriptAssert(b.value == 97 * 5))
	
			LINE(b = a * TestOperator(8))
			LINE(scriptAssert(b.value == 97 * 8))
	
			LINE(b = a * TestObject(15))
			LINE(scriptAssert(b.value == 97 * 15))
	
			
			LINE(a = TestOperator(99))
			LINE(b = a / 5)
			LINE(scriptAssert(b.value == math.floor(99 / 5)))
	
			LINE(b = a / TestOperator(8))
			LINE(scriptAssert(b.value == math.floor(99 / 8)))
	
			LINE(b = a / TestObject(15))
			LINE(scriptAssert(b.value == math.floor(99 / 15)))
	
			
			LINE(a = TestOperator(88))
			LINE(b = a % 5)
			LINE(scriptAssert(b.value == 88 % 5))
	
			LINE(b = a % TestOperator(8))
			LINE(scriptAssert(b.value == 88 % 8))
	
			LINE(b = a % TestObject(15))
			LINE(scriptAssert(b.value == 88 % 15))
	
	
			LINE(a = TestOperator(99))
			LINE(b = -a)
			LINE(scriptAssert(b.value == -99))
			
			
			LINE(d = TestOperator(3))
			"b = d(5, 7, 9, 1, 2, 6, 8) \n"
			LINE(scriptAssert(b == 3 + 5 + 7 + 9 + 1 + 2 + 6 + 8))
			
			
		)
	);
}




}
