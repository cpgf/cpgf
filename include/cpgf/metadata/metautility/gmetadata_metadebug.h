#ifndef __GMETADATA_METADEBUG_H
#define __GMETADATA_METADEBUG_H


#include "cpgf/gmetadefine.h"

#include "cpgf/metadata/gmetadataconfig.h"


#include "cpgf/metadata/private/gmetadata_header.h"

namespace cpgf {

namespace metadata_internal {


template <typename T, typename MetaDefine>
void doBuildMetaData_metaDebug(const GMetaDataConfigFlags & /*config*/, MetaDefine define)
{
	M(T, debugBreak)
	
	M(T, trace)
	M(T, traceError)
	M(T, traceLine)
	M(T, traceErrorLine)
	
	M(T, inspect)
}


} // namespace metadata_internal


template <typename MetaDefine>
void buildMetaData_metaDebug(const GMetaDataConfigFlags & config, MetaDefine define)
{
	metadata_internal::doBuildMetaData_metaDebug<typename MetaDefine::ClassType>(config, define);
}

template <typename MetaDefine>
void buildMetaData_metaDebug(MetaDefine define)
{
	buildMetaData_metaDebug(GMetaDataConfigFlags(), define);
}


} // namespace cpgf


#include "cpgf/metadata/private/gmetadata_footer.h"


#endif

