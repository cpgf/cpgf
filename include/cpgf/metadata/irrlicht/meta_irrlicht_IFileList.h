// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IFILELIST_H
#define __META_IRRLICHT_IFILELIST_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IFileList(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getFileCount", &D::ClassType::getFileCount);
    _d.CPGF_MD_TEMPLATE _method("getFileName", &D::ClassType::getFileName, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method("getFullFileName", &D::ClassType::getFullFileName, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method("getFileSize", &D::ClassType::getFileSize);
    _d.CPGF_MD_TEMPLATE _method("getFileOffset", &D::ClassType::getFileOffset);
    _d.CPGF_MD_TEMPLATE _method("getID", &D::ClassType::getID);
    _d.CPGF_MD_TEMPLATE _method("isDirectory", &D::ClassType::isDirectory);
    _d.CPGF_MD_TEMPLATE _method("findFile", &D::ClassType::findFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("getPath", &D::ClassType::getPath, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method("addItem", &D::ClassType::addItem, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("sort", &D::ClassType::sort);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
