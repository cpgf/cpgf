// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IEventReceiver.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IEventReceiver.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_ieventreceiver()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_ieventreceiver(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IEventReceiver()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::IEventReceiver> _nd = GDefineMetaClass<irr::IEventReceiver>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IEventReceiver");
        buildMetaClass_IEventReceiver(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<IEventReceiverWrapper, irr::IEventReceiver> _nd = GDefineMetaClass<IEventReceiverWrapper, irr::IEventReceiver>::declare("IEventReceiverWrapper");
        buildMetaClass_IEventReceiverWrapper(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SEvent()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::SEvent> _nd = GDefineMetaClass<irr::SEvent>::declare("SEvent");
        buildMetaClass_SEvent(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SJoystickInfo()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::SJoystickInfo> _nd = GDefineMetaClass<irr::SJoystickInfo>::declare("SJoystickInfo");
        buildMetaClass_SJoystickInfo(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


