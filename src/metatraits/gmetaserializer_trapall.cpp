#include "cpgf/metatraits/gmetaserializer_trapall.h"
#include "cpgf/serialization/gmetaarchivecommon.h"
#include "cpgf/serialization/gmetaarchivereader.h"
#include "cpgf/serialization/gmetaarchivewriter.h"
#include "cpgf/gsharedinterface.h"


namespace cpgf {

class GMetaSerializerTrapAll : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	GMetaSerializerTrapAll(const GMetaType & metaType, IMetaSerializer * serializer)
		: metaType(metaType), serializer(serializer)
	{
	}
	
	virtual ~GMetaSerializerTrapAll() {}

	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * archiveWriter, const void * instance, IMetaClass * metaClass) {
		if(this->serializer) {
			return this->serializer->getClassTypeName(archiveWriter, instance, metaClass);
		}
		else {
			return this->metaType.getBaseName();
		}
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * /*serializerWriter*/, GMetaArchiveWriterParam * param) {
		GMetaTypeData typeData = this->metaType.refData();
		archiveWriter->writeData(param->name, param->instance, &typeData, this->serializer.get());
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass) {
		if(this->serializer) {
			return this->serializer->allocateObject(archiveReader, metaClass);
		}
		else {
			GScopedInterface<IMetaClass> metaClassHolder;
			if(metaClass == nullptr && this->metaType.getBaseName() != nullptr) {
				GScopedInterface<IMetaService> service(archiveReader->getMetaService());
				metaClassHolder.reset(service->findClassByName(this->metaType.getBaseName()));
				metaClass = metaClassHolder.get();
			}

			if(metaClass != nullptr) {
				return metaClass->createInstance();
			}
			else {
				return nullptr;
			}
		}
	}

	virtual void G_API_CC freeObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass, void * instance) {
		if(this->serializer) {
			this->serializer->freeObject(archiveReader, metaClass, instance);
		}
		else {
			GScopedInterface<IMetaClass> metaClassHolder;
			if(metaClass == nullptr && this->metaType.getBaseName() != nullptr) {
				GScopedInterface<IMetaService> service(archiveReader->getMetaService());
				metaClassHolder.reset(service->findClassByName(this->metaType.getBaseName()));
				metaClass = metaClassHolder.get();
			}

			if(metaClass != nullptr) {
				metaClass->destroyInstance(instance);
			}
		}
	}

	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaSerializerReader * /*serializerReader*/, GMetaArchiveReaderParam * param) {
		GMetaTypeData typeData = this->metaType.refData();
		void * ptr = param->instance;
		if(this->metaType.isPointer()) {
			ptr = &param->instance;
		}
		archiveReader->readData(param->name, ptr, &typeData, this->serializer.get());
	}

private:
	GMetaType metaType;
	GSharedInterface<IMetaSerializer> serializer;
};

namespace metatraits_internal {

IMetaSerializer * doCreateTrapAllSerializer(const GMetaType & metaType, IMetaSerializer * serializer)
{
	if(serializer != nullptr || canSerializeMetaType(metaType)) {
		return new GMetaSerializerTrapAll(metaType, serializer);
	}
	else {
		return nullptr;
	}
}

} // namespace metatraits_internal


} // namespace cpgf

