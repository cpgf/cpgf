// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2POLYGONCONTACT_H
#define __META_BOX2D_B2POLYGONCONTACT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2PolygonContact(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (b2Fixture *, b2Fixture *)>();
    _d.CPGF_MD_TEMPLATE _method("Create", &D::ClassType::Create);
    _d.CPGF_MD_TEMPLATE _method("Destroy", &D::ClassType::Destroy);
    _d.CPGF_MD_TEMPLATE _method("Evaluate", &D::ClassType::Evaluate);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
