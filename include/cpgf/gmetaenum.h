#ifndef __GMETAENUM_H
#define __GMETAENUM_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"


#define GMETA_ENUM(e, ...) \
	getMetaClassObject()->addEnum(new cpgf::GMetaEnum(cpgf::normalizeReflectName(GPP_STRINGIZE(e)).c_str(), cpgf::meta_internal::makeMetaEnumTypeInfo(__VA_ARGS__), &(*cpgf::meta_internal::makeMetaEnumData(# __VA_ARGS__, __VA_ARGS__) , __VA_ARGS__)))

#define GMETA_QUALIFIED_ENUM(e, ...) \
	using namespace cpgf; \
	getMetaClassObject()->addEnum(new GMetaEnum(cpgf::normalizeReflectName(GPP_STRINGIZE(e)).c_str(), meta_internal::makeMetaEnumTypeInfo(__VA_ARGS__), &(*meta_internal::makeMetaEnumData(# __VA_ARGS__, __VA_ARGS__) , __VA_ARGS__)))



namespace cpgf {

class GMetaEnum;

template <typename T>
cpgf::GMetaEnum & reflectEnum(const char * name);

namespace meta_internal {

class GMetaEnumDataImplement;

class GMetaEnumData
{
public:
	GMetaEnumData(const char * keyNames, size_t typeSize);
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


template <typename T>
inline GMetaEnumData * makeMetaEnumData(const char * keyNames, const T & first, ...) {
    (void)first;

	GMetaEnumData * d = new GMetaEnumData(keyNames, sizeof(T));

	return d;
}

template <typename T>
inline GMetaType makeMetaEnumTypeInfo(const T & first, ...) {
    (void)first;
	return createMetaType<T>();
}


} // namespace meta_internal


class GMetaEnum : public GMetaTypedItem
{
private:
	typedef GMetaTypedItem super;

public:
	GMetaEnum(const char * name, const GMetaType & itemType, meta_internal::GMetaEnumData * baseData);

	virtual size_t getTypeSize() const;
	size_t getCount() const;
	const char * getKey(size_t index) const;
	GVariant getValue(size_t index) const;
	int findKey(const char * key) const;

	GMetaEnum & operator () (const char * key, long long value);

	virtual void * createInstance() const;
	virtual void * createInplace(void * placement) const;
	virtual void * cloneInstance(void * instance) const;
	virtual void * cloneInplace(void * instance, void * placement) const;
	virtual void destroyInstance(void * instance) const;
	
private:
	GScopedPointer<meta_internal::GMetaEnumData> baseData;
};


GMetaEnum & globalAddEnum(GMetaEnum * en);

template <typename T>
cpgf::GMetaEnum & reflectEnum(const char * name)
{
	return globalAddEnum(new GMetaEnum(name, cpgf::createMetaType<T>(), new cpgf::meta_internal::GMetaEnumData(NULL, sizeof(T))));
}


} // namespace cpgf


#endif
