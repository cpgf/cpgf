#ifndef __GMETADEFINE_H
#define __GMETADEFINE_H

#include "cpgf/gmetaannotation.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetafield.h"
#include "cpgf/gmetamethod.h"
#include "cpgf/gmetaoperator.h"
#include "cpgf/gmetaproperty.h"
#include "cpgf/gglobal.h"
#include "cpgf/gpp.h"


#define MAX_BASE_COUNT 20
#define BASE_DEFAULT(N, unused) , typename GPP_CONCAT(BaseType, N) = void


namespace cpgf {


namespace meta_internal {


#define CASE_SUPERLIST_ARG(N, unused) case N: superList->add<ClassType, typename TypeList_GetWithDefault<TList, N, void>::Result>(); break;

template <typename TList, typename ClassType>
GMetaSuperList * doMakeSuperList() {
	GMetaSuperList * superList = new GMetaSuperList;

	for(int i = 0; i < MAX_BASE_COUNT; ++i) {
		if(i >= static_cast<int>(TypeList_Length<TList>::Result)) {
			break;
		}

		switch(i) {
			GPP_REPEAT(19, CASE_SUPERLIST_ARG, GPP_EMPTY())

			default:
				break;
		}
	}

	return superList;
}

#undef CASE_SUPERLIST_ARG

template <typename DefineClass>
struct GLazyDefineClassHelper
{
	static void (*registerAddress)(DefineClass define);

	static void metaRegister(GMetaClass * metaClass)
	{
		registerAddress(DefineClass(metaClass, metaClass));
	}
};

template <typename DefineClass>
void (*GLazyDefineClassHelper<DefineClass>::registerAddress)(DefineClass define) = NULL;


} // namespace meta_internal


template <typename BaseType>
class GDefineMetaMethod : public BaseType
{
public:
	GDefineMetaMethod(GMetaClass * metaClass, GMetaMethod * method) : BaseType(metaClass, method) {
	}
};

template <typename BaseType>
class GDefineMetaEnum : public BaseType
{
private:
	typedef GDefineMetaEnum<BaseType> ThisType;

public:
	GDefineMetaEnum(GMetaClass * metaClass, GMetaEnum * metaEnum) : BaseType(metaClass, metaEnum) {
	}

	ThisType & _element(const char * key, long long value) {
		(*gdynamic_cast<GMetaEnum *>(this->currentItem))(key, value);

		return *this;
	}

};

template <typename BaseType>
class GDefineMetaAnnotation : public BaseType
{
public:
	GDefineMetaAnnotation(GMetaClass * metaClass, GMetaAnnotation * annotation) : BaseType(metaClass, annotation) {
	}

	template <typename T>
	GDefineMetaAnnotation & _element(const char * name, const T & value) {
		(*gdynamic_cast<GMetaAnnotation *>(this->currentItem))(name, value);

		return *this;
	}

};

template <typename BaseType>
class GDefineMetaField : public BaseType
{
public:
	GDefineMetaField(GMetaClass * metaClass, GMetaField * field) : BaseType(metaClass, field) {
	}

};

template <typename BaseType>
class GDefineMetaProperty : public BaseType
{
public:
	GDefineMetaProperty(GMetaClass * metaClass, GMetaProperty * prop) : BaseType(metaClass, prop) {
	}

};

template <typename BaseType>
class GDefineMetaOperator : public BaseType
{
public:
	GDefineMetaOperator(GMetaClass * metaClass, GMetaOperator * op) : BaseType(metaClass, op) {
	}

};

template <typename BaseType>
class GDefineMetaInnerClass : public BaseType
{
public:
	GDefineMetaInnerClass(GMetaClass * metaClass, GMetaClass * inner) : BaseType(metaClass, inner) {
	}

};


template <typename ClassType GPP_REPEAT(MAX_BASE_COUNT, BASE_DEFAULT, GPP_EMPTY)>
class GDefineMetaClass;

template <typename ClassType, typename DerivedType>
class GDefineMetaCommon
{
private:
	typedef GDefineMetaCommon<ClassType, DerivedType> ThisType;

public:
	GDefineMetaCommon(GMetaClass * metaClass, GMetaItem * currentItem)
		: metaClass(metaClass), currentItem(currentItem) {
	}

	GDefineMetaCommon(const GDefineMetaCommon & other)
		: metaClass(other.metaClass), currentItem(other.currentItem) {
	}

	GDefineMetaCommon & operator = (const GDefineMetaCommon & other) {
		this->metaClass = other.metaClass;
		this->currentItem = other.currentItem;

		return *this;
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
	GDefineMetaField<DerivedType> _field(const char * name, FT field) {
		return GDefineMetaField<DerivedType>(
			this->metaClass,
			this->metaClass->addField(new GMetaField(name, field, GMetaPolicyDefault()))
		);
	}

	template <typename FT, typename Policy>
	GDefineMetaField<DerivedType> _field(const char * name, FT field, const Policy & policy) {
		return GDefineMetaField<DerivedType>(
			this->metaClass,
			this->metaClass->addField(new GMetaField(name, field, policy))
		);
	}

	template <typename Getter, typename Setter>
	GDefineMetaProperty<DerivedType> _property(const char * name, const Getter & getter, const Setter & setter) {
		return GDefineMetaProperty<DerivedType>(
			this->metaClass,
			this->metaClass->addProperty(new GMetaProperty(name, getter, setter, GMetaPolicyDefault()))
		);
	}

	template <typename Getter, typename Setter, typename Policy>
	GDefineMetaProperty<DerivedType> _property(const char * name, const Getter & getter, const Setter & setter, const Policy & policy) {
		return GDefineMetaProperty<DerivedType>(
			this->metaClass,
			this->metaClass->addProperty(new GMetaProperty(name, getter, setter, policy))
		);
	}

	template <typename FT, typename Creator>
	GDefineMetaOperator<DerivedType> _operator(const Creator & creator) {
		return GDefineMetaOperator<DerivedType>(
			this->metaClass,
			this->metaClass->addOperator(new GMetaOperator(creator.template create<ClassType, FT>(GMetaPolicyDefault())))
		);
	}

	template <typename FT, typename Creator, typename Policy>
	GDefineMetaOperator<DerivedType> _operator(const Creator & creator, const Policy & policy) {
		return GDefineMetaOperator<DerivedType>(
			this->metaClass,
			this->metaClass->addOperator(new GMetaOperator(creator.template create<ClassType, FT>(policy)))
		);
	}

	template <typename T>
	GDefineMetaEnum<DerivedType> _enum(const char * name) {
		return GDefineMetaEnum<DerivedType>(
			this->metaClass,
			this->metaClass->addEnum(new GMetaEnum(name, createMetaType<T>(), new meta_internal::GMetaEnumData(sizeof(T))))
		);
	}

	template <typename T>
	GDefineMetaInnerClass<DerivedType> _class(GDefineMetaClass<T> defineClass) {
		return GDefineMetaInnerClass<DerivedType>(
			this->metaClass,
			this->metaClass->addClass(defineClass.getMetaClass())
		);
	}

	GDefineMetaAnnotation<DerivedType> _annotation(const char * name) {
		return GDefineMetaAnnotation<DerivedType>(
			this->metaClass,
			this->currentItem->addItemAnnotation(new GMetaAnnotation(name))
		);
	}

protected:
	GMetaClass * metaClass;
	GMetaItem * currentItem;
};


template <typename ClassType GPP_REPEAT_TAIL_PARAMS(MAX_BASE_COUNT, typename BaseType)>
class GDefineMetaClass : public GDefineMetaCommon<ClassType, GDefineMetaClass<ClassType GPP_REPEAT_TAIL_PARAMS(MAX_BASE_COUNT, BaseType)> >
{
private:
	typedef GDefineMetaClass<ClassType GPP_REPEAT_TAIL_PARAMS(MAX_BASE_COUNT, BaseType)> ThisType;
	typedef GDefineMetaCommon<ClassType, ThisType > super;

public:
	static ThisType define(const char * className) {
		ThisType c(NULL, NULL);
		c.init(className, NULL, true, GMetaPolicyDefault());
		return c;
	}

	static ThisType inner(const char * className) {
		ThisType c(NULL, NULL);
		c.init(className, NULL, false, GMetaPolicyDefault());
		return c;
	}

	static ThisType lazy(const char * className, void (*reg)(ThisType define)) {
		GASSERT(reg != NULL);

		ThisType c(NULL, NULL);
		meta_internal::GLazyDefineClassHelper<ThisType>::registerAddress = reg;
		c.init(className, &meta_internal::GLazyDefineClassHelper<ThisType>::metaRegister, true, GMetaPolicyDefault());
		return c;
	}

	static ThisType lazyInner(const char * className, void (*reg)(ThisType define)) {
		GASSERT(reg != NULL);

		ThisType c(NULL, NULL);
		meta_internal::GLazyDefineClassHelper<ThisType>::registerAddress = reg;
		c.init(className, &meta_internal::GLazyDefineClassHelper<ThisType>::metaRegister, false, GMetaPolicyDefault());
		return c;
	}

	template <typename P>
	struct Policy {
		static ThisType define(const char * className) {
			ThisType c(NULL, NULL);
			c.init(className, NULL, true, P());
			return c;
		}

		static ThisType inner(const char * className) {
			ThisType c(NULL, NULL);
			c.init(className, NULL, false, P());
			return c;
		}

		static ThisType lazy(const char * className, void (*reg)(ThisType define)) {
			GASSERT(reg != NULL);

			ThisType c(NULL, NULL);
			meta_internal::GLazyDefineClassHelper<ThisType>::registerAddress = reg;
			c.init(className, &meta_internal::GLazyDefineClassHelper<ThisType>::metaRegister, true, P());
			return c;
		}

		static ThisType lazyInner(const char * className, void (*reg)(ThisType define)) {
			GASSERT(reg != NULL);

			ThisType c(NULL, NULL);
			meta_internal::GLazyDefineClassHelper<ThisType>::registerAddress = reg;
			c.init(className, &meta_internal::GLazyDefineClassHelper<ThisType>::metaRegister, false, P());
			return c;
		}
	};
	

protected:
	GDefineMetaClass(GMetaClass * metaClass, GMetaItem * currentItem) : super(metaClass, currentItem) {
	}

	explicit GDefineMetaClass(const char * className) : super(NULL, NULL) {
		this->init(className, true, GMetaPolicyDefault());
	}

public:
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

	GMetaClass * getMetaClass() const {
		return this->metaClass;
	}

private:
	static void destroyMetaObject(void * instance) {
		delete static_cast<ClassType *>(instance);
	}

	typedef typename cpgf::TypeList_Make<GPP_REPEAT_PARAMS(MAX_BASE_COUNT, BaseType)>::Result BaseListType;
	
	template <typename Policy>
	void init(const char * className, void (*reg)(GMetaClass *), bool addToGlobal, const Policy & policy) {
		GMetaClass * classToAdd = NULL;

		if(addToGlobal) {
			classToAdd = const_cast<GMetaClass *>(getGlobalMetaClass()->getClass(className));
		}
		if(classToAdd == NULL) {
			classToAdd = new GMetaClass(
				(ClassType *)0, meta_internal::doMakeSuperList<BaseListType, ClassType>(),
				className, &destroyMetaObject, reg, policy
			);

			if(addToGlobal) {
				getGlobalMetaClass()->addClass(classToAdd);
			}
		}

		this->metaClass = classToAdd;
		this->currentItem = classToAdd;
	}

private:
	template <typename DefineClass>
	friend struct meta_internal::GLazyDefineClassHelper;
};


class GDefineMetaGlobal : public GDefineMetaCommon<void, GDefineMetaGlobal >
{
private:
	typedef GDefineMetaCommon<void, GDefineMetaGlobal > super;
	typedef GDefineMetaGlobal ThisType;

public:
	GDefineMetaGlobal() : super(getGlobalMetaClass(), getGlobalMetaClass()) {
	}

	GDefineMetaGlobal(GMetaClass * metaClass, GMetaItem * currentItem) : super(metaClass, currentItem) {
	}

};




} // namespace cpgf



#undef MAX_BASE_COUNT
#undef BASE_DEFAULT


#endif

