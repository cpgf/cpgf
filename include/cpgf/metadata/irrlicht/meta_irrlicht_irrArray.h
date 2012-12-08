// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IRRARRAY_H
#define __META_IRRLICHT_IRRARRAY_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "IQ3Shader.h"
#include "SMeshBuffer.h"
#include "ISceneNode.h"
#include "ILightSceneNode.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <class T, typename TAlloc>
inline const array< T, TAlloc > & opErAToRWrapper_array__opAssign(array<T, TAlloc> * self, const array< T, TAlloc > & other) {
    return (*self) = other;
}
template <class T, typename TAlloc>
inline T & opErAToRWrapper_array__opArrayGet(array<T, TAlloc> * self, u32 index) {
    return (*self)[index];
}
template <class T, typename TAlloc>
inline void opErAToRWrapper_array__opArraySet(array<T, TAlloc> * self, u32 index, const typename cpgf::RemoveReference<T & >::Result & OpsEt_vALue) {
    (*self)[index] = OpsEt_vALue;
}
template <class T, typename TAlloc>
inline const T & opErAToRWrapper_array__opArrayGet(const array<T, TAlloc> * self, u32 index) {
    return (*self)[index];
}


template <typename D, class T, typename TAlloc>
void buildMetaClass_Array(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (u32)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const array< T, TAlloc > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("reallocate", &D::ClassType::reallocate)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("setAllocStrategy", &D::ClassType::setAllocStrategy)
        ._default(copyVariantFromCopyable(ALLOC_STRATEGY_DOUBLE))
    ;
    _d.CPGF_MD_TEMPLATE _method("push_back", &D::ClassType::push_back);
    _d.CPGF_MD_TEMPLATE _method("push_front", &D::ClassType::push_front);
    _d.CPGF_MD_TEMPLATE _method("insert", &D::ClassType::insert)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("set_pointer", &D::ClassType::set_pointer)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("set_free_when_destroyed", &D::ClassType::set_free_when_destroyed);
    _d.CPGF_MD_TEMPLATE _method("set_used", &D::ClassType::set_used);
    _d.CPGF_MD_TEMPLATE _method("getLast", (T & (D::ClassType::*) ())&D::ClassType::getLast);
    _d.CPGF_MD_TEMPLATE _method("getLast", (const T & (D::ClassType::*) () const)&D::ClassType::getLast);
    _d.CPGF_MD_TEMPLATE _method("pointer", &D::ClassType::pointer);
    _d.CPGF_MD_TEMPLATE _method("const_pointer", &D::ClassType::const_pointer);
    _d.CPGF_MD_TEMPLATE _method("size", &D::ClassType::size);
    _d.CPGF_MD_TEMPLATE _method("allocated_size", &D::ClassType::allocated_size);
    _d.CPGF_MD_TEMPLATE _method("empty", &D::ClassType::empty);
    _d.CPGF_MD_TEMPLATE _method("sort", &D::ClassType::sort);
    _d.CPGF_MD_TEMPLATE _method("binary_search", (s32 (D::ClassType::*) (const T &))&D::ClassType::binary_search);
    _d.CPGF_MD_TEMPLATE _method("binary_search", (s32 (D::ClassType::*) (const T &) const)&D::ClassType::binary_search);
    _d.CPGF_MD_TEMPLATE _method("binary_search", (s32 (D::ClassType::*) (const T &, s32, s32) const)&D::ClassType::binary_search);
    _d.CPGF_MD_TEMPLATE _method("binary_search_multi", &D::ClassType::binary_search_multi);
    _d.CPGF_MD_TEMPLATE _method("linear_search", &D::ClassType::linear_search);
    _d.CPGF_MD_TEMPLATE _method("linear_reverse_search", &D::ClassType::linear_reverse_search);
    _d.CPGF_MD_TEMPLATE _method("erase", (void (D::ClassType::*) (u32))&D::ClassType::erase);
    _d.CPGF_MD_TEMPLATE _method("erase", (void (D::ClassType::*) (u32, s32))&D::ClassType::erase);
    _d.CPGF_MD_TEMPLATE _method("set_sorted", &D::ClassType::set_sorted);
    _d.CPGF_MD_TEMPLATE _method("swap", &D::ClassType::swap);
    _d.CPGF_MD_TEMPLATE _operator<const array< T, TAlloc > & (*)(cpgf::GMetaSelf, const array< T, TAlloc > &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (const array< T, TAlloc > & (*) (array<T, TAlloc> *, const array< T, TAlloc > &))&opErAToRWrapper_array__opAssign<T, TAlloc>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1>, cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<T & (*)(cpgf::GMetaSelf, u32)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _method("_opArrayGet", (T & (*) (array<T, TAlloc> *, u32))&opErAToRWrapper_array__opArrayGet<T, TAlloc>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _method("_opArraySet", (void (*) (array<T, TAlloc> *, u32, const typename cpgf::RemoveReference<T & >::Result &))&opErAToRWrapper_array__opArraySet<T, TAlloc>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<const T & (*)(const cpgf::GMetaSelf &, u32)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _method("_opArrayGet", (const T & (*) (const array<T, TAlloc> *, u32))&opErAToRWrapper_array__opArrayGet<T, TAlloc>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
