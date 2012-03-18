#ifndef __GMETACONVERTER_STRING_H
#define __GMETACONVERTER_STRING_H


#include <string>


namespace cpgf {

struct IMetaConverter;

IMetaConverter * metaTraitsCreateConverter(std::string *);
IMetaConverter * metaTraitsCreateConverter(const std::string *);



} // namespace cpgf



#endif
