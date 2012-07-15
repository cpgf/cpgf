// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio/AudioResource.hpp"

#include "cpgf/metadata/sfml/meta_sfml_AudioResource.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_AudioResource()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::AudioResource> _nd = GDefineMetaClass<sf::AudioResource>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("AudioResource");
        buildMetaClass_AudioResource(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


