// Auto generated file, don't modify.

#include "SFML/Audio.hpp"
#include "SFML/Config.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio/SoundRecorder.hpp"

#include "cpgf/metadata/sfml/meta_sfml_SoundRecorder.h"

using namespace cpgf;

namespace meta_sfml { 


GDefineMetaInfo createMetaClass_SoundRecorder()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<sf::SoundRecorder> _nd = GDefineMetaClass<sf::SoundRecorder>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("SoundRecorder");
        buildMetaClass_SoundRecorder(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_sfml


