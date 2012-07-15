// Auto generated file, don't modify.

#ifndef __META_SFML_RECT_H
#define __META_SFML_RECT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D, typename T>
void buildMetaClass_Rect(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T, T, T)>();
    _d.CPGF_MD_TEMPLATE _field("Left", &D::ClassType::Left);
    _d.CPGF_MD_TEMPLATE _field("Top", &D::ClassType::Top);
    _d.CPGF_MD_TEMPLATE _field("Right", &D::ClassType::Right);
    _d.CPGF_MD_TEMPLATE _field("Bottom", &D::ClassType::Bottom);
    _d.CPGF_MD_TEMPLATE _method("GetWidth", &D::ClassType::GetWidth);
    _d.CPGF_MD_TEMPLATE _method("GetHeight", &D::ClassType::GetHeight);
    _d.CPGF_MD_TEMPLATE _method("Offset", &D::ClassType::Offset);
    _d.CPGF_MD_TEMPLATE _method("Contains", &D::ClassType::Contains);
    _d.CPGF_MD_TEMPLATE _method("Intersects", &D::ClassType::Intersects)
        ._default(copyVariantFromCopyable((Rect< T > *)NULL))
    ;
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
