// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2WORLDCALLBACKS_H
#define __META_BOX2D_B2WORLDCALLBACKS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2Color(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32, float32, float32)>();
    _d.CPGF_MD_TEMPLATE _field("r", &D::ClassType::r);
    _d.CPGF_MD_TEMPLATE _field("g", &D::ClassType::g);
    _d.CPGF_MD_TEMPLATE _field("b", &D::ClassType::b);
    _d.CPGF_MD_TEMPLATE _method("Set", &D::ClassType::Set);
}


template <typename D>
void buildMetaClass_B2ContactFilter(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("ShouldCollide", &D::ClassType::ShouldCollide);
}


template <typename D>
void buildMetaClass_B2ContactImpulse(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("normalImpulses", &D::ClassType::normalImpulses);
    _d.CPGF_MD_TEMPLATE _field("tangentImpulses", &D::ClassType::tangentImpulses);
}


template <typename D>
void buildMetaClass_B2ContactListener(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("BeginContact", &D::ClassType::BeginContact);
    _d.CPGF_MD_TEMPLATE _method("EndContact", &D::ClassType::EndContact);
    _d.CPGF_MD_TEMPLATE _method("PreSolve", &D::ClassType::PreSolve);
    _d.CPGF_MD_TEMPLATE _method("PostSolve", &D::ClassType::PostSolve);
}


template <typename D>
void buildMetaClass_B2DebugDraw(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("SetFlags", &D::ClassType::SetFlags);
    _d.CPGF_MD_TEMPLATE _method("GetFlags", &D::ClassType::GetFlags);
    _d.CPGF_MD_TEMPLATE _method("AppendFlags", &D::ClassType::AppendFlags);
    _d.CPGF_MD_TEMPLATE _method("ClearFlags", &D::ClassType::ClearFlags);
    _d.CPGF_MD_TEMPLATE _method("DrawPolygon", &D::ClassType::DrawPolygon);
    _d.CPGF_MD_TEMPLATE _method("DrawSolidPolygon", &D::ClassType::DrawSolidPolygon);
    _d.CPGF_MD_TEMPLATE _method("DrawCircle", &D::ClassType::DrawCircle);
    _d.CPGF_MD_TEMPLATE _method("DrawSolidCircle", &D::ClassType::DrawSolidCircle);
    _d.CPGF_MD_TEMPLATE _method("DrawSegment", &D::ClassType::DrawSegment);
    _d.CPGF_MD_TEMPLATE _method("DrawTransform", &D::ClassType::DrawTransform);
    _d.CPGF_MD_TEMPLATE _enum<long long>("GlobalEnum_box2d_4")
        ._element("e_shapeBit", D::ClassType::e_shapeBit)
        ._element("e_jointBit", D::ClassType::e_jointBit)
        ._element("e_aabbBit", D::ClassType::e_aabbBit)
        ._element("e_pairBit", D::ClassType::e_pairBit)
        ._element("e_centerOfMassBit", D::ClassType::e_centerOfMassBit)
    ;
}


template <typename D>
void buildMetaClass_B2DestructionListener(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("SayGoodbye", (void (D::ClassType::*) (b2Joint *))&D::ClassType::SayGoodbye);
    _d.CPGF_MD_TEMPLATE _method("SayGoodbye", (void (D::ClassType::*) (b2Fixture *))&D::ClassType::SayGoodbye);
}


template <typename D>
void buildMetaClass_B2QueryCallback(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("ReportFixture", &D::ClassType::ReportFixture);
}


template <typename D>
void buildMetaClass_B2RayCastCallback(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("ReportFixture", &D::ClassType::ReportFixture);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
