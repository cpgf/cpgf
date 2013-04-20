#include "../pinclude/gmetaarchivecommonimpl.h"

#include "cpgf/gstdint.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gfixedtype.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gerrorcode.h"


namespace cpgf {


bool GBaseClassMap::hasMetaClass(void * instance, IMetaClass * metaClass) const
{
	MapType::const_iterator it = this->itemMap.find(metaClass->getQualifiedName());
	return it != this->itemMap.end() && it->second == instance;
}

void GBaseClassMap::addMetaClass(void * instance, IMetaClass * metaClass)
{
	this->itemMap.set(metaClass->getQualifiedName(), instance);
}


GMetaArchiveConfig GMetaArchiveConfigMap::getConfig(IMetaClass * metaClass) const
{
	if(metaClass == NULL) {
		return GMetaArchiveConfig();
	}

	const char * name = metaClass->getQualifiedName();
	MapType::const_iterator it = this->itemMap.find(name);
	if(it == this->itemMap.end()) {
		GMetaArchiveConfig config = getItemMetaArchiveConfig(metaClass);
		this->itemMap.set(name, config);
		it = this->itemMap.find(name);
	}
	
	return it->second;
}


GMetaArchiveConfig::GMetaArchiveConfig() : version(0), flags(ConfigFlags(defaultConfig))
{
}

GMetaArchiveConfig::GMetaArchiveConfig(const GMetaArchiveConfigData & data)
	: version(data.version), flags(ConfigFlags(data.flags))
{
}

void GMetaArchiveConfig::setAllowSerialize(bool allow)
{
	this->flags.setByBool(macAllowSerialize, allow);
}

bool GMetaArchiveConfig::allowSerialize() const
{
	return this->flags.has(macAllowSerialize);
}

void GMetaArchiveConfig::setAllowTrackPointer(bool allow)
{
	this->flags.setByBool(macAllowTrackPointers, allow);
}

bool GMetaArchiveConfig::allowTrackPointer() const
{
	return this->allowSerialize() && this->flags.has(macAllowTrackPointers);
}

void GMetaArchiveConfig::setAllowSerializeField(bool allow)
{
	this->flags.setByBool(macAllowSerializeField, allow);
}

bool GMetaArchiveConfig::allowSerializeField() const
{
	return this->allowSerialize() && this->flags.has(macAllowSerializeField);
}

void GMetaArchiveConfig::setAllowSerializeProperty(bool allow)
{
	this->flags.setByBool(macAllowSerializeProperty, allow);
}

bool GMetaArchiveConfig::allowSerializeProperty() const
{
	return this->allowSerialize() && this->flags.has(macAllowSerializeProperty);
}

void GMetaArchiveConfig::setDefaultSerializeAll(bool defaultSerializeAll)
{
	this->flags.setByBool(macDefaultSerializeAll, defaultSerializeAll);
}

bool GMetaArchiveConfig::defaultSerializeAll() const
{
	return this->flags.has(macDefaultSerializeAll);
}

void GMetaArchiveConfig::setVersion(uint32_t version)
{
	this->version = version;
}

uint32_t GMetaArchiveConfig::getVersion() const
{
	return this->version;
}

GMetaArchiveConfigData GMetaArchiveConfig::getData() const
{
	GMetaArchiveConfigData data;
	data.flags = this->flags;
	data.version = this->version;
	return data;
}


GMetaArchiveConfig getItemMetaArchiveConfig(IMetaItem * item)
{
	GMetaArchiveConfig config;

	if(item != NULL) {
		GScopedInterface<IMetaAnnotation> annotation(item->getAnnotation(SerializationAnnotation));

		if(annotation) {
			GScopedInterface<IMetaAnnotationValue> annotationValue;
		
			annotationValue.reset(annotation->getValue(SerializationAnnotationEnable));
			if(annotationValue) {
				if(! annotationValue->toBoolean()) {
					config.setAllowSerialize(false);
				}
			}

			annotationValue.reset(annotation->getValue(SerializationAnnotationDefaultAll));
			if(annotationValue) {
				if(! annotationValue->toBoolean()) {
					config.setDefaultSerializeAll(false);
				}
			}
		
			annotationValue.reset(annotation->getValue(SerializationAnnotationFields));
			if(annotationValue) {
				if(! annotationValue->toBoolean()) {
					config.setAllowSerializeField(false);
				}
			}
		
			annotationValue.reset(annotation->getValue(SerializationAnnotationProperties));
			if(annotationValue) {
				if(! annotationValue->toBoolean()) {
					config.setAllowSerializeProperty(false);
				}
			}
		
			annotationValue.reset(annotation->getValue(SerializationAnnotationTrackPointers));
			if(annotationValue) {
				if(! annotationValue->toBoolean()) {
					config.setAllowTrackPointer(false);
				}
			}
		
			annotationValue.reset(annotation->getValue(SerializationAnnotationVersion));
			if(annotationValue) {
				uint32_t version = annotationValue->toInt32();
				config.setVersion(version);
			}
		}
	}
	
	return config;
}

bool canSerializeItem(const GMetaArchiveConfig & config, IMetaItem * item)
{
	if(item == NULL) {
		return false;
	}

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

	if(! config.defaultSerializeAll()) {
		return false;
	}

	return true;
}

bool canSerializeObject(const GMetaArchiveConfig & config, IMetaClass * metaClass)
{
	return canSerializeItem(config, metaClass);
}

bool canSerializeField(const GMetaArchiveConfig & config, IMetaAccessible * accessible, IMetaService * service)
{
	if(accessible == NULL) {
		return false;
	}

	if(! accessible->canGet()) {
		return false;
	}
	if(! accessible->canSet()) {
		return false;
	}

	GMetaType metaType = metaGetItemType(accessible);
	if(! canSerializeMetaType(metaType)) {
		return false;
	}

	if(metaType.getBaseName() != NULL) {
		GScopedInterface<IMetaClass> metaClass(service->findClassByName(metaType.getBaseName()));
		if(! canSerializeObject(config, metaClass.get())) {
			return false;
		}
	}

	return canSerializeItem(config, accessible);
}

bool canSerializeBaseClass(const GMetaArchiveConfig & config, IMetaClass * baseClass, IMetaClass * /*metaClass*/)
{
	if(baseClass == NULL) {
		return false;
	}

	return canSerializeItem(config, baseClass);
}

bool canSerializeMetaType(const GMetaType & metaType)
{
	if(metaType.getPointerDimension() > 1) {
		return false;
	}

	if(metaType.getPointerDimension() > 0) {
		return metaType.baseIsClass();
	}

	return true;
}


void serializeCheckType(int type, int expected)
{
	if(type != expected) {
		serializeError(Error_Serialization_TypeMismatch);
	}
}

void serializeError(int errorCode)
{
	raiseCoreException(errorCode);
}

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4800) // warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#endif

// C++ builder doesn't like typename below
#ifdef G_COMPILER_CPPBUILDER
	#define TYPENAME
#else
	#define TYPENAME typename
#endif


template <typename T>
GVariant doReadInteger(const void * address, size_t size)
{
	switch(size) {
		case 1:
			return (T)(*(TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt8::Signed, GFixedTypeInt8::Unsigned>::Result *)(address));

		case 2:
			return (T)(*(TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt16::Signed, GFixedTypeInt16::Unsigned>::Result *)(address));

		case 4:
			return (T)(*(TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt32::Signed, GFixedTypeInt32::Unsigned>::Result *)(address));

		case 8:
			return (T)(*(TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt64::Signed, GFixedTypeInt64::Unsigned>::Result *)(address));
	}

	GASSERT(false);

	return GVariant();
}

GVariant readFundamental(const void * address, const GMetaType & metaType)
{
	size_t size = metaType.getVariantSize();
	if(vtIsReal(metaType.getVariantType())) {
		switch(size) {
			case 4:
				return *(GFixedTypeFloat32::Signed *)(address);

			case 8:
				return *(GFixedTypeFloat64::Signed *)(address);

			default:
				if(metaType.getVariantType() == vtLongDouble) { // long double has vary size on GCC...
					return *(GFixedTypeFloat80::Signed *)(address);
					break;
				}
				GASSERT(false);
				break;
		}
	}
	else {
		switch(metaType.getVariantType()) {
			case vtBool:
				return doReadInteger<bool>(address, size);

			case vtChar:
				return doReadInteger<char>(address, size);

			case vtWchar:
				return doReadInteger<wchar_t>(address, size);

			case vtSignedChar:
				return doReadInteger<signed char>(address, size);

			case vtUnsignedChar:
				return doReadInteger<unsigned char>(address, size);

			case vtSignedShort:
				return doReadInteger<signed short>(address, size);

			case vtUnsignedShort:
				return doReadInteger<unsigned short>(address, size);

			case vtSignedInt:
				return doReadInteger<signed int>(address, size);

			case vtUnsignedInt:
				return doReadInteger<unsigned int>(address, size);

			case vtSignedLong:
				return doReadInteger<signed long>(address, size);

			case vtUnsignedLong:
				return doReadInteger<unsigned long>(address, size);

			case vtSignedLongLong:
				return doReadInteger<signed long long>(address, size);

			case vtUnsignedLongLong:
				return doReadInteger<unsigned long long>(address, size);

			default:
				GASSERT(false);
				break;

		}
	}

	return GVariant();
}

template <typename T>
void doWriteInteger(void * address, size_t size, const GVariant & v)
{
	switch(size) {
		case 1:
			*(TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt8::Signed, GFixedTypeInt8::Unsigned>::Result *)(address) = fromVariant<TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt8::Signed, GFixedTypeInt8::Unsigned>::Result>(v);
			break;

		case 2:
			*(TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt16::Signed, GFixedTypeInt16::Unsigned>::Result *)(address) = fromVariant<TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt16::Signed, GFixedTypeInt16::Unsigned>::Result>(v);
			break;

		case 4:
			*(TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt32::Signed, GFixedTypeInt32::Unsigned>::Result *)(address) = fromVariant<TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt32::Signed, GFixedTypeInt32::Unsigned>::Result>(v);
			break;

		case 8:
			*(TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt64::Signed, GFixedTypeInt64::Unsigned>::Result *)(address) = fromVariant<TYPENAME GIfElse<IsSigned<T>::Result, GFixedTypeInt64::Signed, GFixedTypeInt64::Unsigned>::Result>(v);
			break;

		default:
			GASSERT(false);
	}
}

void writeFundamental(void * address, const GMetaType & metaType, const GVariant & v)
{
	size_t size = metaType.getVariantSize();;
	if(vtIsReal(metaType.getVariantType())) {
		switch(size) {
			case 4:
				*(GFixedTypeFloat32::Signed *)(address) = fromVariant<GFixedTypeFloat32::Signed>(v);
				break;

			case 8:
				*(GFixedTypeFloat64::Signed *)(address) = fromVariant<GFixedTypeFloat64::Signed>(v);
				break;

			default:
				if(metaType.getVariantType() == vtLongDouble) { // long double has vary size on GCC...
					*(GFixedTypeFloat80::Signed *)(address) = fromVariant<GFixedTypeFloat80::Signed>(v);
					break;
				}
				GASSERT(false);
				break;
		}
	}
	else {
		switch(metaType.getVariantType()) {
			case vtBool:
				doWriteInteger<bool>(address, size, v);
				break;

			case vtChar:
				doWriteInteger<char>(address, size, v);
				break;

			case vtWchar:
				doWriteInteger<wchar_t>(address, size, v);
				break;

			case vtSignedChar:
				doWriteInteger<signed char>(address, size, v);
				break;

			case vtUnsignedChar:
				doWriteInteger<unsigned char>(address, size, v);
				break;

			case vtSignedShort:
				doWriteInteger<signed short>(address, size, v);
				break;

			case vtUnsignedShort:
				doWriteInteger<unsigned short>(address, size, v);
				break;

			case vtSignedInt:
				doWriteInteger<signed int>(address, size, v);
				break;

			case vtUnsignedInt:
				doWriteInteger<unsigned int>(address, size, v);
				break;

			case vtSignedLong:
				doWriteInteger<signed long>(address, size, v);
				break;

			case vtUnsignedLong:
				doWriteInteger<unsigned long>(address, size, v);
				break;

			case vtSignedLongLong:
				doWriteInteger<signed long long>(address, size, v);
				break;

			case vtUnsignedLongLong:
				doWriteInteger<unsigned long long>(address, size, v);
				break;

			default:
				GASSERT(false);
				break;

		}
	}
}



} // namespace cpgf



