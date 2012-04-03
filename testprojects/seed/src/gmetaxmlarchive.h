#ifndef __GMETAXmlARCHIVE_H
#define __GMETAXmlARCHIVE_H

#include "gmetastreamfundamental.h"


namespace cpgf {

struct IMetaWriter;
struct IMetaReader;
struct IMetaService;


namespace serialization_internal {

	IMetaWriter * doCreateXmlMetaWriter(std::ostream & outputStream, FuncStreamWriteFundamental func);
	IMetaReader * doCreateXmlMetaReader(IMetaService * service, char * xmlContent, FuncStreamReadFundamental func);

} // namespace serialization_internal



template <template<typename T> class TypeMap>
IMetaWriter * createXmlMetaWriter(std::ostream & outputStream, int)
{
	return serialization_internal::doCreateXmlMetaWriter(outputStream, &streamWriteFundamental<TypeMap>);
}

inline IMetaWriter * createXmlMetaWriter(std::ostream & outputStream)
{
	return serialization_internal::doCreateXmlMetaWriter(outputStream, &streamWriteFundamental<PermenentTypeMap>);
}

template <template<typename T> class TypeMap>
IMetaReader * createXmlMetaReader(IMetaService * service, char * xmlContent, int)
{
	return serialization_internal::doCreateXmlMetaReader(service, xmlContent, &streamReadFundamental<TypeMap>);
}

inline IMetaReader * createXmlMetaReader(IMetaService * service, char * xmlContent)
{
	return serialization_internal::doCreateXmlMetaReader(service, xmlContent, &streamReadFundamental<PermenentTypeMap>);
}


} // namespace cpgf



#endif

