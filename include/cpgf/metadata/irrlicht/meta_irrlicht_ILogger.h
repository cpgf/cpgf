// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ILOGGER_H
#define __META_IRRLICHT_ILOGGER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ilogger(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<ELOG_LEVEL>("ELOG_LEVEL")
        ._element("ELL_DEBUG", irr::ELL_DEBUG)
        ._element("ELL_INFORMATION", irr::ELL_INFORMATION)
        ._element("ELL_WARNING", irr::ELL_WARNING)
        ._element("ELL_ERROR", irr::ELL_ERROR)
        ._element("ELL_NONE", irr::ELL_NONE)
    ;
}


template <typename D>
void buildMetaClass_ILogger(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getLogLevel", &D::ClassType::getLogLevel);
    _d.CPGF_MD_TEMPLATE _method("setLogLevel", &D::ClassType::setLogLevel);
    _d.CPGF_MD_TEMPLATE _method("log", (void (D::ClassType::*) (const c8 *, ELOG_LEVEL))&D::ClassType::log)
        ._default(copyVariantFromCopyable(ELL_INFORMATION))
    ;
    _d.CPGF_MD_TEMPLATE _method("log", (void (D::ClassType::*) (const c8 *, const c8 *, ELOG_LEVEL))&D::ClassType::log)
        ._default(copyVariantFromCopyable(ELL_INFORMATION))
    ;
    _d.CPGF_MD_TEMPLATE _method("log", (void (D::ClassType::*) (const c8 *, const wchar_t *, ELOG_LEVEL))&D::ClassType::log)
        ._default(copyVariantFromCopyable(ELL_INFORMATION))
    ;
    _d.CPGF_MD_TEMPLATE _method("log", (void (D::ClassType::*) (const wchar_t *, const wchar_t *, ELOG_LEVEL))&D::ClassType::log)
        ._default(copyVariantFromCopyable(ELL_INFORMATION))
    ;
    _d.CPGF_MD_TEMPLATE _method("log", (void (D::ClassType::*) (const wchar_t *, ELOG_LEVEL))&D::ClassType::log)
        ._default(copyVariantFromCopyable(ELL_INFORMATION))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
