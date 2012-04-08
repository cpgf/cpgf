#ifndef __GMETAJSONARCHIVE_H
#define __GMETAJSONARCHIVE_H

#include "cpgf/gscopedptr.h"

#include <ostream>

namespace cpgf {

struct IMetaWriter;
struct IMetaReader;
struct IMetaService;

class GMetaJsonArchive;


namespace serialization_internal {

	IMetaWriter * doCreateJsonMetaWriter(const GMetaJsonArchive & jsonArchive);
	IMetaReader * doCreateJsonMetaReader(IMetaService * service, const GMetaJsonArchive & jsonArchive);

} // namespace serialization_internal


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


inline IMetaWriter * createJsonMetaWriter(const GMetaJsonArchive & jsonArchive)
{
	return serialization_internal::doCreateJsonMetaWriter(jsonArchive);
}

inline IMetaReader * createJsonMetaReader(IMetaService * service, const GMetaJsonArchive & jsonArchive)
{
	return serialization_internal::doCreateJsonMetaReader(service, jsonArchive);
}


} // namespace cpgf



#endif

