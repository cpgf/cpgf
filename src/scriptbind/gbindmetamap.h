#ifndef GBINDMETAMAP_H
#define GBINDMETAMAP_H

#include "cpgf/gsharedinterface.h"
#include "cpgf/gsharedptr.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gstringutil.h"
#include "cpgf/scriptbind/gscriptvalue.h"

#include <map>
#include <unordered_map>

namespace cpgf {

namespace bind_internal {

class GUserData
{
public:
	GUserData() {}
	virtual ~GUserData() {}
};

class GMetaMapItem
{
public:
	GMetaMapItem();
	explicit GMetaMapItem(const GScriptValue & scriptValue);
	GMetaMapItem(const GMetaMapItem & other);
	~GMetaMapItem();

	GMetaMapItem & operator = (GMetaMapItem other);

	void swap(GMetaMapItem & other);

	void setUserData(GUserData * userData) const {
		this->userData.reset(userData);
	}

	GUserData * getUserData() const {
		return this->userData.get();
	}
	
	const GScriptValue & getScriptValue() const {
		return this->scriptValue;
	}

private:
	GScriptValue scriptValue;
	mutable GScopedPointer<GUserData> userData;
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

	void setUserData(GUserData * data) {
		this->userData.reset(data);
	}

	GUserData * getUserData() const {
		return this->userData.get();
	}

private:
	void buildMap(IMetaClass * metaClass, void * instance);

private:
	MapType itemMap;
	GScopedPointer<GUserData> userData;
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
typedef GSharedPointer<GMethodGlueData> GMethodGlueDataPointer;

class GMapItemMethodData : public GUserData
{
public:
	explicit GMapItemMethodData(const GMethodGlueDataPointer & methodData);
	~GMapItemMethodData();

	const GMethodGlueDataPointer & getMethodData() const {
		return this->methodData;
	}

private:
	GMethodGlueDataPointer methodData;
};


} //namespace bind_internal

} //namespace cpgf


#endif
