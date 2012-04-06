#include "cpgf/metatraits/gmetaserializer_trapall.h"
#include "cpgf/serialization/gmetaarchivecommon.h"
#include "cpgf/serialization/gmetaarchivereader.h"
#include "cpgf/serialization/gmetaarchivewriter.h"


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

	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * archiveWriter, const void * instance, IMetaClass * metaClass) {
		if(this->serializer) {
			return this->serializer->getClassTypeName(archiveWriter, instance, metaClass);
		}
		else {
			return this->metaType.getBaseName();
		}
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaWriter * /*metaWriter*/, GMetaArchiveWriterParam * param) {
		GMetaTypeData typeData = this->metaType.getData();
		archiveWriter->writeData(param->name, param->instance, &typeData, this->serializer.get());
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass) {
		if(this->serializer) {
			return this->serializer->allocateObject(archiveReader, metaClass);
		}
		else {
			GScopedInterface<IMetaClass> metaClassHolder;
			if(metaClass == NULL && this->metaType.getBaseName() != NULL) {
				GScopedInterface<IMetaService> service(archiveReader->getMetaService());
				metaClassHolder.reset(service->findClassByName(this->metaType.getBaseName()));
				metaClass = metaClassHolder.get();
			}

			if(metaClass != NULL) {
				return metaClass->createInstance();
			}
			else {
				return NULL;
			}
		}
	}

	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaReader * metaReader, GMetaArchiveReaderParam * param) {
		GMetaTypeData typeData = this->metaType.getData();
		void * ptr = param->instance;
		if(this->metaType.isPointer()) {
			ptr = &param->instance;
		}
		archiveReader->readData(param->name, ptr, &typeData, this->serializer.get());
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

