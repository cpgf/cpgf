#ifndef __GMETATEXTSTREAMARCHIVE_H
#define __GMETATEXTSTREAMARCHIVE_H


#include "cpgf/serialization/gmetaarchivetypemap.h"
#include "cpgf/serialization/gmetaarchivecommon.h"
#include "cpgf/serialization/gmetastreamfundamental.h"


namespace cpgf {

struct IMetaWriter;
struct IMetaReader;

namespace serialization_internal {

	IMetaWriter * doCreateTextStreamMetaWriter(std::ostream & outputStream, FuncStreamWriteFundamental func);
	IMetaReader * doCreateTextStreamMetaReader(IMetaService * service, std::istream & inputStream, FuncStreamReadFundamental func);

} // namespace serialization_internal



template <template<typename T> class TypeMap>
IMetaWriter * createTextStreamMetaWriter(std::ostream & outputStream, int)
{
	return serialization_internal::doCreateTextStreamMetaWriter(outputStream, &streamWriteFundamental<TypeMap>);
}

inline IMetaWriter * createTextStreamMetaWriter(std::ostream & outputStream)
{
	return serialization_internal::doCreateTextStreamMetaWriter(outputStream, &streamWriteFundamental<PermenentTypeMap>);
}

template <template<typename T> class TypeMap>
IMetaReader * createTextStreamMetaReader(IMetaService * service, std::istream & inputStream, int)
{
	return serialization_internal::doCreateTextStreamMetaReader(service, inputStream, &streamReadFundamental<TypeMap>);
}

inline IMetaReader * createTextStreamMetaReader(IMetaService * service, std::istream & inputStream)
{
	return serialization_internal::doCreateTextStreamMetaReader(service, inputStream, &streamReadFundamental<PermenentTypeMap>);
}



} // namespace cpgf


#endif

