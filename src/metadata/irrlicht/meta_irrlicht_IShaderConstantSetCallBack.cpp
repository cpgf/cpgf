// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IShaderConstantSetCallBack.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IShaderConstantSetCallBack.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IShaderConstantSetCallBack()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::IShaderConstantSetCallBack, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::video::IShaderConstantSetCallBack, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IShaderConstantSetCallBack");
        buildMetaClass_IShaderConstantSetCallBack(0, _nd);
        _d._class(_nd);
    }
    {
        GDefineMetaClass<IShaderConstantSetCallBackWrapper, irr::video::IShaderConstantSetCallBack> _nd = GDefineMetaClass<IShaderConstantSetCallBackWrapper, irr::video::IShaderConstantSetCallBack>::declare("IShaderConstantSetCallBackWrapper");
        buildMetaClass_IShaderConstantSetCallBackWrapper(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


