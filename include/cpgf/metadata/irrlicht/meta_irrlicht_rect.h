// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_RECT_H
#define __META_IRRLICHT_RECT_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <class T>
inline rect< T > opErAToRWrapper_rect__opAdd(const rect<T> * self, const position2d< T > & pos) {
    return (*self) + pos;
}
template <class T>
inline rect< T > & opErAToRWrapper_rect__opAddAssign(rect<T> * self, const position2d< T > & pos) {
    return (*self) += pos;
}
template <class T>
inline rect< T > opErAToRWrapper_rect__opSub(const rect<T> * self, const position2d< T > & pos) {
    return (*self) - pos;
}
template <class T>
inline rect< T > & opErAToRWrapper_rect__opSubAssign(rect<T> * self, const position2d< T > & pos) {
    return (*self) -= pos;
}
template <class T>
inline bool opErAToRWrapper_rect__opEqual(const rect<T> * self, const rect< T > & other) {
    return (*self) == other;
}
template <class T>
inline bool opErAToRWrapper_rect__opNotEqual(const rect<T> * self, const rect< T > & other) {
    return (*self) != other;
}
template <class T>
inline bool opErAToRWrapper_rect__opLess(const rect<T> * self, const rect< T > & other) {
    return (*self) < other;
}


template <typename D, class T>
void buildMetaClass_Rect(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T, T, T)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const position2d< T > &, const position2d< T > &)>();
    _d.CPGF_MD_TEMPLATE _field("UpperLeftCorner", &D::ClassType::UpperLeftCorner);
    _d.CPGF_MD_TEMPLATE _field("LowerRightCorner", &D::ClassType::LowerRightCorner);
    _d.CPGF_MD_TEMPLATE _method("getArea", &D::ClassType::getArea);
    _d.CPGF_MD_TEMPLATE _method("isPointInside", &D::ClassType::isPointInside);
    _d.CPGF_MD_TEMPLATE _method("isRectCollided", &D::ClassType::isRectCollided);
    _d.CPGF_MD_TEMPLATE _method("clipAgainst", &D::ClassType::clipAgainst);
    _d.CPGF_MD_TEMPLATE _method("constrainTo", &D::ClassType::constrainTo);
    _d.CPGF_MD_TEMPLATE _method("getWidth", &D::ClassType::getWidth);
    _d.CPGF_MD_TEMPLATE _method("getHeight", &D::ClassType::getHeight);
    _d.CPGF_MD_TEMPLATE _method("repair", &D::ClassType::repair);
    _d.CPGF_MD_TEMPLATE _method("isValid", &D::ClassType::isValid);
    _d.CPGF_MD_TEMPLATE _method("getCenter", &D::ClassType::getCenter);
    _d.CPGF_MD_TEMPLATE _method("getSize", &D::ClassType::getSize);
    _d.CPGF_MD_TEMPLATE _method("addInternalPoint", (void (D::ClassType::*) (const position2d< T > &))&D::ClassType::addInternalPoint);
    _d.CPGF_MD_TEMPLATE _method("addInternalPoint", (void (D::ClassType::*) (T, T))&D::ClassType::addInternalPoint);
    _d.CPGF_MD_TEMPLATE _operator<rect< T > (*)(const cpgf::GMetaSelf &, const position2d< T > &)>(mopHolder + mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAdd", (rect< T > (*) (const rect<T> *, const position2d< T > &))&opErAToRWrapper_rect__opAdd<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<rect< T > & (*)(cpgf::GMetaSelf, const position2d< T > &)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAddAssign", (rect< T > & (*) (rect<T> *, const position2d< T > &))&opErAToRWrapper_rect__opAddAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<rect< T > (*)(const cpgf::GMetaSelf &, const position2d< T > &)>(mopHolder - mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opSub", (rect< T > (*) (const rect<T> *, const position2d< T > &))&opErAToRWrapper_rect__opSub<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<rect< T > & (*)(cpgf::GMetaSelf, const position2d< T > &)>(mopHolder -= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opSubAssign", (rect< T > & (*) (rect<T> *, const position2d< T > &))&opErAToRWrapper_rect__opSubAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const rect< T > &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const rect<T> *, const rect< T > &))&opErAToRWrapper_rect__opEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const rect< T > &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const rect<T> *, const rect< T > &))&opErAToRWrapper_rect__opNotEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const rect< T > &)>(mopHolder < mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLess", (bool (*) (const rect<T> *, const rect< T > &))&opErAToRWrapper_rect__opLess<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
