// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2FRICTIONJOINT_H
#define __META_BOX2D_B2FRICTIONJOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_B2FrictionJoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorA", _r), &D::ClassType::GetAnchorA, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorB", _r), &D::ClassType::GetAnchorB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionForce", _r), &D::ClassType::GetReactionForce, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionTorque", _r), &D::ClassType::GetReactionTorque, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetMaxForce", _r), &D::ClassType::SetMaxForce, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMaxForce", _r), &D::ClassType::GetMaxForce, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetMaxTorque", _r), &D::ClassType::SetMaxTorque, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMaxTorque", _r), &D::ClassType::GetMaxTorque, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2FrictionJointDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorA", _r), &D::ClassType::localAnchorA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localAnchorB", _r), &D::ClassType::localAnchorB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxForce", _r), &D::ClassType::maxForce, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxTorque", _r), &D::ClassType::maxTorque, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Initialize", _r), &D::ClassType::Initialize, _p);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
