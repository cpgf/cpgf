#include "test_reflection_common.h"

#define CLASS TestClass_Property
#define NAME_CLASS GPP_STRINGIZE(CLASS)

#define PROP(f) pointerAssign(prop, metaClass->getProperty(# f))

using namespace std;
using namespace cpgf;

namespace Test_Property { namespace {

class CLASS {
public:
	int propInt;
	string propString;
	
	CLASS_DATA propData;
	
	int propReadonlyInt;
	string propWriteonlyString;
	CLASS_DATA propNoncopyableData;
	
	int propExplicit;

public:
	int getPropInt() const {
		return propInt + 1;
	}
	
	void setPropString(const string & s) {
		propString = s + ";";
	}
	
	CLASS_DATA getPropData() const {
		return propData;
	}
	
	void setPropData(const CLASS_DATA & data) {
		propData = data;
	}
}; // class CLASS

int propExplicitGet(CLASS * pobj)
{
	(void)pobj;

	return pobj->propExplicit;
}

void propExplicitSet(CLASS * pobj, int data)
{
	(void)pobj;

	pobj->propExplicit = data;
}

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<CLASS>
		::define(NAME_CLASS)
	
		._property("propInt", &CLASS::getPropInt, &CLASS::propInt)
		._property("propString", &CLASS::propString, &CLASS::setPropString, GMetaPolicyCopyAllConstReference())
		._property("propData", &CLASS::getPropData, &CLASS::setPropData)
	
		._property("propReadonlyInt", &CLASS::propReadonlyInt, &CLASS::propReadonlyInt, GMetaPolicyReadOnly())
		._property("propWriteonlyString", &CLASS::propWriteonlyString, &CLASS::propWriteonlyString, GMetaPolicyWriteOnly())
		._property("propNoncopyableData", &CLASS::propNoncopyableData, &CLASS::propNoncopyableData, GMetaPolicyNoncopyable())
		
		._property("propExplicit", &propExplicitGet, &propExplicitSet, GMetaPolicyExplicitThis())
	;
}


GTEST(Lib_Exists)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
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
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
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
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
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
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaProperty * prop;
	
	CLASS instance;
	CLASS * pobj = &instance;
	(void)pobj;
	
	PROP(propInt);
	GEQUAL(prop->getSize(), sizeof(pobj->propInt));
	
	PROP(propString);
	GEQUAL(prop->getSize(), sizeof(pobj->propString));
	
	PROP(propData);
	GEQUAL(prop->getSize(), sizeof(pobj->propData));
	
	PROP(propReadonlyInt);
	GEQUAL(prop->getSize(), sizeof(pobj->propReadonlyInt));
	
	PROP(propWriteonlyString);
	GEQUAL(prop->getSize(), sizeof(pobj->propWriteonlyString));
	
	PROP(propNoncopyableData);
	GEQUAL(prop->getSize(), sizeof(pobj->propNoncopyableData));
	
	PROP(propExplicit);
	GEQUAL(prop->getSize(), sizeof(pobj->propExplicit));
}


GTEST(API_GetSize)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaProperty> prop;

	CLASS instance;
	CLASS * pobj = &instance;
	(void)pobj;
	
	PROP(propInt);
	GEQUAL(prop->getSize(), sizeof(pobj->propInt));
	
	PROP(propString);
	GEQUAL(prop->getSize(), sizeof(pobj->propString));
	
	PROP(propData);
	GEQUAL(prop->getSize(), sizeof(pobj->propData));
	
	PROP(propReadonlyInt);
	GEQUAL(prop->getSize(), sizeof(pobj->propReadonlyInt));
	
	PROP(propWriteonlyString);
	GEQUAL(prop->getSize(), sizeof(pobj->propWriteonlyString));
	
	PROP(propNoncopyableData);
	GEQUAL(prop->getSize(), sizeof(pobj->propNoncopyableData));
	
	PROP(propExplicit);
	GEQUAL(prop->getSize(), sizeof(pobj->propExplicit));
}


GTEST(Lib_Set)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaProperty * prop;
	
	CLASS instance;
	CLASS * pobj = &instance;
	
	pobj->propInt = 0;
	pobj->propString = "";
	pobj->propData = CLASS_DATA();
	pobj->propReadonlyInt = 0;
	pobj->propWriteonlyString = "";
	pobj->propNoncopyableData = CLASS_DATA();
	pobj->propExplicit = 0;
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	const int valueExplicit = 78;
	
	GDIFF(pobj->propInt, valueInt);
	GDIFF(pobj->propString, valueString);
	GDIFF(pobj->propData, valueData);
	GDIFF(pobj->propReadonlyInt, valueReadonlyInt);
	GDIFF(pobj->propWriteonlyString, valueWriteonlyString);
	GDIFF(pobj->propNoncopyableData, valueNoncopyableData);
	GDIFF(pobj->propExplicit, valueExplicit);

	PROP(propInt);
	prop->set(pobj, valueInt);
	GEQUAL(valueInt, pobj->propInt);
	
	PROP(propString);
	prop->set(pobj, valueString);
	GEQUAL(valueString + ";", pobj->propString);
	
	PROP(propData);
	prop->set(pobj, valueData);
	GEQUAL(valueData, pobj->propData);
	
	PROP(propReadonlyInt);
	EXCEPT_META(prop->set(pobj, valueReadonlyInt))
	
	PROP(propWriteonlyString);
	prop->set(pobj, valueWriteonlyString);
	GEQUAL(valueWriteonlyString, pobj->propWriteonlyString);
	
	PROP(propNoncopyableData);
	EXCEPT_META(prop->set(pobj, valueNoncopyableData))

	PROP(propExplicit);
	prop->set(pobj, valueExplicit);
	GEQUAL(valueExplicit, pobj->propExplicit);
}


GTEST(API_Set)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaProperty> prop;

	CLASS instance;
	CLASS * pobj = &instance;

	pobj->propInt = 0;
	pobj->propString = "";
	pobj->propData = CLASS_DATA();
	pobj->propReadonlyInt = 0;
	pobj->propWriteonlyString = "";
	pobj->propNoncopyableData = CLASS_DATA();
	pobj->propExplicit = 0;
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	const int valueExplicit = 78;
	
	GDIFF(pobj->propInt, valueInt);
	GDIFF(pobj->propString, valueString);
	GDIFF(pobj->propData, valueData);
	GDIFF(pobj->propReadonlyInt, valueReadonlyInt);
	GDIFF(pobj->propWriteonlyString, valueWriteonlyString);
	GDIFF(pobj->propNoncopyableData, valueNoncopyableData);
	GDIFF(pobj->propExplicit, valueExplicit);

	PROP(propInt);
	metaSetValue(prop.get(), pobj, valueInt);
	GEQUAL(valueInt, pobj->propInt);
	
	PROP(propString);
	metaSetValue(prop.get(), pobj, valueString);
	GEQUAL(valueString + ";", pobj->propString);
	
	PROP(propData);
	metaSetValue(prop.get(), pobj, valueData);
	GEQUAL(valueData, pobj->propData);
	
	PROP(propReadonlyInt);
	EXCEPT_META(metaSetValue(prop.get(), pobj, valueReadonlyInt))
	
	PROP(propWriteonlyString);
	metaSetValue(prop.get(), pobj, valueWriteonlyString);
	GEQUAL(valueWriteonlyString, pobj->propWriteonlyString);
	
	PROP(propNoncopyableData);
	EXCEPT_META(metaSetValue(prop.get(), pobj, valueNoncopyableData))

	PROP(propExplicit);
	metaSetValue(prop.get(), pobj, valueExplicit);
	GEQUAL(valueExplicit, pobj->propExplicit);
}


GTEST(Lib_Get)
{
	const GMetaClass * metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass);

	const GMetaProperty * prop;
	
	CLASS instance;
	CLASS * pobj = &instance;
	
	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	const int valueExplicit = 78;

	pobj->propInt = valueInt;
	pobj->propString = valueString;
	pobj->propData = valueData;
	pobj->propReadonlyInt = valueReadonlyInt;
	pobj->propWriteonlyString = valueWriteonlyString;
	pobj->propNoncopyableData = valueNoncopyableData;
	pobj->propExplicit = valueExplicit;
	
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

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	GScopedInterface<IMetaProperty> prop;

	CLASS instance;
	CLASS * pobj = &instance;

	const int valueInt = 5;
	const string valueString = "A string";
	const CLASS_DATA valueData = CLASS_DATA("data string", 38);
	const int valueReadonlyInt = 1999;
	const string valueWriteonlyString = "Antoerh string";
	const CLASS_DATA valueNoncopyableData = CLASS_DATA("Noncopyable data string", 68);
	const int valueExplicit = 78;

	pobj->propInt = valueInt;
	pobj->propString = valueString;
	pobj->propData = valueData;
	pobj->propReadonlyInt = valueReadonlyInt;
	pobj->propWriteonlyString = valueWriteonlyString;
	pobj->propNoncopyableData = valueNoncopyableData;
	pobj->propExplicit = valueExplicit;
	
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

