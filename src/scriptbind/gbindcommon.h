#ifndef __GBINDCOMMON_H
#define __GBINDCOMMON_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gflags.h"


namespace cpgf {


enum ObjectPointerCV {
	opcvNone,
	opcvConst,
	opcvVolatile,
	opcvConstVolatile,
};


ObjectPointerCV metaTypeToCV(const GMetaType & type);
void cvToFilters(ObjectPointerCV cv, GFlags<GMetaFilters> * filters);

int rankCallable(IMetaCallable * callable, GVarData * paramsData, GScriptDataType * paramsType, size_t paramCount);
bool checkCallable(IMetaCallable * callable, GVarData * paramsData, size_t paramCount);

IMetaAccessible * findAccessible(IMetaClass * metaClass, const char * name, bool checkGet, bool checkSet, void ** instance);



} // namespace cpgf



#endif

