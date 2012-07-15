// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/System/Mutex.hpp"

#include "cpgf/metadata/sfml/meta_sfml_Mutex.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_Mutex()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::Mutex> _nd = GDefineMetaClass<sf::Mutex>::Policy<MakePolicy<GMetaRuleCopyConstructorAbsent> >::declare("Mutex");
        buildMetaClass_Mutex(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


