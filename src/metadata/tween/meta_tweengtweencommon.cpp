// Auto generated file, don't modify.

#include "cpgf/tween/gtimeline.h"
#include "cpgf/tween/gtweencommon.h"

#include "cpgf/metadata/tween/meta_tweengtweencommon.h"

using namespace cpgf;

namespace meta_tween { 


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_Global_gtweencommon()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_gtweencommon(_d);
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_GTweenEaseParam()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<cpgf::GTweenEaseParam> _nd = GDefineMetaClass<cpgf::GTweenEaseParam>::lazyDeclare("GTweenEaseParam", &buildMetaClass_GTweenEaseParam);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


#ifdef CPGF_METAGEN_LINKAGE_SPEC
CPGF_METAGEN_LINKAGE_SPEC
#endif
GDefineMetaInfo createMetaClass_GTweenable()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<cpgf::GTweenable, GNoncopyable> _nd = GDefineMetaClass<cpgf::GTweenable, GNoncopyable>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::lazyDeclare("GTweenable", &buildMetaClass_GTweenable);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_tween


