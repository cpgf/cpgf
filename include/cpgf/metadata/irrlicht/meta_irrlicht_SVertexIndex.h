// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SVERTEXINDEX_H
#define __META_IRRLICHT_SVERTEXINDEX_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_svertexindex(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_INDEX_TYPE>("E_INDEX_TYPE")
        ._element("EIT_16BIT", irr::video::EIT_16BIT)
        ._element("EIT_32BIT", irr::video::EIT_32BIT)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
