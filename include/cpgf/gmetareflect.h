#ifndef __GMETAREFLECT_H
#define __GMETAREFLECT_H

#include "cpgf/gmetaannotation.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetafield.h"
#include "cpgf/gmetamethod.h"
#include "cpgf/gmetaoperator.h"
#include "cpgf/gmetaproperty.h"


// define class

#define I_GMETA_DO_COMMON_CLASS \
	public: \
		static const cpgf::GMetaClass * getMetaClass() { \
			return getMetaClassObject(); \
		} \
	private: \
		static void destroyMetaObject(void * o) { delete static_cast<MetaCurrentClassType *>(o); } \
		template <typename i_Reflect_Signature> static void reflectConstructor() { getMetaClassObject()->template addConstructor<MetaCurrentClassType, i_Reflect_Signature>(cpgf::GMetaPolicyDefault()); } \
		template <typename i_Reflect_Signature, typename i_Reflect_Policy> static void reflectConstructor(const i_Reflect_Policy & policy) { getMetaClassObject()->template addConstructor<MetaCurrentClassType, i_Reflect_Signature>(policy); } \
		template <typename i_Reflect_FT> static void reflectMethod(const char * name, i_Reflect_FT func) { getMetaClassObject()->addMethod(cpgf::GMetaMethod::newMethod<MetaCurrentClassType>(name, func, cpgf::GMetaPolicyDefault())); } \
		template <typename i_Reflect_FT, typename i_Reflect_Policy> static void reflectMethod(const char * name, i_Reflect_FT func, const i_Reflect_Policy & policy) { getMetaClassObject()->addMethod(cpgf::GMetaMethod::newMethod<MetaCurrentClassType>(name, func, policy)); } \
		template <typename i_Reflect_FT> static void reflectField(const char * name, i_Reflect_FT field) { getMetaClassObject()->addField(new cpgf::GMetaField(name, field, cpgf::GMetaPolicyDefault())); } \
		template <typename i_Reflect_FT, typename i_Reflect_Policy> static void reflectField(const char * name, i_Reflect_FT field, const i_Reflect_Policy & policy) { getMetaClassObject()->addField(new cpgf::GMetaField(name, field, policy)); } \
		template <typename i_Reflect_Signature, typename i_Reflect_Creator> static void reflectOperator(const i_Reflect_Creator & creator) { getMetaClassObject()->addOperator(new cpgf::GMetaOperator(creator.template create<MetaCurrentClassType, i_Reflect_Signature>(cpgf::GMetaPolicyDefault()))); } \
		template <typename i_Reflect_Signature, typename i_Reflect_Creator, typename i_Reflect_Policy> static void reflectOperator(const i_Reflect_Creator & creator, const i_Reflect_Policy & policy) { getMetaClassObject()->addOperator(new cpgf::GMetaOperator(creator.template create<MetaCurrentClassType, i_Reflect_Signature>(policy))); } \
		template <typename i_Reflect_Getter, typename i_Reflect_Setter> static void reflectProperty(const char * name, const i_Reflect_Getter & getter, const i_Reflect_Setter & setter) { getMetaClassObject()->addProperty(new cpgf::GMetaProperty(name, getter, setter, cpgf::GMetaPolicyDefault())); } \
		template <typename i_Reflect_Getter, typename i_Reflect_Setter, typename i_Reflect_Policy> static void reflectProperty(const char * name, const i_Reflect_Getter & getter, const i_Reflect_Setter & setter, const i_Reflect_Policy & policy) { getMetaClassObject()->addProperty(new cpgf::GMetaProperty(name, getter, setter, policy)); } \
		template <typename i_Reflect_T> static cpgf::GMetaEnum & reflectEnum(const char * name) { return getMetaClassObject()->addEnum(new cpgf::GMetaEnum(name, cpgf::createMetaType<i_Reflect_T>(), new cpgf::meta_internal::GMetaEnumData(NULL, sizeof(i_Reflect_T)))); } \
		static void reflectClass(const cpgf::GMetaClass * metaClass) { getMetaClassObject()->addClass(metaClass); } \
		static cpgf::GMetaAnnotation & reflectAnnotation(const char * name) { return getMetaClassObject()->addAnnotation(new cpgf::GMetaAnnotation(name)); }; \
		static void flushAnnotation() { getMetaClassObject()->flushAnnotation(); } \
		inline static void registerMetaInformation(cpgf::GMetaClass *)

#define I_GMETA_DO_DEFINE_CLASS(cls, clsname, reg, policy, ...) \
	private: \
		typedef cls MetaCurrentClassType; \
		static cpgf::GMetaClass * getMetaClassObject() { \
			static cpgf::GMetaClass metaClass((MetaCurrentClassType *)0, cpgf::meta_internal::MakeSuperList<cpgf::TypeList_Make<__VA_ARGS__>::Result, MetaCurrentClassType >(), clsname, &destroyMetaObject, &reg, policy); \
			metaClass.addModifier(cpgf::metaModifierNoFree); \
			return &metaClass; \
		} \
		I_GMETA_DO_COMMON_CLASS

#define GMETA_INTRUSIVE_CLASS(cls, clsname, ...) I_GMETA_DO_DEFINE_CLASS(cls, clsname, cls::registerMetaInformation, cpgf::GMetaPolicyDefault(), __VA_ARGS__)
#define GMETA_INTRUSIVE_POLICIED_CLASS(cls, clsname, policy, ...) I_GMETA_DO_DEFINE_CLASS(cls, clsname, cls::registerMetaInformation, policy, __VA_ARGS__)

#define I_GMETA_DEFINE_CLASS(cls, alias, clsname, policy, ...) \
	class GMETA_WRAP_CLASS(alias) { \
		I_GMETA_DO_DEFINE_CLASS(cls, clsname, GMETA_WRAP_CLASS(alias)::registerMetaInformation, policy, __VA_ARGS__); \
	}; \
	static cpgf::meta_internal::GMetaClassRegister GPP_CONCAT(i_offClass_, __LINE__) (GMETA_WRAP_CLASS(alias)::getMetaClass()); \
	inline void GMETA_WRAP_CLASS(alias)::registerMetaInformation(cpgf::GMetaClass *)

#define GMETA_DEFINE_CLASS(cls, alias, clsname, ...) I_GMETA_DEFINE_CLASS(cls, alias, clsname, cpgf::GMetaPolicyDefault(), __VA_ARGS__)
#define GMETA_DEFINE_POLICIED_CLASS(cls, alias, clsname, policy, ...) I_GMETA_DEFINE_CLASS(cls, alias, clsname, policy, __VA_ARGS__)

// This macro is almost same as GMETA_DEFINE_POLICIED_TEMPLATE except the policy
// but we can't call GMETA_DEFINE_POLICIED_TEMPLATE in this macro
// because GPP_COMMA can't be used in nested macro call in GCC.
#define GMETA_DEFINE_TEMPLATE(cls, alias, types, params, clsname, ...) \
	template <types> \
	class GMETA_WRAP_CLASS(alias) { \
	private: \
		typedef cls < params > MetaCurrentClassType; \
		static cpgf::GMetaClass * getMetaClassObject() { \
			static cpgf::GMetaClass metaClass((MetaCurrentClassType *)0, cpgf::meta_internal::MakeSuperList<typename cpgf::TypeList_Make<__VA_ARGS__>::Result, MetaCurrentClassType >(), clsname, &destroyMetaObject, &GMETA_WRAP_CLASS(alias)::registerMetaInformation, cpgf::GMetaPolicyDefault()); \
			metaClass.addModifier(cpgf::metaModifierNoFree); \
			return &metaClass; \
		} \
		I_GMETA_DO_COMMON_CLASS; \
	public: \
		static const cpgf::GMetaClass * instantiate(const char * name) { getMetaClassObject()->rebindName(name); return getMetaClass(); } \
	}; \
	template <types> \
	inline void GMETA_WRAP_CLASS(alias) < params >::registerMetaInformation(cpgf::GMetaClass *)

#define GMETA_DEFINE_POLICIED_TEMPLATE(cls, alias, types, params, clsname, policy, ...) \
	template <types> \
	class GMETA_WRAP_CLASS(alias) { \
	private: \
		typedef cls < params > MetaCurrentClassType; \
		static cpgf::GMetaClass * getMetaClassObject() { \
			static cpgf::GMetaClass metaClass((MetaCurrentClassType *)0, cpgf::meta_internal::MakeSuperList<typename cpgf::TypeList_Make<__VA_ARGS__>::Result, MetaCurrentClassType >(), clsname, &destroyMetaObject, &GMETA_WRAP_CLASS(alias)::registerMetaInformation, policy); \
			metaClass.addModifier(cpgf::metaModifierNoFree); \
			return &metaClass; \
		} \
		I_GMETA_DO_COMMON_CLASS; \
	public: \
		static const cpgf::GMetaClass * instantiate(const char * name) { getMetaClassObject()->rebindName(name); return getMetaClass(); } \
	}; \
	template <types> \
	inline void GMETA_WRAP_CLASS(alias) < params >::registerMetaInformation(cpgf::GMetaClass *)

#define GMETA_INSTANTIATE_TEMPLATE(name, alias, params) \
	static cpgf::meta_internal::GMetaClassRegister GPP_CONCAT(i_offClass_, __LINE__) (GMETA_WRAP_CLASS(alias) < params >::instantiate(name))



#define GMETA_WRAP_CLASS(cls) cls ## _oFf_mEta_

#define GMETA_FRIEND(cls) friend class GMETA_WRAP_CLASS(cls)


#define GMETA_CLASS(cls) \
	using namespace cpgf; \
	reflectClass(findMetaClass<MetaCurrentClassType::cls>())

#define GMETA_QUALIFIED_CLASS(cls) \
	using namespace cpgf; \
	reflectClass(findMetaClass<cls>())

#define GMETA_DEFINE_GLOBAL() \
	struct GPP_CONCAT(GMetaGlobalRegister_, __LINE__) { \
		static cpgf::GMetaClass * getMetaClass() { return cpgf::getGlobalMetaClass(); } \
		static cpgf::GMetaClass * getMetaClassObject() { return cpgf::getGlobalMetaClass(); } \
		inline static void registerGlobalMetaInformation(); \
		GPP_CONCAT(GMetaGlobalRegister_, __LINE__)() { \
			static bool initialized = false; \
			if(!initialized) { registerGlobalMetaInformation(); initialized = true; } \
		} \
	}; \
	static GPP_CONCAT(GMetaGlobalRegister_, __LINE__) GPP_CONCAT(mgr_i_, __LINE__); \
	inline void GPP_CONCAT(GMetaGlobalRegister_, __LINE__)::registerGlobalMetaInformation()


// annotation	

#define GMETA_ANNOTATION(anno) \
	using namespace cpgf; \
	reflectAnnotation(GPP_STRINGIZE(anno))

#define GMETA_QUALIFIED_ANNOTATION(anno) GMETA_ANNOTATION(anno)


// enum

#define GMETA_ENUM(e, ...) \
	getMetaClassObject()->addEnum(new cpgf::GMetaEnum(cpgf::normalizeReflectName(GPP_STRINGIZE(e)).c_str(), cpgf::meta_internal::makeMetaEnumTypeInfo(__VA_ARGS__), &(*cpgf::meta_internal::makeMetaEnumData(# __VA_ARGS__, __VA_ARGS__) , __VA_ARGS__)))

#define GMETA_QUALIFIED_ENUM(e, ...) \
	using namespace cpgf; \
	getMetaClassObject()->addEnum(new GMetaEnum(cpgf::normalizeReflectName(GPP_STRINGIZE(e)).c_str(), meta_internal::makeMetaEnumTypeInfo(__VA_ARGS__), &(*meta_internal::makeMetaEnumData(# __VA_ARGS__, __VA_ARGS__) , __VA_ARGS__)))


// field
		
#define GMETA_FIELD(field) \
	reflectField(cpgf::normalizeReflectName(GPP_STRINGIZE(field)).c_str(), &MetaCurrentClassType::field)

#define GMETA_QUALIFIED_FIELD(field) \
	using namespace cpgf; \
	reflectField(cpgf::normalizeReflectName(GPP_STRINGIZE(field)).c_str(), &field)


// property

#define GMETA_PROPERTY(prop, getter, setter) \
	reflectProperty(cpgf::normalizeReflectName(GPP_STRINGIZE(prop)).c_str(), &MetaCurrentClassType::getter, &MetaCurrentClassType::setter)

#define GMETA_QUALIFIED_PROPERTY(prop, getter, setter) \
	using namespace cpgf; \
	reflectProperty(cpgf::normalizeReflectName(GPP_STRINGIZE(prop)).c_str(), &getter, &setter)


// method

#define GMETA_METHOD(method) \
	reflectMethod(cpgf::normalizeReflectName(GPP_STRINGIZE(method)).c_str(), &MetaCurrentClassType::method)

#define GMETA_QUALIFIED_METHOD(method) \
	using namespace cpgf; \
	reflectMethod(cpgf::normalizeReflectName(GPP_STRINGIZE(method)).c_str(), &method)


//
	

namespace cpgf {


namespace meta_internal {


#define CASE_SUPERLIST_ARG(N, unused) case N: superList->add<ClassType, typename TypeList_GetWithDefault<TList, N, void>::Result>(); break;

template <typename TList, typename ClassType>
GMetaSuperList * MakeSuperList() {
	GMetaSuperList * superList = new GMetaSuperList;

	for(int i = 0; i < 20; ++i) {
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


} // namespace meta_internal



// class

void reflectClass(const GMetaClass * metaClass);


// annotation

GMetaAnnotation & globaAddAnnotation(GMetaAnnotation * annotation);
void globalFlushAnnotation();

GMetaAnnotation & reflectAnnotation(const char * name);
void flushAnnotation();


// enum

GMetaEnum & globalAddEnum(GMetaEnum * en);

template <typename T>
cpgf::GMetaEnum & reflectEnum(const char * name)
{
	return globalAddEnum(new GMetaEnum(name, cpgf::createMetaType<T>(), new cpgf::meta_internal::GMetaEnumData(NULL, sizeof(T))));
}


// field

void globalAddField(GMetaField * field);

template <typename FT>
void reflectField(const char * name, FT field)
{
	globalAddField(new cpgf::GMetaField(name, field, GMetaPolicyDefault()));
}

template <typename FT, typename Policy>
void reflectField(const char * name, FT field, const Policy & policy)
{
	globalAddField(new cpgf::GMetaField(name, field, policy));
}


// property

void globalAddProperty(GMetaProperty * field);

template <typename Getter, typename Setter>
void reflectProperty(const char * name, const Getter & getter, const Setter & setter)
{
	globalAddProperty(new cpgf::GMetaProperty(name, getter, setter, GMetaPolicyDefault()));
}

template <typename Getter, typename Setter, typename Policy>
void reflectProperty(const char * name, const Getter & getter, const Setter & setter, const Policy & policy)
{
	globalAddProperty(new cpgf::GMetaProperty(name, getter, setter, policy));
}


// method

void globalAddMethod(GMetaMethod * method);

template <typename FT>
void reflectMethod(const char * name, FT func)
{
	GASSERT_STATIC(GFunctionTraits<FT>::IsFunction && !GFunctionTraits<FT>::IsMember);

	globalAddMethod(cpgf::GMetaMethod::newMethod<void>(name, func, GMetaPolicyDefault()));
}

template <typename FT, typename Policy>
void reflectMethod(const char * name, FT func, const Policy & policy)
{
	GASSERT_STATIC(GFunctionTraits<FT>::IsFunction && !GFunctionTraits<FT>::IsMember);

	globalAddMethod(cpgf::GMetaMethod::newMethod<void>(name, func, policy));
}


// operator
void globalAddOperator(GMetaOperator * metaOperator);

template <typename Signature, typename Creator>
void reflectOperator(const Creator & creator)
{
	globalAddOperator(new cpgf::GMetaOperator(creator.template create<void, Signature>(GMetaPolicyDefault())));
}

template <typename Signature, typename Creator, typename Policy>
void reflectOperator(const Creator & creator, const Policy & policy)
{
	globalAddOperator(new cpgf::GMetaOperator(creator.template create<void, Signature>(policy)));
}




} // namespace cpgf



#endif
