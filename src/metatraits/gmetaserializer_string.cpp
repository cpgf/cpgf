#include "cpgf/metatraits/gmetaserializer.h"
#include "../../testprojects/seed/src/gmetaarchivecommon.h"

#include <string>


namespace cpgf {

class GMetaSerializerString : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * archiveWriter, const void * instance, IMetaClass * metaClass) {
		(void)archiveWriter;
		(void)instance;
		(void)metaClass;

		return "meta_ser_std_string";
	}
	
	virtual void G_API_CC writeObject(const char * name, IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, uint32_t archiveID, const void * instance, IMetaClass * metaClass, uint32_t pointers) {
		(void)archiveWriter;
		(void)metaClass;

		archiveWriter->trackPointer(archiveID, instance, metaClass, this, pointers);
		metaWriter->writeString(name, archiveID, static_cast<const std::string *>(instance)->c_str());
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass) {
		(void)archiveReader;
		(void)metaClass;

		return new std::string;
	}

	virtual void G_API_CC readObject(const char * name, IMetaArchiveReader * archiveReader, IMetaReader * metaReader, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		(void)archiveReader;
		(void)metaClass;

		archiveReader->trackPointer(archiveID, instance);

		char * s = metaReader->readString(name, archiveReader->getAllocator(), &archiveID);
		*static_cast<std::string *>(instance) = s;
		archiveReader->getAllocator()->free(s);
	}
};


IMetaSerializer * metaTraitsCreateSerializer(const std::string &)
{
	return new cpgf::GMetaSerializerString;
}



} // namespace cpgf

