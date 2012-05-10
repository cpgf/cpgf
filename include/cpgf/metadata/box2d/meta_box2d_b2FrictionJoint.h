// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2FRICTIONJOINT_H
#define __META_BOX2D_B2FRICTIONJOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2FrictionJoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorA", _r), &D::ClassType::GetAnchorA);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorB", _r), &D::ClassType::GetAnchorB);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionForce", _r), &D::ClassType::GetReactionForce);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionTorque", _r), &D::ClassType::GetReactionTorque);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetMaxForce", _r), &D::ClassType::SetMaxForce);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMaxForce", _r), &D::ClassType::GetMaxForce);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetMaxTorque", _r), &D::ClassType::SetMaxTorque);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMaxTorque", _r), &D::ClassType::GetMaxTorque);
}


template <typename D>
void buildMetaClass_B2FrictionJointDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorA", _r), &D::ClassType::localAnchorA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorB", _r), &D::ClassType::localAnchorB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxForce", _r), &D::ClassType::maxForce);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxTorque", _r), &D::ClassType::maxTorque);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Initialize", _r), &D::ClassType::Initialize);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
