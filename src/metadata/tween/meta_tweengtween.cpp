// Auto generated file, don't modify.

#include "cpgf/tween/gtimeline.h"
#include "cpgf/tween/gtween.h"

#include "cpgf/metadata/tween/meta_tweengtween.h"

using namespace cpgf;

namespace meta_tween { 


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_GTween()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<cpgf::GTween, cpgf::GTweenable> _nd = GDefineMetaClass<cpgf::GTween, cpgf::GTweenable>::lazyDeclare("GTween", &buildMetaClass_GTween);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_tween


