// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IRRARRAY_H
#define __META_IRRLICHT_IRRARRAY_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D, typename T, typename TAlloc>
void buildMetaClass_Array(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (u32)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const array< T, TAlloc > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("reallocate", &D::ClassType::reallocate);
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
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const array< T, TAlloc > &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const array< T, TAlloc > &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<T & (*)(cpgf::GMetaSelf, u32)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _operator<const T & (*)(const cpgf::GMetaSelf &, u32)>(mopHolder[0]);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
