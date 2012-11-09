// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUIFONTBITMAP_H
#define __META_IRRLICHT_IGUIFONTBITMAP_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUIFontBitmap(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
    _d.CPGF_MD_TEMPLATE _method("getSpriteBank", &D::ClassType::getSpriteBank);
    _d.CPGF_MD_TEMPLATE _method("getSpriteNoFromChar", &D::ClassType::getSpriteNoFromChar);
    _d.CPGF_MD_TEMPLATE _method("getKerningWidth", &D::ClassType::getKerningWidth)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
