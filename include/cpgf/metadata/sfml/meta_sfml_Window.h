// Auto generated file, don't modify.

#ifndef __META_SFML_WINDOW_H
#define __META_SFML_WINDOW_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindapi.h"


using namespace sf;
using namespace sf::priv;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Window(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (VideoMode, const std::string &, unsigned long, const WindowSettings &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(WindowSettings()))
        ._default(copyVariantFromCopyable(Style::Resize|Style::Close))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (WindowHandle, const WindowSettings &)>()
        ._default(copyVariantFromCopyable(WindowSettings()))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Create", _r), (void (D::ClassType::*) (VideoMode, const std::string &, unsigned long, const WindowSettings &))&D::ClassType::Create, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(WindowSettings()))
        ._default(copyVariantFromCopyable(Style::Resize|Style::Close))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Create", _r), (void (D::ClassType::*) (WindowHandle, const WindowSettings &))&D::ClassType::Create)
        ._default(copyVariantFromCopyable(WindowSettings()))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Close", _r), &D::ClassType::Close);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsOpened", _r), &D::ClassType::IsOpened);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetWidth", _r), &D::ClassType::GetWidth);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetHeight", _r), &D::ClassType::GetHeight);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSettings", _r), &D::ClassType::GetSettings);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetEvent", _r), &D::ClassType::GetEvent);
    _d.CPGF_MD_TEMPLATE _method(replaceName("UseVerticalSync", _r), &D::ClassType::UseVerticalSync);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ShowMouseCursor", _r), &D::ClassType::ShowMouseCursor);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetCursorPosition", _r), &D::ClassType::SetCursorPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetPosition", _r), &D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetSize", _r), &D::ClassType::SetSize);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Show", _r), &D::ClassType::Show);
    _d.CPGF_MD_TEMPLATE _method(replaceName("EnableKeyRepeat", _r), &D::ClassType::EnableKeyRepeat);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetIcon", _r), &D::ClassType::SetIcon);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetActive", _r), &D::ClassType::SetActive)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method(replaceName("Display", _r), &D::ClassType::Display);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetInput", _r), &D::ClassType::GetInput);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetFramerateLimit", _r), &D::ClassType::SetFramerateLimit);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFrameTime", _r), &D::ClassType::GetFrameTime);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetJoystickThreshold", _r), &D::ClassType::SetJoystickThreshold);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
