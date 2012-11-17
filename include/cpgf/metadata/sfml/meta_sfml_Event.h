// Auto generated file, don't modify.

#ifndef __META_SFML_EVENT_H
#define __META_SFML_EVENT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace sf;
using namespace sf::Key;
using namespace sf::Mouse;
using namespace sf::Joy;


namespace meta_sfml { 


template <typename D>
void buildMetaClass_Global_event(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<Axis>("Axis")
        ._element("AxisX", sf::Joy::AxisX)
        ._element("AxisY", sf::Joy::AxisY)
        ._element("AxisZ", sf::Joy::AxisZ)
        ._element("AxisR", sf::Joy::AxisR)
        ._element("AxisU", sf::Joy::AxisU)
        ._element("AxisV", sf::Joy::AxisV)
        ._element("AxisPOV", sf::Joy::AxisPOV)
        ._element("AxisCount", sf::Joy::AxisCount)
    ;
    _d.CPGF_MD_TEMPLATE _enum<long long>("GlobalEnum_sfml_2")
        ._element("Count", sf::Joy::Count)
        ._element("ButtonCount", sf::Joy::ButtonCount)
    ;
    _d.CPGF_MD_TEMPLATE _enum<Code>("Code")
        ._element("A", sf::Key::A)
        ._element("B", sf::Key::B)
        ._element("C", sf::Key::C)
        ._element("D", sf::Key::D)
        ._element("E", sf::Key::E)
        ._element("F", sf::Key::F)
        ._element("G", sf::Key::G)
        ._element("H", sf::Key::H)
        ._element("I", sf::Key::I)
        ._element("J", sf::Key::J)
        ._element("K", sf::Key::K)
        ._element("L", sf::Key::L)
        ._element("M", sf::Key::M)
        ._element("N", sf::Key::N)
        ._element("O", sf::Key::O)
        ._element("P", sf::Key::P)
        ._element("Q", sf::Key::Q)
        ._element("R", sf::Key::R)
        ._element("S", sf::Key::S)
        ._element("T", sf::Key::T)
        ._element("U", sf::Key::U)
        ._element("V", sf::Key::V)
        ._element("W", sf::Key::W)
        ._element("X", sf::Key::X)
        ._element("Y", sf::Key::Y)
        ._element("Z", sf::Key::Z)
        ._element("Num0", sf::Key::Num0)
        ._element("Num1", sf::Key::Num1)
        ._element("Num2", sf::Key::Num2)
        ._element("Num3", sf::Key::Num3)
        ._element("Num4", sf::Key::Num4)
        ._element("Num5", sf::Key::Num5)
        ._element("Num6", sf::Key::Num6)
        ._element("Num7", sf::Key::Num7)
        ._element("Num8", sf::Key::Num8)
        ._element("Num9", sf::Key::Num9)
        ._element("Escape", sf::Key::Escape)
        ._element("LControl", sf::Key::LControl)
        ._element("LShift", sf::Key::LShift)
        ._element("LAlt", sf::Key::LAlt)
        ._element("LSystem", sf::Key::LSystem)
        ._element("RControl", sf::Key::RControl)
        ._element("RShift", sf::Key::RShift)
        ._element("RAlt", sf::Key::RAlt)
        ._element("RSystem", sf::Key::RSystem)
        ._element("Menu", sf::Key::Menu)
        ._element("LBracket", sf::Key::LBracket)
        ._element("RBracket", sf::Key::RBracket)
        ._element("SemiColon", sf::Key::SemiColon)
        ._element("Comma", sf::Key::Comma)
        ._element("Period", sf::Key::Period)
        ._element("Quote", sf::Key::Quote)
        ._element("Slash", sf::Key::Slash)
        ._element("BackSlash", sf::Key::BackSlash)
        ._element("Tilde", sf::Key::Tilde)
        ._element("Equal", sf::Key::Equal)
        ._element("Dash", sf::Key::Dash)
        ._element("Space", sf::Key::Space)
        ._element("Return", sf::Key::Return)
        ._element("Back", sf::Key::Back)
        ._element("Tab", sf::Key::Tab)
        ._element("PageUp", sf::Key::PageUp)
        ._element("PageDown", sf::Key::PageDown)
        ._element("End", sf::Key::End)
        ._element("Home", sf::Key::Home)
        ._element("Insert", sf::Key::Insert)
        ._element("Delete", sf::Key::Delete)
        ._element("Add", sf::Key::Add)
        ._element("Subtract", sf::Key::Subtract)
        ._element("Multiply", sf::Key::Multiply)
        ._element("Divide", sf::Key::Divide)
        ._element("Left", sf::Key::Left)
        ._element("Right", sf::Key::Right)
        ._element("Up", sf::Key::Up)
        ._element("Down", sf::Key::Down)
        ._element("Numpad0", sf::Key::Numpad0)
        ._element("Numpad1", sf::Key::Numpad1)
        ._element("Numpad2", sf::Key::Numpad2)
        ._element("Numpad3", sf::Key::Numpad3)
        ._element("Numpad4", sf::Key::Numpad4)
        ._element("Numpad5", sf::Key::Numpad5)
        ._element("Numpad6", sf::Key::Numpad6)
        ._element("Numpad7", sf::Key::Numpad7)
        ._element("Numpad8", sf::Key::Numpad8)
        ._element("Numpad9", sf::Key::Numpad9)
        ._element("F1", sf::Key::F1)
        ._element("F2", sf::Key::F2)
        ._element("F3", sf::Key::F3)
        ._element("F4", sf::Key::F4)
        ._element("F5", sf::Key::F5)
        ._element("F6", sf::Key::F6)
        ._element("F7", sf::Key::F7)
        ._element("F8", sf::Key::F8)
        ._element("F9", sf::Key::F9)
        ._element("F10", sf::Key::F10)
        ._element("F11", sf::Key::F11)
        ._element("F12", sf::Key::F12)
        ._element("F13", sf::Key::F13)
        ._element("F14", sf::Key::F14)
        ._element("F15", sf::Key::F15)
        ._element("Pause", sf::Key::Pause)
        ._element("Count", sf::Key::Count)
    ;
    _d.CPGF_MD_TEMPLATE _enum<Button>("Button")
        ._element("Left", sf::Mouse::Left)
        ._element("Right", sf::Mouse::Right)
        ._element("Middle", sf::Mouse::Middle)
        ._element("XButton1", sf::Mouse::XButton1)
        ._element("XButton2", sf::Mouse::XButton2)
        ._element("ButtonCount", sf::Mouse::ButtonCount)
    ;
}


template <typename D>
void buildMetaClass_Event(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("Type", &D::ClassType::Type);
    _d.CPGF_MD_TEMPLATE _field("Key", &D::ClassType::Key);
    _d.CPGF_MD_TEMPLATE _field("Text", &D::ClassType::Text);
    _d.CPGF_MD_TEMPLATE _field("MouseMove", &D::ClassType::MouseMove);
    _d.CPGF_MD_TEMPLATE _field("MouseButton", &D::ClassType::MouseButton);
    _d.CPGF_MD_TEMPLATE _field("MouseWheel", &D::ClassType::MouseWheel);
    _d.CPGF_MD_TEMPLATE _field("JoyMove", &D::ClassType::JoyMove);
    _d.CPGF_MD_TEMPLATE _field("JoyButton", &D::ClassType::JoyButton);
    _d.CPGF_MD_TEMPLATE _field("Size", &D::ClassType::Size);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::EventType>("EventType")
        ._element("Closed", D::ClassType::Closed)
        ._element("Resized", D::ClassType::Resized)
        ._element("LostFocus", D::ClassType::LostFocus)
        ._element("GainedFocus", D::ClassType::GainedFocus)
        ._element("TextEntered", D::ClassType::TextEntered)
        ._element("KeyPressed", D::ClassType::KeyPressed)
        ._element("KeyReleased", D::ClassType::KeyReleased)
        ._element("MouseWheelMoved", D::ClassType::MouseWheelMoved)
        ._element("MouseButtonPressed", D::ClassType::MouseButtonPressed)
        ._element("MouseButtonReleased", D::ClassType::MouseButtonReleased)
        ._element("MouseMoved", D::ClassType::MouseMoved)
        ._element("MouseEntered", D::ClassType::MouseEntered)
        ._element("MouseLeft", D::ClassType::MouseLeft)
        ._element("JoyButtonPressed", D::ClassType::JoyButtonPressed)
        ._element("JoyButtonReleased", D::ClassType::JoyButtonReleased)
        ._element("JoyMoved", D::ClassType::JoyMoved)
        ._element("Count", D::ClassType::Count)
    ;
    {
        GDefineMetaClass<Event::JoyButtonEvent> _nd = GDefineMetaClass<Event::JoyButtonEvent>::declare("JoyButtonEvent");
        _nd.CPGF_MD_TEMPLATE _field("JoystickId", &Event::JoyButtonEvent::JoystickId);
        _nd.CPGF_MD_TEMPLATE _field("Button", &Event::JoyButtonEvent::Button);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::JoyMoveEvent> _nd = GDefineMetaClass<Event::JoyMoveEvent>::declare("JoyMoveEvent");
        _nd.CPGF_MD_TEMPLATE _field("JoystickId", &Event::JoyMoveEvent::JoystickId);
        _nd.CPGF_MD_TEMPLATE _field("Axis", &Event::JoyMoveEvent::Axis);
        _nd.CPGF_MD_TEMPLATE _field("Position", &Event::JoyMoveEvent::Position);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::KeyEvent> _nd = GDefineMetaClass<Event::KeyEvent>::declare("KeyEvent");
        _nd.CPGF_MD_TEMPLATE _field("Code", &Event::KeyEvent::Code);
        _nd.CPGF_MD_TEMPLATE _field("Alt", &Event::KeyEvent::Alt);
        _nd.CPGF_MD_TEMPLATE _field("Control", &Event::KeyEvent::Control);
        _nd.CPGF_MD_TEMPLATE _field("Shift", &Event::KeyEvent::Shift);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::MouseButtonEvent> _nd = GDefineMetaClass<Event::MouseButtonEvent>::declare("MouseButtonEvent");
        _nd.CPGF_MD_TEMPLATE _field("Button", &Event::MouseButtonEvent::Button);
        _nd.CPGF_MD_TEMPLATE _field("X", &Event::MouseButtonEvent::X);
        _nd.CPGF_MD_TEMPLATE _field("Y", &Event::MouseButtonEvent::Y);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::MouseMoveEvent> _nd = GDefineMetaClass<Event::MouseMoveEvent>::declare("MouseMoveEvent");
        _nd.CPGF_MD_TEMPLATE _field("X", &Event::MouseMoveEvent::X);
        _nd.CPGF_MD_TEMPLATE _field("Y", &Event::MouseMoveEvent::Y);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::MouseWheelEvent> _nd = GDefineMetaClass<Event::MouseWheelEvent>::declare("MouseWheelEvent");
        _nd.CPGF_MD_TEMPLATE _field("Delta", &Event::MouseWheelEvent::Delta);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::SizeEvent> _nd = GDefineMetaClass<Event::SizeEvent>::declare("SizeEvent");
        _nd.CPGF_MD_TEMPLATE _field("Width", &Event::SizeEvent::Width);
        _nd.CPGF_MD_TEMPLATE _field("Height", &Event::SizeEvent::Height);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<Event::TextEvent> _nd = GDefineMetaClass<Event::TextEvent>::declare("TextEvent");
        _nd.CPGF_MD_TEMPLATE _field("Unicode", &Event::TextEvent::Unicode);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_sfml




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
