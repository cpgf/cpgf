#include "test_reflection_common.h"

#define CLASS TestClass_Field
#define NAME_CLASS GPP_STRINGIZE(CLASS)

using namespace std;
using namespace cpgf;

namespace Test_Field { namespace {

class CLASS {
public:
	int fieldInt;
	string fieldString;
	
	CLASS_DATA fieldData;
	
	int fieldReadonlyInt;
	string fieldWriteonlyString;
	CLASS_DATA fieldNoncopyableData;
	int fieldArray[3];
}; // class CLASS


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<CLASS>
		::define(NAME_CLASS)

		._field("fieldInt", &CLASS::fieldInt)
		._field("fieldString", &CLASS::fieldString)
		._field("fieldData", &CLASS::fieldData)
		._field("fieldReadonlyInt", &CLASS::fieldReadonlyInt, GMetaPolicyReadOnly())
		._field("fieldWriteonlyString", &CLASS::fieldWriteonlyString, GMetaPolicyWriteOnly())
		._field("fieldNoncopyableData", &CLASS::fieldNoncopyableData, GMetaPolicyNoncopyable())
		._field("fieldArray", &CLASS::fieldArray)
	;
}


GTEST(Lib_Exists)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
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
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
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
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
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
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaField * field;
	
	CLASS instance;
	CLASS * pobj = &instance;

	FIELD(fieldInt);
	GEQUAL(field->getAddress(pobj), &pobj->fieldInt);
	
	FIELD(fieldString);
	GEQUAL(field->getAddress(pobj), &pobj->fieldString);
	
	FIELD(fieldData);
	GEQUAL(field->getAddress(pobj), &pobj->fieldData);
	
	FIELD(fieldReadonlyInt);
	GEQUAL(field->getAddress(pobj), &pobj->fieldReadonlyInt);
	
	FIELD(fieldWriteonlyString);
	GEQUAL(field->getAddress(pobj), &pobj->fieldWriteonlyString);
	
	FIELD(fieldNoncopyableData);
	GEQUAL(field->getAddress(pobj), &pobj->fieldNoncopyableData);
}


GTEST(API_GetAddress)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;

	CLASS instance;
	CLASS * pobj = &instance;

	FIELD(fieldInt);
	GEQUAL(field->getAddress(pobj), &pobj->fieldInt);
	
	FIELD(fieldString);
	GEQUAL(field->getAddress(pobj), &pobj->fieldString);
	
	FIELD(fieldData);
	GEQUAL(field->getAddress(pobj), &pobj->fieldData);
	
	FIELD(fieldReadonlyInt);
	GEQUAL(field->getAddress(pobj), &pobj->fieldReadonlyInt);
	
	FIELD(fieldWriteonlyString);
	GEQUAL(field->getAddress(pobj), &pobj->fieldWriteonlyString);
	
	FIELD(fieldNoncopyableData);
	GEQUAL(field->getAddress(pobj), &pobj->fieldNoncopyableData);
}


GTEST(Lib_GetSize)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaField * field;
	
	CLASS instance;
	CLASS * pobj = &instance;
	(void)pobj;
	
	FIELD(fieldInt);
	GEQUAL(field->getSize(), sizeof(pobj->fieldInt));
	
	FIELD(fieldString);
	GEQUAL(field->getSize(), sizeof(pobj->fieldString));
	
	FIELD(fieldData);
	GEQUAL(field->getSize(), sizeof(pobj->fieldData));
	
	FIELD(fieldReadonlyInt);
	GEQUAL(field->getSize(), sizeof(pobj->fieldReadonlyInt));
	
	FIELD(fieldWriteonlyString);
	GEQUAL(field->getSize(), sizeof(pobj->fieldWriteonlyString));
	
	FIELD(fieldNoncopyableData);
	GEQUAL(field->getSize(), sizeof(pobj->fieldNoncopyableData));
}


GTEST(API_GetSize)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;

	CLASS instance;
	CLASS * pobj = &instance;
	(void)pobj;
	
	FIELD(fieldInt);
	GEQUAL(field->getSize(), sizeof(pobj->fieldInt));
	
	FIELD(fieldString);
	GEQUAL(field->getSize(), sizeof(pobj->fieldString));
	
	FIELD(fieldData);
	GEQUAL(field->getSize(), sizeof(pobj->fieldData));
	
	FIELD(fieldReadonlyInt);
	GEQUAL(field->getSize(), sizeof(pobj->fieldReadonlyInt));
	
	FIELD(fieldWriteonlyString);
	GEQUAL(field->getSize(), sizeof(pobj->fieldWriteonlyString));
	
	FIELD(fieldNoncopyableData);
	GEQUAL(field->getSize(), sizeof(pobj->fieldNoncopyableData));
}


GTEST(Lib_Set)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaField * field;
	
	CLASS instance;
	CLASS * pobj = &instance;
	
	pobj->fieldInt = 0;
	pobj->fieldString = "";
	pobj->fieldData = CLASS_DATA();
	pobj->fieldReadonlyInt = 0;
	pobj->fieldWriteonlyString = "";
	pobj->fieldNoncopyableData = CLASS_DATA();
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	
	GDIFF(pobj->fieldInt, valueInt);
	GDIFF(pobj->fieldString, valueString);
	GDIFF(pobj->fieldData, valueData);
	GDIFF(pobj->fieldReadonlyInt, valueReadonlyInt);
	GDIFF(pobj->fieldWriteonlyString, valueWriteonlyString);
	GDIFF(pobj->fieldNoncopyableData, valueNoncopyableData);

	FIELD(fieldInt);
	field->set(pobj, valueInt);
	GEQUAL(valueInt, pobj->fieldInt);
	
	FIELD(fieldString);
	field->set(pobj, valueString);
	GEQUAL(valueString, pobj->fieldString);
	
	FIELD(fieldData);
	field->set(pobj, valueData);
	GEQUAL(valueData, pobj->fieldData);
	
	FIELD(fieldReadonlyInt);
	EXCEPT_META(field->set(pobj, valueReadonlyInt))
	
	FIELD(fieldWriteonlyString);
	field->set(pobj, valueWriteonlyString);
	GEQUAL(valueWriteonlyString, pobj->fieldWriteonlyString);
	
	FIELD(fieldNoncopyableData);
	EXCEPT_META(field->set(pobj, valueNoncopyableData))
}


GTEST(API_Set)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;

	CLASS instance;
	CLASS * pobj = &instance;

	pobj->fieldInt = 0;
	pobj->fieldString = "";
	pobj->fieldData = CLASS_DATA();
	pobj->fieldReadonlyInt = 0;
	pobj->fieldWriteonlyString = "";
	pobj->fieldNoncopyableData = CLASS_DATA();
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	
	GDIFF(pobj->fieldInt, valueInt);
	GDIFF(pobj->fieldString, valueString);
	GDIFF(pobj->fieldData, valueData);
	GDIFF(pobj->fieldReadonlyInt, valueReadonlyInt);
	GDIFF(pobj->fieldWriteonlyString, valueWriteonlyString);
	GDIFF(pobj->fieldNoncopyableData, valueNoncopyableData);

	FIELD(fieldInt);
	metaSetValue(field.get(), pobj, valueInt);
	GEQUAL(valueInt, pobj->fieldInt);
	
	FIELD(fieldString);
	metaSetValue(field.get(), pobj, valueString);
	GEQUAL(valueString, pobj->fieldString);
	
	FIELD(fieldData);
	metaSetValue(field.get(), pobj, valueData);
	GEQUAL(valueData, pobj->fieldData);
	
	FIELD(fieldReadonlyInt);
	EXCEPT_META(metaSetValue(field.get(), pobj, valueReadonlyInt))
	
	FIELD(fieldWriteonlyString);
	metaSetValue(field.get(), pobj, valueWriteonlyString);
	GEQUAL(valueWriteonlyString, pobj->fieldWriteonlyString);
	
	FIELD(fieldNoncopyableData);
	EXCEPT_META(metaSetValue(field.get(), pobj, valueNoncopyableData))
}


GTEST(Lib_Get)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaField * field;
	
	CLASS instance;
	CLASS * pobj = &instance;
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	const int valueArray[3] = { 3, 5, 6 };

	pobj->fieldInt = valueInt;
	pobj->fieldString = valueString;
	pobj->fieldData = valueData;
	pobj->fieldReadonlyInt = valueReadonlyInt;
	pobj->fieldWriteonlyString = valueWriteonlyString;
	pobj->fieldNoncopyableData = valueNoncopyableData;
	for(int i = 0; i < 3; ++i) {
		pobj->fieldArray[i] = valueArray[i];
	}
	
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

	FIELD(fieldArray);
	// not support yet
//	GEQUAL(fromVariant<int *>(field->get(pobj))[0], valueArray[0]);
}


GTEST(API_Get)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaField> field;

	CLASS instance;
	CLASS * pobj = &instance;

	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);

	pobj->fieldInt = valueInt;
	pobj->fieldString = valueString;
	pobj->fieldData = valueData;
	pobj->fieldReadonlyInt = valueReadonlyInt;
	pobj->fieldWriteonlyString = valueWriteonlyString;
	pobj->fieldNoncopyableData = valueNoncopyableData;
	
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

