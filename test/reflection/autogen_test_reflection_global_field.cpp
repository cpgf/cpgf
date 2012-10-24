// Auto generated test code
// Don't modify manually.

#define AUTOGEN_TEST
#include "test_reflection_common.h"

#define CLASS void
#define NAME_CLASS GPP_STRINGIZE(CLASS)

using namespace std;
using namespace cpgf;

namespace Test_GlobalField { namespace {



	int fieldInt;
	string fieldString;
	
	CLASS_DATA fieldData;
	
	int fieldReadonlyInt;
	string fieldWriteonlyString;
	CLASS_DATA fieldNoncopyableData;



G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaGlobal()
		

		._field("fieldInt", &fieldInt)
		._field("fieldString", &fieldString)
		._field("fieldData", &fieldData)
		._field("fieldReadonlyInt", &fieldReadonlyInt, GMetaPolicyReadOnly())
		._field("fieldWriteonlyString", &fieldWriteonlyString, GMetaPolicyWriteOnly())
		._field("fieldNoncopyableData", &fieldNoncopyableData, GMetaPolicyNoncopyable())
	;
}


GTEST(Lib_Exists)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);
	
	const GMetaField * field;

	FIELD(fieldInt);
	GCHECK(field);
	
	FIELD(fieldString);
	GCHECK(field);
	
	FIELD(fieldData);
	GCHECK(field);
	
	FIELD(fieldReadonlyInt);
	GCHECK(field);
	
	FIELD(fieldWriteonlyString);
	GCHECK(field);
	
	FIELD(fieldNoncopyableData);
	GCHECK(field);
}


GTEST(API_Exists)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;

	FIELD(fieldInt);
	GCHECK(field);
	
	FIELD(fieldString);
	GCHECK(field);
	
	FIELD(fieldData);
	GCHECK(field);
	
	FIELD(fieldReadonlyInt);
	GCHECK(field);
	
	FIELD(fieldWriteonlyString);
	GCHECK(field);
	
	FIELD(fieldNoncopyableData);
	GCHECK(field);
}


GTEST(Lib_CheckItemType)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);
	
	const GMetaField * field;

	FIELD(fieldInt);
	GEQUAL(field->getItemType(), createMetaType<int>());
	
	FIELD(fieldString);
	GEQUAL(field->getItemType(), createMetaType<string>());
	
	FIELD(fieldData);
	GEQUAL(field->getItemType(), createMetaType<CLASS_DATA>());
	
	FIELD(fieldReadonlyInt);
	GEQUAL(field->getItemType(), createMetaType<int>());
	
	FIELD(fieldWriteonlyString);
	GEQUAL(field->getItemType(), createMetaType<string>());
	
	FIELD(fieldNoncopyableData);
	GEQUAL(field->getItemType(), createMetaType<CLASS_DATA>());
}


GTEST(Lib_CanGet)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);
	
	const GMetaField * field;

	FIELD(fieldInt);
	GCHECK(field->canGet());
	
	FIELD(fieldString);
	GCHECK(field->canGet());
	
	FIELD(fieldData);
	GCHECK(field->canGet());
	
	FIELD(fieldReadonlyInt);
	GCHECK(field->canGet());
	
	FIELD(fieldWriteonlyString);
	GCHECK(! field->canGet());
	
	FIELD(fieldNoncopyableData);
	GCHECK(! field->canGet());
}


GTEST(API_CanGet)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;

	FIELD(fieldInt);
	GCHECK(field->canGet());
	
	FIELD(fieldString);
	GCHECK(field->canGet());
	
	FIELD(fieldData);
	GCHECK(field->canGet());
	
	FIELD(fieldReadonlyInt);
	GCHECK(field->canGet());
	
	FIELD(fieldWriteonlyString);
	GCHECK(! field->canGet());
	
	FIELD(fieldNoncopyableData);
	GCHECK(! field->canGet());
}


GTEST(Lib_CanSet)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaField * field;

	FIELD(fieldInt);
	GCHECK(field->canSet());
	
	FIELD(fieldString);
	GCHECK(field->canSet());
	
	FIELD(fieldData);
	GCHECK(field->canSet());
	
	FIELD(fieldReadonlyInt);
	GCHECK(! field->canSet());
	
	FIELD(fieldWriteonlyString);
	GCHECK(field->canSet());
	
	FIELD(fieldNoncopyableData);
	GCHECK(! field->canSet());
}


GTEST(API_CanSet)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;

	FIELD(fieldInt);
	GCHECK(field->canSet());
	
	FIELD(fieldString);
	GCHECK(field->canSet());
	
	FIELD(fieldData);
	GCHECK(field->canSet());
	
	FIELD(fieldReadonlyInt);
	GCHECK(! field->canSet());
	
	FIELD(fieldWriteonlyString);
	GCHECK(field->canSet());
	
	FIELD(fieldNoncopyableData);
	GCHECK(! field->canSet());
}


GTEST(Lib_GetAddress)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaField * field;
	

	CLASS * pobj = NULL;

	FIELD(fieldInt);
	GEQUAL(field->getAddress(pobj), &fieldInt);
	
	FIELD(fieldString);
	GEQUAL(field->getAddress(pobj), &fieldString);
	
	FIELD(fieldData);
	GEQUAL(field->getAddress(pobj), &fieldData);
	
	FIELD(fieldReadonlyInt);
	GEQUAL(field->getAddress(pobj), &fieldReadonlyInt);
	
	FIELD(fieldWriteonlyString);
	GEQUAL(field->getAddress(pobj), &fieldWriteonlyString);
	
	FIELD(fieldNoncopyableData);
	GEQUAL(field->getAddress(pobj), &fieldNoncopyableData);
}


GTEST(API_GetAddress)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;


	CLASS * pobj = NULL;

	FIELD(fieldInt);
	GEQUAL(field->getAddress(pobj), &fieldInt);
	
	FIELD(fieldString);
	GEQUAL(field->getAddress(pobj), &fieldString);
	
	FIELD(fieldData);
	GEQUAL(field->getAddress(pobj), &fieldData);
	
	FIELD(fieldReadonlyInt);
	GEQUAL(field->getAddress(pobj), &fieldReadonlyInt);
	
	FIELD(fieldWriteonlyString);
	GEQUAL(field->getAddress(pobj), &fieldWriteonlyString);
	
	FIELD(fieldNoncopyableData);
	GEQUAL(field->getAddress(pobj), &fieldNoncopyableData);
}


GTEST(Lib_GetSize)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaField * field;
	

	CLASS * pobj = NULL;
	(void)pobj;
	
	FIELD(fieldInt);
	GEQUAL(field->getSize(), sizeof(fieldInt));
	
	FIELD(fieldString);
	GEQUAL(field->getSize(), sizeof(fieldString));
	
	FIELD(fieldData);
	GEQUAL(field->getSize(), sizeof(fieldData));
	
	FIELD(fieldReadonlyInt);
	GEQUAL(field->getSize(), sizeof(fieldReadonlyInt));
	
	FIELD(fieldWriteonlyString);
	GEQUAL(field->getSize(), sizeof(fieldWriteonlyString));
	
	FIELD(fieldNoncopyableData);
	GEQUAL(field->getSize(), sizeof(fieldNoncopyableData));
}


GTEST(API_GetSize)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;


	CLASS * pobj = NULL;
	(void)pobj;
	
	FIELD(fieldInt);
	GEQUAL(field->getSize(), sizeof(fieldInt));
	
	FIELD(fieldString);
	GEQUAL(field->getSize(), sizeof(fieldString));
	
	FIELD(fieldData);
	GEQUAL(field->getSize(), sizeof(fieldData));
	
	FIELD(fieldReadonlyInt);
	GEQUAL(field->getSize(), sizeof(fieldReadonlyInt));
	
	FIELD(fieldWriteonlyString);
	GEQUAL(field->getSize(), sizeof(fieldWriteonlyString));
	
	FIELD(fieldNoncopyableData);
	GEQUAL(field->getSize(), sizeof(fieldNoncopyableData));
}


GTEST(Lib_Set)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaField * field;
	

	CLASS * pobj = NULL;
	
	fieldInt = 0;
	fieldString = "";
	fieldData = CLASS_DATA();
	fieldReadonlyInt = 0;
	fieldWriteonlyString = "";
	fieldNoncopyableData = CLASS_DATA();
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	
	GDIFF(fieldInt, valueInt);
	GDIFF(fieldString, valueString);
	GDIFF(fieldData, valueData);
	GDIFF(fieldReadonlyInt, valueReadonlyInt);
	GDIFF(fieldWriteonlyString, valueWriteonlyString);
	GDIFF(fieldNoncopyableData, valueNoncopyableData);

	FIELD(fieldInt);
	field->set(pobj, valueInt);
	GEQUAL(valueInt, fieldInt);
	
	FIELD(fieldString);
	field->set(pobj, valueString);
	GEQUAL(valueString, fieldString);
	
	FIELD(fieldData);
	field->set(pobj, valueData);
	GEQUAL(valueData, fieldData);
	
	FIELD(fieldReadonlyInt);
	EXCEPT_META(field->set(pobj, valueReadonlyInt))
	
	FIELD(fieldWriteonlyString);
	field->set(pobj, valueWriteonlyString);
	GEQUAL(valueWriteonlyString, fieldWriteonlyString);
	
	FIELD(fieldNoncopyableData);
	EXCEPT_META(field->set(pobj, valueNoncopyableData))
}


GTEST(API_Set)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;


	CLASS * pobj = NULL;

	fieldInt = 0;
	fieldString = "";
	fieldData = CLASS_DATA();
	fieldReadonlyInt = 0;
	fieldWriteonlyString = "";
	fieldNoncopyableData = CLASS_DATA();
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	
	GDIFF(fieldInt, valueInt);
	GDIFF(fieldString, valueString);
	GDIFF(fieldData, valueData);
	GDIFF(fieldReadonlyInt, valueReadonlyInt);
	GDIFF(fieldWriteonlyString, valueWriteonlyString);
	GDIFF(fieldNoncopyableData, valueNoncopyableData);

	FIELD(fieldInt);
	metaSetValue(field.get(), pobj, valueInt);
	GEQUAL(valueInt, fieldInt);
	
	FIELD(fieldString);
	metaSetValue(field.get(), pobj, valueString);
	GEQUAL(valueString, fieldString);
	
	FIELD(fieldData);
	metaSetValue(field.get(), pobj, valueData);
	GEQUAL(valueData, fieldData);
	
	FIELD(fieldReadonlyInt);
	EXCEPT_META(metaSetValue(field.get(), pobj, valueReadonlyInt))
	
	FIELD(fieldWriteonlyString);
	metaSetValue(field.get(), pobj, valueWriteonlyString);
	GEQUAL(valueWriteonlyString, fieldWriteonlyString);
	
	FIELD(fieldNoncopyableData);
	EXCEPT_META(metaSetValue(field.get(), pobj, valueNoncopyableData))
}


GTEST(Lib_Get)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaField * field;
	

	CLASS * pobj = NULL;
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);

	fieldInt = valueInt;
	fieldString = valueString;
	fieldData = valueData;
	fieldReadonlyInt = valueReadonlyInt;
	fieldWriteonlyString = valueWriteonlyString;
	fieldNoncopyableData = valueNoncopyableData;
	
	FIELD(fieldInt);
	GEQUAL(fromVariant<int>(field->get(pobj)), valueInt);
	
	FIELD(fieldString);
	GEQUAL(fromVariant<string>(field->get(pobj)), valueString);
	
	FIELD(fieldData);
	GEQUAL(fromVariant<CLASS_DATA>(field->get(pobj)), valueData);
	
	FIELD(fieldReadonlyInt);
	GEQUAL(fromVariant<int>(field->get(pobj)), valueReadonlyInt);
	
	FIELD(fieldWriteonlyString);
	EXCEPT_META(field->get(pobj))
	
	FIELD(fieldNoncopyableData);
	EXCEPT_META(field->get(pobj))
}


GTEST(API_Get)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;


	CLASS * pobj = NULL;

	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);

	fieldInt = valueInt;
	fieldString = valueString;
	fieldData = valueData;
	fieldReadonlyInt = valueReadonlyInt;
	fieldWriteonlyString = valueWriteonlyString;
	fieldNoncopyableData = valueNoncopyableData;
	
	FIELD(fieldInt);
	GEQUAL(fromVariant<int>(metaGetValue(field.get(), pobj)), valueInt);
	
	FIELD(fieldString);
	GEQUAL(fromVariant<string>(metaGetValue(field.get(), pobj)), valueString);
	
	FIELD(fieldData);
	GEQUAL(fromVariant<CLASS_DATA>(metaGetValue(field.get(), pobj)), valueData);
	
	FIELD(fieldReadonlyInt);
	GEQUAL(fromVariant<int>(metaGetValue(field.get(), pobj)), valueReadonlyInt);
	
	FIELD(fieldWriteonlyString);
	EXCEPT_META(metaGetValue(field.get(), pobj))
	
	FIELD(fieldNoncopyableData);
	EXCEPT_META(metaGetValue(field.get(), pobj))
}



} }

