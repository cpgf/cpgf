// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IEVENTRECEIVER_H
#define __META_IRRLICHT_IEVENTRECEIVER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gscopedinterface.h"


using namespace irr;
using namespace irr::gui;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ieventreceiver(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<EEVENT_TYPE>("EEVENT_TYPE")
        ._element("EET_GUI_EVENT", irr::EET_GUI_EVENT)
        ._element("EET_MOUSE_INPUT_EVENT", irr::EET_MOUSE_INPUT_EVENT)
        ._element("EET_KEY_INPUT_EVENT", irr::EET_KEY_INPUT_EVENT)
        ._element("EET_JOYSTICK_INPUT_EVENT", irr::EET_JOYSTICK_INPUT_EVENT)
        ._element("EET_LOG_TEXT_EVENT", irr::EET_LOG_TEXT_EVENT)
        ._element("EET_USER_EVENT", irr::EET_USER_EVENT)
        ._element("EGUIET_FORCE_32_BIT", irr::EGUIET_FORCE_32_BIT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EMOUSE_INPUT_EVENT>("EMOUSE_INPUT_EVENT")
        ._element("EMIE_LMOUSE_PRESSED_DOWN", irr::EMIE_LMOUSE_PRESSED_DOWN)
        ._element("EMIE_RMOUSE_PRESSED_DOWN", irr::EMIE_RMOUSE_PRESSED_DOWN)
        ._element("EMIE_MMOUSE_PRESSED_DOWN", irr::EMIE_MMOUSE_PRESSED_DOWN)
        ._element("EMIE_LMOUSE_LEFT_UP", irr::EMIE_LMOUSE_LEFT_UP)
        ._element("EMIE_RMOUSE_LEFT_UP", irr::EMIE_RMOUSE_LEFT_UP)
        ._element("EMIE_MMOUSE_LEFT_UP", irr::EMIE_MMOUSE_LEFT_UP)
        ._element("EMIE_MOUSE_MOVED", irr::EMIE_MOUSE_MOVED)
        ._element("EMIE_MOUSE_WHEEL", irr::EMIE_MOUSE_WHEEL)
        ._element("EMIE_LMOUSE_DOUBLE_CLICK", irr::EMIE_LMOUSE_DOUBLE_CLICK)
        ._element("EMIE_RMOUSE_DOUBLE_CLICK", irr::EMIE_RMOUSE_DOUBLE_CLICK)
        ._element("EMIE_MMOUSE_DOUBLE_CLICK", irr::EMIE_MMOUSE_DOUBLE_CLICK)
        ._element("EMIE_LMOUSE_TRIPLE_CLICK", irr::EMIE_LMOUSE_TRIPLE_CLICK)
        ._element("EMIE_RMOUSE_TRIPLE_CLICK", irr::EMIE_RMOUSE_TRIPLE_CLICK)
        ._element("EMIE_MMOUSE_TRIPLE_CLICK", irr::EMIE_MMOUSE_TRIPLE_CLICK)
        ._element("EMIE_COUNT", irr::EMIE_COUNT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_MOUSE_BUTTON_STATE_MASK>("E_MOUSE_BUTTON_STATE_MASK")
        ._element("EMBSM_LEFT", irr::EMBSM_LEFT)
        ._element("EMBSM_RIGHT", irr::EMBSM_RIGHT)
        ._element("EMBSM_MIDDLE", irr::EMBSM_MIDDLE)
        ._element("EMBSM_EXTRA1", irr::EMBSM_EXTRA1)
        ._element("EMBSM_EXTRA2", irr::EMBSM_EXTRA2)
        ._element("EMBSM_FORCE_32_BIT", irr::EMBSM_FORCE_32_BIT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<EGUI_EVENT_TYPE>("EGUI_EVENT_TYPE")
        ._element("EGET_ELEMENT_FOCUS_LOST", irr::gui::EGET_ELEMENT_FOCUS_LOST)
        ._element("EGET_ELEMENT_FOCUSED", irr::gui::EGET_ELEMENT_FOCUSED)
        ._element("EGET_ELEMENT_HOVERED", irr::gui::EGET_ELEMENT_HOVERED)
        ._element("EGET_ELEMENT_LEFT", irr::gui::EGET_ELEMENT_LEFT)
        ._element("EGET_ELEMENT_CLOSED", irr::gui::EGET_ELEMENT_CLOSED)
        ._element("EGET_BUTTON_CLICKED", irr::gui::EGET_BUTTON_CLICKED)
        ._element("EGET_SCROLL_BAR_CHANGED", irr::gui::EGET_SCROLL_BAR_CHANGED)
        ._element("EGET_CHECKBOX_CHANGED", irr::gui::EGET_CHECKBOX_CHANGED)
        ._element("EGET_LISTBOX_CHANGED", irr::gui::EGET_LISTBOX_CHANGED)
        ._element("EGET_LISTBOX_SELECTED_AGAIN", irr::gui::EGET_LISTBOX_SELECTED_AGAIN)
        ._element("EGET_FILE_SELECTED", irr::gui::EGET_FILE_SELECTED)
        ._element("EGET_DIRECTORY_SELECTED", irr::gui::EGET_DIRECTORY_SELECTED)
        ._element("EGET_FILE_CHOOSE_DIALOG_CANCELLED", irr::gui::EGET_FILE_CHOOSE_DIALOG_CANCELLED)
        ._element("EGET_MESSAGEBOX_YES", irr::gui::EGET_MESSAGEBOX_YES)
        ._element("EGET_MESSAGEBOX_NO", irr::gui::EGET_MESSAGEBOX_NO)
        ._element("EGET_MESSAGEBOX_OK", irr::gui::EGET_MESSAGEBOX_OK)
        ._element("EGET_MESSAGEBOX_CANCEL", irr::gui::EGET_MESSAGEBOX_CANCEL)
        ._element("EGET_EDITBOX_ENTER", irr::gui::EGET_EDITBOX_ENTER)
        ._element("EGET_EDITBOX_CHANGED", irr::gui::EGET_EDITBOX_CHANGED)
        ._element("EGET_EDITBOX_MARKING_CHANGED", irr::gui::EGET_EDITBOX_MARKING_CHANGED)
        ._element("EGET_TAB_CHANGED", irr::gui::EGET_TAB_CHANGED)
        ._element("EGET_MENU_ITEM_SELECTED", irr::gui::EGET_MENU_ITEM_SELECTED)
        ._element("EGET_COMBO_BOX_CHANGED", irr::gui::EGET_COMBO_BOX_CHANGED)
        ._element("EGET_SPINBOX_CHANGED", irr::gui::EGET_SPINBOX_CHANGED)
        ._element("EGET_TABLE_CHANGED", irr::gui::EGET_TABLE_CHANGED)
        ._element("EGET_TABLE_HEADER_CHANGED", irr::gui::EGET_TABLE_HEADER_CHANGED)
        ._element("EGET_TABLE_SELECTED_AGAIN", irr::gui::EGET_TABLE_SELECTED_AGAIN)
        ._element("EGET_TREEVIEW_NODE_DESELECT", irr::gui::EGET_TREEVIEW_NODE_DESELECT)
        ._element("EGET_TREEVIEW_NODE_SELECT", irr::gui::EGET_TREEVIEW_NODE_SELECT)
        ._element("EGET_TREEVIEW_NODE_EXPAND", irr::gui::EGET_TREEVIEW_NODE_EXPAND)
        ._element("EGET_TREEVIEW_NODE_COLLAPSE", irr::gui::EGET_TREEVIEW_NODE_COLLAPSE)
        ._element("EGET_TREEVIEW_NODE_COLLAPS", irr::gui::EGET_TREEVIEW_NODE_COLLAPS)
        ._element("EGET_COUNT", irr::gui::EGET_COUNT)
    ;
}


template <typename D>
void buildMetaClass_IEventReceiver(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("OnEvent", &D::ClassType::OnEvent);
}


class IEventReceiverWrapper : public irr::IEventReceiver, public cpgf::GScriptWrapper {
public:
    
    bool OnEvent(const SEvent & event)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnEvent"));
        if(func)
        {
            return cpgf::fromVariant<bool >(cpgf::invokeScriptFunction(func.get(), this, event));
        }
        throw "Abstract method";
    }
    bool super_OnEvent(const SEvent & event)
    {
        throw "Abstract method";
    }
};


template <typename D>
void buildMetaClass_IEventReceiverWrapper(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    
    _d.CPGF_MD_TEMPLATE _method("super_OnEvent", (bool (D::ClassType::*) (const SEvent &))&D::ClassType::super_OnEvent);
    
    buildMetaClass_IEventReceiver<D>(config, _d);
}


inline bool bItFiEldWrapper_SEvent_SKeyInput_PressedDown_getter(SEvent::SKeyInput * self) {
    return self->PressedDown;
}

inline void bItFiEldWrapper_SEvent_SKeyInput_PressedDown_setter(SEvent::SKeyInput * self, bool value) {
    self->PressedDown = value;
}
inline bool bItFiEldWrapper_SEvent_SKeyInput_Shift_getter(SEvent::SKeyInput * self) {
    return self->Shift;
}

inline void bItFiEldWrapper_SEvent_SKeyInput_Shift_setter(SEvent::SKeyInput * self, bool value) {
    self->Shift = value;
}
inline bool bItFiEldWrapper_SEvent_SKeyInput_Control_getter(SEvent::SKeyInput * self) {
    return self->Control;
}

inline void bItFiEldWrapper_SEvent_SKeyInput_Control_setter(SEvent::SKeyInput * self, bool value) {
    self->Control = value;
}


inline bool bItFiEldWrapper_SEvent_SMouseInput_Shift_getter(SEvent::SMouseInput * self) {
    return self->Shift;
}

inline void bItFiEldWrapper_SEvent_SMouseInput_Shift_setter(SEvent::SMouseInput * self, bool value) {
    self->Shift = value;
}
inline bool bItFiEldWrapper_SEvent_SMouseInput_Control_getter(SEvent::SMouseInput * self) {
    return self->Control;
}

inline void bItFiEldWrapper_SEvent_SMouseInput_Control_setter(SEvent::SMouseInput * self, bool value) {
    self->Control = value;
}


template <typename D>
void buildMetaClass_SEvent(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("EventType", &D::ClassType::EventType);
    _d.CPGF_MD_TEMPLATE _field("GUIEvent", &D::ClassType::GUIEvent);
    _d.CPGF_MD_TEMPLATE _field("MouseInput", &D::ClassType::MouseInput);
    _d.CPGF_MD_TEMPLATE _field("KeyInput", &D::ClassType::KeyInput);
    _d.CPGF_MD_TEMPLATE _field("JoystickEvent", &D::ClassType::JoystickEvent);
    _d.CPGF_MD_TEMPLATE _field("LogEvent", &D::ClassType::LogEvent);
    _d.CPGF_MD_TEMPLATE _field("UserEvent", &D::ClassType::UserEvent);
    {
        GDefineMetaClass<SEvent::SGUIEvent> _nd = GDefineMetaClass<SEvent::SGUIEvent>::declare("SGUIEvent");
        _nd.CPGF_MD_TEMPLATE _field("Caller", &SEvent::SGUIEvent::Caller);
        _nd.CPGF_MD_TEMPLATE _field("Element", &SEvent::SGUIEvent::Element);
        _nd.CPGF_MD_TEMPLATE _field("EventType", &SEvent::SGUIEvent::EventType);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<SEvent::SJoystickEvent> _nd = GDefineMetaClass<SEvent::SJoystickEvent>::declare("SJoystickEvent");
        _nd.CPGF_MD_TEMPLATE _field("ButtonStates", &SEvent::SJoystickEvent::ButtonStates);
        _nd.CPGF_MD_TEMPLATE _field("Axis", &SEvent::SJoystickEvent::Axis);
        _nd.CPGF_MD_TEMPLATE _field("POV", &SEvent::SJoystickEvent::POV);
        _nd.CPGF_MD_TEMPLATE _field("Joystick", &SEvent::SJoystickEvent::Joystick);
        _nd.CPGF_MD_TEMPLATE _method("IsButtonPressed", &SEvent::SJoystickEvent::IsButtonPressed);
        _nd.CPGF_MD_TEMPLATE _enum<long long>("GlobalEnum_irrlicht_1")
            ._element("NUMBER_OF_BUTTONS", SEvent::SJoystickEvent::NUMBER_OF_BUTTONS)
            ._element("AXIS_X", SEvent::SJoystickEvent::AXIS_X)
            ._element("AXIS_Y", SEvent::SJoystickEvent::AXIS_Y)
            ._element("AXIS_Z", SEvent::SJoystickEvent::AXIS_Z)
            ._element("AXIS_R", SEvent::SJoystickEvent::AXIS_R)
            ._element("AXIS_U", SEvent::SJoystickEvent::AXIS_U)
            ._element("AXIS_V", SEvent::SJoystickEvent::AXIS_V)
            ._element("NUMBER_OF_AXES", SEvent::SJoystickEvent::NUMBER_OF_AXES)
        ;
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<SEvent::SKeyInput> _nd = GDefineMetaClass<SEvent::SKeyInput>::declare("SKeyInput");
        _nd.CPGF_MD_TEMPLATE _field("Char", &SEvent::SKeyInput::Char);
        _nd.CPGF_MD_TEMPLATE _field("Key", &SEvent::SKeyInput::Key);
        _nd.CPGF_MD_TEMPLATE _property("PressedDown", &bItFiEldWrapper_SEvent_SKeyInput_PressedDown_getter, &bItFiEldWrapper_SEvent_SKeyInput_PressedDown_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
        _nd.CPGF_MD_TEMPLATE _property("Shift", &bItFiEldWrapper_SEvent_SKeyInput_Shift_getter, &bItFiEldWrapper_SEvent_SKeyInput_Shift_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
        _nd.CPGF_MD_TEMPLATE _property("Control", &bItFiEldWrapper_SEvent_SKeyInput_Control_getter, &bItFiEldWrapper_SEvent_SKeyInput_Control_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<SEvent::SLogEvent> _nd = GDefineMetaClass<SEvent::SLogEvent>::declare("SLogEvent");
        _nd.CPGF_MD_TEMPLATE _field("Text", &SEvent::SLogEvent::Text);
        _nd.CPGF_MD_TEMPLATE _field("Level", &SEvent::SLogEvent::Level);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<SEvent::SMouseInput> _nd = GDefineMetaClass<SEvent::SMouseInput>::declare("SMouseInput");
        _nd.CPGF_MD_TEMPLATE _field("X", &SEvent::SMouseInput::X);
        _nd.CPGF_MD_TEMPLATE _field("Y", &SEvent::SMouseInput::Y);
        _nd.CPGF_MD_TEMPLATE _field("Wheel", &SEvent::SMouseInput::Wheel);
        _nd.CPGF_MD_TEMPLATE _property("Shift", &bItFiEldWrapper_SEvent_SMouseInput_Shift_getter, &bItFiEldWrapper_SEvent_SMouseInput_Shift_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
        _nd.CPGF_MD_TEMPLATE _property("Control", &bItFiEldWrapper_SEvent_SMouseInput_Control_getter, &bItFiEldWrapper_SEvent_SMouseInput_Control_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
        _nd.CPGF_MD_TEMPLATE _field("ButtonStates", &SEvent::SMouseInput::ButtonStates);
        _nd.CPGF_MD_TEMPLATE _field("Event", &SEvent::SMouseInput::Event);
        _nd.CPGF_MD_TEMPLATE _method("isLeftPressed", &SEvent::SMouseInput::isLeftPressed);
        _nd.CPGF_MD_TEMPLATE _method("isRightPressed", &SEvent::SMouseInput::isRightPressed);
        _nd.CPGF_MD_TEMPLATE _method("isMiddlePressed", &SEvent::SMouseInput::isMiddlePressed);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<SEvent::SUserEvent> _nd = GDefineMetaClass<SEvent::SUserEvent>::declare("SUserEvent");
        _nd.CPGF_MD_TEMPLATE _field("UserData1", &SEvent::SUserEvent::UserData1);
        _nd.CPGF_MD_TEMPLATE _field("UserData2", &SEvent::SUserEvent::UserData2);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


template <typename D>
void buildMetaClass_SJoystickInfo(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("Joystick", &D::ClassType::Joystick);
    _d.CPGF_MD_TEMPLATE _field("Name", &D::ClassType::Name);
    _d.CPGF_MD_TEMPLATE _field("Buttons", &D::ClassType::Buttons);
    _d.CPGF_MD_TEMPLATE _field("Axes", &D::ClassType::Axes);
    _d.CPGF_MD_TEMPLATE _field("PovHat", &D::ClassType::PovHat);
    _d.CPGF_MD_TEMPLATE _enum<long long>("GlobalEnum_irrlicht_1")
        ._element("POV_HAT_PRESENT", D::ClassType::POV_HAT_PRESENT)
        ._element("POV_HAT_ABSENT", D::ClassType::POV_HAT_ABSENT)
        ._element("POV_HAT_UNKNOWN", D::ClassType::POV_HAT_UNKNOWN)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
