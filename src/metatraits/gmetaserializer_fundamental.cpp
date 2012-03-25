#include "cpgf/metatraits/gmetaserializer.h"
#include "../../testprojects/seed/src/gmetaarchivecommon.h"


namespace cpgf {

template <typename T>
class GMetaSerializerFundamental : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		(void)archiveWriter;
		(void)metaClass;

		GVariant v(*static_cast<T *>(instance));
		metaWriter->writeFundamental("", archiveID, &v.data);
	}
	
	virtual void * G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaReader * metaReader, uint32_t archiveID, void * instance, IMetaClass * metaClass) {
		(void)archiveReader;
		(void)metaClass;

		GVariant v;
		metaReader->readFundamental("", &archiveID, &v.data);
		*static_cast<T *>(instance) = fromVariant<T>(v);

		return instance;
	}
};


#define DEF(T) IMetaSerializer * metaTraitsCreateSerializer(const T &) { return new GMetaSerializerFundamental<T>; }

DEF(bool)
DEF(char)
DEF(wchar_t)
DEF(signed char)
DEF(unsigned char)
DEF(signed short)
DEF(unsigned short)
DEF(signed int)
DEF(unsigned int)
DEF(signed long)
DEF(unsigned long)
DEF(signed long long)
DEF(unsigned long long)
DEF(float)
DEF(double)
DEF(long double)


#undef DEF



} // namespace cpgf

