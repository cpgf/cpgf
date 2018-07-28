#ifndef CPGF_GMETAANNOTATION_H
#define CPGF_GMETAANNOTATION_H

#include "cpgf/private/gmetaannotation_p.h"

#include <string>
#include <string.h>

namespace cpgf {

class GAnnotationValue final
{
public:
	GAnnotationValue() {
	}

	template <typename T>
	explicit GAnnotationValue(const T & value) {
		meta_internal::InitAnnoVariant<T>::init(this->var, value);
	}

	GAnnotationValue(const GAnnotationValue & other);
	GAnnotationValue & operator = (GAnnotationValue other);

	~GAnnotationValue();

	void swap(GAnnotationValue & other);

	const GVariant * getVariant() const;

	bool canToString() const;
	bool canToWideString() const;
	bool canToInt() const;

	const char * toString() const;
	const wchar_t * toWideString() const;
	int toInt() const;
	bool toBoolean() const;

	template <typename T>
	T toObject() const {
		return fromVariant<T>(this->var);
	}

private:
	GVariant var;
};


class GAnnotationItem final
{
public:
	GAnnotationItem();
	~GAnnotationItem();

	GAnnotationItem(const char * name, const GAnnotationValue & value);

	GAnnotationItem(const GAnnotationItem & other);

	GAnnotationItem & operator = (const GAnnotationItem & other);

	const char * getName() const;
	const GAnnotationValue * getValue() const;
	
private:
	std::unique_ptr<meta_internal::GAnnotationItemImplement> implement;
	GAnnotationValue value;
};


class GMetaAnnotation final : public GMetaItem
{
private:
	typedef GMetaItem super;

public:
	GMetaAnnotation(const char * name);
	virtual ~GMetaAnnotation();

	template <typename T>
	void addItem(const char * name, const T & value) {
		this->doAddItem(new GAnnotationItem(name, GAnnotationValue(value)));
	}

	template <typename T>
	GMetaAnnotation & operator() (const char * name, const T & value) {
		this->addItem(name, value);

		return *this;
	}

	const GMetaItem * getMetaItem() const;

	const GAnnotationValue * getValue(const char * name) const;
	size_t getCount() const;
	const char * getNameAt(size_t index) const;
	const GAnnotationValue * getValueAt(size_t index) const;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const override;

private:
	void setMetaItem(const GMetaItem * metaItem);

	void clear();
	
	void doAddItem(GAnnotationItem * item);

private:
	std::unique_ptr<meta_internal::GMetaAnnotationImplement> implement;
	const GMetaItem * metaItem;

private:
	friend class GMetaClass;
	friend class GMetaItem;
};


} // namespace cpgf

#endif
