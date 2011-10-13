#include "test_reflection_common.h"

#define CLASS TestClass_Method
#define NAME_CLASS GPP_STRINGIZE(CLASS)

using namespace std;
using namespace cpgf;

namespace Test_Method { namespace {

class CLASS {
public:
	int fieldMethodInt;
	string fieldMethodString;

public:
	CLASS() : fieldMethodInt(0) {} // class CLASS

	int methodGetInt() const {
		return fieldMethodInt;
	}

	virtual void methodAddInt(int n) {
		fieldMethodInt += n;
	}

	string & methodRefString() {
		return fieldMethodString;
	}

	string methodConcatString(const string & append) {
		fieldMethodString = fieldMethodString + append;
		return fieldMethodString;
	}

	CLASS_DATA methodAddData(CLASS_DATA data) const {
		data.s = data.s + fieldMethodString;
		data.i += fieldMethodInt;

		return data;
	}

	void methodMakeData(CLASS_DATA & data) const {
		data.s = fieldMethodString;
		data.i = fieldMethodInt;
	}

	void methodMakeDataByPointer(CLASS_DATA * data) const {
		data->s = fieldMethodString;
		data->i = fieldMethodInt;
	}

	void methodManyParams(
		char c,	int i, long long ll,
		double d, string s, const char * pc,
		CLASS_DATA data
	) {
		GEQUAL(c, 'A');
		GEQUAL(i, 38);
		GEQUAL(ll, 9876532198765321LL);
		GNEAR(d, 1.99, 0.0001);
		GEQUAL(s, "Many");
		GEQUAL(string(pc), string("Too Many"));
		GEQUAL(data.s, "Data");
		GEQUAL(data.i, 8);
	}

	int methodSum(const GMetaVariadicParam * params) const {
		int total = 0;
		for(size_t i = 0; i < params->paramCount; ++i) {
			total += cpgf::fromVariant<int>(*(params->params[i]));
		}

		return total;
	}

}; // class CLASS

GMETA_DEFINE_CLASS(CLASS, CLASS, NAME_CLASS) {
	GMETA_METHOD(methodGetInt);
	GMETA_METHOD(methodAddInt);
	GMETA_METHOD(methodRefString);
	GMETA_METHOD(methodConcatString);
	GMETA_METHOD(methodAddData);
	GMETA_METHOD(methodMakeData);
	GMETA_METHOD(methodMakeDataByPointer);
	GMETA_METHOD(methodManyParams);
	GMETA_METHOD(methodSum);
}


GTEST(Lib_Exists)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaMethod * method;

	METHOD(methodGetInt);
	GCHECK(method);

	METHOD(methodAddInt);
	GCHECK(method);

	METHOD(methodRefString);
	GCHECK(method);

	METHOD(methodConcatString);
	GCHECK(method);

	METHOD(methodAddData);
	GCHECK(method);

	METHOD(methodMakeData);
	GCHECK(method);

	METHOD(methodMakeDataByPointer);
	GCHECK(method);

	METHOD(methodManyParams);
	GCHECK(method);

	METHOD(methodSum);
	GCHECK(method);
}


GTEST(API_Exists)
{
	GApiScopedPointer<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GApiScopedPointer<IMetaMethod> method;

	METHOD(methodGetInt);
	GCHECK(method);

	METHOD(methodAddInt);
	GCHECK(method);

	METHOD(methodRefString);
	GCHECK(method);

	METHOD(methodConcatString);
	GCHECK(method);

	METHOD(methodAddData);
	GCHECK(method);

	METHOD(methodMakeData);
	GCHECK(method);

	METHOD(methodMakeDataByPointer);
	GCHECK(method);

	METHOD(methodManyParams);
	GCHECK(method);

	METHOD(methodSum);
	GCHECK(method);
}


GTEST(Lib_ResultType)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaMethod * method;

	METHOD(methodGetInt);
	GEQUAL(method->getResultType(), createMetaType<int>());
	GCHECK(method->hasResult());

	METHOD(methodAddInt);
	GEQUAL(method->getResultType(), createMetaType<void>());
	GCHECK(! method->hasResult());

	METHOD(methodRefString);
	GEQUAL(method->getResultType(), createMetaType<string &>());
	GCHECK(method->hasResult());
	GCHECK(! method->getItemType().isConstFunction());

	METHOD(methodConcatString);
	GEQUAL(method->getResultType(), createMetaType<string>());
	GCHECK(method->hasResult());

	METHOD(methodAddData);
	GEQUAL(method->getResultType(), createMetaType<CLASS_DATA>());
	GCHECK(method->hasResult());

	METHOD(methodMakeData);
	GEQUAL(method->getResultType(), createMetaType<void>());
	GCHECK(! method->hasResult());
	GCHECK(method->getItemType().isConstFunction()); // class CLASS

	METHOD(methodMakeDataByPointer);
	GEQUAL(method->getResultType(), createMetaType<void>());
	GCHECK(! method->hasResult());

	METHOD(methodManyParams);
	GEQUAL(method->getResultType(), createMetaType<void>());
	GCHECK(! method->hasResult());

	METHOD(methodSum);
	GEQUAL(method->getResultType(), createMetaType<int>());
	GCHECK(method->hasResult());
}


GTEST(Lib_ParamType)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaMethod * method;

	METHOD(methodGetInt);
	GCHECK(! method->isVariadic());
	GEQUAL(method->getParamCount(), 0);

	METHOD(methodAddInt);
	GCHECK(! method->isVariadic());
	GEQUAL(method->getParamCount(), 1);
	GEQUAL(method->getParamType(0), createMetaType<int>());

	METHOD(methodRefString);
	GCHECK(! method->isVariadic());
	GEQUAL(method->getParamCount(), 0);

	METHOD(methodConcatString);
	GCHECK(! method->isVariadic());
	GEQUAL(method->getParamCount(), 1);
	GEQUAL(method->getParamType(0), createMetaType<string>());

	METHOD(methodAddData);
	GCHECK(! method->isVariadic());
	GEQUAL(method->getParamCount(), 1);
	GEQUAL(method->getParamType(0), createMetaType<CLASS_DATA>());

	METHOD(methodMakeData);
	GCHECK(! method->isVariadic());
	GEQUAL(method->getParamCount(), 1);
	GEQUAL(method->getParamType(0), createMetaType<CLASS_DATA>());

	METHOD(methodMakeDataByPointer);
	GCHECK(! method->isVariadic());
	GEQUAL(method->getParamCount(), 1);
	GEQUAL(method->getParamType(0), createMetaType<CLASS_DATA *>());

	METHOD(methodManyParams);
	GCHECK(! method->isVariadic());
	GEQUAL(method->getParamCount(), 7);
	GEQUAL(method->getParamType(0), createMetaType<char>());
	GEQUAL(method->getParamType(1), createMetaType<int>());
	GEQUAL(method->getParamType(2), createMetaType<long long>());
	GEQUAL(method->getParamType(3), createMetaType<double>());
	GEQUAL(method->getParamType(4), createMetaType<string>());
	GEQUAL(method->getParamType(5), createMetaType<const char *>());
	GEQUAL(method->getParamType(6), createMetaType<CLASS_DATA>());

	METHOD(methodSum);
	GCHECK(method->isVariadic());
	GEQUAL(method->getParamCount(), 1);
	GEQUAL(method->getParamType(0), createMetaType<const GMetaVariadicParam *>());
}


GTEST(Lib_CheckParam)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaMethod * method;

	METHOD(methodGetInt);
	GCHECK(! method->checkParam(38, 0));
	GCHECK(! method->checkParam(38, 50));

	METHOD(methodAddInt);
	GCHECK(method->checkParam(38, 0));
	GCHECK(! method->checkParam("d", 0));
	GCHECK(! method->checkParam(38, 1));
	GCHECK(! method->checkParam(38, 130));

	METHOD(methodRefString);
	GCHECK(! method->checkParam(38, 0));
	GCHECK(! method->checkParam(38, 68));

	METHOD(methodConcatString);
	GCHECK(method->checkParam("abc", 0));
	GCHECK(method->checkParam(string("abc"), 0));
	GCHECK(! method->checkParam(3, 0));
	GCHECK(! method->checkParam(38, 1));
	GCHECK(! method->checkParam(38, 130));

	METHOD(methodAddData);
	GCHECK(method->checkParam(string("abc"), 0)); // even it passes, it will cause crash if we real do the invoke.
	GCHECK(method->checkParam((void *)0, 0)); // again, dangerous
	GCHECK(method->checkParam(CLASS_DATA(3), 0));
	GCHECK(! method->checkParam("abc", 0));
	GCHECK(! method->checkParam(38, 1));
	GCHECK(! method->checkParam(38, 130));

	METHOD(methodMakeData);
	GCHECK(method->checkParam(CLASS_DATA(3), 0));
	GCHECK(! method->checkParam("abc", 0));
	GCHECK(! method->checkParam(38, 1));

	METHOD(methodMakeDataByPointer);
	GCHECK(method->checkParam((CLASS_DATA *)0, 0));
	GCHECK(method->checkParam("abc", 0)); // dangerous
	GCHECK(! method->checkParam(CLASS_DATA(3), 0));
	GCHECK(! method->checkParam(38, 1));

	METHOD(methodManyParams);
	GCHECK(method->checkParam(98765321L, 0));
	GCHECK(method->checkParam(98765321L, 1));
	GCHECK(method->checkParam(98765321L, 2));
	GCHECK(method->checkParam(98765321L, 3));
	GCHECK(method->checkParam(98765321.5, 3));

	METHOD(methodSum);
	GCHECK(method->checkParam(38, 0));
	GCHECK(method->checkParam("abc", 1));
	GCHECK(method->checkParam(38, 10));
	GCHECK(method->checkParam(38, 100));
	GCHECK(method->checkParam(38, 1000));
}


GTEST(API_CheckParam)
{
	GApiScopedPointer<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GApiScopedPointer<IMetaMethod> method;

	METHOD(methodGetInt);
	GCHECK(! metaCheckParam(method, 38, 0));
	GCHECK(! metaCheckParam(method, 38, 50));

	METHOD(methodAddInt);
	GCHECK(metaCheckParam(method, 38, 0));
	GCHECK(! metaCheckParam(method, "d", 0));
	GCHECK(! metaCheckParam(method, 38, 1));
	GCHECK(! metaCheckParam(method, 38, 130));

	METHOD(methodRefString);
	GCHECK(! metaCheckParam(method, 38, 0));
	GCHECK(! metaCheckParam(method, 38, 68));

	METHOD(methodConcatString);
	GCHECK(metaCheckParam(method, "abc", 0));
	GCHECK(metaCheckParam(method, string("abc"), 0));
	GCHECK(! metaCheckParam(method, 3, 0));
	GCHECK(! metaCheckParam(method, 38, 1));
	GCHECK(! metaCheckParam(method, 38, 130));

	METHOD(methodAddData);
	GCHECK(metaCheckParam(method, string("abc"), 0)); // even it passes, it will cause crash if we real do the invoke.
	GCHECK(metaCheckParam(method, (void *)0, 0)); // again, dangerous
	GCHECK(metaCheckParam(method, CLASS_DATA(3), 0));
	GCHECK(! metaCheckParam(method, "abc", 0));
	GCHECK(! metaCheckParam(method, 38, 1));
	GCHECK(! metaCheckParam(method, 38, 130));

	METHOD(methodMakeData);
	GCHECK(metaCheckParam(method, CLASS_DATA(3), 0));
	GCHECK(! metaCheckParam(method, "abc", 0));
	GCHECK(! metaCheckParam(method, 38, 1));

	METHOD(methodMakeDataByPointer);
	GCHECK(metaCheckParam(method, (CLASS_DATA *)0, 0));
	GCHECK(metaCheckParam(method, "abc", 0)); // dangerous
	GCHECK(! metaCheckParam(method, CLASS_DATA(3), 0));
	GCHECK(! metaCheckParam(method, 38, 1));

	METHOD(methodManyParams);
	GCHECK(metaCheckParam(method, 98765321L, 0));
	GCHECK(metaCheckParam(method, 98765321L, 1));
	GCHECK(metaCheckParam(method, 98765321L, 2));
	GCHECK(metaCheckParam(method, 98765321L, 3));
	GCHECK(metaCheckParam(method, 98765321.5, 3));

	METHOD(methodSum);
	GCHECK(metaCheckParam(method, 38, 0));
	GCHECK(metaCheckParam(method, "abc", 1));
	GCHECK(metaCheckParam(method, 38, 10));
	GCHECK(metaCheckParam(method, 38, 100));
	GCHECK(metaCheckParam(method, 38, 1000));
}


GTEST(Lib_Invoke)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaMethod * method;

	CLASS instance;
	CLASS * pobj = &instance;

	pobj->fieldMethodInt = 910;
	METHOD(methodGetInt);
	GEQUAL(fromVariant<int>(method->invoke(pobj)), 910);
	EXCEPT_META(method->invoke(pobj, 1));

	METHOD(methodAddInt);
	method->invoke(pobj, 2);
	GEQUAL(pobj->fieldMethodInt, 912);
	EXCEPT_META(method->invoke(pobj, 1, 5));

	pobj->fieldMethodString = "";
	METHOD(methodRefString);
	string & refString = fromVariant<string &>(method->invoke(pobj));
	GEQUAL(&pobj->fieldMethodString, &refString);
	refString = "reffff";
	GEQUAL(pobj->fieldMethodString, "reffff");
	EXCEPT_META(method->invoke(pobj, 1));

	pobj->fieldMethodString = "a";
	METHOD(methodConcatString);
	string s = fromVariant<string>(method->invoke(pobj, "bc"));
	GEQUAL(s, "abc");
	GEQUAL(pobj->fieldMethodString, "abc");
	EXCEPT_META(method->invoke(pobj, 1, 2));

	pobj->fieldMethodString = "bc";
	pobj->fieldMethodInt = 6;
	CLASS_DATA data("a", 5);
	METHOD(methodAddData);
	CLASS_DATA newData = fromVariant<CLASS_DATA>(method->invoke(pobj, data));
	GEQUAL(data.s, "a");
	GEQUAL(data.i, 5);
	GEQUAL(newData.s, "abc");
	GEQUAL(newData.i, 11);
	EXCEPT_META(method->invoke(pobj));

	pobj->fieldMethodString = "abc";
	pobj->fieldMethodInt = 6;
	data = CLASS_DATA("", 0);
	GDIFF(data.s, "abc");
	GDIFF(data.i, 6);
	METHOD(methodMakeData);
	method->invoke(pobj, data);
	GEQUAL(data.s, "abc");
	GEQUAL(data.i, 6);
	EXCEPT_META(method->invoke(pobj, 1, 2));

	pobj->fieldMethodString = "abc";
	pobj->fieldMethodInt = 6;
	data = CLASS_DATA("", 0);
	GDIFF(data.s, "abc");
	GDIFF(data.i, 6);
	METHOD(methodMakeDataByPointer);
	method->invoke(pobj, &data);
	GEQUAL(data.s, "abc");
	GEQUAL(data.i, 6);
	EXCEPT_META(method->invoke(pobj, 1, 2));

	METHOD(methodManyParams);
	method->invoke(pobj, 'A', 38, 9876532198765321LL, 1.99, "Many", "Too Many", CLASS_DATA("Data", 8));
	EXCEPT_META(method->invoke(pobj, 1));

	METHOD(methodSum);
	GEQUAL(fromVariant<int>(method->invoke(pobj)), (0));
	GEQUAL(fromVariant<int>(method->invoke(pobj, 1, 2, 3)), (1 + 2 + 3));
	GEQUAL(fromVariant<int>(method->invoke(pobj,
		18, 56, 102, 192, 3103, 39, 52, 691, 819, 130, 397, 19385
		)), (
		18 + 56 + 102 + 192 + 3103 + 39 + 52 + 691 + 819 + 130 + 397 + 19385
		));
}


GTEST(API_Invoke)
{
	GApiScopedPointer<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GApiScopedPointer<IMetaMethod> method;

	CLASS instance;
	CLASS * pobj = &instance;

	pobj->fieldMethodInt = 910;
	METHOD(methodGetInt);
	GEQUAL(fromVariant<int>(metaInvokeMethod(method, pobj)), 910);
	EXCEPT_META(metaInvokeMethod(method, pobj, 1));

	METHOD(methodAddInt);
	metaInvokeMethod(method, pobj, 2);
	GEQUAL(pobj->fieldMethodInt, 912);
	EXCEPT_META(metaInvokeMethod(method, pobj));

	pobj->fieldMethodString = "";
	METHOD(methodRefString);
	string & refString = fromVariant<string &>(metaInvokeMethod(method, pobj));
	GEQUAL(&pobj->fieldMethodString, &refString);
	refString = "reffff";
	GEQUAL(pobj->fieldMethodString, "reffff");
	EXCEPT_META(metaInvokeMethod(method, pobj, 1));

	pobj->fieldMethodString = "a";
	METHOD(methodConcatString);
	string s = fromVariant<string>(metaInvokeMethod(method, pobj, "bc"));
	GEQUAL(s, "abc");
	GEQUAL(pobj->fieldMethodString, "abc");
	EXCEPT_META(metaInvokeMethod(method, pobj, 1, 2));

	pobj->fieldMethodString = "bc";
	pobj->fieldMethodInt = 6;
	CLASS_DATA data("a", 5);
	METHOD(methodAddData);
	CLASS_DATA newData = fromVariant<CLASS_DATA>(metaInvokeMethod(method, pobj, data));
	GEQUAL(data.s, "a");
	GEQUAL(data.i, 5);
	GEQUAL(newData.s, "abc");
	GEQUAL(newData.i, 11);
	EXCEPT_META(metaInvokeMethod(method, pobj, 1, 3));

	pobj->fieldMethodString = "abc";
	pobj->fieldMethodInt = 6;
	data = CLASS_DATA("", 0);
	GDIFF(data.s, "abc");
	GDIFF(data.i, 6);
	METHOD(methodMakeData);
	metaInvokeMethod(method, pobj, data);
	GEQUAL(data.s, "abc");
	GEQUAL(data.i, 6);
	EXCEPT_META(metaInvokeMethod(method, pobj));

	pobj->fieldMethodString = "abc";
	pobj->fieldMethodInt = 6;
	data = CLASS_DATA("", 0);
	GDIFF(data.s, "abc");
	GDIFF(data.i, 6);
	METHOD(methodMakeDataByPointer);
	metaInvokeMethod(method, pobj, &data);
	GEQUAL(data.s, "abc");
	GEQUAL(data.i, 6);
	EXCEPT_META(metaInvokeMethod(method, pobj, 1, 5));

	METHOD(methodManyParams);
	metaInvokeMethod(method, pobj, 'A', 38, 9876532198765321LL, 1.99, "Many", "Too Many", CLASS_DATA("Data", 8));
	EXCEPT_META(metaInvokeMethod(method, pobj, 1));

	METHOD(methodSum);
	GEQUAL(fromVariant<int>(metaInvokeMethod(method, pobj)), (0));
	GEQUAL(fromVariant<int>(metaInvokeMethod(method, pobj, 1, 2, 3)), (1 + 2 + 3));
	GEQUAL(fromVariant<int>(metaInvokeMethod(method, pobj,
		18, 56, 102, 192, 3103, 39, 52, 691, 819, 130, 397, 19385
		)), (
		18 + 56 + 102 + 192 + 3103 + 39 + 52 + 691 + 819 + 130 + 397 + 19385
		));
}


GTEST(Lib_Execute)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaMethod * method;

	CLASS instance;
	CLASS * pobj = &instance;

	GVariant params[20];

	pobj->fieldMethodInt = 910;
	METHOD(methodGetInt);
	GEQUAL(fromVariant<int>(method->execute(pobj, params, 0)), 910);

	METHOD(methodAddInt);
	method->invoke(pobj, 2);
	GEQUAL(pobj->fieldMethodInt, 912);

	pobj->fieldMethodString = "";
	METHOD(methodRefString);
	string & refString = fromVariant<string &>(method->execute(pobj, params, 0));
	GEQUAL(&pobj->fieldMethodString, &refString);
	refString = "reffff";
	GEQUAL(pobj->fieldMethodString, "reffff");

	pobj->fieldMethodString = "a";
	METHOD(methodConcatString);
	params[0] = "bc";
	string s = fromVariant<string>(method->execute(pobj, params, 1));
	GEQUAL(s, "abc");
	GEQUAL(pobj->fieldMethodString, "abc");

	pobj->fieldMethodString = "bc";
	pobj->fieldMethodInt = 6;
	CLASS_DATA data("a", 5);
	METHOD(methodAddData);
	params[0] = data;
	CLASS_DATA newData = fromVariant<CLASS_DATA>(method->execute(pobj, params, 1));
	GEQUAL(data.s, "a");
	GEQUAL(data.i, 5);
	GEQUAL(newData.s, "abc");
	GEQUAL(newData.i, 11);

	pobj->fieldMethodString = "abc";
	pobj->fieldMethodInt = 6;
	data = CLASS_DATA("", 0);
	GDIFF(data.s, "abc");
	GDIFF(data.i, 6);
	METHOD(methodMakeData);
	params[0] = data;
	method->execute(pobj, params, 1);
	GEQUAL(data.s, "abc");
	GEQUAL(data.i, 6);

	pobj->fieldMethodString = "abc";
	pobj->fieldMethodInt = 6;
	data = CLASS_DATA("", 0);
	GDIFF(data.s, "abc");
	GDIFF(data.i, 6);
	METHOD(methodMakeDataByPointer);
	params[0] = &data;
	method->execute(pobj, params, 1);
	GEQUAL(data.s, "abc");
	GEQUAL(data.i, 6);

	METHOD(methodManyParams);
	params[0] = 'A'; params[1] = 38; params[2] = 9876532198765321LL;
	params[3] = 1.99; params[4] = "Many"; params[5] = "Too Many";
	data = CLASS_DATA("Data", 8);
	params[6] = data;
	method->execute(pobj, params, 7);

	METHOD(methodSum);
	GEQUAL(fromVariant<int>(method->execute(pobj, params, 0)), (0));
	params[0] = 1; params[1] = 2; params[2] = 3;
	GEQUAL(fromVariant<int>(method->execute(pobj, params, 3)), (1 + 2 + 3));
	params[0] = 18; params[1] = 56; params[2] = 102; params[3] = 192; params[4] = 3103;
	params[5] = 39; params[6] = 52; params[7] = 691, params[8] = 819; params[9] = 130;
	params[10] = 397; params[11] = 19385;
	GEQUAL(fromVariant<int>(method->execute(pobj, params, 12)), (
		18 + 56 + 102 + 192 + 3103 + 39 + 52 + 691 + 819 + 130 + 397 + 19385
		));
}


GTEST(Lib_PassParamByValueAndRef)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaMethod * method;

	CLASS instance;
	CLASS * pobj = &instance;
	
	CLASS_DATA data("abc", 5);
	CLASS_DATA newData;

	pobj->fieldMethodInt = 2;
	pobj->fieldMethodString = "def";

	METHOD(methodAddData);
	newData = fromVariant<CLASS_DATA>(method->invoke(pobj, data));
	GEQUAL(data.i, 5); // data should not be changed because it was passed by value
	GDIFF(newData, data);
	GEQUAL(newData, pobj->methodAddData(data));

	pobj->fieldMethodInt = 2;
	pobj->fieldMethodString = "def";
	data.reset();

	GDIFF(data.i, pobj->fieldMethodInt);
	GDIFF(data.s, pobj->fieldMethodString);
	METHOD(methodMakeData);
	method->invoke(pobj, data);
	GEQUAL(data.i, pobj->fieldMethodInt);
	GEQUAL(data.s, pobj->fieldMethodString);
	EXCEPT_VARIANT(method->invoke(pobj, 1))

	pobj->fieldMethodInt = 2;
	pobj->fieldMethodString = "def";
	data.reset();

	GDIFF(data.i, pobj->fieldMethodInt);
	GDIFF(data.s, pobj->fieldMethodString);
	METHOD(methodMakeDataByPointer);
	method->invoke(pobj, &data);
	GEQUAL(data.i, pobj->fieldMethodInt);
	GEQUAL(data.s, pobj->fieldMethodString);
	EXCEPT_VARIANT(method->invoke(pobj, data))
}



} }

