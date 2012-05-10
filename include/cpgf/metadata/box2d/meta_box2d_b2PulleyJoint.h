// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2PULLEYJOINT_H
#define __META_BOX2D_B2PULLEYJOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2pulleyjoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_minPulleyLength", _r), &b2_minPulleyLength);
}


template <typename D>
void buildMetaClass_B2PulleyJoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorA", _r), &D::ClassType::GetAnchorA);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorB", _r), &D::ClassType::GetAnchorB);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionForce", _r), &D::ClassType::GetReactionForce);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionTorque", _r), &D::ClassType::GetReactionTorque);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetGroundAnchorA", _r), &D::ClassType::GetGroundAnchorA);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetGroundAnchorB", _r), &D::ClassType::GetGroundAnchorB);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLength1", _r), &D::ClassType::GetLength1);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetLength2", _r), &D::ClassType::GetLength2);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetRatio", _r), &D::ClassType::GetRatio);
}


template <typename D>
void buildMetaClass_B2PulleyJointDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("groundAnchorA", _r), &D::ClassType::groundAnchorA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("groundAnchorB", _r), &D::ClassType::groundAnchorB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorA", _r), &D::ClassType::localAnchorA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorB", _r), &D::ClassType::localAnchorB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("lengthA", _r), &D::ClassType::lengthA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxLengthA", _r), &D::ClassType::maxLengthA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("lengthB", _r), &D::ClassType::lengthB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxLengthB", _r), &D::ClassType::maxLengthB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("ratio", _r), &D::ClassType::ratio);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Initialize", _r), &D::ClassType::Initialize);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
