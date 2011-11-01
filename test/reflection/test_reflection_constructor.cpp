#include "test_reflection_common.h"

#define CLASS TestClass_Constructor
#define NAME_CLASS GPP_STRINGIZE(CLASS)

#define CTOR(f) pointerAssign(ctor, metaClass->getConstructorAt(f))

using namespace std;
using namespace cpgf;

namespace Test_Constructor { namespace {

class CLASS {
public:
	int ci;
	string cs;

public:
	// 0
	CLASS() : ci(0), cs("") {
	}

	// 1
	CLASS(const CLASS & other) : ci(other.ci), cs(other.cs) {
	}

	// 2
	CLASS(int i) : ci(i), cs("") {
	}

	// 3
	CLASS(const string & s) : ci(0), cs(s) {
	}

	// 4
	CLASS(int i, const string & s) : ci(i), cs(s) {
	}

	// 5
	CLASS(const CLASS_DATA & data) : ci(data.i), cs(data.s) {
	}

	// 6
	CLASS(const CLASS_DATA * data) : ci(data->i), cs(data->s) {
	}

	// 7
	CLASS(const GMetaVariadicParam * params) {
		int total = 0;
		for(size_t i = 0; i < params->paramCount; ++i) {
			total += cpgf::fromVariant<int>(*(params->params[i]));
		}

		this->ci = total;
		this->cs = "Sum";
	}

};

GMETA_DEFINE_CLASS(CLASS, CLASS, NAME_CLASS) {
	reflectConstructor<void * ()>();
	reflectConstructor<void * (const CLASS &)>(GMetaPolicyCopyAllConstReference());
	reflectConstructor<void * (int)>();
	reflectConstructor<void * (const string &)>(GMetaPolicyCopyAllConstReference());
	reflectConstructor<void * (int, const string &)>(GMetaPolicyCopyAllConstReference());
	reflectConstructor<void * (const CLASS_DATA &)>();
	reflectConstructor<void * (const CLASS_DATA *)>();
	reflectConstructor<void * (const GMetaVariadicParam *)>();
}


GTEST(Lib_Exists)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaConstructor * ctor;

	CTOR(0);
	GCHECK(ctor);

	CTOR(1);
	GCHECK(ctor);

	CTOR(2);
	GCHECK(ctor);

	CTOR(3);
	GCHECK(ctor);

	CTOR(4);
	GCHECK(ctor);

	CTOR(5);
	GCHECK(ctor);

	CTOR(6);
	GCHECK(ctor);

	CTOR(7);
	GCHECK(ctor);

	CTOR(8);
	GCHECK(! ctor);
}


GTEST(API_Exists)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaConstructor> ctor;

	CTOR(0);
	GCHECK(ctor);

	CTOR(1);
	GCHECK(ctor);

	CTOR(2);
	GCHECK(ctor);

	CTOR(3);
	GCHECK(ctor);

	CTOR(4);
	GCHECK(ctor);

	CTOR(5);
	GCHECK(ctor);

	CTOR(6);
	GCHECK(ctor);

	CTOR(7);
	GCHECK(ctor);

	CTOR(8);
	GCHECK(! ctor);
}


GTEST(Lib_ParamType)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaConstructor * ctor;

	CTOR(0);
	GCHECK(! ctor->isVariadic());
	GEQUAL(ctor->getParamCount(), 0);

	CTOR(1);
	GCHECK(! ctor->isVariadic());
	GEQUAL(ctor->getParamCount(), 1);
	GEQUAL(ctor->getParamType(0), createMetaType<const CLASS &>());

	CTOR(2);
	GCHECK(! ctor->isVariadic());
	GEQUAL(ctor->getParamCount(), 1);
	GEQUAL(ctor->getParamType(0), createMetaType<int>());

	CTOR(3);
	GCHECK(! ctor->isVariadic());
	GEQUAL(ctor->getParamCount(), 1);
	GEQUAL(ctor->getParamType(0), createMetaType<string>());

	CTOR(4);
	GCHECK(! ctor->isVariadic());
	GEQUAL(ctor->getParamCount(), 2);
	GEQUAL(ctor->getParamType(0), createMetaType<int>());
	GEQUAL(ctor->getParamType(1), createMetaType<const string &>());

	CTOR(5);
	GCHECK(! ctor->isVariadic());
	GEQUAL(ctor->getParamCount(), 1);
	GEQUAL(ctor->getParamType(0), createMetaType<const CLASS_DATA &>());

	CTOR(6);
	GCHECK(! ctor->isVariadic());
	GEQUAL(ctor->getParamCount(), 1);
	GEQUAL(ctor->getParamType(0), createMetaType<const CLASS_DATA *>());

	CTOR(7);
	GCHECK(ctor->isVariadic());
	GEQUAL(ctor->getParamType(0), createMetaType<const GMetaVariadicParam *>());
}


GTEST(Lib_CheckParam)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);
	
	const GMetaConstructor * ctor;

	CTOR(0);
	GCHECK(! ctor->checkParam(38, 0));
	GCHECK(! ctor->checkParam("", 1));

	CTOR(1);
	GCHECK(ctor->checkParam(CLASS(), 0));
	GCHECK(! ctor->checkParam("", 0));
	GCHECK(! ctor->checkParam("", 1));

	CTOR(2);
	GCHECK(ctor->checkParam(19, 0));
	GCHECK(! ctor->checkParam("", 0));
	GCHECK(! ctor->checkParam("", 1));

	CTOR(3);
	GCHECK(ctor->checkParam("a", 0));
	GCHECK(ctor->checkParam(string("bc"), 0));
	GCHECK(! ctor->checkParam(3, 1));
	GCHECK(! ctor->checkParam("", 1));

	CTOR(4);
	GCHECK(ctor->checkParam(19, 0));
	GCHECK(ctor->checkParam("a", 1));
	GCHECK(ctor->checkParam(string("bc"), 1));
	GCHECK(! ctor->checkParam("", 0));
	GCHECK(! ctor->checkParam(38, 1));

	CTOR(5);
	GCHECK(ctor->checkParam(CLASS_DATA(), 0));
	GCHECK(! ctor->checkParam("", 0));
	GCHECK(! ctor->checkParam("", 1));

	CTOR(6);
	GCHECK(ctor->checkParam((CLASS_DATA *)0, 0));
	GCHECK(ctor->checkParam("abc", 0)); // dangerous
	GCHECK(! ctor->checkParam("", 1));

	CTOR(7);
	GCHECK(ctor->checkParam("abc", 0));
	GCHECK(ctor->checkParam(50, 1));
	GCHECK(ctor->checkParam(50, 10));
	GCHECK(ctor->checkParam(50, 100));
	GCHECK(ctor->checkParam(50, 1000));
}


GTEST(API_CheckParam)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaConstructor> ctor;

	CTOR(0);
	GCHECK(! metaCheckParam(ctor, 38, 0));
	GCHECK(! metaCheckParam(ctor, "", 1));

	CTOR(1);
	GCHECK(metaCheckParam(ctor, CLASS(), 0));
	GCHECK(! metaCheckParam(ctor, "", 0));
	GCHECK(! metaCheckParam(ctor, "", 1));

	CTOR(2);
	GCHECK(metaCheckParam(ctor, 19, 0));
	GCHECK(! metaCheckParam(ctor, "", 0));
	GCHECK(! metaCheckParam(ctor, "", 1));

	CTOR(3);
	GCHECK(metaCheckParam(ctor, "a", 0));
	GCHECK(metaCheckParam(ctor, string("bc"), 0));
	GCHECK(! metaCheckParam(ctor, 3, 1));
	GCHECK(! metaCheckParam(ctor, "", 1));

	CTOR(4);
	GCHECK(metaCheckParam(ctor, 19, 0));
	GCHECK(metaCheckParam(ctor, "a", 1));
	GCHECK(metaCheckParam(ctor, string("bc"), 1));
	GCHECK(! metaCheckParam(ctor, "", 0));
	GCHECK(! metaCheckParam(ctor, 38, 1));

	CTOR(5);
	GCHECK(metaCheckParam(ctor, CLASS_DATA(), 0));
	GCHECK(! metaCheckParam(ctor, "", 0));
	GCHECK(! metaCheckParam(ctor, "", 1));

	CTOR(6);
	GCHECK(metaCheckParam(ctor, (CLASS_DATA *)0, 0));
	GCHECK(metaCheckParam(ctor, "abc", 0)); // dangerous
	GCHECK(! metaCheckParam(ctor, "", 1));

	CTOR(7);
	GCHECK(metaCheckParam(ctor, "abc", 0));
	GCHECK(metaCheckParam(ctor, 50, 1));
	GCHECK(metaCheckParam(ctor, 50, 10));
	GCHECK(metaCheckParam(ctor, 50, 100));
	GCHECK(metaCheckParam(ctor, 50, 1000));
}


GTEST(Lib_Construct)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaConstructor * ctor;

	CLASS * pobj;

	CTOR(0);
	pobj = (CLASS * )ctor->invoke();
	GEQUAL(pobj->ci, 0);
	GEQUAL(pobj->cs, "");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(ctor->invoke(1));

	CTOR(1);
	pobj = (CLASS * )ctor->invoke(CLASS(8, "clone me"));
	GEQUAL(pobj->ci, 8);
	GEQUAL(pobj->cs, "clone me");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(ctor->invoke(CLASS(), ""));

	CTOR(2);
	pobj = (CLASS * )ctor->invoke(5);
	GEQUAL(pobj->ci, 5);
	GEQUAL(pobj->cs, "");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(ctor->invoke(1, 2));

	CTOR(3);
	pobj = (CLASS * )ctor->invoke("haha");
	GEQUAL(pobj->ci, 0);
	GEQUAL(pobj->cs, "haha");
	metaClass->destroyInstance(pobj);
	EXCEPT_VARIANT(ctor->invoke(1));

	CTOR(4);
	pobj = (CLASS * )ctor->invoke(2, "another");
	GEQUAL(pobj->ci, 2);
	GEQUAL(pobj->cs, "another");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(ctor->invoke());

	CLASS_DATA data("oh", 38);

	CTOR(5);
	pobj = (CLASS * )ctor->invoke(data);
	GEQUAL(pobj->ci, 38);
	GEQUAL(pobj->cs, "oh");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(ctor->invoke());

	data = CLASS_DATA("?", 1999);

	CTOR(6);
	pobj = (CLASS * )ctor->invoke(&data);
	GEQUAL(pobj->ci, 1999);
	GEQUAL(pobj->cs, "?");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(ctor->invoke());

	CTOR(7);
	pobj = (CLASS * )ctor->invoke();
	GEQUAL(pobj->ci, 0);
	GEQUAL(pobj->cs, "Sum");
	metaClass->destroyInstance(pobj);
	
	CTOR(7);
	pobj = (CLASS * )ctor->invoke(1, 2, 3, 5, 6, 7, 8, 9, 10);
	GEQUAL(pobj->ci, 1 + 2 + 3 + 5 + 6 + 7 + 8 + 9 + 10);
	GEQUAL(pobj->cs, "Sum");
	metaClass->destroyInstance(pobj);
}


GTEST(API_Construct)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaConstructor> ctor;

	CLASS * pobj;

	CTOR(0);
	pobj = (CLASS * )metaInvokeConstructor(ctor);
	GEQUAL(pobj->ci, 0);
	GEQUAL(pobj->cs, "");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(metaInvokeConstructor(ctor, 1));

	CLASS clone(8, "clone me");

	CTOR(1);
	pobj = (CLASS * )metaInvokeConstructor(ctor, clone);
	GEQUAL(pobj->ci, 8);
	GEQUAL(pobj->cs, "clone me");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(metaInvokeConstructor(ctor, clone, ""));

	CTOR(2);
	pobj = (CLASS * )metaInvokeConstructor(ctor, 5);
	GEQUAL(pobj->ci, 5);
	GEQUAL(pobj->cs, "");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(metaInvokeConstructor(ctor, 1, 2));

	CTOR(3);
	pobj = (CLASS * )metaInvokeConstructor(ctor, "haha");
	GEQUAL(pobj->ci, 0);
	GEQUAL(pobj->cs, "haha");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(metaInvokeConstructor(ctor, 1));

	CTOR(4);
	pobj = (CLASS * )metaInvokeConstructor(ctor, 2, "another");
	GEQUAL(pobj->ci, 2);
	GEQUAL(pobj->cs, "another");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(metaInvokeConstructor(ctor));

	CLASS_DATA data("oh", 38);

	CTOR(5);
	pobj = (CLASS * )metaInvokeConstructor(ctor, data);
	GEQUAL(pobj->ci, 38);
	GEQUAL(pobj->cs, "oh");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(metaInvokeConstructor(ctor));

	data = CLASS_DATA("?", 1999);

	CTOR(6);
	pobj = (CLASS * )metaInvokeConstructor(ctor, &data);
	GEQUAL(pobj->ci, 1999);
	GEQUAL(pobj->cs, "?");
	metaClass->destroyInstance(pobj);
	EXCEPT_META(metaInvokeConstructor(ctor));

	CTOR(7);
	pobj = (CLASS * )metaInvokeConstructor(ctor);
	GEQUAL(pobj->ci, 0);
	GEQUAL(pobj->cs, "Sum");
	metaClass->destroyInstance(pobj);
	
	CTOR(7);
	pobj = (CLASS * )metaInvokeConstructor(ctor, 1, 2, 3, 5, 6, 7, 8, 9, 10);
	GEQUAL(pobj->ci, 1 + 2 + 3 + 5 + 6 + 7 + 8 + 9 + 10);
	GEQUAL(pobj->cs, "Sum");
	metaClass->destroyInstance(pobj);
}






} }

