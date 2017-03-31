#ifndef GBINDOBJECTCACHE_H
#define GBINDOBJECTCACHE_H

#include "gbindobject.h"

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
	GScriptObjectCacheKey() : key(nullptr), className(nullptr), cv(opcvNone) {
	}

	GScriptObjectCacheKey(void * key, const char * className, ObjectPointerCV cv)
		: key(key), className(className), cv(cv) {
	}

	bool operator < (const GScriptObjectCacheKey & other) const {
		if(key < other.key) {
			return true;
		}
		if(key == other.key) {
			if(cv < other.cv) {
				return true;
			}
			if(cv > other.cv) {
				return false;
			}

			if(className == other.className) {
				return false;
			}
			else {
				return strcmp(className, other.className) < 0;
			}
		}
		return false;
	}

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
	T * findScriptObject(const GVariant & instance, const GClassGlueDataPointer & classData,
		ObjectPointerCV cv)
	{
		GScriptObjectCacheKey key(
			getInstanceHash(instance),
			classData->getMetaClass()->getQualifiedName(),
			cv
		);
		typename ObjectMapType::iterator it = this->objectMap.find(key);
		if(it != this->objectMap.end()) {
			return dynamic_cast<T *>(it->second.get());
		}
		return nullptr;
	}

	void addScriptObject(
		const GVariant & instance,
		const GClassGlueDataPointer & classData,
		ObjectPointerCV cv,
		GScriptObjectCacheData * scriptObject
	)
	{
		GScriptObjectCacheKey key(
			getInstanceHash(instance),
			classData->getMetaClass()->getQualifiedName(),
			cv
		);
		this->objectMap.insert(std::make_pair(key, GSharedPointer<GScriptObjectCacheData>(scriptObject)));
	}

	void freeScriptObject(GGlueDataWrapper * dataWrapper)
	{
		GVariant instance = getGlueDataInstance(dataWrapper->getData());
		if(instance.isEmpty()) {
			return;
		}
		GScriptObjectCacheKey entry(
			getInstanceHash(instance),
			getGlueDataMetaClass(dataWrapper->getData())->getQualifiedName(),
			getGlueDataCV(dataWrapper->getData())
		);

		this->objectMap.erase(entry);
	}

	void clear() {
		this->objectMap.clear();
	}

private:
	ObjectMapType objectMap;
};


} //namespace bind_internal

} //namespace cpgf


#endif
