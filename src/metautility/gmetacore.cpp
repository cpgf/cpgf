#include "cpgf/metautility/gmetacore.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gscopedinterface.h"

namespace cpgf {

GMetaCore::GMetaCore(GScriptObject * scriptObject)
	: scriptObject(scriptObject)
{
}

IMetaClass * GMetaCore::cloneClass(IMetaClass * metaClass)
{
	return this->scriptObject->cloneMetaClass(metaClass);
}

GVariant GMetaCore::cast(const GVariant & instance, IMetaClass * targetMetaClass)
{
	GVariant value = getVariantRealValue(instance);
	GMetaType type = getVariantRealMetaType(instance);
	
	if(canFromVariant<void *>(value) && type.getBaseName() != nullptr) {
		GScopedInterface<IMetaService> metaService(this->scriptObject->getMetaService());
		GScopedInterface<IMetaClass> sourceClass(metaService->findClassByName(type.getBaseName()));
		if(sourceClass) {
			void * ptr = objectAddressFromVariant(instance);
			void * oldPtr = ptr;
			if(ptr != nullptr) {
				GMetaType targetType;
				if(targetMetaClass != nullptr) {
					ptr = metaCastToDerived(oldPtr, sourceClass.get(), targetMetaClass);
					if(ptr == nullptr) {
						ptr = metaCastToBase(oldPtr, sourceClass.get(), targetMetaClass);
					}
					targetType = metaGetItemType(targetMetaClass);
				}
				else {
					GScopedInterface<IMetaClass> derivedClass(findAppropriateDerivedClass(oldPtr, sourceClass.get(), &ptr));
					if(derivedClass) {
						targetType = metaGetItemType(derivedClass.get());
					}
					else {
						ptr = nullptr;
					}
				}

				if(ptr != nullptr) {
					targetType.addPointer();
					return createTypedVariant(createObjectVariantFromPointer(ptr), targetType);
				}
			}
		}
	}

	return (void *)0;
}


} // namespace cpgf
