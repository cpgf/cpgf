#include "cpgf/ginterface.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaapiutil.h"

#include <iostream>
using namespace std;


/*
IArchiveWriter -- the primary writer to write data to different output, such as binary, text, etc.
Meta writer -- write meta object/value to archive writer. It just writes, no care of pointers resolve, etc.
Meta archive -- archiving. It does the pointers resolve.
*/

namespace cpgf {

struct IMetaArchiveWriter;

struct IArchiveWriter : public IObject
{
	virtual void G_API_CC writeFundamental(IMetaArchiveWriter * archiveWriter, const char * name, const GVariant & value) = 0;
	virtual void G_API_CC writeString(IMetaArchiveWriter * archiveWriter, const char * name, const char * value) = 0;
};


class GMetaArchive;

class GMetaArchiveWriter
{
public:
	explicit GMetaArchiveWriter(IArchiveWriter * writer, GMetaArchive * archive);

	void writeObject(const char * name, void * instance, IMetaClass * metaClass);
	void writeMetaItem(const char * name, void * instance, IMetaItem * metaItem);

protected:
	void doWriteMetaAccessible(IMetaAccessible * accessible, void * instance);
	void doWriteValue(const GVariant & value, const GMetaType & metaType);

private:
	IArchiveWriter * writer;
	GMetaArchive * archive;
};


class GMetaArchive
{
public:
	explicit GMetaArchive(IArchiveWriter * writer);

private:
	GScopedPointer<GMetaArchiveWriter> archiveWriter;
};

GMetaArchive::GMetaArchive(IArchiveWriter * writer)
{
	this->archiveWriter.reset(new GMetaArchiveWriter(writer, this));
}


GMetaArchiveWriter::GMetaArchiveWriter(IArchiveWriter * writer, GMetaArchive * archive)
	: writer(writer), archive(archive)
{
}

void GMetaArchiveWriter::writeObject(const char * name, void * instance, IMetaClass * metaClass)
{
}

void GMetaArchiveWriter::writeMetaItem(const char * name, void * instance, IMetaItem * metaItem)
{
	switch(static_cast<GMetaCategory>(metaItem->getCategory())) {
		case mcatField:
		case mcatProperty:
			this->doWriteMetaAccessible(static_cast<IMetaAccessible *>(metaItem), instance);
			break;

		default:
			break;
	}
}

void GMetaArchiveWriter::doWriteMetaAccessible(IMetaAccessible * accessible, void * instance)
{
	this->doWriteValue(metaGetValue(accessible, instance), metaGetItemType(accessible));
}

void GMetaArchiveWriter::doWriteValue(const GVariant & value, const GMetaType & metaType)
{
	size_t pointers = metaType.getPointerDimension();

	if(pointers == 0) {
	}
}


} // namespace cpgf


void testSer()
{
}

