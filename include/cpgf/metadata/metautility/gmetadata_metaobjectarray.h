#ifndef __GMETADATA_METAOBJECTARRAY_H
#define __GMETADATA_METAOBJECTARRAY_H


#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/gmetadataconfig.h"


#include "cpgf/metadata/private/gmetadata_header.h"

namespace cpgf {

namespace metadata_internal {


template <typename T, typename MetaDefine>
void doBuildMetaData_metaObjectArray(const GMetaDataConfigFlags & /*config*/, MetaDefine define)
{
	M(T, getItem)
	M(T, setItem)

	M(T, getCount)
}


} // namespace metadata_internal


template <typename MetaDefine>
void buildMetaData_metaObjectArray(const GMetaDataConfigFlags & config, MetaDefine define)
{
	metadata_internal::doBuildMetaData_metaObjectArray<typename MetaDefine::ClassType>(config, define);
}

template <typename MetaDefine>
void buildMetaData_metaObjectArray(MetaDefine define)
{
	buildMetaData_metaObjectArray(GMetaDataConfigFlags(), define);
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"


#endif

