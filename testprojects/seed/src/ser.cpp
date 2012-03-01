#include "cpgf/ginterface.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaapiutil.h"

#include <iostream>
using namespace std;


/*
IMetaWriter -- the primary writer to write data to different output, such as binary, text, etc.
Meta archive writer -- write meta object/value to archive writer. It does the pointers resolve. It doesn't use meta item's archive class.
Meta archive -- archiving. It uses meta item's archive class.
*/

namespace cpgf {

struct IMetaArchiveWriter {};

struct IMetaWriter : public IObject
{
	virtual void G_API_CC writeFundamental(IMetaArchiveWriter * archiveWriter, const char * name, const GVariant & value) = 0;
	virtual void G_API_CC writeString(IMetaArchiveWriter * archiveWriter, const char * name, const char * value) = 0;

	virtual void G_API_CC beginWriteObject(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaClass * metaClass) = 0;
	virtual void G_API_CC endWriteObject(IMetaArchiveWriter * archiveWriter, const char * name, void * instance, IMetaClass * metaClass) = 0;

	virtual void G_API_CC beginWriteArray(IMetaArchiveWriter * archiveWriter, const char * name, uint32_t length, IMetaTypedItem * typeItem) = 0;
	virtual void G_API_CC endWriteArray(IMetaArchiveWriter * archiveWriter, const char * name, uint32_t length, IMetaTypedItem * typeItem) = 0;
};


class GMetaArchive;

class GMetaArchiveWriter : public IMetaArchiveWriter
{
public:
	explicit GMetaArchiveWriter(IMetaWriter * writer, GMetaArchive * archive);

	void writeObject(const char * name, void * instance, IMetaClass * metaClass);
	void writeMetaItem(const char * name, void * instance, IMetaItem * metaItem);

protected:
	void doWriteObject(const char * name, void * instance, IMetaClass * metaClass);
	void doWriteMetaAccessible(const char * name, void * instance, IMetaAccessible * accessible);
	void doWriteValue(const char * name, const GVariant & value, const GMetaType & metaType);

private:
	IMetaWriter * writer;
	GMetaArchive * archive;
};


class GMetaArchive
{
public:
	explicit GMetaArchive(IMetaWriter * writer);

	void writeObject(const char * name, void * instance, IMetaClass * metaClass);
	void writeMetaItem(const char * name, void * instance, IMetaItem * metaItem);

private:
	GScopedPointer<GMetaArchiveWriter> archiveWriter;
};

GMetaArchive::GMetaArchive(IMetaWriter * writer)
{
	this->archiveWriter.reset(new GMetaArchiveWriter(writer, this));
}

void GMetaArchive::writeObject(const char * name, void * instance, IMetaClass * metaClass)
{
	this->archiveWriter->writeObject(name, instance, metaClass);
}

void GMetaArchive::writeMetaItem(const char * name, void * instance, IMetaItem * metaItem)
{
	this->archiveWriter->writeMetaItem(name, instance, metaItem);
}



GMetaArchiveWriter::GMetaArchiveWriter(IMetaWriter * writer, GMetaArchive * archive)
	: writer(writer), archive(archive)
{
}

void GMetaArchiveWriter::writeObject(const char * name, void * instance, IMetaClass * metaClass)
{
	this->doWriteObject(name, instance, metaClass);
}

void GMetaArchiveWriter::writeMetaItem(const char * name, void * instance, IMetaItem * metaItem)
{
	switch(static_cast<GMetaCategory>(metaItem->getCategory())) {
		case mcatField:
		case mcatProperty:
			this->doWriteMetaAccessible(name, instance, static_cast<IMetaAccessible *>(metaItem));
			break;

		default:
			break;
	}
}

void GMetaArchiveWriter::doWriteObject(const char * name, void * instance, IMetaClass * metaClass)
{
	// todo: resolve field pointers.

}

void GMetaArchiveWriter::doWriteMetaAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	this->doWriteValue(name, metaGetValue(accessible, instance), metaGetItemType(accessible));
}

void GMetaArchiveWriter::doWriteValue(const char * name, const GVariant & value, const GMetaType & metaType)
{
	size_t pointers = metaType.getPointerDimension();

	if(pointers == 0) {
		if(metaType.isFundamental()) {
			this->writer->writeFundamental(this, name, value);
		}
		else if(metaType.baseIsClass()) {
		}
		else {
			// error
		}
	}
	else if(pointers == 1) {
	}
}


} // namespace cpgf


void testSer()
{
}

