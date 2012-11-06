#ifndef __GMETADATA_METACORE_H
#define __GMETADATA_METACORE_H


#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/gmetadataconfig.h"


#include "cpgf/metadata/private/gmetadata_header.h"

namespace cpgf {

namespace metadata_internal {


template <typename T, typename MetaDefine>
void doBuildMetaData_metaCore(const GMetaDataConfigFlags & /*config*/, MetaDefine define)
{
	M(T, cloneClass)
	define._method("cast", &T::cast)
		._default((void *)0);
}


} // namespace metadata_internal


template <typename MetaDefine>
void buildMetaData_metaCore(const GMetaDataConfigFlags & config, MetaDefine define)
{
	metadata_internal::doBuildMetaData_metaCore<typename MetaDefine::ClassType>(config, define);
}

template <typename MetaDefine>
void buildMetaData_metaCore(MetaDefine define)
{
	buildMetaData_metaCore(GMetaDataConfigFlags(), define);
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"


#endif

