// Auto generated file, don't modify.

#ifndef __META_SFML_WINDOW_H
#define __META_SFML_WINDOW_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::priv;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Window(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (VideoMode, const std::string &, unsigned long, const WindowSettings &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(WindowSettings()))
        ._default(copyVariantFromCopyable(Style::Resize|Style::Close))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (WindowHandle, const WindowSettings &)>()
        ._default(copyVariantFromCopyable(WindowSettings()))
    ;
    _d.CPGF_MD_TEMPLATE _method("Create", (void (D::ClassType::*) (VideoMode, const std::string &, unsigned long, const WindowSettings &))&D::ClassType::Create, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(WindowSettings()))
        ._default(copyVariantFromCopyable(Style::Resize|Style::Close))
    ;
    _d.CPGF_MD_TEMPLATE _method("Create", (void (D::ClassType::*) (WindowHandle, const WindowSettings &))&D::ClassType::Create)
        ._default(copyVariantFromCopyable(WindowSettings()))
    ;
    _d.CPGF_MD_TEMPLATE _method("Close", &D::ClassType::Close);
    _d.CPGF_MD_TEMPLATE _method("IsOpened", &D::ClassType::IsOpened);
    _d.CPGF_MD_TEMPLATE _method("GetWidth", &D::ClassType::GetWidth);
    _d.CPGF_MD_TEMPLATE _method("GetHeight", &D::ClassType::GetHeight);
    _d.CPGF_MD_TEMPLATE _method("GetSettings", &D::ClassType::GetSettings);
    _d.CPGF_MD_TEMPLATE _method("GetEvent", &D::ClassType::GetEvent);
    _d.CPGF_MD_TEMPLATE _method("UseVerticalSync", &D::ClassType::UseVerticalSync);
    _d.CPGF_MD_TEMPLATE _method("ShowMouseCursor", &D::ClassType::ShowMouseCursor);
    _d.CPGF_MD_TEMPLATE _method("SetCursorPosition", &D::ClassType::SetCursorPosition);
    _d.CPGF_MD_TEMPLATE _method("SetPosition", &D::ClassType::SetPosition);
    _d.CPGF_MD_TEMPLATE _method("SetSize", &D::ClassType::SetSize);
    _d.CPGF_MD_TEMPLATE _method("Show", &D::ClassType::Show);
    _d.CPGF_MD_TEMPLATE _method("EnableKeyRepeat", &D::ClassType::EnableKeyRepeat);
    _d.CPGF_MD_TEMPLATE _method("SetIcon", &D::ClassType::SetIcon);
    _d.CPGF_MD_TEMPLATE _method("SetActive", &D::ClassType::SetActive)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("Display", &D::ClassType::Display);
    _d.CPGF_MD_TEMPLATE _method("GetInput", &D::ClassType::GetInput);
    _d.CPGF_MD_TEMPLATE _method("SetFramerateLimit", &D::ClassType::SetFramerateLimit);
    _d.CPGF_MD_TEMPLATE _method("GetFrameTime", &D::ClassType::GetFrameTime);
    _d.CPGF_MD_TEMPLATE _method("SetJoystickThreshold", &D::ClassType::SetJoystickThreshold);
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
