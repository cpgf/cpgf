#ifndef __GMETACOMMON_H
#define __GMETACOMMON_H

#include "cpgf/gcompiler.h"
#include "cpgf/gconfig.h"
#include "cpgf/gvariant.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gclassutil.h"

#include <stdexcept>
#include <vector>

#include <ctype.h>
#include <string.h>


#define REF_MAX_ARITY G_MAX_ARITY

namespace cpgf {

class GMetaType;
class GTypeInfo;
class GMetaClass;
class GMetaEnum;
class GMetaFundamental;
class GMetaAnnotation;
class GMetaConverter;
struct IMemoryAllocator;


struct GMetaVariadicParam
{
	GVariant const * const * params;
	size_t paramCount;
};


namespace meta_internal {

void handleForbidAccessError(bool isRead);

void * newZeroBuffer(void * buffer, size_t size, void * copy);

class GMetaItemImplement;

struct CStringCompare
{
	bool operator () (const char * a, const char * b) const {
		return strcmp(a, b) < 0;
	}
};


} // namespace meta_internal



enum GMetaCategory {
	mcatField = 0,
	mcatProperty = 1,
	mcatMethod = 2,
	mcatEnum = 3,
	mcatOperator = 4,
	mcatConstructor = 5,
	mcatClass = 6,
	mcatAnnotation = 7,
	mcatFundamental = 8,

	mcatCount = 9
};

const int metaModifierStatic = 1 << 0;
const int metaModifierNoFree = 1 << 1;

class GMetaItem : public GNoncopyable
{
public:
	GMetaItem(const char * name, const GMetaType & itemType, GMetaCategory category);
	virtual ~GMetaItem();

	bool isStatic() const {
		return this->hasModifier(metaModifierStatic);
	}

	GMetaCategory getCategory() const {
		return this->category;
	}

	void addModifier(int m) {
		this->modifiers |= m;
	}

	bool hasModifier(int m) const {
		return (this->modifiers & m) != 0;
	}

	const GMetaItem * getOwnerItem() const {
		return this->ownerItem;
	}

	virtual const GMetaType & getItemType() const;

	const std::string & getName() const;
	const std::string & getQualifiedName() const;
	std::string makeQualifiedName(const char * delimiter) const;

	const GMetaAnnotation * getAnnotation(const char * name) const;
	size_t getAnnotationCount() const;
	const GMetaAnnotation * getAnnotationAt(size_t index) const;

protected:
	void setName(const char * name);

private:
	GMetaAnnotation * addItemAnnotation(GMetaAnnotation * annotation);

protected:
	GScopedPointer<meta_internal::GMetaItemImplement> implement;

private:
	int modifiers;
	GMetaCategory category;
	GMetaItem * ownerItem;

private:
	friend class GMetaClass;

	template <typename ClassType, typename DerivedType>
	friend class GDefineMetaCommon;
};


class GMetaTypedItem : public GMetaItem
{
private:
	typedef GMetaItem super;

public:
	GMetaTypedItem(const char * name, const GMetaType & itemType, GMetaCategory category);

	const GMetaType & getMetaType() const;
	virtual size_t getTypeSize() const = 0;

	const std::string & getTypeName() const;

	virtual void * createInstance() const = 0;
	virtual void * createInplace(void * placement) const = 0;
	virtual void * cloneInstance(void * obj) const = 0;
	virtual void * cloneInplace(void * obj, void * placement) const = 0;

	virtual void destroyInstance(void * obj) const = 0;
	
private:
	mutable std::string typeName;
};


class GMetaAccessible : public GMetaItem
{
private:
	typedef GMetaItem super;

public:
	GMetaAccessible(const char * name, const GMetaType & itemType, GMetaCategory category);

	virtual bool canGet() const = 0;
	virtual bool canSet() const = 0;

	virtual GVariant get(void * obj) const = 0;
	virtual void set(void * obj, const GVariant & v) const = 0;
	
	virtual void * getAddress(void * instance) const = 0;

	virtual size_t getSize() const = 0;
	
	virtual GMetaConverter * createConverter() const = 0;

};


class GMetaCallable : public GMetaItem
{
private:
	typedef GMetaItem super;

public:
	GMetaCallable(const char * name, const GMetaType & itemType, GMetaCategory category);

	virtual size_t getParamCount() const = 0;
	virtual GMetaType getParamType(size_t index) const = 0;
	virtual bool hasResult() const = 0;
	virtual GMetaType getResultType() const = 0;
	virtual bool isVariadic() const = 0;
	virtual bool checkParam(const GVariant & param, size_t paramIndex) const = 0;
	virtual bool isParamTransferOwnership(size_t paramIndex) const = 0;
	virtual bool isResultTransferOwnership() const = 0;
	virtual GVariant execute(void * obj, const GVariant * params, size_t paramCount) const = 0;
	virtual GMetaConverter * createResultConverter() const = 0;

};


GMAKE_FINAL(GMetaList)

class GMetaList : GFINAL_BASE(GMetaList)
{
public:
	GMetaList();
	~GMetaList();

	GMetaList(const GMetaList & other);
	GMetaList & operator = (const GMetaList & other);

	void add(const GMetaItem * item, void * instance);
	size_t getCount() const;
	const GMetaItem * getAt(size_t index) const;
	void * getInstanceAt(size_t index) const;
	void clear();

private:
	std::vector<const GMetaItem *> itemList;
	std::vector<void *> instanceList;
};


class GMetaConverter
{
public:
	virtual ~GMetaConverter() {
	}

	virtual bool canToCString() = 0;
	virtual const char * toCString(const void * instance, int * needFree, IMemoryAllocator * allocator) = 0;
};

class GMetaConverterDefault : public GMetaConverter
{
public:
	virtual bool canToCString();
	virtual const char * toCString(const void * instance, int * needFree, IMemoryAllocator * allocator);
};


const GMetaTypedItem * findMetaType(const GMetaType & type);
const GMetaTypedItem * findMetaType(const char * name);

const GMetaEnum * findMetaEnum(const GMetaType & type);
const GMetaEnum * findMetaEnum(const char * name);

const GMetaFundamental * findMetaFundamental(const GMetaType & type);
const GMetaFundamental * findMetaFundamental(const char * name);
const GMetaFundamental * findMetaFundamental(GVariantType vt);

bool metaIsField(int category);
bool metaIsProperty(int category);
bool metaIsMethod(int category);
bool metaIsEnum(int category);
bool metaIsOperator(int category);
bool metaIsConstructor(int category);
bool metaIsClass(int category);;
bool metaIsAnnotation(int category);
bool metaIsFundamental(int category);



} // namespace cpgf


template <typename T>
struct GMetaConverterTraits
{
	static cpgf::GMetaConverter * createConverter()
	{
		return NULL;
	}
};



#endif
