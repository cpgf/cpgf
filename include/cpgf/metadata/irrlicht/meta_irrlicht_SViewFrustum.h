// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SVIEWFRUSTUM_H
#define __META_IRRLICHT_SVIEWFRUSTUM_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_SViewFrustum(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SViewFrustum &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::matrix4 &)>();
    _d.CPGF_MD_TEMPLATE _field("cameraPosition", &D::ClassType::cameraPosition);
    _d.CPGF_MD_TEMPLATE _field("planes", &D::ClassType::planes);
    _d.CPGF_MD_TEMPLATE _field("boundingBox", &D::ClassType::boundingBox);
    _d.CPGF_MD_TEMPLATE _method("setFrom", &D::ClassType::setFrom);
    _d.CPGF_MD_TEMPLATE _method("transform", &D::ClassType::transform);
    _d.CPGF_MD_TEMPLATE _method("getFarLeftUp", &D::ClassType::getFarLeftUp);
    _d.CPGF_MD_TEMPLATE _method("getFarLeftDown", &D::ClassType::getFarLeftDown);
    _d.CPGF_MD_TEMPLATE _method("getFarRightUp", &D::ClassType::getFarRightUp);
    _d.CPGF_MD_TEMPLATE _method("getFarRightDown", &D::ClassType::getFarRightDown);
    _d.CPGF_MD_TEMPLATE _method("getNearLeftUp", &D::ClassType::getNearLeftUp);
    _d.CPGF_MD_TEMPLATE _method("getNearLeftDown", &D::ClassType::getNearLeftDown);
    _d.CPGF_MD_TEMPLATE _method("getNearRightUp", &D::ClassType::getNearRightUp);
    _d.CPGF_MD_TEMPLATE _method("getNearRightDown", &D::ClassType::getNearRightDown);
    _d.CPGF_MD_TEMPLATE _method("getBoundingBox", &D::ClassType::getBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("recalculateBoundingBox", &D::ClassType::recalculateBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("getTransform", (core::matrix4 & (D::ClassType::*) (video::E_TRANSFORMATION_STATE))&D::ClassType::getTransform);
    _d.CPGF_MD_TEMPLATE _method("getTransform", (const core::matrix4 & (D::ClassType::*) (video::E_TRANSFORMATION_STATE) const)&D::ClassType::getTransform);
    _d.CPGF_MD_TEMPLATE _method("clipLine", &D::ClassType::clipLine);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::VFPLANES>("VFPLANES")
        ._element("VF_FAR_PLANE", D::ClassType::VF_FAR_PLANE)
        ._element("VF_NEAR_PLANE", D::ClassType::VF_NEAR_PLANE)
        ._element("VF_LEFT_PLANE", D::ClassType::VF_LEFT_PLANE)
        ._element("VF_RIGHT_PLANE", D::ClassType::VF_RIGHT_PLANE)
        ._element("VF_BOTTOM_PLANE", D::ClassType::VF_BOTTOM_PLANE)
        ._element("VF_TOP_PLANE", D::ClassType::VF_TOP_PLANE)
        ._element("VF_PLANE_COUNT", D::ClassType::VF_PLANE_COUNT)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
