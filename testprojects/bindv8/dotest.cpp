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

	GDefineMetaClass<VectorType> define = GDefineMetaClass<VectorType>::declare("vector_int");
	bindMetaData_vector(define, true);

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

	QDO(i = a.begin())
	QASSERT(i.value() == 1)
	QDO(i = i.inc())
	QASSERT(i.value() == 2)

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
	doTest(createTestScriptContext(tslLua, tsaLib));
}

