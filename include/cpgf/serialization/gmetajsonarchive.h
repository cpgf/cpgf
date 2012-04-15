#ifndef __GMETAJSONARCHIVE_H
#define __GMETAJSONARCHIVE_H

#include "cpgf/gscopedptr.h"

#include <ostream>

namespace cpgf {

struct IMetaWriter;
struct IMetaReader;
struct IMetaService;

class GMetaJsonArchive;


class GMetaJsonArchiveImplement;

class GMetaJsonArchive
{
public:
	GMetaJsonArchive();
	~GMetaJsonArchive();

	void load(const char * jsonContent) const;
	void saveToStream(std::ostream & outputStream) const;

	// internal use
	GMetaJsonArchiveImplement * getImplement() const;

private:
	mutable GScopedPointer<GMetaJsonArchiveImplement> implement;
};


IMetaWriter * createJsonMetaWriter(const GMetaJsonArchive & jsonArchive);

IMetaReader * createJsonMetaReader(IMetaService * service, const GMetaJsonArchive & jsonArchive);


} // namespace cpgf



#endif

