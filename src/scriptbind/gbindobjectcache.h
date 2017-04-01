#ifndef GBINDOBJECTCACHE_H
#define GBINDOBJECTCACHE_H

#include "gbindobject.h"
#include "gbindgluedata.h"

#include "cpgf/gvariant.h"
#include "cpgf/gsharedptr.h"

#include <map>

namespace cpgf {

namespace bind_internal {

inline void * getInstanceHash(const GVariant & instance)
{
	return objectAddressFromVariant(instance);
}

struct GScriptObjectCacheKey {
	GScriptObjectCacheKey();
	GScriptObjectCacheKey(void * key, const char * className, ObjectPointerCV cv);

	bool operator < (const GScriptObjectCacheKey & other) const;

	void * key;
	const char * className;
	ObjectPointerCV cv;
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
			const ObjectPointerCV cv
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
		ObjectPointerCV cv,
		GScriptObjectCacheData * scriptObject
	);
	void freeScriptObject(GGlueDataWrapper * dataWrapper);
	void clear();

private:
	GScriptObjectCacheData * doFindScriptObject(
		const GVariant & instance,
		const GClassGlueDataPointer & classData,
		const ObjectPointerCV cv
	);

private:
	ObjectMapType objectMap;
};


} //namespace bind_internal

} //namespace cpgf


#endif
