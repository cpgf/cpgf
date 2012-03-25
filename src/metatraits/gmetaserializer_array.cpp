#include "cpgf/metatraits/gmetaserializer.h"
#include "../../testprojects/seed/src/gmetaarchivecommon.h"


namespace cpgf {

namespace metatraits_internal {

class GMetaSerializerArray : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT

public:
	GMetaSerializerArray(IMetaSerializer * elementSerializer, unsigned int elementSize, unsigned int elementCount)
		: elementSerializer(elementSerializer), elementSize(elementSize), elementCount(elementCount)
	{
		this->elementSerializer->addReference();
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		for(unsigned int i = 0; i < this->elementCount; ++i) {
			this->elementSerializer->writeObject(archiveWriter, metaWriter, archiveID, instance, metaClass);
			instance = static_cast<char *>(instance) + this->elementSize;
		}
	}
	
	virtual void * G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaReader * metaReader, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		for(unsigned int i = 0; i < this->elementCount; ++i) {
			this->elementSerializer->readObject(archiveReader, metaReader, archiveID, instance, metaClass);
			instance = static_cast<char *>(instance) + this->elementSize;
		}

		return instance;
	}
	
private:
	GScopedInterface<IMetaSerializer> elementSerializer;
	unsigned int elementSize;
	unsigned int elementCount;	
};


IMetaSerializer * createArraySerializer(IMetaSerializer * elementSerializer, unsigned int elementSize, unsigned int elementCount)
{
	if(elementSerializer == NULL) {
		return NULL;
	}
	else {
		return new GMetaSerializerArray(elementSerializer, elementSize, elementCount);
	}
}

} // namespace metatraits_internal



} // namespace cpgf

