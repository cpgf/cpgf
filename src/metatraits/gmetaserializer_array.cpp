#include "cpgf/metatraits/gmetaserializer.h"
#include "../../testprojects/seed/src/gmetaarchivecommon.h"


#include <string>
#include <stdio.h>

using namespace std;


namespace cpgf {

namespace metatraits_internal {

const char * const elementName = "item";

class GMetaSerializerArray : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

public:
	GMetaSerializerArray(IMetaSerializer * elementSerializer, const GMetaType & metaType, unsigned int elementSize, unsigned int elementCount)
		: elementSerializer(elementSerializer), metaType(metaType), elementSize(elementSize), elementCount(elementCount)
	{
		this->elementSerializer->addReference();
	}
	
	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * archiveWriter, const void * instance, IMetaClass * metaClass) {
			if(this->classType == "") {
				const char * typeName = this->elementSerializer->getClassTypeName(archiveWriter, instance, metaClass);
				if(typeName != NULL) {
					char buffer[128];
					sprintf(buffer, "[%u]", this->elementCount);
					this->classType = string(typeName) + buffer;
				}
			}
			
			if(this->classType.size() == 0) {
				return NULL;
			}
			else {
				return this->classType.c_str();
			}
	}

	virtual void G_API_CC writeObject(const char * name, IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, uint32_t archiveID, const void * instance, IMetaClass * metaClass, uint32_t pointers) {
		metaWriter->beginWriteArray(name, this->elementCount);

		GMetaTypeData typeData = this->metaType.getData();
		for(unsigned int i = 0; i < this->elementCount; ++i) {
			const void * ptr;
			if(this->metaType.isPointer() && ! this->metaType.baseIsArray()) {
				ptr = *(void **)instance;
			}
			else {
				ptr = instance;
			}
			archiveWriter->writeObject(elementName, ptr, &typeData, this->elementSerializer.get());
			instance = static_cast<const char *>(instance) + this->elementSize;
		}
		
		metaWriter->endWriteArray(name, this->elementCount);
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass) {
		(void)archiveReader;
		(void)metaClass;

		return NULL;
	}

	virtual void G_API_CC readObject(const char * name, IMetaArchiveReader * archiveReader, IMetaReader * metaReader, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		uint32_t length = metaReader->beginReadArray(name);
		
		GASSERT(length == this->elementCount);

		GMetaTypeData typeData = this->metaType.getData();
		for(unsigned int i = 0; i < this->elementCount; ++i) {
			archiveReader->readObject(elementName, instance, &typeData, this->elementSerializer.get());
			instance = static_cast<char *>(instance) + this->elementSize;
		}

		metaReader->endReadArray(name);
	}
	
private:
	GScopedInterface<IMetaSerializer> elementSerializer;
	GMetaType metaType;
	unsigned int elementSize;
	unsigned int elementCount;
	string classType;
};


IMetaSerializer * createArraySerializer(IMetaSerializer * elementSerializer, const GMetaType & metaType, unsigned int elementSize, unsigned int elementCount)
{
	if(elementSerializer == NULL) {
		return NULL;
	}
	else {
		return new GMetaSerializerArray(elementSerializer, metaType, elementSize, elementCount);
	}
}

} // namespace metatraits_internal



} // namespace cpgf

