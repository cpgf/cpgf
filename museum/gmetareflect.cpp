#include "cpgf/gmetareflect.h"


namespace cpgf {


void globalAddField(GMetaField * field)
{
	getGlobalMetaClass()->addField(field);
}

void globalAddProperty(GMetaProperty * field)
{
	getGlobalMetaClass()->addProperty(field);
}

void globalAddMethod(GMetaMethod * method)
{
	getGlobalMetaClass()->addMethod(method);
}

void globalAddOperator(GMetaOperator * metaOperator)
{
	getGlobalMetaClass()->addOperator(metaOperator);
}

GMetaEnum & globalAddEnum(GMetaEnum * en)
{
	return getGlobalMetaClass()->addEnum(en);
}

void globalAddClass(const GMetaClass * cls)
{
	getGlobalMetaClass()->addClass(cls);
}

GMetaAnnotation & globaAddAnnotation(GMetaAnnotation * annotation)
{
	return getGlobalMetaClass()->addAnnotation(annotation);
}

void globalFlushAnnotation()
{
	getGlobalMetaClass()->flushAnnotation();
}

void reflectClass(const GMetaClass * metaClass)
{
	getGlobalMetaClass()->addClass(metaClass);
}

GMetaAnnotation & reflectAnnotation(const char * name)
{
	return globaAddAnnotation(new GMetaAnnotation(name));
};

void flushAnnotation()
{
	globalFlushAnnotation();
}




} // namespace cpgf



