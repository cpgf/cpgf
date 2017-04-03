#include "gbindconverttoscript.h"

namespace cpgf {

namespace bind_internal {

GScriptValue doCreateScriptValueFromVariant(const GContextPointer & context, const GVariant & value, const GMetaType & type, const bool transferOwnership)
{
	const GVariantType vt = static_cast<GVariantType>((GVtType)value.getType() & ~(GVtType)GVariantType::maskByReference);

	if(! type.isEmpty() && type.getPointerDimension() <= 1) {
		GScopedInterface<IMetaTypedItem> typedItem(context->getService()->findTypedItemByName(type.getBaseName()));
		if(typedItem) {
			GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

			return GScriptValue::fromObject(value, gdynamic_cast<IMetaClass *>(typedItem.get()), transferOwnership, metaTypeToCV(type));
		}
		else {
			if(vtIsInterface(vt)) {
				IObject * obj = fromVariant<IObject *>(value);
				if(dynamic_cast<IMetaClass *>(obj)) { // !!! GUID
					IMetaClass * metaClass = dynamic_cast<IMetaClass *>(obj);
					return GScriptValue::fromClass(metaClass);
				}
			}
		}
	}

	return GScriptValue::fromPrimary(value);
}


} //namespace bind_internal

} //namespace cpgf

