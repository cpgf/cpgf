#include "cpgf/metadata/stl/gmetadata_vector.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"
#include "../../test/scriptbind/bind_common.h"

#include <vector>
#include <iostream>

using namespace cpgf;
using namespace std;

typedef vector<int> VectorType;


template <typename T>
void doTestBind(T * binding, TestScriptContext * c)
{
	GScopedPointer<TestScriptContext> context(c);

	GMetaDataNameReplacer replacer;
	initializeLuaReplacer(&replacer);
	GDefineMetaClass<VectorType> define = GDefineMetaClass<VectorType>::declare("vector_int");
	bindMetaData_vector(define, true, &replacer);

	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(define.getMetaClass())));
	binding->bindClass("vector_int", metaClass.get());

	QNEWOBJ(a, vector_int())
	QDO(it = a.begin())
	QDO(i = a._end())
	QASSERT(it.equal(a._end()))
	
	QDO(a.push_back(1))
	QDO(a.push_back(2))
	QDO(a.push_back(3))
	QASSERT(a.size() == 3)

	QASSERT(a.at(0) == 1)
	QASSERT(a.at(1) == 2)
	QASSERT(a.at(2) == 3)
	
	QASSERT(a.front() == 1)
	QASSERT(a.back() == 3)

	QDO(i = a.begin())
	QASSERT(i.value() == 1)
	QDO(i = i.inc())
	QASSERT(i.value() == 2)
	QDO(i.inc())
	QASSERT(i.value() == 3)

	QDO(a.assign(2, 5))
	QASSERT(a.size() == 2)

	QASSERT(a.at(0) == 5)
	QASSERT(a.at(1) == 5)

	context.reset();
}

void doTest(TestScriptContext * context)
{
	IScriptObject * bindingApi = context->getBindingApi();
	GScriptObject * bindingLib = context->getBindingLib();

	if(bindingLib) {
		doTestBind(bindingLib, context);
	}
	else	
	if(bindingApi) {
		doTestBind(bindingApi, context);
	}
}

void doTest()
{
//	doTest(createTestScriptContext(tslLua, tsaLib));
	doTest(createTestScriptContext(tslV8, tsaLib));
}

