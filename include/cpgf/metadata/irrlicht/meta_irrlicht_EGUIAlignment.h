// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_EGUIALIGNMENT_H
#define __META_IRRLICHT_EGUIALIGNMENT_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_eguialignment(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("GUIAlignmentNames", &GUIAlignmentNames);
    _d.CPGF_MD_TEMPLATE _enum<EGUI_ALIGNMENT>("EGUI_ALIGNMENT")
        ._element("EGUIA_UPPERLEFT", irr::gui::EGUIA_UPPERLEFT)
        ._element("EGUIA_LOWERRIGHT", irr::gui::EGUIA_LOWERRIGHT)
        ._element("EGUIA_CENTER", irr::gui::EGUIA_CENTER)
        ._element("EGUIA_SCALE", irr::gui::EGUIA_SCALE)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
