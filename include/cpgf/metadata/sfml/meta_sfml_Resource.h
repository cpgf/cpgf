// Auto generated file, don't modify.

#ifndef __META_SFML_RESOURCE_H
#define __META_SFML_RESOURCE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D, typename T>
void buildMetaClass_Resource(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
}


template <typename D, typename T>
void buildMetaClass_ResourcePtr(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const T *)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const ResourcePtr< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("OnResourceDestroyed", _r), &D::ClassType::OnResourceDestroyed);
    _d.CPGF_MD_TEMPLATE _operator<ResourcePtr< T > & (*)(cpgf::GMetaSelf, const ResourcePtr< T > &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<ResourcePtr< T > & (*)(cpgf::GMetaSelf, const T *)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _operator< const T * (GMetaSelf)>(mopHolder(), cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<-1> >());
    _d.CPGF_MD_TEMPLATE _operator<const T & (*)(const cpgf::GMetaSelf &)>(*mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<const T * (*)(const cpgf::GMetaSelf &)>(mopHolder -> mopHolder);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
