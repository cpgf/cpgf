// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IGUITABLE_H
#define __META_IRRLICHT_IGUITABLE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_iguitable(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("GUIColumnOrderingNames", &GUIColumnOrderingNames);
    _d.CPGF_MD_TEMPLATE _field("GUIOrderingModeNames", &GUIOrderingModeNames);
    _d.CPGF_MD_TEMPLATE _enum<EGUI_COLUMN_ORDERING>("EGUI_COLUMN_ORDERING")
        ._element("EGCO_NONE", irr::gui::EGCO_NONE)
        ._element("EGCO_CUSTOM", irr::gui::EGCO_CUSTOM)
        ._element("EGCO_ASCENDING", irr::gui::EGCO_ASCENDING)
        ._element("EGCO_DESCENDING", irr::gui::EGCO_DESCENDING)
        ._element("EGCO_FLIP_ASCENDING_DESCENDING", irr::gui::EGCO_FLIP_ASCENDING_DESCENDING)
        ._element("EGCO_COUNT", irr::gui::EGCO_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EGUI_ORDERING_MODE>("EGUI_ORDERING_MODE")
        ._element("EGOM_NONE", irr::gui::EGOM_NONE)
        ._element("EGOM_ASCENDING", irr::gui::EGOM_ASCENDING)
        ._element("EGOM_DESCENDING", irr::gui::EGOM_DESCENDING)
        ._element("EGOM_COUNT", irr::gui::EGOM_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EGUI_TABLE_DRAW_FLAGS>("EGUI_TABLE_DRAW_FLAGS")
        ._element("EGTDF_ROWS", irr::gui::EGTDF_ROWS)
        ._element("EGTDF_COLUMNS", irr::gui::EGTDF_COLUMNS)
        ._element("EGTDF_ACTIVE_ROW", irr::gui::EGTDF_ACTIVE_ROW)
        ._element("EGTDF_COUNT", irr::gui::EGTDF_COUNT)
    ;
}


template <typename D>
void buildMetaClass_IGUITable(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("addColumn", &D::ClassType::addColumn)
        ._default(copyVariantFromCopyable(-1))
    ;
    _d.CPGF_MD_TEMPLATE _method("removeColumn", &D::ClassType::removeColumn);
    _d.CPGF_MD_TEMPLATE _method("getColumnCount", &D::ClassType::getColumnCount);
    _d.CPGF_MD_TEMPLATE _method("setActiveColumn", &D::ClassType::setActiveColumn)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("getActiveColumn", &D::ClassType::getActiveColumn);
    _d.CPGF_MD_TEMPLATE _method("getActiveColumnOrdering", &D::ClassType::getActiveColumnOrdering);
    _d.CPGF_MD_TEMPLATE _method("setColumnWidth", &D::ClassType::setColumnWidth);
    _d.CPGF_MD_TEMPLATE _method("getColumnWidth", &D::ClassType::getColumnWidth);
    _d.CPGF_MD_TEMPLATE _method("setResizableColumns", &D::ClassType::setResizableColumns);
    _d.CPGF_MD_TEMPLATE _method("hasResizableColumns", &D::ClassType::hasResizableColumns);
    _d.CPGF_MD_TEMPLATE _method("setColumnOrdering", &D::ClassType::setColumnOrdering);
    _d.CPGF_MD_TEMPLATE _method("getSelected", &D::ClassType::getSelected);
    _d.CPGF_MD_TEMPLATE _method("setSelected", &D::ClassType::setSelected);
    _d.CPGF_MD_TEMPLATE _method("getRowCount", &D::ClassType::getRowCount);
    _d.CPGF_MD_TEMPLATE _method("addRow", &D::ClassType::addRow);
    _d.CPGF_MD_TEMPLATE _method("removeRow", &D::ClassType::removeRow);
    _d.CPGF_MD_TEMPLATE _method("clearRows", &D::ClassType::clearRows);
    _d.CPGF_MD_TEMPLATE _method("swapRows", &D::ClassType::swapRows);
    _d.CPGF_MD_TEMPLATE _method("orderRows", &D::ClassType::orderRows)
        ._default(copyVariantFromCopyable(EGOM_NONE))
        ._default(copyVariantFromCopyable(-1))
    ;
    _d.CPGF_MD_TEMPLATE _method("setCellText", (void (D::ClassType::*) (u32, u32, const core::stringw &))&D::ClassType::setCellText, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >());
    _d.CPGF_MD_TEMPLATE _method("setCellText", (void (D::ClassType::*) (u32, u32, const core::stringw &, video::SColor))&D::ClassType::setCellText, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >());
    _d.CPGF_MD_TEMPLATE _method("setCellData", &D::ClassType::setCellData);
    _d.CPGF_MD_TEMPLATE _method("setCellColor", &D::ClassType::setCellColor);
    _d.CPGF_MD_TEMPLATE _method("getCellText", &D::ClassType::getCellText);
    _d.CPGF_MD_TEMPLATE _method("getCellData", &D::ClassType::getCellData);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("setDrawFlags", &D::ClassType::setDrawFlags);
    _d.CPGF_MD_TEMPLATE _method("getDrawFlags", &D::ClassType::getDrawFlags);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
