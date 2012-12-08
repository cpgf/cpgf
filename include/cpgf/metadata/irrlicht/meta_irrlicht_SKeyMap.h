// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SKEYMAP_H
#define __META_IRRLICHT_SKEYMAP_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_skeymap(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<EKEY_ACTION>("EKEY_ACTION")
        ._element("EKA_MOVE_FORWARD", irr::EKA_MOVE_FORWARD)
        ._element("EKA_MOVE_BACKWARD", irr::EKA_MOVE_BACKWARD)
        ._element("EKA_STRAFE_LEFT", irr::EKA_STRAFE_LEFT)
        ._element("EKA_STRAFE_RIGHT", irr::EKA_STRAFE_RIGHT)
        ._element("EKA_JUMP_UP", irr::EKA_JUMP_UP)
        ._element("EKA_CROUCH", irr::EKA_CROUCH)
        ._element("EKA_COUNT", irr::EKA_COUNT)
        ._element("EKA_FORCE_32BIT", irr::EKA_FORCE_32BIT)
    ;
}


template <typename D>
void buildMetaClass_SKeyMap(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (EKEY_ACTION, EKEY_CODE)>();
    _d.CPGF_MD_TEMPLATE _field("Action", &D::ClassType::Action);
    _d.CPGF_MD_TEMPLATE _field("KeyCode", &D::ClassType::KeyCode);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
