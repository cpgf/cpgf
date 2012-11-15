// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUIFONT_H
#define __META_IRRLICHT_IGUIFONT_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_iguifont(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<EGUI_FONT_TYPE>("EGUI_FONT_TYPE")
        ._element("EGFT_BITMAP", irr::gui::EGFT_BITMAP)
        ._element("EGFT_VECTOR", irr::gui::EGFT_VECTOR)
        ._element("EGFT_OS", irr::gui::EGFT_OS)
        ._element("EGFT_CUSTOM", irr::gui::EGFT_CUSTOM)
    ;
}


template <typename D>
void buildMetaClass_IGUIFont(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("draw", &D::ClassType::draw, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("getDimension", &D::ClassType::getDimension);
    _d.CPGF_MD_TEMPLATE _method("getCharacterFromPos", &D::ClassType::getCharacterFromPos);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
    _d.CPGF_MD_TEMPLATE _method("setKerningWidth", &D::ClassType::setKerningWidth);
    _d.CPGF_MD_TEMPLATE _method("setKerningHeight", &D::ClassType::setKerningHeight);
    _d.CPGF_MD_TEMPLATE _method("getKerningWidth", &D::ClassType::getKerningWidth)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getKerningHeight", &D::ClassType::getKerningHeight);
    _d.CPGF_MD_TEMPLATE _method("setInvisibleCharacters", &D::ClassType::setInvisibleCharacters);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
