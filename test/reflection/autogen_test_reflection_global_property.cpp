// Auto generated test code
// Don't modify manually.

#define AUTOGEN_TEST
#include "test_reflection_common.h"

#define CLASS void
#define NAME_CLASS GPP_STRINGIZE(CLASS)

#define PROP(f) pointerAssign(prop, metaClass->getProperty(# f))

using namespace std;
using namespace cpgf;

namespace Test_GlobalProperty { namespace {



	int propInt;
	string propString;
	
	CLASS_DATA propData;
	
	int propReadonlyInt;
	string propWriteonlyString;
	CLASS_DATA propNoncopyableData;
	
	int propExplicit;


	int getPropInt() {
		return propInt + 1;
	}
	
	void setPropString(const string & s) {
		propString = s + ";";
	}
	
	CLASS_DATA getPropData() {
		return propData;
	}
	
	void setPropData(const CLASS_DATA & data) {
		propData = data;
	}


int propExplicitGet(CLASS * pobj)
{
	(void)pobj;

	return propExplicit;
}

void propExplicitSet(CLASS * pobj, int data)
{
	(void)pobj;

	propExplicit = data;
}

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaGlobal()
		
	
		._property("propInt", &getPropInt, &propInt)
		._property("propString", &propString, &setPropString, GMetaPolicyCopyAllConstReference())
		._property("propData", &getPropData, &setPropData)
	
		._property("propReadonlyInt", &propReadonlyInt, &propReadonlyInt, GMetaPolicyReadOnly())
		._property("propWriteonlyString", &propWriteonlyString, &propWriteonlyString, GMetaPolicyWriteOnly())
		._property("propNoncopyableData", &propNoncopyableData, &propNoncopyableData, GMetaPolicyNoncopyable())
		
		._property("propExplicit", &propExplicitGet, &propExplicitSet, GMetaPolicyExplicitThis())
	;
}


GTEST(Lib_Exists)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);
	
	const GMetaProperty * prop;

	PROP(propInt);
	GCHECK(prop);
	
	PROP(propString);
	GCHECK(prop);
	
	PROP(propData);
	GCHECK(prop);
	
	PROP(propReadonlyInt);
	GCHECK(prop);
	
	PROP(propWriteonlyString);
	GCHECK(prop);
	
	PROP(propNoncopyableData);
	GCHECK(prop);
	
	PROP(propExplicit);
	GCHECK(prop);
}


GTEST(API_Exists)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaProperty> prop;

	PROP(propInt);
	GCHECK(prop);
	
	PROP(propString);
	GCHECK(prop);
	
	PROP(propData);
	GCHECK(prop);
	
	PROP(propReadonlyInt);
	GCHECK(prop);
	
	PROP(propWriteonlyString);
	GCHECK(prop);
	
	PROP(propNoncopyableData);
	GCHECK(prop);
	
	PROP(propExplicit);
	GCHECK(prop);
}


GTEST(Lib_CheckItemType)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);
	
	const GMetaProperty * prop;

	PROP(propInt);
	GEQUAL(prop->getItemType(), createMetaType<int>());
	
	PROP(propString);
	GEQUAL(prop->getItemType(), createMetaType<string>());
	
	PROP(propData);
	GEQUAL(prop->getItemType(), createMetaType<CLASS_DATA>());
	
	PROP(propReadonlyInt);
	GEQUAL(prop->getItemType(), createMetaType<int>());
	
	PROP(propWriteonlyString);
	GEQUAL(prop->getItemType(), createMetaType<string>());
	
	PROP(propNoncopyableData);
	GEQUAL(prop->getItemType(), createMetaType<CLASS_DATA>());
	
	PROP(propExplicit);
	GEQUAL(prop->getItemType(), createMetaType<int>());
}


GTEST(Lib_CanGet)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);
	
	const GMetaProperty * prop;

	PROP(propInt);
	GCHECK(prop->canGet());
	
	PROP(propString);
	GCHECK(prop->canGet());
	
	PROP(propData);
	GCHECK(prop->canGet());
	
	PROP(propReadonlyInt);
	GCHECK(prop->canGet());
	
	PROP(propWriteonlyString);
	GCHECK(! prop->canGet());
	
	PROP(propNoncopyableData);
	GCHECK(! prop->canGet());
	
	PROP(propExplicit);
	GCHECK(prop->canGet());
}



GTEST(API_CanGet)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaProperty> prop;

	PROP(propInt);
	GCHECK(prop->canGet());
	
	PROP(propString);
	GCHECK(prop->canGet());
	
	PROP(propData);
	GCHECK(prop->canGet());
	
	PROP(propReadonlyInt);
	GCHECK(prop->canGet());
	
	PROP(propWriteonlyString);
	GCHECK(! prop->canGet());
	
	PROP(propNoncopyableData);
	GCHECK(! prop->canGet());
	
	PROP(propExplicit);
	GCHECK(prop->canGet());
}



GTEST(Lib_CanSet)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaProperty * prop;

	PROP(propInt);
	GCHECK(prop->canSet());
	
	PROP(propString);
	GCHECK(prop->canSet());
	
	PROP(propData);
	GCHECK(prop->canSet());
	
	PROP(propReadonlyInt);
	GCHECK(! prop->canSet());
	
	PROP(propWriteonlyString);
	GCHECK(prop->canSet());
	
	PROP(propNoncopyableData);
	GCHECK(! prop->canSet());
	
	PROP(propExplicit);
	GCHECK(prop->canSet());
}


GTEST(API_CanSet)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaProperty> prop;

	PROP(propInt);
	GCHECK(prop->canSet());
	
	PROP(propString);
	GCHECK(prop->canSet());
	
	PROP(propData);
	GCHECK(prop->canSet());
	
	PROP(propReadonlyInt);
	GCHECK(! prop->canSet());
	
	PROP(propWriteonlyString);
	GCHECK(prop->canSet());
	
	PROP(propNoncopyableData);
	GCHECK(! prop->canSet());
	
	PROP(propExplicit);
	GCHECK(prop->canSet());
}


GTEST(Lib_GetSize)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaProperty * prop;
	

	CLASS * pobj = NULL;
	(void)pobj;
	
	PROP(propInt);
	GEQUAL(prop->getSize(), sizeof(propInt));
	
	PROP(propString);
	GEQUAL(prop->getSize(), sizeof(propString));
	
	PROP(propData);
	GEQUAL(prop->getSize(), sizeof(propData));
	
	PROP(propReadonlyInt);
	GEQUAL(prop->getSize(), sizeof(propReadonlyInt));
	
	PROP(propWriteonlyString);
	GEQUAL(prop->getSize(), sizeof(propWriteonlyString));
	
	PROP(propNoncopyableData);
	GEQUAL(prop->getSize(), sizeof(propNoncopyableData));
	
	PROP(propExplicit);
	GEQUAL(prop->getSize(), sizeof(propExplicit));
}


GTEST(API_GetSize)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaProperty> prop;


	CLASS * pobj = NULL;
	(void)pobj;
	
	PROP(propInt);
	GEQUAL(prop->getSize(), sizeof(propInt));
	
	PROP(propString);
	GEQUAL(prop->getSize(), sizeof(propString));
	
	PROP(propData);
	GEQUAL(prop->getSize(), sizeof(propData));
	
	PROP(propReadonlyInt);
	GEQUAL(prop->getSize(), sizeof(propReadonlyInt));
	
	PROP(propWriteonlyString);
	GEQUAL(prop->getSize(), sizeof(propWriteonlyString));
	
	PROP(propNoncopyableData);
	GEQUAL(prop->getSize(), sizeof(propNoncopyableData));
	
	PROP(propExplicit);
	GEQUAL(prop->getSize(), sizeof(propExplicit));
}


GTEST(Lib_Set)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaProperty * prop;
	

	CLASS * pobj = NULL;
	
	propInt = 0;
	propString = "";
	propData = CLASS_DATA();
	propReadonlyInt = 0;
	propWriteonlyString = "";
	propNoncopyableData = CLASS_DATA();
	propExplicit = 0;
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	const int valueExplicit = 78;
	
	GDIFF(propInt, valueInt);
	GDIFF(propString, valueString);
	GDIFF(propData, valueData);
	GDIFF(propReadonlyInt, valueReadonlyInt);
	GDIFF(propWriteonlyString, valueWriteonlyString);
	GDIFF(propNoncopyableData, valueNoncopyableData);
	GDIFF(propExplicit, valueExplicit);

	PROP(propInt);
	prop->set(pobj, valueInt);
	GEQUAL(valueInt, propInt);
	
	PROP(propString);
	prop->set(pobj, valueString);
	GEQUAL(valueString + ";", propString);
	
	PROP(propData);
	prop->set(pobj, valueData);
	GEQUAL(valueData, propData);
	
	PROP(propReadonlyInt);
	EXCEPT_META(prop->set(pobj, valueReadonlyInt))
	
	PROP(propWriteonlyString);
	prop->set(pobj, valueWriteonlyString);
	GEQUAL(valueWriteonlyString, propWriteonlyString);
	
	PROP(propNoncopyableData);
	EXCEPT_META(prop->set(pobj, valueNoncopyableData))

	PROP(propExplicit);
	prop->set(pobj, valueExplicit);
	GEQUAL(valueExplicit, propExplicit);
}


GTEST(API_Set)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaProperty> prop;


	CLASS * pobj = NULL;

	propInt = 0;
	propString = "";
	propData = CLASS_DATA();
	propReadonlyInt = 0;
	propWriteonlyString = "";
	propNoncopyableData = CLASS_DATA();
	propExplicit = 0;
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	const int valueExplicit = 78;
	
	GDIFF(propInt, valueInt);
	GDIFF(propString, valueString);
	GDIFF(propData, valueData);
	GDIFF(propReadonlyInt, valueReadonlyInt);
	GDIFF(propWriteonlyString, valueWriteonlyString);
	GDIFF(propNoncopyableData, valueNoncopyableData);
	GDIFF(propExplicit, valueExplicit);

	PROP(propInt);
	metaSetValue(prop.get(), pobj, valueInt);
	GEQUAL(valueInt, propInt);
	
	PROP(propString);
	metaSetValue(prop.get(), pobj, valueString);
	GEQUAL(valueString + ";", propString);
	
	PROP(propData);
	metaSetValue(prop.get(), pobj, valueData);
	GEQUAL(valueData, propData);
	
	PROP(propReadonlyInt);
	EXCEPT_META(metaSetValue(prop.get(), pobj, valueReadonlyInt))
	
	PROP(propWriteonlyString);
	metaSetValue(prop.get(), pobj, valueWriteonlyString);
	GEQUAL(valueWriteonlyString, propWriteonlyString);
	
	PROP(propNoncopyableData);
	EXCEPT_META(metaSetValue(prop.get(), pobj, valueNoncopyableData))

	PROP(propExplicit);
	metaSetValue(prop.get(), pobj, valueExplicit);
	GEQUAL(valueExplicit, propExplicit);
}


GTEST(Lib_Get)
{
	const GMetaClass * metaClass = getGlobalMetaClass();
	GCHECK(metaClass);

	const GMetaProperty * prop;
	

	CLASS * pobj = NULL;
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	const int valueExplicit = 78;

	propInt = valueInt;
	propString = valueString;
	propData = valueData;
	propReadonlyInt = valueReadonlyInt;
	propWriteonlyString = valueWriteonlyString;
	propNoncopyableData = valueNoncopyableData;
	propExplicit = valueExplicit;
	
	PROP(propInt);
	GEQUAL(fromVariant<int>(prop->get(pobj)), valueInt + 1);
	
	PROP(propString);
	GEQUAL(fromVariant<string>(prop->get(pobj)), valueString);
	
	PROP(propData);
	GEQUAL(fromVariant<CLASS_DATA>(prop->get(pobj)), valueData);
	
	PROP(propReadonlyInt);
	GEQUAL(fromVariant<int>(prop->get(pobj)), valueReadonlyInt);
	
	PROP(propWriteonlyString);
	EXCEPT_META(prop->get(pobj))
	
	PROP(propNoncopyableData);
	EXCEPT_META(prop->get(pobj))
	
	PROP(propExplicit);
	GEQUAL(fromVariant<int>(prop->get(pobj)), valueExplicit);
}


GTEST(API_Get)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(metaGetGlobalMetaClass(service.get(), 0));
	GCHECK(metaClass);

	GScopedInterface<IMetaProperty> prop;


	CLASS * pobj = NULL;

	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	const int valueExplicit = 78;

	propInt = valueInt;
	propString = valueString;
	propData = valueData;
	propReadonlyInt = valueReadonlyInt;
	propWriteonlyString = valueWriteonlyString;
	propNoncopyableData = valueNoncopyableData;
	propExplicit = valueExplicit;
	
	PROP(propInt);
	GEQUAL(fromVariant<int>(metaGetValue(prop.get(), pobj)), valueInt + 1);
	
	PROP(propString);
	GEQUAL(fromVariant<string>(metaGetValue(prop.get(), pobj)), valueString);
	
	PROP(propData);
	GEQUAL(fromVariant<CLASS_DATA>(metaGetValue(prop.get(), pobj)), valueData);
	
	PROP(propReadonlyInt);
	GEQUAL(fromVariant<int>(metaGetValue(prop.get(), pobj)), valueReadonlyInt);
	
	PROP(propWriteonlyString);
	EXCEPT_META(metaGetValue(prop.get(), pobj))
	
	PROP(propNoncopyableData);
	EXCEPT_META(metaGetValue(prop.get(), pobj))
	
	PROP(propExplicit);
	GEQUAL(fromVariant<int>(metaGetValue(prop.get(), pobj)), valueExplicit);
}



} }

