#ifndef __GMETASERIALIZER_STRING_H
#define __GMETASERIALIZER_STRING_H


#include <string>


namespace cpgf {

struct IMetaSerializer;

IMetaSerializer * metaTraitsCreateSerializer(const std::string &);
IMetaSerializer * metaTraitsCreateSerializer(std::string * &);
IMetaSerializer * metaTraitsCreateSerializer(const std::string * &);



} // namespace cpgf


#endif
