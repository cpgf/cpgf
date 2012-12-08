// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IFILEARCHIVE_H
#define __META_IRRLICHT_IFILEARCHIVE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ifilearchive(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<EFileSystemType>("EFileSystemType")
        ._element("FILESYSTEM_NATIVE", irr::io::FILESYSTEM_NATIVE)
        ._element("FILESYSTEM_VIRTUAL", irr::io::FILESYSTEM_VIRTUAL)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_FILE_ARCHIVE_TYPE>("E_FILE_ARCHIVE_TYPE")
        ._element("EFAT_ZIP", irr::io::EFAT_ZIP)
        ._element("EFAT_GZIP", irr::io::EFAT_GZIP)
        ._element("EFAT_FOLDER", irr::io::EFAT_FOLDER)
        ._element("EFAT_PAK", irr::io::EFAT_PAK)
        ._element("EFAT_NPK", irr::io::EFAT_NPK)
        ._element("EFAT_TAR", irr::io::EFAT_TAR)
        ._element("EFAT_WAD", irr::io::EFAT_WAD)
        ._element("EFAT_UNKNOWN", irr::io::EFAT_UNKNOWN)
    ;
}


template <typename D>
void buildMetaClass_IArchiveLoader(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("isALoadableFileFormat", (bool (D::ClassType::*) (const path &) const)&D::ClassType::isALoadableFileFormat, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isALoadableFileFormat", (bool (D::ClassType::*) (io::IReadFile *) const)&D::ClassType::isALoadableFileFormat);
    _d.CPGF_MD_TEMPLATE _method("isALoadableFileFormat", (bool (D::ClassType::*) (E_FILE_ARCHIVE_TYPE) const)&D::ClassType::isALoadableFileFormat);
    _d.CPGF_MD_TEMPLATE _method("createArchive", (IFileArchive * (D::ClassType::*) (const path &, bool, bool) const)&D::ClassType::createArchive, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("createArchive", (IFileArchive * (D::ClassType::*) (io::IReadFile *, bool, bool) const)&D::ClassType::createArchive);
}


template <typename D>
void buildMetaClass_IFileArchive(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("Password", &D::ClassType::Password);
    _d.CPGF_MD_TEMPLATE _method("createAndOpenFile", (IReadFile * (D::ClassType::*) (const path &))&D::ClassType::createAndOpenFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("createAndOpenFile", (IReadFile * (D::ClassType::*) (u32))&D::ClassType::createAndOpenFile);
    _d.CPGF_MD_TEMPLATE _method("getFileList", &D::ClassType::getFileList);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
