// Auto generated file, don't modify.

#ifndef __META_SFML_HTTP_H
#define __META_SFML_HTTP_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Http(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const std::string &, unsigned short)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetHost", _r), &D::ClassType::SetHost, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("SendRequest", _r), &D::ClassType::SendRequest)
        ._default(copyVariantFromCopyable(0.f))
    ;
    {
        GDefineMetaClass<Http::Request> _nd = GDefineMetaClass<Http::Request>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("Request");
        _nd.CPGF_MD_TEMPLATE _constructor<void * (Http::Request::Method, const std::string &, const std::string &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleCopyConstReference<2> >())
            ._default(copyVariantFromCopyable(""))
            ._default(copyVariantFromCopyable("/"))
            ._default(copyVariantFromCopyable(Http::Request::Get))
        ;
        _nd.CPGF_MD_TEMPLATE _method(replaceName("SetField", _r), &sf::Http::Request::SetField, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method(replaceName("SetMethod", _r), &sf::Http::Request::SetMethod);
        _nd.CPGF_MD_TEMPLATE _method(replaceName("SetURI", _r), &sf::Http::Request::SetURI, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _method(replaceName("SetHttpVersion", _r), &sf::Http::Request::SetHttpVersion);
        _nd.CPGF_MD_TEMPLATE _method(replaceName("SetBody", _r), &sf::Http::Request::SetBody, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _enum<typename sf::Http::Request::Method>(replaceName("Method", _r))
            ._element(replaceName("Get", _r), sf::Http::Request::Get)
            ._element(replaceName("Post", _r), sf::Http::Request::Post)
            ._element(replaceName("Head", _r), sf::Http::Request::Head)
        ;
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Http::Response> _nd = GDefineMetaClass<Http::Response>::declare("Response");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _method(replaceName("GetField", _r), &sf::Http::Response::GetField, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1>, cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _method(replaceName("GetStatus", _r), &sf::Http::Response::GetStatus);
        _nd.CPGF_MD_TEMPLATE _method(replaceName("GetMajorHttpVersion", _r), &sf::Http::Response::GetMajorHttpVersion);
        _nd.CPGF_MD_TEMPLATE _method(replaceName("GetMinorHttpVersion", _r), &sf::Http::Response::GetMinorHttpVersion);
        _nd.CPGF_MD_TEMPLATE _method(replaceName("GetBody", _r), &sf::Http::Response::GetBody, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
        _nd.CPGF_MD_TEMPLATE _enum<typename sf::Http::Response::Status>(replaceName("Status", _r))
            ._element(replaceName("Ok", _r), sf::Http::Response::Ok)
            ._element(replaceName("Created", _r), sf::Http::Response::Created)
            ._element(replaceName("Accepted", _r), sf::Http::Response::Accepted)
            ._element(replaceName("NoContent", _r), sf::Http::Response::NoContent)
            ._element(replaceName("MultipleChoices", _r), sf::Http::Response::MultipleChoices)
            ._element(replaceName("MovedPermanently", _r), sf::Http::Response::MovedPermanently)
            ._element(replaceName("MovedTemporarily", _r), sf::Http::Response::MovedTemporarily)
            ._element(replaceName("NotModified", _r), sf::Http::Response::NotModified)
            ._element(replaceName("BadRequest", _r), sf::Http::Response::BadRequest)
            ._element(replaceName("Unauthorized", _r), sf::Http::Response::Unauthorized)
            ._element(replaceName("Forbidden", _r), sf::Http::Response::Forbidden)
            ._element(replaceName("NotFound", _r), sf::Http::Response::NotFound)
            ._element(replaceName("InternalServerError", _r), sf::Http::Response::InternalServerError)
            ._element(replaceName("NotImplemented", _r), sf::Http::Response::NotImplemented)
            ._element(replaceName("BadGateway", _r), sf::Http::Response::BadGateway)
            ._element(replaceName("ServiceNotAvailable", _r), sf::Http::Response::ServiceNotAvailable)
            ._element(replaceName("InvalidResponse", _r), sf::Http::Response::InvalidResponse)
            ._element(replaceName("ConnectionFailed", _r), sf::Http::Response::ConnectionFailed)
        ;
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
