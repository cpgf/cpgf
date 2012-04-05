#ifndef __GMETAXmlARCHIVE_H
#define __GMETAXmlARCHIVE_H

#include "gmetastreamfundamental.h"


namespace cpgf {

struct IMetaWriter;
struct IMetaReader;
struct IMetaService;

class GMetaXmlArchive;


namespace serialization_internal {

	IMetaWriter * doCreateXmlMetaWriter(const GMetaXmlArchive & xmlArchive, FuncStreamWriteFundamental func);
	IMetaReader * doCreateXmlMetaReader(IMetaService * service, const GMetaXmlArchive & xmlArchive, FuncStreamReadFundamental func);

} // namespace serialization_internal


class GMetaXmlArchiveImplement;

class GMetaXmlArchive
{
public:
	GMetaXmlArchive();
	~GMetaXmlArchive();

	void loadIntrusive(char * xmlContent) const;
	void loadNonIntrusive(const char * xmlContent) const;
	void saveToStream(std::ostream & outputStream) const;

	// internal use
	GMetaXmlArchiveImplement * getImplement() const;

private:
	mutable GScopedPointer<GMetaXmlArchiveImplement> implement;
};


template <template<typename T> class TypeMap>
IMetaWriter * createXmlMetaWriter(const GMetaXmlArchive & xmlArchive, int)
{
	return serialization_internal::doCreateXmlMetaWriter(xmlArchive, &streamWriteFundamental<TypeMap>);
}

inline IMetaWriter * createXmlMetaWriter(const GMetaXmlArchive & xmlArchive)
{
	return serialization_internal::doCreateXmlMetaWriter(xmlArchive, &streamWriteFundamental<PermenentTypeMap>);
}

template <template<typename T> class TypeMap>
IMetaReader * createXmlMetaReader(IMetaService * service, const GMetaXmlArchive & xmlArchive, int)
{
	return serialization_internal::doCreateXmlMetaReader(service, xmlArchive, &streamReadFundamental<TypeMap>);
}

inline IMetaReader * createXmlMetaReader(IMetaService * service, const GMetaXmlArchive & xmlArchive)
{
	return serialization_internal::doCreateXmlMetaReader(service, xmlArchive, &streamReadFundamental<PermenentTypeMap>);
}


} // namespace cpgf



#endif

