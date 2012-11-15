// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ICURSORCONTROL_H
#define __META_IRRLICHT_ICURSORCONTROL_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ICursorControl(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setVisible", &D::ClassType::setVisible);
    _d.CPGF_MD_TEMPLATE _method("isVisible", &D::ClassType::isVisible);
    _d.CPGF_MD_TEMPLATE _method("setPosition", (void (D::ClassType::*) (const core::position2d< f32 > &))&D::ClassType::setPosition);
    _d.CPGF_MD_TEMPLATE _method("setPosition", (void (D::ClassType::*) (f32, f32))&D::ClassType::setPosition);
    _d.CPGF_MD_TEMPLATE _method("setPosition", (void (D::ClassType::*) (const core::position2d< s32 > &))&D::ClassType::setPosition);
    _d.CPGF_MD_TEMPLATE _method("setPosition", (void (D::ClassType::*) (s32, s32))&D::ClassType::setPosition);
    _d.CPGF_MD_TEMPLATE _method("getPosition", &D::ClassType::getPosition);
    _d.CPGF_MD_TEMPLATE _method("getRelativePosition", &D::ClassType::getRelativePosition);
    _d.CPGF_MD_TEMPLATE _method("setReferenceRect", &D::ClassType::setReferenceRect)
        ._default(copyVariantFromCopyable(0))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
