// Auto generated file, don't modify.

#ifndef __META_SFML_RECT_H
#define __META_SFML_RECT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D, typename T>
void buildMetaClass_Rect(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T, T, T)>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("Left", _r), &D::ClassType::Left);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Top", _r), &D::ClassType::Top);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Right", _r), &D::ClassType::Right);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Bottom", _r), &D::ClassType::Bottom);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWidth", _r), &D::ClassType::GetWidth);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetHeight", _r), &D::ClassType::GetHeight);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Offset", _r), &D::ClassType::Offset);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Contains", _r), &D::ClassType::Contains);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Intersects", _r), &D::ClassType::Intersects)
        ._default(copyVariantFromCopyable((Rect< T > *)NULL))
    ;
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
