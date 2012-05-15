// Auto generated file, don't modify.

#ifndef __META_SFML_CONTEXT_H
#define __META_SFML_CONTEXT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::priv;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Context(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetActive", _r), &D::ClassType::SetActive);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsContextActive", _r), &D::ClassType::IsContextActive);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetGlobal", _r), &D::ClassType::GetGlobal, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<-1> >());
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
