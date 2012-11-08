// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_EMESSAGEBOXFLAGS_H
#define __META_IRRLICHT_EMESSAGEBOXFLAGS_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_emessageboxflags(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<EMESSAGE_BOX_FLAG>("EMESSAGE_BOX_FLAG")
        ._element("EMBF_OK", irr::gui::EMBF_OK)
        ._element("EMBF_CANCEL", irr::gui::EMBF_CANCEL)
        ._element("EMBF_YES", irr::gui::EMBF_YES)
        ._element("EMBF_NO", irr::gui::EMBF_NO)
        ._element("EMBF_FORCE_32BIT", irr::gui::EMBF_FORCE_32BIT)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
