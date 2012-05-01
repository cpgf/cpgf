// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2WORLDCALLBACKS_H
#define __META_BOX2D_B2WORLDCALLBACKS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_B2Color(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32, float32, float32)>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("r", _r), &D::ClassType::r, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("g", _r), &D::ClassType::g, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b", _r), &D::ClassType::b, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), &D::ClassType::Set, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2ContactFilter(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("ShouldCollide", _r), &D::ClassType::ShouldCollide, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2ContactImpulse(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("normalImpulses", _r), &D::ClassType::normalImpulses, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("tangentImpulses", _r), &D::ClassType::tangentImpulses, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2ContactListener(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("BeginContact", _r), &D::ClassType::BeginContact, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("EndContact", _r), &D::ClassType::EndContact, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("PreSolve", _r), &D::ClassType::PreSolve, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("PostSolve", _r), &D::ClassType::PostSolve, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2DebugDraw(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFlags", _r), &D::ClassType::SetFlags, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFlags", _r), &D::ClassType::GetFlags, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("AppendFlags", _r), &D::ClassType::AppendFlags, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ClearFlags", _r), &D::ClassType::ClearFlags, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawPolygon", _r), &D::ClassType::DrawPolygon, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawSolidPolygon", _r), &D::ClassType::DrawSolidPolygon, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawCircle", _r), &D::ClassType::DrawCircle, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawSolidCircle", _r), &D::ClassType::DrawSolidCircle, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawSegment", _r), &D::ClassType::DrawSegment, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DrawTransform", _r), &D::ClassType::DrawTransform, _p);
    _d.CPGF_MD_TEMPLATE _enum<long long>(replaceName("GlobalEnum_box2d_4", _r))
        ._element(replaceName("e_shapeBit", _r), D::ClassType::e_shapeBit)
        ._element(replaceName("e_jointBit", _r), D::ClassType::e_jointBit)
        ._element(replaceName("e_aabbBit", _r), D::ClassType::e_aabbBit)
        ._element(replaceName("e_pairBit", _r), D::ClassType::e_pairBit)
        ._element(replaceName("e_centerOfMassBit", _r), D::ClassType::e_centerOfMassBit)
    ;
}


template <typename D, typename Policy>
void buildMetaClass_B2DestructionListener(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("SayGoodbye", _r), (void (D::ClassType::*) (b2Joint *))&D::ClassType::SayGoodbye, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SayGoodbye", _r), (void (D::ClassType::*) (b2Fixture *))&D::ClassType::SayGoodbye, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2QueryCallback(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("ReportFixture", _r), &D::ClassType::ReportFixture, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2RayCastCallback(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("ReportFixture", _r), &D::ClassType::ReportFixture, _p);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
