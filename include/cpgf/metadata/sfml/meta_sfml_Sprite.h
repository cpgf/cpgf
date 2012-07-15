// Auto generated file, don't modify.

#ifndef __META_SFML_SPRITE_H
#define __META_SFML_SPRITE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Sprite(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const Image &, const Vector2f &, const Vector2f &, float, const Color &)>()
        ._default(copyVariantFromCopyable(Color(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(0.f))
        ._default(copyVariantFromCopyable(Vector2f(1, 1)))
        ._default(copyVariantFromCopyable(Vector2f(0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _method("SetImage", &D::ClassType::SetImage);
    _d.CPGF_MD_TEMPLATE _method("SetSubRect", &D::ClassType::SetSubRect);
    _d.CPGF_MD_TEMPLATE _method("Resize", (void (D::ClassType::*) (float, float))&D::ClassType::Resize);
    _d.CPGF_MD_TEMPLATE _method("Resize", (void (D::ClassType::*) (const Vector2f &))&D::ClassType::Resize);
    _d.CPGF_MD_TEMPLATE _method("FlipX", &D::ClassType::FlipX);
    _d.CPGF_MD_TEMPLATE _method("FlipY", &D::ClassType::FlipY);
    _d.CPGF_MD_TEMPLATE _method("GetImage", &D::ClassType::GetImage);
    _d.CPGF_MD_TEMPLATE _method("GetSubRect", &D::ClassType::GetSubRect);
    _d.CPGF_MD_TEMPLATE _method("GetSize", &D::ClassType::GetSize);
    _d.CPGF_MD_TEMPLATE _method("GetPixel", &D::ClassType::GetPixel);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
