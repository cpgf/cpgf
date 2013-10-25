#ifndef CPGF_GMETASTORAGE_TEXTSTREAM_H
#define CPGF_GMETASTORAGE_TEXTSTREAM_H


#include "cpgf/serialization/gmetastreamfundamental.h"

#include <iostream>

namespace cpgf {

struct IMetaService;
struct IMetaStorageWriter;
struct IMetaStorageReader;

IMetaStorageWriter * createTextStreamStorageWriter(std::ostream & outputStream);
IMetaStorageReader * createTextStreamStorageReader(std::istream & inputStream);



} // namespace cpgf


#endif

