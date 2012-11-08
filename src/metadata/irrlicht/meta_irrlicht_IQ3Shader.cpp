// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IQ3Shader.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IQ3Shader.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iq3shader()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iq3shader(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IShader()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::quake3::IShader> _nd = GDefineMetaClass<irr::scene::quake3::IShader>::declare("IShader");
        buildMetaClass_IShader(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IShaderManager()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::quake3::IShaderManager, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::quake3::IShaderManager, irr::IReferenceCounted>::declare("IShaderManager");
        buildMetaClass_IShaderManager(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_Noiser()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::quake3::Noiser> _nd = GDefineMetaClass<irr::scene::quake3::Noiser>::declare("Noiser");
        buildMetaClass_Noiser(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_Q3LevelLoadParameter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::quake3::Q3LevelLoadParameter> _nd = GDefineMetaClass<irr::scene::quake3::Q3LevelLoadParameter>::declare("Q3LevelLoadParameter");
        buildMetaClass_Q3LevelLoadParameter(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SBlendFunc()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::quake3::SBlendFunc> _nd = GDefineMetaClass<irr::scene::quake3::SBlendFunc>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SBlendFunc");
        buildMetaClass_SBlendFunc(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SModifierFunction()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::quake3::SModifierFunction> _nd = GDefineMetaClass<irr::scene::quake3::SModifierFunction>::declare("SModifierFunction");
        buildMetaClass_SModifierFunction(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SVarGroup()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::quake3::SVarGroup> _nd = GDefineMetaClass<irr::scene::quake3::SVarGroup>::declare("SVarGroup");
        buildMetaClass_SVarGroup(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SVarGroupList()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::quake3::SVarGroupList, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::quake3::SVarGroupList, irr::IReferenceCounted>::declare("SVarGroupList");
        buildMetaClass_SVarGroupList(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SVariable()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::quake3::SVariable> _nd = GDefineMetaClass<irr::scene::quake3::SVariable>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SVariable");
        buildMetaClass_SVariable(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


