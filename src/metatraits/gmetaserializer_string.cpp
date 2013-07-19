#include "cpgf/metatraits/gmetaserializer_string.h"
#include "cpgf/serialization/gmetaarchivecommon.h"
#include "cpgf/serialization/gmetaarchivereader.h"
#include "cpgf/serialization/gmetaarchivewriter.h"

#include <string>


namespace cpgf {

class GMetaSerializerString : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual ~GMetaSerializerString() {}

	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * /*archiveWriter*/, const void * /*instance*/, IMetaClass * /*metaClass*/) {
		return "meta_ser_std_string";
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * /*serializerWriter*/, GMetaArchiveWriterParam * param) {
		GScopedInterface<IMetaStorageWriter> metaWriter(archiveWriter->getStorageWriter());
		archiveWriter->trackPointer(param->archiveID, param->instance, param->metaClass, this, param->pointers);
		metaWriter->writeString(param->name, param->archiveID, static_cast<const std::string *>(param->instance)->c_str());
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * /*archiveReader*/, IMetaClass * /*metaClass*/) {
		return new std::string;
	}

	virtual void G_API_CC freeObject(IMetaArchiveReader * /*archiveReader*/, IMetaClass * /*metaClass*/, void * instance) {
		delete static_cast<std::string *>(instance);
	}

	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaSerializerReader * /*serializerReader*/, GMetaArchiveReaderParam * param) {
		GScopedInterface<IMetaStorageReader> metaReader(archiveReader->getStorageReader());
		uint32_t archiveID;
		char * s = metaReader->readString(param->name, archiveReader->getAllocator(), &archiveID);
		*static_cast<std::string *>(param->instance) = s;
		archiveReader->getAllocator()->free(s);
		
		archiveReader->trackPointer(archiveID, param->instance);
	}
};

namespace metatraits_internal {
	IMetaSerializer * doCreateSerializerForStdString()
	{
		return new cpgf::GMetaSerializerString();
	}
} // namespace metatraits_internal


} // namespace cpgf


