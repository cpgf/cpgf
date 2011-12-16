#ifndef __GMETAENUM_P_H
#define __GMETAENUM_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"


namespace cpgf {

namespace meta_internal {

class GMetaEnumDataImplement;

class GMetaEnumData
{
public:
	explicit GMetaEnumData(size_t typeSize);
	~GMetaEnumData();

	size_t getTypeSize() const;

	size_t getCount() const;

	const char * getKey(size_t index) const;
	long long getValue(size_t index) const;
	int findKey(const char * key) const;

	void addEnum(const char * key, long long value);

	template <typename T>
	GMetaEnumData & operator , (const T & data) {
		this->addValue(data);

		return *this;
	}

	void * createInstance() const;
	void * createInplace(void * placement) const;
	void * cloneInstance(void * instance) const;
	void * cloneInplace(void * instance, void * placement) const;

	void destroyInstance(void * instance) const;
	
private:
	void addValue(long long value);

private:
	GScopedPointer<GMetaEnumDataImplement> implement;
	size_t typeSize;
};



} // namespace meta_internal



} // namespace cpgf


#endif
