#include "cpgf/metatraits/gmetaserializer.h"
#include "../../testprojects/seed/src/gmetaarchivecommon.h"

#include <string>


namespace cpgf {

class GMetaSerializerString : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		(void)archiveWriter;
		(void)metaClass;

		metaWriter->writeString("", archiveID, static_cast<std::string *>(instance)->c_str());
	}
	
	virtual void * G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaReader * metaReader, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		(void)archiveReader;
		(void)metaClass;

		if(instance == NULL) {
			instance = new std::string;
		}
		
		char * s = metaReader->readString("", archiveReader->getAllocator(), &archiveID);
		*static_cast<std::string *>(instance) = s;
		archiveReader->getAllocator()->free(s);

		return instance;
	}
};


IMetaSerializer * metaTraitsCreateSerializer(const std::string &)
{
	return new cpgf::GMetaSerializerString;
}

IMetaSerializer * metaTraitsCreateSerializer(std::string * &)
{
	return new cpgf::GMetaSerializerString;
}

IMetaSerializer * metaTraitsCreateSerializer(const std::string * &)
{
	return new cpgf::GMetaSerializerString;
}



} // namespace cpgf

