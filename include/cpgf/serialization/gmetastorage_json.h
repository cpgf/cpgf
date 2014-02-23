#ifndef CPGF_GMETASTORAGE_JSON_H
#define CPGF_GMETASTORAGE_JSON_H

#include "cpgf/gscopedptr.h"

#include <ostream>

namespace cpgf {

struct IMetaStorageWriter;
struct IMetaStorageReader;
struct IMetaService;

class GMetaJsonStorage;


class GMetaJsonStorageImplement;

class GMetaJsonStorage
{
public:
	GMetaJsonStorage();
	~GMetaJsonStorage();

	void load(const char * jsonContent) const;
	void saveToStream(std::ostream & outputStream) const;

	// internal use
	GMetaJsonStorageImplement * getImplement() const;

private:
	mutable GScopedPointer<GMetaJsonStorageImplement> implement;
};


IMetaStorageWriter * createJsonStorageWriter(const GMetaJsonStorage & jsonStorage);
IMetaStorageReader * createJsonStorageReader(const GMetaJsonStorage & jsonStorage);


} // namespace cpgf



#endif

