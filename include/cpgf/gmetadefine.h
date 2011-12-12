#ifndef __GMETADEFINE_H
#define __GMETADEFINE_H

#include "cpgf/gmetareflect.h"
#include "cpgf/gmetaannotation.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetafield.h"
#include "cpgf/gmetamethod.h"
#include "cpgf/gmetaoperator.h"
#include "cpgf/gmetaproperty.h"


namespace cpgf {



template <typename BaseType>
class GDefineMetaMethod : public BaseType
{
public:
	GDefineMetaMethod(GMetaClass * metaClass, GMetaMethod * method) : BaseType(metaClass) {
		this->currentItem = method;
	}
};

template <typename BaseType>
class GDefineMetaEnum : public BaseType
{
private:
	typedef GDefineMetaEnum<BaseType> ThisType;

public:
	GDefineMetaEnum(GMetaClass * metaClass, GMetaEnum * metaEnum) : BaseType(metaClass), metaEnum(metaEnum) {
		this->currentItem = metaEnum;
	}

	ThisType & _value(const char * key, long long value) {
		(*metaEnum)(key, value);

		return *this;
	}

private:
	GMetaEnum * metaEnum;
};

template <typename BaseType>
class GDefineMetaAnnotation : public BaseType
{
public:
	GDefineMetaAnnotation(GMetaClass * metaClass, GMetaAnnotation * annotation) : BaseType(metaClass), annotation(annotation) {
		this->currentItem = annotation;
	}

	template <typename T>
	GDefineMetaAnnotation & _value(const char * name, const T & value) {
		(*annotation)(name, value);

		return *this;
	}

private:
	GMetaAnnotation * annotation;
};


template <typename ClassType>
class GDefineMetaClass;

template <typename ClassType, typename DerivedType>
class GDefineMetaCommon
{
private:
	typedef GDefineMetaCommon<ClassType, DerivedType> ThisType;

public:
	GDefineMetaCommon(GMetaClass * metaClass) : metaClass(metaClass), currentItem(metaClass) {
	}

	template <typename FT>
	GDefineMetaMethod<DerivedType> _method(const char * name, FT func) {
		return GDefineMetaMethod<DerivedType>(
			this->metaClass,
			this->metaClass->addMethod(GMetaMethod::newMethod<ClassType>(name, func, GMetaPolicyDefault()))
		);
	}

	template <typename FT, typename Policy>
	GDefineMetaMethod<DerivedType> _method(const char * name, FT func, const Policy & policy) {
		return GDefineMetaMethod<DerivedType>(
			this->metaClass,
			this->metaClass->addMethod(GMetaMethod::newMethod<ClassType>(name, func, policy))
		);
	}
	
	template <typename FT>
	DerivedType & _field(const char * name, FT field) {
		this->metaClass->addField(new GMetaField(name, field, GMetaPolicyDefault()));
		
		return static_cast<DerivedType &>(*this);
	}

	template <typename FT, typename Policy>
	DerivedType & _field(const char * name, FT field, const Policy & policy) {
		this->metaClass->addField(new GMetaField(name, field, policy));
		
		return static_cast<DerivedType &>(*this);
	}

	template <typename FT>
	DerivedType & _property(const char * name, FT prop) {
		this->metaClass->addProperty(new GMetaProperty(name, prop, GMetaPolicyDefault()));
		
		return static_cast<DerivedType &>(*this);
	}

	template <typename FT, typename Policy>
	DerivedType & _property(const char * name, FT prop, const Policy & policy) {
		this->metaClass->addProperty(new GMetaProperty(name, prop, policy));
		
		return static_cast<DerivedType &>(*this);
	}

	template <typename FT, typename Creator>
	DerivedType & _operator(const Creator & creator) {
		this->metaClass->addOperator(new GMetaOperator(creator.template create<ClassType, FT>(GMetaPolicyDefault())));

		return static_cast<DerivedType &>(*this);
	}

	template <typename FT, typename Creator, typename Policy>
	DerivedType & _operator(const Creator & creator, const Policy & policy) {
		this->metaClass->addOperator(new GMetaOperator(creator.template create<ClassType, FT>(policy)));

		return static_cast<DerivedType &>(*this);
	}

	template <typename T>
	GDefineMetaEnum<DerivedType> _enum(const char * name) {
		return GDefineMetaEnum<DerivedType>(
			this->metaClass,
			&this->metaClass->addEnum(new GMetaEnum(name, createMetaType<T>(), new meta_internal::GMetaEnumData(NULL, sizeof(T))))
		);
	}

	template <typename T>
	DerivedType & _class(const GDefineMetaClass<T> & defineClass) {
		this->metaClass->addClass(defineClass.metaClass);

		return static_cast<DerivedType &>(*this);
	}

	GDefineMetaAnnotation<DerivedType> _annotation(const char * name) {
		GMetaAnnotation * annotation = new GMetaAnnotation(name);
		this->currentItem->addItemAnnotation(annotation);
		return GDefineMetaAnnotation<DerivedType>(
			this->metaClass,
			annotation
		);
	}

protected:
	GMetaClass * metaClass;
	GMetaItem * currentItem;
};




template <typename ClassType>
class GDefineMetaClass : public GDefineMetaCommon<ClassType, GDefineMetaClass<ClassType> >
{
private:
	typedef GDefineMetaCommon<ClassType, GDefineMetaClass<ClassType> > super;
	typedef GDefineMetaClass<ClassType> ThisType;

public:
	static void destroyMetaObject(void * instance) {
		delete static_cast<ClassType *>(instance);
	}

public:
	GDefineMetaClass(GMetaClass * metaClass) : super(metaClass) {
	}

	template <typename BaseType>
	ThisType & _base() {
		this->metaClass->template addBaseClass<ClassType, BaseType>();

		return *this;
	}

	template <typename FT>
	ThisType & _constructor() {
		this->metaClass->template addConstructor<ClassType, FT>(GMetaPolicyDefault());

		return *this;
	}

	template <typename FT, typename Policy>
	ThisType & _constructor(const Policy & policy) {
		this->metaClass->template addConstructor<ClassType, FT>(policy);

		return *this;
	}

	ThisType & _global() {
		getGlobalMetaClass()->addClass(this->metaClass);
		return *this;
	}

};


class GDefineMetaGlobal : public GDefineMetaCommon<void, GDefineMetaGlobal >
{
private:
	typedef GDefineMetaCommon<void, GDefineMetaGlobal > super;
	typedef GDefineMetaGlobal ThisType;

public:
	GDefineMetaGlobal() : super(getGlobalMetaClass()) {
	}
};


template <typename ClassType>
GDefineMetaClass<ClassType> defineMetaClass(const char * className)
{
	return GDefineMetaClass<ClassType>(
		new GMetaClass(
			(ClassType *)0, meta_internal::MakeSuperList<TypeList_Make<>::Result, ClassType>(),
			className, &GDefineMetaClass<ClassType>::destroyMetaObject, NULL, GMetaPolicyDefault()
		)
	);
}


} // namespace cpgf

inline void testDefineMeta();
class AAA {
public:
	AAA() { testDefineMeta(); }
	void abc() {}
	int value;
};

inline void testDefineMeta()
{
	using namespace cpgf;

	defineMetaClass<AAA>("AAA")
		._constructor<void *()>()
		._method("abc", &AAA::abc)
	;
}

static AAA bbb;


#endif
