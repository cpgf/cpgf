// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IXMLWRITER_H
#define __META_IRRLICHT_IXMLWRITER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IXMLWriter(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("writeXMLHeader", &D::ClassType::writeXMLHeader);
    _d.CPGF_MD_TEMPLATE _method("writeElement", (void (D::ClassType::*) (const wchar_t *, bool, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *))&D::ClassType::writeElement)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("writeElement", (void (D::ClassType::*) (const wchar_t *, bool, core::array< core::stringw > &, core::array< core::stringw > &))&D::ClassType::writeElement);
    _d.CPGF_MD_TEMPLATE _method("writeComment", &D::ClassType::writeComment);
    _d.CPGF_MD_TEMPLATE _method("writeClosingTag", &D::ClassType::writeClosingTag);
    _d.CPGF_MD_TEMPLATE _method("writeText", &D::ClassType::writeText);
    _d.CPGF_MD_TEMPLATE _method("writeLineBreak", &D::ClassType::writeLineBreak);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
