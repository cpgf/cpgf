// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IRANDOMIZER_H
#define __META_IRRLICHT_IRANDOMIZER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IRandomizer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("reset", &D::ClassType::reset)
        ._default(copyVariantFromCopyable(0x0f0f0f0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("rand", &D::ClassType::rand);
    _d.CPGF_MD_TEMPLATE _method("frand", &D::ClassType::frand);
    _d.CPGF_MD_TEMPLATE _method("randMax", &D::ClassType::randMax);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
