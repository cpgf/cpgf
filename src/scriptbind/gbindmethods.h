#ifndef GBINDMETHODS_H
#define GBINDMETHODS_H

#include "gbindobject.h"
#include "gbindgluedata.h"

namespace cpgf {

namespace bind_internal {

class InvokeCallableResult
{
public:
	int resultCount;
	GVariant resultData;
	GSharedInterface<IMetaCallable> callable;
};

GScriptInstanceCv metaTypeToCV(const GMetaType & type);
bool shouldRemoveReference(const GMetaType & type);

IMetaObjectLifeManager * createObjectLifeManagerForInterface(const GVariant & value);

GVariant getAccessibleValueAndType(
	void * instance,
	IMetaAccessible * accessible,
	GMetaType * outType,
	bool instanceIsConst
);

GScriptInstanceCv getGlueDataCV(const GGlueDataPointer & glueData);
GVariant getGlueDataInstance(const GGlueDataPointer & glueData);
void * getGlueDataInstanceAddress(const GGlueDataPointer & glueData);
IMetaClass * getGlueDataMetaClass(const GGlueDataPointer & glueData);
IMetaSharedPointerTraits * getGlueDataSharedPointerTraits(const GGlueDataPointer & glueData);


} //namespace bind_internal

} //namespace cpgf


#endif
