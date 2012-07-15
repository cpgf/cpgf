// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Contacts/b2TOISolver.h"

#include "cpgf/metadata/box2d/meta_box2d_b2TOISolver.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2TOISolver()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2TOISolver> _nd = GDefineMetaClass<b2TOISolver>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2TOISolver");
        buildMetaClass_B2TOISolver(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


