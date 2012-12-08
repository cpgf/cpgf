// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUICOMBOBOX_H
#define __META_IRRLICHT_IGUICOMBOBOX_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUIComboBox(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getItemCount", &D::ClassType::getItemCount);
    _d.CPGF_MD_TEMPLATE _method("getItem", &D::ClassType::getItem);
    _d.CPGF_MD_TEMPLATE _method("getItemData", &D::ClassType::getItemData);
    _d.CPGF_MD_TEMPLATE _method("getIndexForItemData", &D::ClassType::getIndexForItemData);
    _d.CPGF_MD_TEMPLATE _method("addItem", &D::ClassType::addItem)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("removeItem", &D::ClassType::removeItem);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("getSelected", &D::ClassType::getSelected);
    _d.CPGF_MD_TEMPLATE _method("setSelected", &D::ClassType::setSelected);
    _d.CPGF_MD_TEMPLATE _method("setTextAlignment", &D::ClassType::setTextAlignment);
    _d.CPGF_MD_TEMPLATE _method("setMaxSelectionRows", &D::ClassType::setMaxSelectionRows);
    _d.CPGF_MD_TEMPLATE _method("getMaxSelectionRows", &D::ClassType::getMaxSelectionRows);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
