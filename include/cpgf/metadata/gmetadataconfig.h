#ifndef __GMETADATACONFIG_H
#define __GMETADATACONFIG_H

#include "cpgf/gflags.h"

namespace cpgf {

enum GMetaDataConfig {
	mdcScriptable = 1 << 0,
	mdcAutoProperty = 1 << 1
};

typedef GFlags<GMetaDataConfig> GMetaDataConfigFlags;

inline bool metaDataConfigIsScriptable(const GMetaDataConfigFlags & flags) {
	return flags.has(mdcScriptable);
}

inline bool metaDataConfigIsAutoProperty(const GMetaDataConfigFlags & flags) {
	return flags.has(mdcAutoProperty);
}


} // namespace cpgf



#endif

