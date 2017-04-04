#ifndef GBINDOBJECTCACHE_H
#define GBINDOBJECTCACHE_H

#include "gbindobject.h"
#include "gbindgluedata.h"

#include "cpgf/gvariant.h"
#include "cpgf/gsharedptr.h"

#include <map>

namespace cpgf {

namespace bind_internal {

struct GScriptObjectCacheKey {
	GScriptObjectCacheKey();
	GScriptObjectCacheKey(void * key, const char * className, const GScriptInstanceCv cv);

	bool operator < (const GScriptObjectCacheKey & other) const;

	void * key;
	const char * className;
	GScriptInstanceCv cv;
};

class GScriptObjectCacheData {
public:
	virtual ~GScriptObjectCacheData() {}
};

class GScriptObjectCache
{
private:
	typedef std::map<GScriptObjectCacheKey, GSharedPointer<GScriptObjectCacheData> > ObjectMapType;

public:
	template <class T>
	T * findScriptObject(
			const GVariant & instance,
			const GClassGlueDataPointer & classData,
			const GScriptInstanceCv cv
		)
	{
		GScriptObjectCacheData * result = this->doFindScriptObject(instance, classData, cv);
		if(result != nullptr) {
			return dynamic_cast<T *>(result);
		}
		return nullptr;
	}

	void addScriptObject(
		const GVariant & instance,
		const GClassGlueDataPointer & classData,
		const GScriptInstanceCv cv,
		GScriptObjectCacheData * scriptObject
	);
	void freeScriptObject(GGlueDataWrapper * dataWrapper);
	void clear();

private:
	GScriptObjectCacheData * doFindScriptObject(
		const GVariant & instance,
		const GClassGlueDataPointer & classData,
		const GScriptInstanceCv cv
	);

private:
	ObjectMapType objectMap;
};


} //namespace bind_internal

} //namespace cpgf


#endif
