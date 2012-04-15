#ifndef __GMETAXMLARCHIVE_H
#define __GMETAXMLARCHIVE_H

#include "cpgf/serialization/gmetastreamfundamental.h"


namespace cpgf {

struct IMetaWriter;
struct IMetaReader;
struct IMetaService;

class GMetaXmlArchive;
class GMetaXmlArchiveImplement;

class GMetaXmlArchive
{
public:
	GMetaXmlArchive();
	~GMetaXmlArchive();

	void load(const char * xmlContent) const;
	void saveToStream(std::ostream & outputStream) const;
	
	// xmlContent must be valid during the GMetaXmlArchive
	// and xmlContent will be destroyed.
	// This is an optimization vesion of load();
	void loadIntrusive(char * xmlContent) const;

	// internal use
	GMetaXmlArchiveImplement * getImplement() const;

private:
	mutable GScopedPointer<GMetaXmlArchiveImplement> implement;
};


IMetaWriter * createXmlMetaWriter(const GMetaXmlArchive & xmlArchive);

IMetaReader * createXmlMetaReader(IMetaService * service, const GMetaXmlArchive & xmlArchive);


} // namespace cpgf



#endif

