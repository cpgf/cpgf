// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IFILESYSTEM_H
#define __META_IRRLICHT_IFILESYSTEM_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;
using namespace irr::io;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IFileSystem(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("createAndOpenFile", &D::ClassType::createAndOpenFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("createMemoryWriteFile", &D::ClassType::createMemoryWriteFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >())
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("createAndWriteFile", &D::ClassType::createAndWriteFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("addFileArchive", (bool (D::ClassType::*) (const path &, bool, bool, E_FILE_ARCHIVE_TYPE, const core::stringc &, IFileArchive **))&D::ClassType::addFileArchive, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<4> >())
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(""))
        ._default(copyVariantFromCopyable(EFAT_UNKNOWN))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("addFileArchive", (bool (D::ClassType::*) (IReadFile *, bool, bool, E_FILE_ARCHIVE_TYPE, const core::stringc &, IFileArchive **))&D::ClassType::addFileArchive, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<4> >())
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(""))
        ._default(copyVariantFromCopyable(EFAT_UNKNOWN))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("addFileArchive", (bool (D::ClassType::*) (IFileArchive *))&D::ClassType::addFileArchive);
    _d.CPGF_MD_TEMPLATE _method("getFileArchiveCount", &D::ClassType::getFileArchiveCount);
    _d.CPGF_MD_TEMPLATE _method("removeFileArchive", (bool (D::ClassType::*) (u32))&D::ClassType::removeFileArchive);
    _d.CPGF_MD_TEMPLATE _method("removeFileArchive", (bool (D::ClassType::*) (const path &))&D::ClassType::removeFileArchive, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("removeFileArchive", (bool (D::ClassType::*) (const IFileArchive *))&D::ClassType::removeFileArchive);
    _d.CPGF_MD_TEMPLATE _method("moveFileArchive", &D::ClassType::moveFileArchive);
    _d.CPGF_MD_TEMPLATE _method("getFileArchive", &D::ClassType::getFileArchive);
    _d.CPGF_MD_TEMPLATE _method("addArchiveLoader", &D::ClassType::addArchiveLoader);
    _d.CPGF_MD_TEMPLATE _method("getArchiveLoaderCount", &D::ClassType::getArchiveLoaderCount);
    _d.CPGF_MD_TEMPLATE _method("getArchiveLoader", &D::ClassType::getArchiveLoader);
    _d.CPGF_MD_TEMPLATE _method("addZipFileArchive", &D::ClassType::addZipFileArchive)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("addFolderFileArchive", &D::ClassType::addFolderFileArchive)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("addPakFileArchive", &D::ClassType::addPakFileArchive)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("getWorkingDirectory", &D::ClassType::getWorkingDirectory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method("changeWorkingDirectoryTo", &D::ClassType::changeWorkingDirectoryTo, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getAbsolutePath", &D::ClassType::getAbsolutePath, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getFileDir", &D::ClassType::getFileDir, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getFileBasename", &D::ClassType::getFileBasename, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("flattenFilename", &D::ClassType::flattenFilename, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable("/"))
    ;
    _d.CPGF_MD_TEMPLATE _method("getRelativeFilename", &D::ClassType::getRelativeFilename, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("createFileList", &D::ClassType::createFileList);
    _d.CPGF_MD_TEMPLATE _method("createEmptyFileList", &D::ClassType::createEmptyFileList, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("setFileListSystem", &D::ClassType::setFileListSystem);
    _d.CPGF_MD_TEMPLATE _method("existFile", &D::ClassType::existFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("createXMLReader", (IXMLReader * (D::ClassType::*) (const path &))&D::ClassType::createXMLReader, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("createXMLReader", (IXMLReader * (D::ClassType::*) (IReadFile *))&D::ClassType::createXMLReader);
    _d.CPGF_MD_TEMPLATE _method("createXMLReaderUTF8", (IXMLReaderUTF8 * (D::ClassType::*) (const path &))&D::ClassType::createXMLReaderUTF8, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("createXMLReaderUTF8", (IXMLReaderUTF8 * (D::ClassType::*) (IReadFile *))&D::ClassType::createXMLReaderUTF8);
    _d.CPGF_MD_TEMPLATE _method("createXMLWriter", (IXMLWriter * (D::ClassType::*) (const path &))&D::ClassType::createXMLWriter, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("createXMLWriter", (IXMLWriter * (D::ClassType::*) (IWriteFile *))&D::ClassType::createXMLWriter);
    _d.CPGF_MD_TEMPLATE _method("createEmptyAttributes", &D::ClassType::createEmptyAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
