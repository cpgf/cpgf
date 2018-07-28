#ifndef GBINDMETAMAP_H
#define GBINDMETAMAP_H

#include "cpgf/gsharedinterface.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gstringutil.h"
#include "cpgf/scriptbind/gscriptvalue.h"

#include <map>
#include <unordered_map>

namespace cpgf {

namespace bind_internal {

class GMetaMapItem
{
public:
	GMetaMapItem();
	explicit GMetaMapItem(const GScriptValue & scriptValue);
	GMetaMapItem(const GMetaMapItem & other);
	~GMetaMapItem();

	GMetaMapItem & operator = (GMetaMapItem other);

	void swap(GMetaMapItem & other);

	const GScriptValue & getScriptValue() const {
		return this->scriptValue;
	}

private:
	GScriptValue scriptValue;
};

inline void swap(GMetaMapItem & a, GMetaMapItem & b)
{
	a.swap(b);
}

class GMetaMapClass : public GNoncopyable
{
public:
	typedef std::unordered_map<const char *, GMetaMapItem, GCStringHash, GCStringEqual> MapType;

public:
	GMetaMapClass(IMetaClass * metaClass, void * instance = nullptr);
	~GMetaMapClass();

	GMetaMapItem * findItem(const char * name);

	const MapType * getMap() const {
		return &this->itemMap;
	}

private:
	void buildMap(IMetaClass * metaClass, void * instance);

private:
	MapType itemMap;
};

class GMetaMap
{
private:
	typedef std::map<const char *, GMetaMapClass *, GCStringCompare> MapType;

public:
	GMetaMap();
	~GMetaMap();

	GMetaMapClass * getMetaClassMap(IMetaClass * metaClass);

private:
	MapType classMap;
};

class GMethodGlueData;
typedef std::shared_ptr<GMethodGlueData> GMethodGlueDataPointer;


} //namespace bind_internal

} //namespace cpgf


#endif
