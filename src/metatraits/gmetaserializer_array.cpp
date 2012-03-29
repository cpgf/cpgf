#include "cpgf/metatraits/gmetaserializer.h"
#include "../../testprojects/seed/src/gmetaarchivecommon.h"


namespace cpgf {

namespace metatraits_internal {

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
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, uint32_t archiveID, const void * instance, IMetaClass * metaClass) {
		GMetaTypeData typeData = this->metaType.getData();
		for(unsigned int i = 0; i < this->elementCount; ++i) {
			const void * ptr;
			if(this->metaType.getPointerDimension() > 0) {
				ptr = *(void **)instance;
			}
			else {
				ptr = instance;
			}
			archiveWriter->writeObject("", ptr, &typeData, this->elementSerializer.get());
			instance = static_cast<const char *>(instance) + this->elementSize;
		}
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * archiveReader, IMetaReader * metaReader, uint32_t archiveID, IMetaClass * metaClass) {
		(void)archiveReader;
		(void)metaReader;
		(void)archiveID;
		(void)metaClass;

		return NULL;
	}

	virtual void * G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaReader * metaReader, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		GMetaTypeData typeData = this->metaType.getData();
		for(unsigned int i = 0; i < this->elementCount; ++i) {
			archiveReader->readObject("", instance, &typeData, this->elementSerializer.get());
			instance = static_cast<char *>(instance) + this->elementSize;
		}

		return instance;
	}
	
private:
	GScopedInterface<IMetaSerializer> elementSerializer;
	GMetaType metaType;
	unsigned int elementSize;
	unsigned int elementCount;	
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

