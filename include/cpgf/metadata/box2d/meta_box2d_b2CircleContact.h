// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2CIRCLECONTACT_H
#define __META_BOX2D_B2CIRCLECONTACT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2CircleContact(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (b2Fixture *, b2Fixture *)>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("Create", _r), &D::ClassType::Create);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Destroy", _r), &D::ClassType::Destroy);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Evaluate", _r), &D::ClassType::Evaluate);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
