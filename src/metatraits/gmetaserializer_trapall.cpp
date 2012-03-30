#include "cpgf/metatraits/gmetaserializer_trapall.h"
#include "../../testprojects/seed/src/gmetaarchivecommon.h"


namespace cpgf {

class GMetaSerializerTrapAll : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	GMetaSerializerTrapAll(const GMetaType & metaType, IMetaSerializer * serializer)
		: metaType(metaType), serializer(serializer)
	{
		if(this->serializer) {
			this->serializer->addReference();
		}
	}

	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, uint32_t archiveID, const void * instance, IMetaClass * metaClass) {
		if(this->serializer) {
			return this->serializer->getClassTypeName(archiveWriter, metaWriter, archiveID, instance, metaClass);
		}
		else {
			return this->metaType.getBaseName();
		}
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, uint32_t archiveID, const void * instance, IMetaClass * metaClass) {
		(void)archiveWriter;
		(void)metaClass;

		GMetaTypeData typeData = this->metaType.getData();
		archiveWriter->writeObject("", instance, &typeData, this->serializer.get());
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * archiveReader, IMetaReader * metaReader, uint32_t archiveID, IMetaClass * metaClass) {
		(void)archiveReader;
		(void)metaReader;
		(void)archiveID;
		(void)metaClass;

		return NULL;
	}

	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaReader * metaReader, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		(void)archiveReader;
		(void)metaClass;

		GMetaTypeData typeData = this->metaType.getData();
		archiveReader->readObject("", instance, &typeData, this->serializer.get());
	}

private:
	GMetaType metaType;
	GScopedInterface<IMetaSerializer> serializer;
};

namespace metatraits_internal {

IMetaSerializer * doCreateTrapAllSerializer(const GMetaType & metaType, IMetaSerializer * serializer)
{
	return new GMetaSerializerTrapAll(metaType, serializer);
}

} // namespace metatraits_internal


} // namespace cpgf

