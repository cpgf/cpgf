// Auto generated file, don't modify.

#ifndef __META_SFML_HTTP_H
#define __META_SFML_HTTP_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Http(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const std::string &, unsigned short)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("SetHost", &D::ClassType::SetHost, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("SendRequest", &D::ClassType::SendRequest)
        ._default(copyVariantFromCopyable(0.f))
    ;
    {
        GDefineMetaClass<Http::Request> _nd = GDefineMetaClass<Http::Request>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("Request");
        _nd.CPGF_MD_TEMPLATE _constructor<void * (Http::Request::Method, const std::string &, const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleCopyConstReference<2> >())
            ._default(copyVariantFromCopyable(""))
            ._default(copyVariantFromCopyable("/"))
            ._default(copyVariantFromCopyable(Http::Request::Get))
        ;
        _nd.CPGF_MD_TEMPLATE _method("SetField", &sf::Http::Request::SetField, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("SetMethod", &sf::Http::Request::SetMethod);
        _nd.CPGF_MD_TEMPLATE _method("SetURI", &sf::Http::Request::SetURI, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _method("SetHttpVersion", &sf::Http::Request::SetHttpVersion);
        _nd.CPGF_MD_TEMPLATE _method("SetBody", &sf::Http::Request::SetBody, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _enum<typename sf::Http::Request::Method>("Method")
            ._element("Get", sf::Http::Request::Get)
            ._element("Post", sf::Http::Request::Post)
            ._element("Head", sf::Http::Request::Head)
        ;
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Http::Response> _nd = GDefineMetaClass<Http::Response>::declare("Response");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _method("GetField", &sf::Http::Response::GetField, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1>, cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _method("GetStatus", &sf::Http::Response::GetStatus);
        _nd.CPGF_MD_TEMPLATE _method("GetMajorHttpVersion", &sf::Http::Response::GetMajorHttpVersion);
        _nd.CPGF_MD_TEMPLATE _method("GetMinorHttpVersion", &sf::Http::Response::GetMinorHttpVersion);
        _nd.CPGF_MD_TEMPLATE _method("GetBody", &sf::Http::Response::GetBody, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
        _nd.CPGF_MD_TEMPLATE _enum<typename sf::Http::Response::Status>("Status")
            ._element("Ok", sf::Http::Response::Ok)
            ._element("Created", sf::Http::Response::Created)
            ._element("Accepted", sf::Http::Response::Accepted)
            ._element("NoContent", sf::Http::Response::NoContent)
            ._element("MultipleChoices", sf::Http::Response::MultipleChoices)
            ._element("MovedPermanently", sf::Http::Response::MovedPermanently)
            ._element("MovedTemporarily", sf::Http::Response::MovedTemporarily)
            ._element("NotModified", sf::Http::Response::NotModified)
            ._element("BadRequest", sf::Http::Response::BadRequest)
            ._element("Unauthorized", sf::Http::Response::Unauthorized)
            ._element("Forbidden", sf::Http::Response::Forbidden)
            ._element("NotFound", sf::Http::Response::NotFound)
            ._element("InternalServerError", sf::Http::Response::InternalServerError)
            ._element("NotImplemented", sf::Http::Response::NotImplemented)
            ._element("BadGateway", sf::Http::Response::BadGateway)
            ._element("ServiceNotAvailable", sf::Http::Response::ServiceNotAvailable)
            ._element("InvalidResponse", sf::Http::Response::InvalidResponse)
            ._element("ConnectionFailed", sf::Http::Response::ConnectionFailed)
        ;
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
