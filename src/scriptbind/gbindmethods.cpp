#include "../pinclude/gbindmethods.h"

#include "cpgf/metatraits/gmetaobjectlifemanager_iobject.h"

namespace cpgf {

namespace bind_internal {

ObjectPointerCV metaTypeToCV(const GMetaType & type)
{
	if(type.isPointer()) {
		if(type.isPointerToConst()) {
			return opcvConst;
		}
		else if(type.isPointerToVolatile()) {
			return opcvVolatile;
		}
		else if(type.isPointerToConstVolatile()) {
			return opcvConstVolatile;
		}
	}
	else if(type.isReference()) {
		if(type.isReferenceToConst()) {
			return opcvConst;
		}
		else if(type.isReferenceToVolatile()) {
			return opcvVolatile;
		}
		else if(type.isReferenceToConstVolatile()) {
			return opcvConstVolatile;
		}
	}
	else {
		if(type.isConst()) {
			return opcvConst;
		}
		else if(type.isVolatile()) {
			return opcvVolatile;
		}
		else if(type.isConstVolatile()) {
			return opcvConstVolatile;
		}
	}

	return opcvNone;
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

ObjectPointerCV getGlueDataCV(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getCV();
		}
	}

	return opcvNone;
}

GVariant getGlueDataInstance(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getInstance();
		}
	}

	return GVariant();
}

void * getGlueDataInstanceAddress(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getInstanceAddress();
		}
	}

	return nullptr;
}

IMetaClass * getGlueDataMetaClass(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getClassData()->getMetaClass();
		}
		else {
			if(glueData->getType() == gdtClass) {
				return sharedStaticCast<GClassGlueData>(glueData)->getMetaClass();
			}
		}
	}

	return nullptr;
}

IMetaSharedPointerTraits * getGlueDataSharedPointerTraits(const GGlueDataPointer & glueData)
{
	if(glueData) {
		if(glueData->getType() == gdtObject) {
			return sharedStaticCast<GObjectGlueData>(glueData)->getSharedPointerTraits();
		}
		else {
			if(glueData->getType() == gdtRaw) {
				return sharedStaticCast<GRawGlueData>(glueData)->getSharedPointerTraits();
			}
		}
	}

	return nullptr;
}

bool allowAccessData(const GScriptConfig & config, bool isInstance, IMetaAccessible * accessible)
{
	if(isInstance) {
		if(! config.allowAccessStaticDataViaInstance()) {
			if(accessible->isStatic()) {
				return false;
			}
		}
	}
	else {
		if(! accessible->isStatic()) {
			return false;
		}
	}

	return true;
}


} //namespace bind_internal

} //namespace cpgf

