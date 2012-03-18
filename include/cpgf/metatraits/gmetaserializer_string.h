#ifndef __GMETASERIALIZER_STRING_H
#define __GMETASERIALIZER_STRING_H

#include <string>


namespace cpgf {

struct IMetaSerializer;

cpgf::IMetaSerializer * metaTraitsCreateSerializer(std::string *);
cpgf::IMetaSerializer * metaTraitsCreateSerializer(const std::string *);



} // namespace cpgf



#endif
