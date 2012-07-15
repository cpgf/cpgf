// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Window/WindowListener.hpp"

#include "cpgf/metadata/sfml/meta_sfml_WindowListener.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_WindowListener()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::WindowListener> _nd = GDefineMetaClass<sf::WindowListener>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("WindowListener");
        buildMetaClass_WindowListener(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


