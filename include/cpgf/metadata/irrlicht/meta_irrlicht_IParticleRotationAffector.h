// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IPARTICLEROTATIONAFFECTOR_H
#define __META_IRRLICHT_IPARTICLEROTATIONAFFECTOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IParticleRotationAffector(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setPivotPoint", &D::ClassType::setPivotPoint);
    _d.CPGF_MD_TEMPLATE _method("setSpeed", &D::ClassType::setSpeed);
    _d.CPGF_MD_TEMPLATE _method("getPivotPoint", &D::ClassType::getPivotPoint);
    _d.CPGF_MD_TEMPLATE _method("getSpeed", &D::ClassType::getSpeed);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
