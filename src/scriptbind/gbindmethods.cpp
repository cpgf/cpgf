#include "gbindmethods.h"

#include "cpgf/metatraits/gmetaobjectlifemanager_iobject.h"

namespace cpgf {

namespace bind_internal {

GScriptInstanceCv metaTypeToCV(const GMetaType & type)
{
	if(type.isPointer()) {
		if(type.isPointerToConst()) {
			return GScriptInstanceCv::sicvConst;
		}
		else if(type.isPointerToVolatile()) {
			return GScriptInstanceCv::sicvVolatile;
		}
		else if(type.isPointerToConstVolatile()) {
			return GScriptInstanceCv::sicvConstVolatile;
		}
	}
	else if(type.isReference()) {
		if(type.isReferenceToConst()) {
			return GScriptInstanceCv::sicvConst;
		}
		else if(type.isReferenceToVolatile()) {
			return GScriptInstanceCv::sicvVolatile;
		}
		else if(type.isReferenceToConstVolatile()) {
			return GScriptInstanceCv::sicvConstVolatile;
		}
	}
	else {
		if(type.isConst()) {
			return GScriptInstanceCv::sicvConst;
		}
		else if(type.isVolatile()) {
			return GScriptInstanceCv::sicvVolatile;
		}
		else if(type.isConstVolatile()) {
			return GScriptInstanceCv::sicvConstVolatile;
		}
	}

	return GScriptInstanceCv::sicvNone;
}

bool shouldRemoveReference(const GMetaType & type)
{
	return type.isReference()
		&& (type.isPointer() || vtIsFundamental(vtGetBaseType(type.getVariantType())))
		;
}

IMetaObjectLifeManager * createObjectLifeManagerForInterface(const GVariant & value)
{
	if(vtIsInterface(value.getType())) {
		return metatraits_internal::doCreateObjectLifeManagerForIObject(GTypeConverter<IObject *, IObject *>());
	}
	else {
		return nullptr;
	}
}

GVariant getAccessibleValueAndType(
		void * instance,
		IMetaAccessible * accessible,
		GMetaType * outType,
		bool instanceIsConst
	)
{
	GVariant value;

	accessible->getItemType(&outType->refData());
	metaCheckError(accessible);

	void * address = accessible->getAddress(instance);
	if(address != nullptr && !outType->isPointer() && outType->baseIsClass()) {
		value = createObjectVariant(address);

		if(instanceIsConst) {
			outType->addConst();
		}
	}
	else {
		accessible->get(&value.refData(), instance);
		metaCheckError(accessible);
	}

	return value;
}

GScriptInstanceCv getGlueDataCV(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return std::static_pointer_cast<GObjectGlueData>(glueData)->getCV();
		}
	}

	return GScriptInstanceCv::sicvNone;
}

GVariant getGlueDataInstance(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return std::static_pointer_cast<GObjectGlueData>(glueData)->getInstance();
		}
	}

	return GVariant();
}

void * getGlueDataInstanceAddress(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return std::static_pointer_cast<GObjectGlueData>(glueData)->getInstanceAddress();
		}
	}

	return nullptr;
}

IMetaClass * getGlueDataMetaClass(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return std::static_pointer_cast<GObjectGlueData>(glueData)->getClassData()->getMetaClass();
		}
		else {
			if(glueData->getType() == gdtClass) {
				return std::static_pointer_cast<GClassGlueData>(glueData)->getMetaClass();
			}
		}
	}

	return nullptr;
}

IMetaSharedPointerTraits * getGlueDataSharedPointerTraits(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return std::static_pointer_cast<GObjectGlueData>(glueData)->getSharedPointerTraits();
		}
		else {
			if(glueData->getType() == gdtRaw) {
				return std::static_pointer_cast<GRawGlueData>(glueData)->getSharedPointerTraits();
			}
		}
	}

	return nullptr;
}


} //namespace bind_internal

} //namespace cpgf

