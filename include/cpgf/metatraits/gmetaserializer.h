#ifndef __GMETASERIALIZER_H
#define __GMETASERIALIZER_H


#include "cpgf/gapiutil.h"


namespace cpgf {

struct IMetaArchiveWriter;
struct IMetaArchiveReader;
struct IMetaWriter;
struct IMetaReader;
struct IMetaClass;

struct IMetaSerializer : public IObject
{
	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * archiveWriter, const void * instance, IMetaClass * metaClass) = 0;
	virtual void G_API_CC writeObject(const char * name, IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, uint32_t archiveID, const void * instance, IMetaClass * metaClass) = 0;
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * archiveReader, IMetaClass * metaClass) = 0;
	virtual void G_API_CC readObject(const char * name, IMetaArchiveReader * archiveReader, IMetaReader * metaReader, uint32_t archiveID, void * instance, IMetaClass * metaClass) = 0;
};


template <typename T>
inline IMetaSerializer * metaTraitsCreateSerializer(const T &)
{
	return NULL;
}



} // namespace cpgf



#endif
