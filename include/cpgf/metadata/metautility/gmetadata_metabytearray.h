#ifndef __GMETADATA_METABYTEARRAY_H
#define __GMETADATA_METABYTEARRAY_H


#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/gmetadataconfig.h"


#include "cpgf/metadata/private/gmetadata_header.h"

namespace cpgf {

namespace metadata_internal {


template <typename T, typename MetaDefine>
void doBuildMetaData_byteArray(const GMetaDataConfigFlags & config, MetaDefine define)
{
	M(T, getPointer)
	M(T, getCurrentPointer)
	
	M(T, getPosition)
	M(T, setPosition)
	
	M(T, getLength)
	M(T, setLength)
	
	M(T, readInt8)
	M(T, readInt16)
	M(T, readInt32)
	M(T, readInt64)
	
	M(T, readUint8)
	M(T, readUint16)
	M(T, readUint32)
	M(T, readUint64)
	
	M(T, readFloat32)
	M(T, readFloat64)
	
	M(T, readBuffer)
	
	M(T, writeInt8)
	M(T, writeInt16)
	M(T, writeInt32)
	M(T, writeInt64)
	
	M(T, writeUint8)
	M(T, writeUint16)
	M(T, writeUint32)
	M(T, writeUint64)
	
	M(T, writeFloat32)
	M(T, writeFloat64)
	
	M(T, writeBuffer)
	
	if(metaDataConfigIsAutoProperty(config)) {
		define._property("position", &T::getPosition, &T::setPosition);
		define._property("length", &T::getLength, &T::setLength);
	}
}


} // namespace metadata_internal


template <typename MetaDefine>
void buildMetaData_metaByteArray(const GMetaDataConfigFlags & config, MetaDefine define)
{
	metadata_internal::doBuildMetaData_byteArray<typename MetaDefine::ClassType>(config, define);
}

template <typename MetaDefine>
void buildMetaData_metaByteArray(MetaDefine define)
{
	buildMetaData_metaByteArray(mdcAutoProperty, define);
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"


#endif

