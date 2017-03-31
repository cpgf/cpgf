#ifndef GBINDMETAMAP_H
#define GBINDMETAMAP_H

#include "cpgf/gsharedinterface.h"
#include "cpgf/gsharedptr.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gstringutil.h"

#include <map>
#include <unordered_map>

namespace cpgf {

namespace bind_internal {

enum GMetaMapItemType {
	mmitMethod = 0,
	mmitMethodList = 1,
	mmitProperty = 2,
	mmitField = 3,
	mmitEnum = 4,
	mmitEnumValue = 5,
	mmitClass = 6,
	mmitNone = 100,
};

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
	GMetaMapItem(IMetaItem * item, GMetaMapItemType type);
	GMetaMapItem(size_t enumIndex, IMetaEnum * item);
	explicit GMetaMapItem(IMetaList * metaList);
	GMetaMapItem(const GMetaMapItem & other);
	~GMetaMapItem();

	GMetaMapItem & operator = (GMetaMapItem other);

	void swap(GMetaMapItem & other);

	GMetaMapItemType getType() const;
	IObject * getItem() const;

	size_t getEnumIndex() const {
		return this->enumIndex;
	}

	void setUserData(GUserData * userData) const {
		this->userData.reset(userData);
	}

	GUserData * getUserData() const {
		return this->userData.get();
	}

private:
	GSharedInterface<IObject> item;
	GMetaMapItemType type;
	size_t enumIndex;
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
	GMetaMapClass(IMetaClass * metaClass);
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
	void buildMap(IMetaClass * metaClass);

private:
	MapType itemMap;
	GScopedPointer<GUserData> userData;
};

class GMetaMap
{
private:
	// Change this to unorder_map may hit performance.
	typedef std::map<const char *, GMetaMapClass *, GCStringCompare> MapType;

public:
	GMetaMap();
	~GMetaMap();

	GMetaMapClass * getClassMap(IMetaClass * metaClass);

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
