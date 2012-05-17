// Auto generated file, don't modify.

#ifndef __META_SFML_FTP_H
#define __META_SFML_FTP_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Ftp(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("Connect", _r), &D::ClassType::Connect, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0.f))
        ._default(copyVariantFromCopyable(21))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Login", _r), (Ftp::Response (D::ClassType::*) ())&D::ClassType::Login);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Login", _r), (Ftp::Response (D::ClassType::*) (const std::string &, const std::string &))&D::ClassType::Login, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("Disconnect", _r), &D::ClassType::Disconnect);
    _d.CPGF_MD_TEMPLATE _method(replaceName("KeepAlive", _r), &D::ClassType::KeepAlive);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWorkingDirectory", _r), &D::ClassType::GetWorkingDirectory);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetDirectoryListing", _r), &D::ClassType::GetDirectoryListing, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(""))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("ChangeDirectory", _r), &D::ClassType::ChangeDirectory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("ParentDirectory", _r), &D::ClassType::ParentDirectory);
    _d.CPGF_MD_TEMPLATE _method(replaceName("MakeDirectory", _r), &D::ClassType::MakeDirectory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("DeleteDirectory", _r), &D::ClassType::DeleteDirectory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("RenameFile", _r), &D::ClassType::RenameFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("DeleteFile", _r), &D::ClassType::DeleteFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method(replaceName("Download", _r), &D::ClassType::Download, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(Ftp::Binary))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Upload", _r), &D::ClassType::Upload, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(Ftp::Binary))
    ;
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::TransferMode>(replaceName("TransferMode", _r))
        ._element(replaceName("Binary", _r), D::ClassType::Binary)
        ._element(replaceName("Ascii", _r), D::ClassType::Ascii)
        ._element(replaceName("Ebcdic", _r), D::ClassType::Ebcdic)
    ;
    {
        GDefineMetaClass<Ftp::DirectoryResponse, sf::Ftp::Response> _nd = GDefineMetaClass<Ftp::DirectoryResponse, sf::Ftp::Response>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("DirectoryResponse");
        _nd.CPGF_MD_TEMPLATE _constructor<void * (Ftp::Response)>();
        _nd.CPGF_MD_TEMPLATE _method(replaceName("GetDirectory", _r), &sf::Ftp::DirectoryResponse::GetDirectory, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Ftp::ListingResponse, sf::Ftp::Response> _nd = GDefineMetaClass<Ftp::ListingResponse, sf::Ftp::Response>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("ListingResponse");
        _nd.CPGF_MD_TEMPLATE _constructor<void * (Ftp::Response, const std::vector< char > &)>();
        _nd.CPGF_MD_TEMPLATE _method(replaceName("GetCount", _r), &sf::Ftp::ListingResponse::GetCount);
        _nd.CPGF_MD_TEMPLATE _method(replaceName("GetFilename", _r), &sf::Ftp::ListingResponse::GetFilename, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Ftp::Response> _nd = GDefineMetaClass<Ftp::Response>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("Response");
        _nd.CPGF_MD_TEMPLATE _constructor<void * (Ftp::Response::Status, const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
            ._default(copyVariantFromCopyable(""))
            ._default(copyVariantFromCopyable(Ftp::Response::InvalidResponse))
        ;
        _nd.CPGF_MD_TEMPLATE _method(replaceName("IsOk", _r), &sf::Ftp::Response::IsOk);
        _nd.CPGF_MD_TEMPLATE _method(replaceName("GetStatus", _r), &sf::Ftp::Response::GetStatus);
        _nd.CPGF_MD_TEMPLATE _method(replaceName("GetMessage", _r), &sf::Ftp::Response::GetMessage, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
        _nd.CPGF_MD_TEMPLATE _enum<typename sf::Ftp::Response::Status>(replaceName("Status", _r))
            ._element(replaceName("RestartMarkerReply", _r), sf::Ftp::Response::RestartMarkerReply)
            ._element(replaceName("ServiceReadySoon", _r), sf::Ftp::Response::ServiceReadySoon)
            ._element(replaceName("DataConnectionAlreadyOpened", _r), sf::Ftp::Response::DataConnectionAlreadyOpened)
            ._element(replaceName("OpeningDataConnection", _r), sf::Ftp::Response::OpeningDataConnection)
            ._element(replaceName("Ok", _r), sf::Ftp::Response::Ok)
            ._element(replaceName("PointlessCommand", _r), sf::Ftp::Response::PointlessCommand)
            ._element(replaceName("SystemStatus", _r), sf::Ftp::Response::SystemStatus)
            ._element(replaceName("DirectoryStatus", _r), sf::Ftp::Response::DirectoryStatus)
            ._element(replaceName("FileStatus", _r), sf::Ftp::Response::FileStatus)
            ._element(replaceName("HelpMessage", _r), sf::Ftp::Response::HelpMessage)
            ._element(replaceName("SystemType", _r), sf::Ftp::Response::SystemType)
            ._element(replaceName("ServiceReady", _r), sf::Ftp::Response::ServiceReady)
            ._element(replaceName("ClosingConnection", _r), sf::Ftp::Response::ClosingConnection)
            ._element(replaceName("DataConnectionOpened", _r), sf::Ftp::Response::DataConnectionOpened)
            ._element(replaceName("ClosingDataConnection", _r), sf::Ftp::Response::ClosingDataConnection)
            ._element(replaceName("EnteringPassiveMode", _r), sf::Ftp::Response::EnteringPassiveMode)
            ._element(replaceName("LoggedIn", _r), sf::Ftp::Response::LoggedIn)
            ._element(replaceName("FileActionOk", _r), sf::Ftp::Response::FileActionOk)
            ._element(replaceName("DirectoryOk", _r), sf::Ftp::Response::DirectoryOk)
            ._element(replaceName("NeedPassword", _r), sf::Ftp::Response::NeedPassword)
            ._element(replaceName("NeedAccountToLogIn", _r), sf::Ftp::Response::NeedAccountToLogIn)
            ._element(replaceName("NeedInformation", _r), sf::Ftp::Response::NeedInformation)
            ._element(replaceName("ServiceUnavailable", _r), sf::Ftp::Response::ServiceUnavailable)
            ._element(replaceName("DataConnectionUnavailable", _r), sf::Ftp::Response::DataConnectionUnavailable)
            ._element(replaceName("TransferAborted", _r), sf::Ftp::Response::TransferAborted)
            ._element(replaceName("FileActionAborted", _r), sf::Ftp::Response::FileActionAborted)
            ._element(replaceName("LocalError", _r), sf::Ftp::Response::LocalError)
            ._element(replaceName("InsufficientStorageSpace", _r), sf::Ftp::Response::InsufficientStorageSpace)
            ._element(replaceName("CommandUnknown", _r), sf::Ftp::Response::CommandUnknown)
            ._element(replaceName("ParametersUnknown", _r), sf::Ftp::Response::ParametersUnknown)
            ._element(replaceName("CommandNotImplemented", _r), sf::Ftp::Response::CommandNotImplemented)
            ._element(replaceName("BadCommandSequence", _r), sf::Ftp::Response::BadCommandSequence)
            ._element(replaceName("ParameterNotImplemented", _r), sf::Ftp::Response::ParameterNotImplemented)
            ._element(replaceName("NotLoggedIn", _r), sf::Ftp::Response::NotLoggedIn)
            ._element(replaceName("NeedAccountToStore", _r), sf::Ftp::Response::NeedAccountToStore)
            ._element(replaceName("FileUnavailable", _r), sf::Ftp::Response::FileUnavailable)
            ._element(replaceName("PageTypeUnknown", _r), sf::Ftp::Response::PageTypeUnknown)
            ._element(replaceName("NotEnoughMemory", _r), sf::Ftp::Response::NotEnoughMemory)
            ._element(replaceName("FilenameNotAllowed", _r), sf::Ftp::Response::FilenameNotAllowed)
            ._element(replaceName("InvalidResponse", _r), sf::Ftp::Response::InvalidResponse)
            ._element(replaceName("ConnectionFailed", _r), sf::Ftp::Response::ConnectionFailed)
            ._element(replaceName("ConnectionClosed", _r), sf::Ftp::Response::ConnectionClosed)
            ._element(replaceName("InvalidFile", _r), sf::Ftp::Response::InvalidFile)
        ;
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
