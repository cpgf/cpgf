// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUITREEVIEW_H
#define __META_IRRLICHT_IGUITREEVIEW_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IGUITreeView(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getRoot", &D::ClassType::getRoot);
    _d.CPGF_MD_TEMPLATE _method("getSelected", &D::ClassType::getSelected);
    _d.CPGF_MD_TEMPLATE _method("getLinesVisible", &D::ClassType::getLinesVisible);
    _d.CPGF_MD_TEMPLATE _method("setLinesVisible", &D::ClassType::setLinesVisible);
    _d.CPGF_MD_TEMPLATE _method("setIconFont", &D::ClassType::setIconFont);
    _d.CPGF_MD_TEMPLATE _method("setImageList", &D::ClassType::setImageList);
    _d.CPGF_MD_TEMPLATE _method("getImageList", &D::ClassType::getImageList);
    _d.CPGF_MD_TEMPLATE _method("setImageLeftOfIcon", &D::ClassType::setImageLeftOfIcon);
    _d.CPGF_MD_TEMPLATE _method("getImageLeftOfIcon", &D::ClassType::getImageLeftOfIcon);
    _d.CPGF_MD_TEMPLATE _method("getLastEventNode", &D::ClassType::getLastEventNode);
}


template <typename D>
void buildMetaClass_IGUITreeViewNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getOwner", &D::ClassType::getOwner);
    _d.CPGF_MD_TEMPLATE _method("getParent", &D::ClassType::getParent);
    _d.CPGF_MD_TEMPLATE _method("getText", &D::ClassType::getText);
    _d.CPGF_MD_TEMPLATE _method("setText", &D::ClassType::setText);
    _d.CPGF_MD_TEMPLATE _method("getIcon", &D::ClassType::getIcon);
    _d.CPGF_MD_TEMPLATE _method("setIcon", &D::ClassType::setIcon);
    _d.CPGF_MD_TEMPLATE _method("getImageIndex", &D::ClassType::getImageIndex);
    _d.CPGF_MD_TEMPLATE _method("setImageIndex", &D::ClassType::setImageIndex);
    _d.CPGF_MD_TEMPLATE _method("getSelectedImageIndex", &D::ClassType::getSelectedImageIndex);
    _d.CPGF_MD_TEMPLATE _method("setSelectedImageIndex", &D::ClassType::setSelectedImageIndex);
    _d.CPGF_MD_TEMPLATE _method("getData", &D::ClassType::getData);
    _d.CPGF_MD_TEMPLATE _method("setData", &D::ClassType::setData);
    _d.CPGF_MD_TEMPLATE _method("getData2", &D::ClassType::getData2);
    _d.CPGF_MD_TEMPLATE _method("setData2", &D::ClassType::setData2);
    _d.CPGF_MD_TEMPLATE _method("getChildCount", &D::ClassType::getChildCount);
    _d.CPGF_MD_TEMPLATE _method("clearChildren", &D::ClassType::clearChildren);
    _d.CPGF_MD_TEMPLATE _method("clearChilds", &D::ClassType::clearChilds);
    _d.CPGF_MD_TEMPLATE _method("hasChildren", &D::ClassType::hasChildren);
    _d.CPGF_MD_TEMPLATE _method("hasChilds", &D::ClassType::hasChilds);
    _d.CPGF_MD_TEMPLATE _method("addChildBack", &D::ClassType::addChildBack)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addChildFront", &D::ClassType::addChildFront)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("insertChildAfter", &D::ClassType::insertChildAfter)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("insertChildBefore", &D::ClassType::insertChildBefore)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getFirstChild", &D::ClassType::getFirstChild);
    _d.CPGF_MD_TEMPLATE _method("getLastChild", &D::ClassType::getLastChild);
    _d.CPGF_MD_TEMPLATE _method("getPrevSibling", &D::ClassType::getPrevSibling);
    _d.CPGF_MD_TEMPLATE _method("getNextSibling", &D::ClassType::getNextSibling);
    _d.CPGF_MD_TEMPLATE _method("getNextVisible", &D::ClassType::getNextVisible);
    _d.CPGF_MD_TEMPLATE _method("deleteChild", &D::ClassType::deleteChild);
    _d.CPGF_MD_TEMPLATE _method("moveChildUp", &D::ClassType::moveChildUp);
    _d.CPGF_MD_TEMPLATE _method("moveChildDown", &D::ClassType::moveChildDown);
    _d.CPGF_MD_TEMPLATE _method("getExpanded", &D::ClassType::getExpanded);
    _d.CPGF_MD_TEMPLATE _method("setExpanded", &D::ClassType::setExpanded);
    _d.CPGF_MD_TEMPLATE _method("getSelected", &D::ClassType::getSelected);
    _d.CPGF_MD_TEMPLATE _method("setSelected", &D::ClassType::setSelected);
    _d.CPGF_MD_TEMPLATE _method("isRoot", &D::ClassType::isRoot);
    _d.CPGF_MD_TEMPLATE _method("getLevel", &D::ClassType::getLevel);
    _d.CPGF_MD_TEMPLATE _method("isVisible", &D::ClassType::isVisible);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
