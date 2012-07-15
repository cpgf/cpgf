// Auto generated file, don't modify.

#ifndef __META_SFML_FTP_H
#define __META_SFML_FTP_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Ftp(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("Connect", &D::ClassType::Connect, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0.f))
        ._default(copyVariantFromCopyable(21))
    ;
    _d.CPGF_MD_TEMPLATE _method("Login", (Ftp::Response (D::ClassType::*) ())&D::ClassType::Login);
    _d.CPGF_MD_TEMPLATE _method("Login", (Ftp::Response (D::ClassType::*) (const std::string &, const std::string &))&D::ClassType::Login, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("Disconnect", &D::ClassType::Disconnect);
    _d.CPGF_MD_TEMPLATE _method("KeepAlive", &D::ClassType::KeepAlive);
    _d.CPGF_MD_TEMPLATE _method("GetWorkingDirectory", &D::ClassType::GetWorkingDirectory);
    _d.CPGF_MD_TEMPLATE _method("GetDirectoryListing", &D::ClassType::GetDirectoryListing, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(""))
    ;
    _d.CPGF_MD_TEMPLATE _method("ChangeDirectory", &D::ClassType::ChangeDirectory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("ParentDirectory", &D::ClassType::ParentDirectory);
    _d.CPGF_MD_TEMPLATE _method("MakeDirectory", &D::ClassType::MakeDirectory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("DeleteDirectory", &D::ClassType::DeleteDirectory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("RenameFile", &D::ClassType::RenameFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("DeleteFile", &D::ClassType::DeleteFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("Download", &D::ClassType::Download, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(Ftp::Binary))
    ;
    _d.CPGF_MD_TEMPLATE _method("Upload", &D::ClassType::Upload, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(Ftp::Binary))
    ;
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::TransferMode>("TransferMode")
        ._element("Binary", D::ClassType::Binary)
        ._element("Ascii", D::ClassType::Ascii)
        ._element("Ebcdic", D::ClassType::Ebcdic)
    ;
    {
        GDefineMetaClass<Ftp::DirectoryResponse, sf::Ftp::Response> _nd = GDefineMetaClass<Ftp::DirectoryResponse, sf::Ftp::Response>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("DirectoryResponse");
        _nd.CPGF_MD_TEMPLATE _constructor<void * (Ftp::Response)>();
        _nd.CPGF_MD_TEMPLATE _method("GetDirectory", &sf::Ftp::DirectoryResponse::GetDirectory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Ftp::ListingResponse, sf::Ftp::Response> _nd = GDefineMetaClass<Ftp::ListingResponse, sf::Ftp::Response>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("ListingResponse");
        _nd.CPGF_MD_TEMPLATE _constructor<void * (Ftp::Response, const std::vector< char > &)>();
        _nd.CPGF_MD_TEMPLATE _method("GetCount", &sf::Ftp::ListingResponse::GetCount);
        _nd.CPGF_MD_TEMPLATE _method("GetFilename", &sf::Ftp::ListingResponse::GetFilename, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Ftp::Response> _nd = GDefineMetaClass<Ftp::Response>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("Response");
        _nd.CPGF_MD_TEMPLATE _constructor<void * (Ftp::Response::Status, const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
            ._default(copyVariantFromCopyable(""))
            ._default(copyVariantFromCopyable(Ftp::Response::InvalidResponse))
        ;
        _nd.CPGF_MD_TEMPLATE _method("IsOk", &sf::Ftp::Response::IsOk);
        _nd.CPGF_MD_TEMPLATE _method("GetStatus", &sf::Ftp::Response::GetStatus);
        _nd.CPGF_MD_TEMPLATE _method("GetMessage", &sf::Ftp::Response::GetMessage, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
        _nd.CPGF_MD_TEMPLATE _enum<typename sf::Ftp::Response::Status>("Status")
            ._element("RestartMarkerReply", sf::Ftp::Response::RestartMarkerReply)
            ._element("ServiceReadySoon", sf::Ftp::Response::ServiceReadySoon)
            ._element("DataConnectionAlreadyOpened", sf::Ftp::Response::DataConnectionAlreadyOpened)
            ._element("OpeningDataConnection", sf::Ftp::Response::OpeningDataConnection)
            ._element("Ok", sf::Ftp::Response::Ok)
            ._element("PointlessCommand", sf::Ftp::Response::PointlessCommand)
            ._element("SystemStatus", sf::Ftp::Response::SystemStatus)
            ._element("DirectoryStatus", sf::Ftp::Response::DirectoryStatus)
            ._element("FileStatus", sf::Ftp::Response::FileStatus)
            ._element("HelpMessage", sf::Ftp::Response::HelpMessage)
            ._element("SystemType", sf::Ftp::Response::SystemType)
            ._element("ServiceReady", sf::Ftp::Response::ServiceReady)
            ._element("ClosingConnection", sf::Ftp::Response::ClosingConnection)
            ._element("DataConnectionOpened", sf::Ftp::Response::DataConnectionOpened)
            ._element("ClosingDataConnection", sf::Ftp::Response::ClosingDataConnection)
            ._element("EnteringPassiveMode", sf::Ftp::Response::EnteringPassiveMode)
            ._element("LoggedIn", sf::Ftp::Response::LoggedIn)
            ._element("FileActionOk", sf::Ftp::Response::FileActionOk)
            ._element("DirectoryOk", sf::Ftp::Response::DirectoryOk)
            ._element("NeedPassword", sf::Ftp::Response::NeedPassword)
            ._element("NeedAccountToLogIn", sf::Ftp::Response::NeedAccountToLogIn)
            ._element("NeedInformation", sf::Ftp::Response::NeedInformation)
            ._element("ServiceUnavailable", sf::Ftp::Response::ServiceUnavailable)
            ._element("DataConnectionUnavailable", sf::Ftp::Response::DataConnectionUnavailable)
            ._element("TransferAborted", sf::Ftp::Response::TransferAborted)
            ._element("FileActionAborted", sf::Ftp::Response::FileActionAborted)
            ._element("LocalError", sf::Ftp::Response::LocalError)
            ._element("InsufficientStorageSpace", sf::Ftp::Response::InsufficientStorageSpace)
            ._element("CommandUnknown", sf::Ftp::Response::CommandUnknown)
            ._element("ParametersUnknown", sf::Ftp::Response::ParametersUnknown)
            ._element("CommandNotImplemented", sf::Ftp::Response::CommandNotImplemented)
            ._element("BadCommandSequence", sf::Ftp::Response::BadCommandSequence)
            ._element("ParameterNotImplemented", sf::Ftp::Response::ParameterNotImplemented)
            ._element("NotLoggedIn", sf::Ftp::Response::NotLoggedIn)
            ._element("NeedAccountToStore", sf::Ftp::Response::NeedAccountToStore)
            ._element("FileUnavailable", sf::Ftp::Response::FileUnavailable)
            ._element("PageTypeUnknown", sf::Ftp::Response::PageTypeUnknown)
            ._element("NotEnoughMemory", sf::Ftp::Response::NotEnoughMemory)
            ._element("FilenameNotAllowed", sf::Ftp::Response::FilenameNotAllowed)
            ._element("InvalidResponse", sf::Ftp::Response::InvalidResponse)
            ._element("ConnectionFailed", sf::Ftp::Response::ConnectionFailed)
            ._element("ConnectionClosed", sf::Ftp::Response::ConnectionClosed)
            ._element("InvalidFile", sf::Ftp::Response::InvalidFile)
        ;
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
