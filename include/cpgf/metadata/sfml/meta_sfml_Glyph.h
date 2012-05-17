// Auto generated file, don't modify.

#ifndef __META_SFML_GLYPH_H
#define __META_SFML_GLYPH_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Glyph(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("Advance", _r), &D::ClassType::Advance);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Rectangle", _r), &D::ClassType::Rectangle);
    _d.CPGF_MD_TEMPLATE _field(replaceName("TexCoords", _r), &D::ClassType::TexCoords);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
