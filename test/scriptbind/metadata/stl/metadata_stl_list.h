#include "metadata_stl_common.h"
#include "cpgf/metadata/stl/gmetadata_list.h"
#include "cpgf/metadata/gmetadatapolicydeduce.h"

#include <list>


template <typename Container, typename Binding>
void testStlList_Helper(Binding * binding, TestScriptContext * c, const char * className, const GTestValueProvider & valueProvider)
{
	GDefineMetaClass<Container> define = GDefineMetaClass<Container>::declare(className);
	buildMetaData_list(define, typename GMetaDataPolicyDeduce<typename Container::value_type>::Result());

	GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(metaItemToInterface(define.takeMetaClass(), true)));
	binding->bindClass(className, metaClass.get());

	GScopedPointer<TestScriptContext> context(c);

	testStlContainer_Empty(c, className, valueProvider);
	testStlContainer_Pushback(c, className, valueProvider);
	testStlContainer_Popback(c, className, valueProvider);
	testStlContainer_FrontBack(c, className, valueProvider);
	testStlContainer_Assign(c, className, valueProvider);
	testStlContainer_Set(c, className, valueProvider);

	testStlContainer_ForwardIterator(c, className, valueProvider);
	testStlContainer_BackwardIterator(c, className, valueProvider);
	testStlContainer_ReverseForwardIterator(c, className, valueProvider);
	testStlContainer_ReverseBackwardIterator(c, className, valueProvider);

	testStlContainer_Swap(c, className, valueProvider);
	testStlContainer_Erase(c, className, valueProvider);
	testStlContainer_Insert(c, className, valueProvider);
	testStlContainer_Clear(c, className, valueProvider);

	testStlContainer_Splice(c, className, valueProvider);
	testStlContainer_Remove(c, className, valueProvider);
	testStlContainer_Unique(c, className, valueProvider);
	testStlContainer_Merge(c, className, valueProvider);
	testStlContainer_Sort(c, className, valueProvider);
	testStlContainer_Reverse(c, className, valueProvider);

	context.reset();
}

