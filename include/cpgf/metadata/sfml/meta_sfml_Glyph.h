// Auto generated file, don't modify.

#ifndef __META_SFML_GLYPH_H
#define __META_SFML_GLYPH_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Glyph(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("Advance", &D::ClassType::Advance);
    _d.CPGF_MD_TEMPLATE _field("Rectangle", &D::ClassType::Rectangle);
    _d.CPGF_MD_TEMPLATE _field("TexCoords", &D::ClassType::TexCoords);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
