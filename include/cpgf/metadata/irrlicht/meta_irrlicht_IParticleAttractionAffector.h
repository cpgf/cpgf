// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IPARTICLEATTRACTIONAFFECTOR_H
#define __META_IRRLICHT_IPARTICLEATTRACTIONAFFECTOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IParticleAttractionAffector(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setPoint", &D::ClassType::setPoint);
    _d.CPGF_MD_TEMPLATE _method("setAttract", &D::ClassType::setAttract);
    _d.CPGF_MD_TEMPLATE _method("setAffectX", &D::ClassType::setAffectX);
    _d.CPGF_MD_TEMPLATE _method("setAffectY", &D::ClassType::setAffectY);
    _d.CPGF_MD_TEMPLATE _method("setAffectZ", &D::ClassType::setAffectZ);
    _d.CPGF_MD_TEMPLATE _method("getPoint", &D::ClassType::getPoint);
    _d.CPGF_MD_TEMPLATE _method("getAttract", &D::ClassType::getAttract);
    _d.CPGF_MD_TEMPLATE _method("getAffectX", &D::ClassType::getAffectX);
    _d.CPGF_MD_TEMPLATE _method("getAffectY", &D::ClassType::getAffectY);
    _d.CPGF_MD_TEMPLATE _method("getAffectZ", &D::ClassType::getAffectZ);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
