// Auto generated file, don't modify.

#ifndef __META_SFML_POSTFX_H
#define __META_SFML_POSTFX_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_PostFX(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const PostFX &)>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("LoadFromFile", _r), &D::ClassType::LoadFromFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("LoadFromMemory", _r), &D::ClassType::LoadFromMemory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetParameter", _r), (void (D::ClassType::*) (const std::string &, float))&D::ClassType::SetParameter, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetParameter", _r), (void (D::ClassType::*) (const std::string &, float, float))&D::ClassType::SetParameter, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetParameter", _r), (void (D::ClassType::*) (const std::string &, float, float, float))&D::ClassType::SetParameter, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetParameter", _r), (void (D::ClassType::*) (const std::string &, float, float, float, float))&D::ClassType::SetParameter, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetTexture", _r), &D::ClassType::SetTexture, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("CanUsePostFX", _r), &D::ClassType::CanUsePostFX);
    _d.CPGF_MD_TEMPLATE _operator<PostFX & (*)(cpgf::GMetaSelf, const PostFX &)>(mopHolder = mopHolder);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
