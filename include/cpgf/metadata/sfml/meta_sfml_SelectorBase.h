// Auto generated file, don't modify.

#ifndef __META_SFML_SELECTORBASE_H
#define __META_SFML_SELECTORBASE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_SelectorBase(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("Add", _r), &D::ClassType::Add);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Remove", _r), &D::ClassType::Remove);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Clear", _r), &D::ClassType::Clear);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Wait", _r), &D::ClassType::Wait)
        ._default(copyVariantFromCopyable(0.f))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSocketReady", _r), &D::ClassType::GetSocketReady);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
