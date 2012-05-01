// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2GEARJOINT_H
#define __META_BOX2D_B2GEARJOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_B2GearJoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorA", _r), &D::ClassType::GetAnchorA, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorB", _r), &D::ClassType::GetAnchorB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionForce", _r), &D::ClassType::GetReactionForce, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionTorque", _r), &D::ClassType::GetReactionTorque, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetRatio", _r), &D::ClassType::SetRatio, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetRatio", _r), &D::ClassType::GetRatio, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2GearJointDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("joint1", _r), &D::ClassType::joint1, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("joint2", _r), &D::ClassType::joint2, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("ratio", _r), &D::ClassType::ratio, _p);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
