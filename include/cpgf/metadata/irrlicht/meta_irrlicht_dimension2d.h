// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_DIMENSION2D_H
#define __META_IRRLICHT_DIMENSION2D_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <class T>
inline bool opErAToRWrapper_dimension2d__opEqual(const dimension2d<T> * self, const dimension2d< T > & other) {
    return (*self) == other;
}
template <class T>
inline bool opErAToRWrapper_dimension2d__opNotEqual(const dimension2d<T> * self, const dimension2d< T > & other) {
    return (*self) != other;
}
template <class T>
inline bool opErAToRWrapper_dimension2d__opEqual(const dimension2d<T> * self, const vector2d< T > & other) {
    return (*self) == other;
}
template <class T>
inline bool opErAToRWrapper_dimension2d__opNotEqual(const dimension2d<T> * self, const vector2d< T > & other) {
    return (*self) != other;
}
template <class T>
inline dimension2d< T > & opErAToRWrapper_dimension2d__opDivAssign(dimension2d<T> * self, const T & scale) {
    return (*self) /= scale;
}
template <class T>
inline dimension2d< T > opErAToRWrapper_dimension2d__opDiv(const dimension2d<T> * self, const T & scale) {
    return (*self) / scale;
}
template <class T>
inline dimension2d< T > & opErAToRWrapper_dimension2d__opStarAssign(dimension2d<T> * self, const T & scale) {
    return (*self) *= scale;
}
template <class T>
inline dimension2d< T > opErAToRWrapper_dimension2d__opStar(const dimension2d<T> * self, const T & scale) {
    return (*self) * scale;
}
template <class T>
inline dimension2d< T > & opErAToRWrapper_dimension2d__opPlusAssign(dimension2d<T> * self, const dimension2d< T > & other) {
    return (*self) += other;
}
template <class T>
inline dimension2d< T > & opErAToRWrapper_dimension2d__opMinusAssign(dimension2d<T> * self, const dimension2d< T > & other) {
    return (*self) -= other;
}
template <class T>
inline dimension2d< T > opErAToRWrapper_dimension2d__opPlus(const dimension2d<T> * self, const dimension2d< T > & other) {
    return (*self) + other;
}


template <typename D, class T>
void buildMetaClass_Dimension2d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const T &, const T &)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector2d< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _field("Width", &D::ClassType::Width);
    _d.CPGF_MD_TEMPLATE _field("Height", &D::ClassType::Height);
    _d.CPGF_MD_TEMPLATE _method("set", &D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("getArea", &D::ClassType::getArea);
    _d.CPGF_MD_TEMPLATE _method("getOptimalSize", &D::ClassType::getOptimalSize)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("getInterpolated", &D::ClassType::getInterpolated, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const dimension2d< T > &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const dimension2d<T> *, const dimension2d< T > &))&opErAToRWrapper_dimension2d__opEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const dimension2d< T > &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const dimension2d<T> *, const dimension2d< T > &))&opErAToRWrapper_dimension2d__opNotEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const dimension2d<T> *, const vector2d< T > &))&opErAToRWrapper_dimension2d__opEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const dimension2d<T> *, const vector2d< T > &))&opErAToRWrapper_dimension2d__opNotEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<dimension2d< T > & (*)(cpgf::GMetaSelf, const T &)>(mopHolder /= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opDivAssign", (dimension2d< T > & (*) (dimension2d<T> *, const T &))&opErAToRWrapper_dimension2d__opDivAssign<T>);
    _d.CPGF_MD_TEMPLATE _operator<dimension2d< T > (*)(const cpgf::GMetaSelf &, const T &)>(mopHolder / mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opDiv", (dimension2d< T > (*) (const dimension2d<T> *, const T &))&opErAToRWrapper_dimension2d__opDiv<T>);
    _d.CPGF_MD_TEMPLATE _operator<dimension2d< T > & (*)(cpgf::GMetaSelf, const T &)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opStarAssign", (dimension2d< T > & (*) (dimension2d<T> *, const T &))&opErAToRWrapper_dimension2d__opStarAssign<T>);
    _d.CPGF_MD_TEMPLATE _operator<dimension2d< T > (*)(const cpgf::GMetaSelf &, const T &)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opStar", (dimension2d< T > (*) (const dimension2d<T> *, const T &))&opErAToRWrapper_dimension2d__opStar<T>);
    _d.CPGF_MD_TEMPLATE _operator<dimension2d< T > & (*)(cpgf::GMetaSelf, const dimension2d< T > &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opPlusAssign", (dimension2d< T > & (*) (dimension2d<T> *, const dimension2d< T > &))&opErAToRWrapper_dimension2d__opPlusAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<dimension2d< T > & (*)(cpgf::GMetaSelf, const dimension2d< T > &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMinusAssign", (dimension2d< T > & (*) (dimension2d<T> *, const dimension2d< T > &))&opErAToRWrapper_dimension2d__opMinusAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<dimension2d< T > (*)(const cpgf::GMetaSelf &, const dimension2d< T > &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opPlus", (dimension2d< T > (*) (const dimension2d<T> *, const dimension2d< T > &))&opErAToRWrapper_dimension2d__opPlus<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
}


template <typename D, class T>
void buildMetaClass_Vector2d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
