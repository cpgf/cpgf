// Auto generated file, don't modify.

#ifndef __META_SFML_EVENT_H
#define __META_SFML_EVENT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::Key;
using namespace sf::Mouse;
using namespace sf::Joy;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_event(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<Axis>(replaceName("Axis", _r))
        ._element(replaceName("AxisX", _r), sf::Joy::AxisX)
        ._element(replaceName("AxisY", _r), sf::Joy::AxisY)
        ._element(replaceName("AxisZ", _r), sf::Joy::AxisZ)
        ._element(replaceName("AxisR", _r), sf::Joy::AxisR)
        ._element(replaceName("AxisU", _r), sf::Joy::AxisU)
        ._element(replaceName("AxisV", _r), sf::Joy::AxisV)
        ._element(replaceName("AxisPOV", _r), sf::Joy::AxisPOV)
        ._element(replaceName("AxisCount", _r), sf::Joy::AxisCount)
    ;
    _d.CPGF_MD_TEMPLATE _enum<long long>(replaceName("GlobalEnum_sfml_2", _r))
        ._element(replaceName("Count", _r), sf::Joy::Count)
        ._element(replaceName("ButtonCount", _r), sf::Joy::ButtonCount)
    ;
    _d.CPGF_MD_TEMPLATE _enum<Code>(replaceName("Code", _r))
        ._element(replaceName("A", _r), sf::Key::A)
        ._element(replaceName("B", _r), sf::Key::B)
        ._element(replaceName("C", _r), sf::Key::C)
        ._element(replaceName("D", _r), sf::Key::D)
        ._element(replaceName("E", _r), sf::Key::E)
        ._element(replaceName("F", _r), sf::Key::F)
        ._element(replaceName("G", _r), sf::Key::G)
        ._element(replaceName("H", _r), sf::Key::H)
        ._element(replaceName("I", _r), sf::Key::I)
        ._element(replaceName("J", _r), sf::Key::J)
        ._element(replaceName("K", _r), sf::Key::K)
        ._element(replaceName("L", _r), sf::Key::L)
        ._element(replaceName("M", _r), sf::Key::M)
        ._element(replaceName("N", _r), sf::Key::N)
        ._element(replaceName("O", _r), sf::Key::O)
        ._element(replaceName("P", _r), sf::Key::P)
        ._element(replaceName("Q", _r), sf::Key::Q)
        ._element(replaceName("R", _r), sf::Key::R)
        ._element(replaceName("S", _r), sf::Key::S)
        ._element(replaceName("T", _r), sf::Key::T)
        ._element(replaceName("U", _r), sf::Key::U)
        ._element(replaceName("V", _r), sf::Key::V)
        ._element(replaceName("W", _r), sf::Key::W)
        ._element(replaceName("X", _r), sf::Key::X)
        ._element(replaceName("Y", _r), sf::Key::Y)
        ._element(replaceName("Z", _r), sf::Key::Z)
        ._element(replaceName("Num0", _r), sf::Key::Num0)
        ._element(replaceName("Num1", _r), sf::Key::Num1)
        ._element(replaceName("Num2", _r), sf::Key::Num2)
        ._element(replaceName("Num3", _r), sf::Key::Num3)
        ._element(replaceName("Num4", _r), sf::Key::Num4)
        ._element(replaceName("Num5", _r), sf::Key::Num5)
        ._element(replaceName("Num6", _r), sf::Key::Num6)
        ._element(replaceName("Num7", _r), sf::Key::Num7)
        ._element(replaceName("Num8", _r), sf::Key::Num8)
        ._element(replaceName("Num9", _r), sf::Key::Num9)
        ._element(replaceName("Escape", _r), sf::Key::Escape)
        ._element(replaceName("LControl", _r), sf::Key::LControl)
        ._element(replaceName("LShift", _r), sf::Key::LShift)
        ._element(replaceName("LAlt", _r), sf::Key::LAlt)
        ._element(replaceName("LSystem", _r), sf::Key::LSystem)
        ._element(replaceName("RControl", _r), sf::Key::RControl)
        ._element(replaceName("RShift", _r), sf::Key::RShift)
        ._element(replaceName("RAlt", _r), sf::Key::RAlt)
        ._element(replaceName("RSystem", _r), sf::Key::RSystem)
        ._element(replaceName("Menu", _r), sf::Key::Menu)
        ._element(replaceName("LBracket", _r), sf::Key::LBracket)
        ._element(replaceName("RBracket", _r), sf::Key::RBracket)
        ._element(replaceName("SemiColon", _r), sf::Key::SemiColon)
        ._element(replaceName("Comma", _r), sf::Key::Comma)
        ._element(replaceName("Period", _r), sf::Key::Period)
        ._element(replaceName("Quote", _r), sf::Key::Quote)
        ._element(replaceName("Slash", _r), sf::Key::Slash)
        ._element(replaceName("BackSlash", _r), sf::Key::BackSlash)
        ._element(replaceName("Tilde", _r), sf::Key::Tilde)
        ._element(replaceName("Equal", _r), sf::Key::Equal)
        ._element(replaceName("Dash", _r), sf::Key::Dash)
        ._element(replaceName("Space", _r), sf::Key::Space)
        ._element(replaceName("Return", _r), sf::Key::Return)
        ._element(replaceName("Back", _r), sf::Key::Back)
        ._element(replaceName("Tab", _r), sf::Key::Tab)
        ._element(replaceName("PageUp", _r), sf::Key::PageUp)
        ._element(replaceName("PageDown", _r), sf::Key::PageDown)
        ._element(replaceName("End", _r), sf::Key::End)
        ._element(replaceName("Home", _r), sf::Key::Home)
        ._element(replaceName("Insert", _r), sf::Key::Insert)
        ._element(replaceName("Delete", _r), sf::Key::Delete)
        ._element(replaceName("Add", _r), sf::Key::Add)
        ._element(replaceName("Subtract", _r), sf::Key::Subtract)
        ._element(replaceName("Multiply", _r), sf::Key::Multiply)
        ._element(replaceName("Divide", _r), sf::Key::Divide)
        ._element(replaceName("Left", _r), sf::Key::Left)
        ._element(replaceName("Right", _r), sf::Key::Right)
        ._element(replaceName("Up", _r), sf::Key::Up)
        ._element(replaceName("Down", _r), sf::Key::Down)
        ._element(replaceName("Numpad0", _r), sf::Key::Numpad0)
        ._element(replaceName("Numpad1", _r), sf::Key::Numpad1)
        ._element(replaceName("Numpad2", _r), sf::Key::Numpad2)
        ._element(replaceName("Numpad3", _r), sf::Key::Numpad3)
        ._element(replaceName("Numpad4", _r), sf::Key::Numpad4)
        ._element(replaceName("Numpad5", _r), sf::Key::Numpad5)
        ._element(replaceName("Numpad6", _r), sf::Key::Numpad6)
        ._element(replaceName("Numpad7", _r), sf::Key::Numpad7)
        ._element(replaceName("Numpad8", _r), sf::Key::Numpad8)
        ._element(replaceName("Numpad9", _r), sf::Key::Numpad9)
        ._element(replaceName("F1", _r), sf::Key::F1)
        ._element(replaceName("F2", _r), sf::Key::F2)
        ._element(replaceName("F3", _r), sf::Key::F3)
        ._element(replaceName("F4", _r), sf::Key::F4)
        ._element(replaceName("F5", _r), sf::Key::F5)
        ._element(replaceName("F6", _r), sf::Key::F6)
        ._element(replaceName("F7", _r), sf::Key::F7)
        ._element(replaceName("F8", _r), sf::Key::F8)
        ._element(replaceName("F9", _r), sf::Key::F9)
        ._element(replaceName("F10", _r), sf::Key::F10)
        ._element(replaceName("F11", _r), sf::Key::F11)
        ._element(replaceName("F12", _r), sf::Key::F12)
        ._element(replaceName("F13", _r), sf::Key::F13)
        ._element(replaceName("F14", _r), sf::Key::F14)
        ._element(replaceName("F15", _r), sf::Key::F15)
        ._element(replaceName("Pause", _r), sf::Key::Pause)
        ._element(replaceName("Count", _r), sf::Key::Count)
    ;
    _d.CPGF_MD_TEMPLATE _enum<Button>(replaceName("Button", _r))
        ._element(replaceName("Left", _r), sf::Mouse::Left)
        ._element(replaceName("Right", _r), sf::Mouse::Right)
        ._element(replaceName("Middle", _r), sf::Mouse::Middle)
        ._element(replaceName("XButton1", _r), sf::Mouse::XButton1)
        ._element(replaceName("XButton2", _r), sf::Mouse::XButton2)
        ._element(replaceName("ButtonCount", _r), sf::Mouse::ButtonCount)
    ;
}


template <typename D>
void buildMetaClass_Event(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("Type", _r), &D::ClassType::Type);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Key", _r), &D::ClassType::Key);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Text", _r), &D::ClassType::Text);
    _d.CPGF_MD_TEMPLATE _field(replaceName("MouseMove", _r), &D::ClassType::MouseMove);
    _d.CPGF_MD_TEMPLATE _field(replaceName("MouseButton", _r), &D::ClassType::MouseButton);
    _d.CPGF_MD_TEMPLATE _field(replaceName("MouseWheel", _r), &D::ClassType::MouseWheel);
    _d.CPGF_MD_TEMPLATE _field(replaceName("JoyMove", _r), &D::ClassType::JoyMove);
    _d.CPGF_MD_TEMPLATE _field(replaceName("JoyButton", _r), &D::ClassType::JoyButton);
    _d.CPGF_MD_TEMPLATE _field(replaceName("Size", _r), &D::ClassType::Size);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::EventType>(replaceName("EventType", _r))
        ._element(replaceName("Closed", _r), D::ClassType::Closed)
        ._element(replaceName("Resized", _r), D::ClassType::Resized)
        ._element(replaceName("LostFocus", _r), D::ClassType::LostFocus)
        ._element(replaceName("GainedFocus", _r), D::ClassType::GainedFocus)
        ._element(replaceName("TextEntered", _r), D::ClassType::TextEntered)
        ._element(replaceName("KeyPressed", _r), D::ClassType::KeyPressed)
        ._element(replaceName("KeyReleased", _r), D::ClassType::KeyReleased)
        ._element(replaceName("MouseWheelMoved", _r), D::ClassType::MouseWheelMoved)
        ._element(replaceName("MouseButtonPressed", _r), D::ClassType::MouseButtonPressed)
        ._element(replaceName("MouseButtonReleased", _r), D::ClassType::MouseButtonReleased)
        ._element(replaceName("MouseMoved", _r), D::ClassType::MouseMoved)
        ._element(replaceName("MouseEntered", _r), D::ClassType::MouseEntered)
        ._element(replaceName("MouseLeft", _r), D::ClassType::MouseLeft)
        ._element(replaceName("JoyButtonPressed", _r), D::ClassType::JoyButtonPressed)
        ._element(replaceName("JoyButtonReleased", _r), D::ClassType::JoyButtonReleased)
        ._element(replaceName("JoyMoved", _r), D::ClassType::JoyMoved)
        ._element(replaceName("Count", _r), D::ClassType::Count)
    ;
    {
        GDefineMetaClass<Event::JoyButtonEvent> _nd = GDefineMetaClass<Event::JoyButtonEvent>::declare("JoyButtonEvent");
        _nd.CPGF_MD_TEMPLATE _field(replaceName("JoystickId", _r), &sf::Event::JoyButtonEvent::JoystickId);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Button", _r), &sf::Event::JoyButtonEvent::Button);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::JoyMoveEvent> _nd = GDefineMetaClass<Event::JoyMoveEvent>::declare("JoyMoveEvent");
        _nd.CPGF_MD_TEMPLATE _field(replaceName("JoystickId", _r), &sf::Event::JoyMoveEvent::JoystickId);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Axis", _r), &sf::Event::JoyMoveEvent::Axis);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Position", _r), &sf::Event::JoyMoveEvent::Position);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::KeyEvent> _nd = GDefineMetaClass<Event::KeyEvent>::declare("KeyEvent");
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Code", _r), &sf::Event::KeyEvent::Code);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Alt", _r), &sf::Event::KeyEvent::Alt);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Control", _r), &sf::Event::KeyEvent::Control);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Shift", _r), &sf::Event::KeyEvent::Shift);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::MouseButtonEvent> _nd = GDefineMetaClass<Event::MouseButtonEvent>::declare("MouseButtonEvent");
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Button", _r), &sf::Event::MouseButtonEvent::Button);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("X", _r), &sf::Event::MouseButtonEvent::X);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Y", _r), &sf::Event::MouseButtonEvent::Y);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::MouseMoveEvent> _nd = GDefineMetaClass<Event::MouseMoveEvent>::declare("MouseMoveEvent");
        _nd.CPGF_MD_TEMPLATE _field(replaceName("X", _r), &sf::Event::MouseMoveEvent::X);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Y", _r), &sf::Event::MouseMoveEvent::Y);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::MouseWheelEvent> _nd = GDefineMetaClass<Event::MouseWheelEvent>::declare("MouseWheelEvent");
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Delta", _r), &sf::Event::MouseWheelEvent::Delta);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::SizeEvent> _nd = GDefineMetaClass<Event::SizeEvent>::declare("SizeEvent");
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Width", _r), &sf::Event::SizeEvent::Width);
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Height", _r), &sf::Event::SizeEvent::Height);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::TextEvent> _nd = GDefineMetaClass<Event::TextEvent>::declare("TextEvent");
        _nd.CPGF_MD_TEMPLATE _field(replaceName("Unicode", _r), &sf::Event::TextEvent::Unicode);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
