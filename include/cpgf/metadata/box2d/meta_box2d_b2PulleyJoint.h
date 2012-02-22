// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2PULLEYJOINT_H
#define __META_BOX2D_B2PULLEYJOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_Global_b2pulleyjoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_minPulleyLength", _r), &b2_minPulleyLength, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2PulleyJoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorA", _r), &D::ClassType::GetAnchorA, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorB", _r), &D::ClassType::GetAnchorB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionForce", _r), &D::ClassType::GetReactionForce, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionTorque", _r), &D::ClassType::GetReactionTorque, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetGroundAnchorA", _r), &D::ClassType::GetGroundAnchorA, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetGroundAnchorB", _r), &D::ClassType::GetGroundAnchorB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLength1", _r), &D::ClassType::GetLength1, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLength2", _r), &D::ClassType::GetLength2, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetRatio", _r), &D::ClassType::GetRatio, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2PulleyJointDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("groundAnchorA", _r), &D::ClassType::groundAnchorA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("groundAnchorB", _r), &D::ClassType::groundAnchorB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorA", _r), &D::ClassType::localAnchorA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorB", _r), &D::ClassType::localAnchorB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("lengthA", _r), &D::ClassType::lengthA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxLengthA", _r), &D::ClassType::maxLengthA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("lengthB", _r), &D::ClassType::lengthB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxLengthB", _r), &D::ClassType::maxLengthB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("ratio", _r), &D::ClassType::ratio, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Initialize", _r), &D::ClassType::Initialize, _p);
}


} // namespace meta_box2d 




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif