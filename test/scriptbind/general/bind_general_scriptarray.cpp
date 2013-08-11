#include "../testscriptbind.h"

#include <string>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

using namespace std;

namespace {

string getCodeScriptArray(TestScriptContext * context, const string & elements)
{
	string code;

	if(context->isLua()) {
		code = "{" + elements + "}";
	}
	else if(context->isPython()) {
		code = "[" + elements + "]";
	}
	else if(context->isJavascript()) {
		code = "[" + elements + "]";
	}

	return code;
}

void doCreateScriptArray(TestScriptContext * context, const string & name, const string & elements)
{
	string code = VAR + name + " = " + getCodeScriptArray(context, elements);
	DO(code);
}

string doCreateScriptArrayIndex(TestScriptContext * context, const string & name, int index)
{
	char buffer[1024];

	if(context->isLua()) {
		++index;
	}

	sprintf(buffer, "%s[%d]", name.c_str(), index);
	return buffer;
}

template <typename T>
void doTestArrayLength(T * binding, TestScriptContext * context)
{
	doCreateScriptArray(context, "a", "99, 98, 97");
	GCHECK(binding->maybeIsScriptArray("a"));

	GScriptValue scriptArrayValue(scriptGetAsScriptArray(binding, "a"));
	GScopedInterface<IScriptArray> scriptArray(scriptArrayValue.toScriptArray());
	GCHECK(scriptArray->getLength() == 3);
}

void testArrayLength(TestScriptContext * context)
{
	if(context->getBindingLib()) {
		doTestArrayLength(context->getBindingLib(), context);
	}
	
	if(context->getBindingApi()) {
		doTestArrayLength(context->getBindingApi(), context);
	}
}

#define CASE testArrayLength
#include "../bind_testcase.h"


template <typename T>
void doTestArrayGetValue(T * binding, TestScriptContext * context)
{
	doCreateScriptArray(context, "a", "99, \"abc\", 97");
	GCHECK(binding->maybeIsScriptArray("a"));

	GScriptValue scriptArrayValue(scriptGetAsScriptArray(binding, "a"));
	GScopedInterface<IScriptArray> scriptArray(scriptArrayValue.toScriptArray());
	GCHECK(scriptArray->getLength() == 3);

	GScriptValue value;

	value = scriptGetScriptArrayValue(scriptArray.get(), 0);
	GCHECK(value.isFundamental());
	GCHECK(fromVariant<int>(value.toFundamental()) == 99);
	
	value = scriptGetScriptArrayValue(scriptArray.get(), 1);
	GCHECK(value.isString());
	GCHECK(value.toString() == "abc");
}

void testArrayGetValue(TestScriptContext * context)
{
	if(context->getBindingLib()) {
		doTestArrayGetValue(context->getBindingLib(), context);
	}
	
	if(context->getBindingApi()) {
		doTestArrayGetValue(context->getBindingApi(), context);
	}
}

#define CASE testArrayGetValue
#include "../bind_testcase.h"


template <typename T>
void doTestArraySetValue(T * binding, TestScriptContext * context)
{
	doCreateScriptArray(context, "a", "99, 98, 97");
	GCHECK(binding->maybeIsScriptArray("a"));

	DOASSERT(doCreateScriptArrayIndex(context, "a", 0) + " == 99");
	DOASSERT(doCreateScriptArrayIndex(context, "a", 1) + " == 98");

	GScriptValue scriptArrayValue(scriptGetAsScriptArray(binding, "a"));
	GScopedInterface<IScriptArray> scriptArray(scriptArrayValue.toScriptArray());

	scriptSetScriptArrayValue(scriptArray.get(), 0, GScriptValue::fromFundamental(18));
	DOASSERT(doCreateScriptArrayIndex(context, "a", 0) + " == 18");

	scriptSetScriptArrayValue(scriptArray.get(), 1, GScriptValue::fromString("abc"));
	DOASSERT(doCreateScriptArrayIndex(context, "a", 1) + " == \"abc\"");
}

void testArraySetValue(TestScriptContext * context)
{
	if(context->getBindingLib()) {
		doTestArraySetValue(context->getBindingLib(), context);
	}
	
	if(context->getBindingApi()) {
		doTestArraySetValue(context->getBindingApi(), context);
	}
}

#define CASE testArraySetValue
#include "../bind_testcase.h"


template <typename T>
void doTestArrayCreateScriptArray(T * binding, TestScriptContext * context)
{
	GScriptValue scriptArrayValue(scriptCreateScriptArray(binding, "a"));
	GScopedInterface<IScriptArray> scriptArray(scriptArrayValue.toScriptArray());

	scriptSetScriptArrayValue(scriptArray.get(), 0, GScriptValue::fromFundamental(18));
	DOASSERT(doCreateScriptArrayIndex(context, "a", 0) + " == 18");

	scriptSetScriptArrayValue(scriptArray.get(), 1, GScriptValue::fromString("abc"));
	DOASSERT(doCreateScriptArrayIndex(context, "a", 1) + " == \"abc\"");
}

void testArrayCreateScriptArray(TestScriptContext * context)
{
	if(context->getBindingLib()) {
		doTestArrayCreateScriptArray(context->getBindingLib(), context);
	}
	
	if(context->getBindingApi()) {
		doTestArrayCreateScriptArray(context->getBindingApi(), context);
	}
}

#define CASE testArrayCreateScriptArray
#include "../bind_testcase.h"


template <typename T>
void doTestArray2DScriptArray(T * binding, TestScriptContext * context)
{
	string code;
	code.append(getCodeScriptArray(context, "1, 2, 3"));
	code.append(", " + getCodeScriptArray(context, "4, 5, 6"));
	code.append(", 7");
	doCreateScriptArray(context, "a", code);

	GCHECK(binding->maybeIsScriptArray("a"));

	GScriptValue scriptArrayValue(scriptGetAsScriptArray(binding, "a"));
	GScopedInterface<IScriptArray> scriptArray(scriptArrayValue.toScriptArray());

	GCHECK(scriptArray->maybeIsScriptArray(0));
	GCHECK(scriptArray->maybeIsScriptArray(1));
	GCHECK(! scriptArray->maybeIsScriptArray(2));

	GScopedInterface<IScriptArray> nestedScriptArray;

	nestedScriptArray.reset(scriptGetAsScriptArray(scriptArray.get(), 0).toScriptArray());
	GCHECK(fromVariant<int>(scriptGetScriptArrayValue(nestedScriptArray.get(), 0).toFundamental()) == 1);
	GCHECK(fromVariant<int>(scriptGetScriptArrayValue(nestedScriptArray.get(), 1).toFundamental()) == 2);
	GCHECK(fromVariant<int>(scriptGetScriptArrayValue(nestedScriptArray.get(), 2).toFundamental()) == 3);

	nestedScriptArray.reset(scriptGetAsScriptArray(scriptArray.get(), 1).toScriptArray());
	GCHECK(fromVariant<int>(scriptGetScriptArrayValue(nestedScriptArray.get(), 0).toFundamental()) == 4);
	GCHECK(fromVariant<int>(scriptGetScriptArrayValue(nestedScriptArray.get(), 1).toFundamental()) == 5);
	GCHECK(fromVariant<int>(scriptGetScriptArrayValue(nestedScriptArray.get(), 2).toFundamental()) == 6);

	GCHECK(fromVariant<int>(scriptGetScriptArrayValue(scriptArray.get(), 2).toFundamental()) == 7);
}

void testArray2DScriptArray(TestScriptContext * context)
{
	if(context->getBindingLib()) {
		doTestArray2DScriptArray(context->getBindingLib(), context);
	}
	
	if(context->getBindingApi()) {
		doTestArray2DScriptArray(context->getBindingApi(), context);
	}
}

#define CASE testArray2DScriptArray
#include "../bind_testcase.h"


}

