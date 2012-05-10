// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2WORLDCALLBACKS_H
#define __META_BOX2D_B2WORLDCALLBACKS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2Color(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32, float32, float32)>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("r", _r), &D::ClassType::r);
    _d.CPGF_MD_TEMPLATE _field(replaceName("g", _r), &D::ClassType::g);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b", _r), &D::ClassType::b);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), &D::ClassType::Set);
}


template <typename D>
void buildMetaClass_B2ContactFilter(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("ShouldCollide", _r), &D::ClassType::ShouldCollide);
}


template <typename D>
void buildMetaClass_B2ContactImpulse(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("normalImpulses", _r), &D::ClassType::normalImpulses);
    _d.CPGF_MD_TEMPLATE _field(replaceName("tangentImpulses", _r), &D::ClassType::tangentImpulses);
}


template <typename D>
void buildMetaClass_B2ContactListener(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("BeginContact", _r), &D::ClassType::BeginContact);
    _d.CPGF_MD_TEMPLATE _method(replaceName("EndContact", _r), &D::ClassType::EndContact);
    _d.CPGF_MD_TEMPLATE _method(replaceName("PreSolve", _r), &D::ClassType::PreSolve);
    _d.CPGF_MD_TEMPLATE _method(replaceName("PostSolve", _r), &D::ClassType::PostSolve);
}


template <typename D>
void buildMetaClass_B2DebugDraw(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFlags", _r), &D::ClassType::SetFlags);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFlags", _r), &D::ClassType::GetFlags);
    _d.CPGF_MD_TEMPLATE _method(replaceName("AppendFlags", _r), &D::ClassType::AppendFlags);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ClearFlags", _r), &D::ClassType::ClearFlags);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawPolygon", _r), &D::ClassType::DrawPolygon);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawSolidPolygon", _r), &D::ClassType::DrawSolidPolygon);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawCircle", _r), &D::ClassType::DrawCircle);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawSolidCircle", _r), &D::ClassType::DrawSolidCircle);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawSegment", _r), &D::ClassType::DrawSegment);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawTransform", _r), &D::ClassType::DrawTransform);
    _d.CPGF_MD_TEMPLATE _enum<long long>(replaceName("GlobalEnum_box2d_4", _r))
        ._element(replaceName("e_shapeBit", _r), D::ClassType::e_shapeBit)
        ._element(replaceName("e_jointBit", _r), D::ClassType::e_jointBit)
        ._element(replaceName("e_aabbBit", _r), D::ClassType::e_aabbBit)
        ._element(replaceName("e_pairBit", _r), D::ClassType::e_pairBit)
        ._element(replaceName("e_centerOfMassBit", _r), D::ClassType::e_centerOfMassBit)
    ;
}


template <typename D>
void buildMetaClass_B2DestructionListener(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("SayGoodbye", _r), (void (D::ClassType::*) (b2Joint *))&D::ClassType::SayGoodbye);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SayGoodbye", _r), (void (D::ClassType::*) (b2Fixture *))&D::ClassType::SayGoodbye);
}


template <typename D>
void buildMetaClass_B2QueryCallback(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("ReportFixture", _r), &D::ClassType::ReportFixture);
}


template <typename D>
void buildMetaClass_B2RayCastCallback(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("ReportFixture", _r), &D::ClassType::ReportFixture);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
