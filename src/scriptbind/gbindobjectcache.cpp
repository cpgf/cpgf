#include "gbindobjectcache.h"
#include "gbindmethods.h"

namespace cpgf {

namespace bind_internal {

GScriptObjectCacheKey::GScriptObjectCacheKey()
	: key(nullptr), className(nullptr), cv(GScriptInstanceCv::sicvNone)
{
}

GScriptObjectCacheKey::GScriptObjectCacheKey(void * key, const char * className, const GScriptInstanceCv cv)
	: key(key), className(className), cv(cv)
{
}

bool GScriptObjectCacheKey::operator < (const GScriptObjectCacheKey & other) const
{
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


void GScriptObjectCache::addScriptObject(
	const GVariant & instance,
	const GClassGlueDataPointer & classData,
	const GScriptInstanceCv cv,
	GScriptObjectCacheData * scriptObject
)
{
	GScriptObjectCacheKey key(
		objectAddressFromVariant(instance),
		classData->getMetaClass()->getQualifiedName(),
		cv
	);
	this->objectMap.insert(std::make_pair(key, GSharedPointer<GScriptObjectCacheData>(scriptObject)));
}

void GScriptObjectCache::freeScriptObject(GGlueDataWrapper * dataWrapper)
{
	GVariant instance = getGlueDataInstance(dataWrapper->getData());
	if(instance.isEmpty()) {
		return;
	}
	GScriptObjectCacheKey entry(
		objectAddressFromVariant(instance),
		getGlueDataMetaClass(dataWrapper->getData())->getQualifiedName(),
		getGlueDataCV(dataWrapper->getData())
	);

	this->objectMap.erase(entry);
}

void GScriptObjectCache::clear()
{
	this->objectMap.clear();
}

GScriptObjectCacheData * GScriptObjectCache::doFindScriptObject(
		const GVariant & instance,
		const GClassGlueDataPointer & classData,
		const GScriptInstanceCv cv
	)
{
	GScriptObjectCacheKey key(
		objectAddressFromVariant(instance),
		classData->getMetaClass()->getQualifiedName(),
		cv
	);
	typename ObjectMapType::iterator it = this->objectMap.find(key);
	if(it != this->objectMap.end()) {
		return it->second.get();
	}
	return nullptr;
}


} //namespace bind_internal

} //namespace cpgf

