#include "gmetaarchivecommon.h"
#include "cpgf/gstdint.h"


namespace cpgf {


bool canSerializeItem(const GMetaArchiveConfig & config, IMetaItem * item)
{
	(void)config;

	GScopedInterface<IMetaAnnotation> annotation(item->getAnnotation(SerializationAnnotation));

	if(! annotation) {
	}
	else {
		GScopedInterface<IMetaAnnotationValue> annotationValue(annotation->getValue(SerializationAnnotationEnable));
		if(annotationValue) {
			if(! annotationValue->toBoolean()) {
				return false;
			}
		}
	}

	return true;
}

bool canSerializeObject(const GMetaArchiveConfig & config, IMetaClass * metaClass)
{
	return canSerializeItem(config, metaClass);
}

bool canSerializeField(const GMetaArchiveConfig & config, IMetaAccessible * accessible, IMetaClass * ownerClass)
{
	(void) ownerClass;

	if(! accessible->canGet()) {
		return false;
	}
	if(! accessible->canSet()) {
		return false;
	}

	return canSerializeItem(config, accessible);
}

bool canSerializeBaseClass(const GMetaArchiveConfig & config, IMetaClass * baseClass, IMetaClass * metaClass)
{
	(void)config;
	(void)metaClass;

	if(baseClass == NULL) {
		return false;
	}

	return true;
}



} // namespace cpgf



