// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IREFERENCECOUNTED_H
#define __META_IRRLICHT_IREFERENCECOUNTED_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IReferenceCounted(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("grab", &D::ClassType::grab);
    _d.CPGF_MD_TEMPLATE _method("drop", &D::ClassType::drop);
    _d.CPGF_MD_TEMPLATE _method("getReferenceCount", &D::ClassType::getReferenceCount);
    _d.CPGF_MD_TEMPLATE _method("getDebugName", &D::ClassType::getDebugName);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
