#ifndef __GMETATEXTSTREAMARCHIVE_H
#define __GMETATEXTSTREAMARCHIVE_H


#include "cpgf/serialization/gmetastreamfundamental.h"

#include <iostream>

namespace cpgf {

struct IMetaService;
struct IMetaWriter;
struct IMetaReader;

IMetaWriter * createTextStreamMetaWriter(std::ostream & outputStream);

IMetaReader * createTextStreamMetaReader(IMetaService * service, std::istream & inputStream);



} // namespace cpgf


#endif

