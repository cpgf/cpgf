#ifndef CPGF_GMETASTORAGE_XML_H
#define CPGF_GMETASTORAGE_XML_H

#include "cpgf/serialization/gmetastreamfundamental.h"

#include <memory>

namespace cpgf {

struct IMetaStorageWriter;
struct IMetaStorageReader;
struct IMetaService;

class GMetaXmlStorage;
class GMetaXmlStorageImplement;

class GMetaXmlStorage
{
public:
	GMetaXmlStorage();
	~GMetaXmlStorage();

	void load(const char * xmlContent) const;
	void saveToStream(std::ostream & outputStream) const;
	
	// xmlContent must be valid during the GMetaXmlStorage
	// and xmlContent will be destroyed.
	// This is an optimization vesion of load();
	void loadIntrusive(char * xmlContent) const;

	// internal use
	GMetaXmlStorageImplement * getImplement() const;

private:
	mutable std::unique_ptr<GMetaXmlStorageImplement> implement;
};


IMetaStorageWriter * createXmlStorageWriter(const GMetaXmlStorage & xmlStorage);
IMetaStorageReader * createXmlStorageReader(const GMetaXmlStorage & xmlStorage);


} // namespace cpgf



#endif

