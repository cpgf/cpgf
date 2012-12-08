// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_COREUTIL_H
#define __META_IRRLICHT_COREUTIL_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_coreutil(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("isFileExtension", (s32 (*) (const io::path &, const io::path &, const io::path &, const io::path &))&isFileExtension, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleCopyConstReference<2>, cpgf::GMetaRuleCopyConstReference<3> >());
    _d.CPGF_MD_TEMPLATE _method("hasFileExtension", (bool (*) (const io::path &, const io::path &, const io::path &, const io::path &))&hasFileExtension, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleCopyConstReference<2>, cpgf::GMetaRuleCopyConstReference<3> >())
        ._default(copyVariantFromCopyable(""))
        ._default(copyVariantFromCopyable(""))
    ;
    _d.CPGF_MD_TEMPLATE _method("cutFilenameExtension", (io::path & (*) (io::path &, const io::path &))&cutFilenameExtension, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("getFileNameExtension", (io::path & (*) (io::path &, const io::path &))&getFileNameExtension, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("deletePathFromFilename", (io::path & (*) (io::path &))&deletePathFromFilename);
    _d.CPGF_MD_TEMPLATE _method("deletePathFromPath", (io::path & (*) (io::path &, s32))&deletePathFromPath);
    _d.CPGF_MD_TEMPLATE _method("isInSameDirectory", (s32 (*) (const io::path &, const io::path &))&isInSameDirectory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("splitFilename", (void (*) (const io::path &, io::path *, io::path *, io::path *, bool))&splitFilename, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
