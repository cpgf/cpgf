// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ILIGHTMANAGER_H
#define __META_IRRLICHT_ILIGHTMANAGER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gscopedinterface.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ILightManager(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("OnPreRender", &D::ClassType::OnPreRender);
    _d.CPGF_MD_TEMPLATE _method("OnPostRender", &D::ClassType::OnPostRender);
    _d.CPGF_MD_TEMPLATE _method("OnRenderPassPreRender", &D::ClassType::OnRenderPassPreRender);
    _d.CPGF_MD_TEMPLATE _method("OnRenderPassPostRender", &D::ClassType::OnRenderPassPostRender);
    _d.CPGF_MD_TEMPLATE _method("OnNodePreRender", &D::ClassType::OnNodePreRender);
    _d.CPGF_MD_TEMPLATE _method("OnNodePostRender", &D::ClassType::OnNodePostRender);
}


class ILightManagerWrapper : public irr::scene::ILightManager, public cpgf::GScriptWrapper {
public:
    
    void OnPreRender(core::array< ISceneNode * > & lightList)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnPreRender"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, lightList);
            return;
        }
        throw "Abstract method";
    }
    void super_OnPreRender(core::array< ISceneNode * > & lightList)
    {
        throw "Abstract method";
    }
    
    void OnPostRender()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnPostRender"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this);
            return;
        }
        throw "Abstract method";
    }
    void super_OnPostRender()
    {
        throw "Abstract method";
    }
    
    void OnRenderPassPreRender(E_SCENE_NODE_RENDER_PASS renderPass)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnRenderPassPreRender"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, renderPass);
            return;
        }
        throw "Abstract method";
    }
    void super_OnRenderPassPreRender(E_SCENE_NODE_RENDER_PASS renderPass)
    {
        throw "Abstract method";
    }
    
    void OnRenderPassPostRender(E_SCENE_NODE_RENDER_PASS renderPass)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnRenderPassPostRender"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, renderPass);
            return;
        }
        throw "Abstract method";
    }
    void super_OnRenderPassPostRender(E_SCENE_NODE_RENDER_PASS renderPass)
    {
        throw "Abstract method";
    }
    
    void OnNodePreRender(ISceneNode * node)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnNodePreRender"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, node);
            return;
        }
        throw "Abstract method";
    }
    void super_OnNodePreRender(ISceneNode * node)
    {
        throw "Abstract method";
    }
    
    void OnNodePostRender(ISceneNode * node)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnNodePostRender"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, node);
            return;
        }
        throw "Abstract method";
    }
    void super_OnNodePostRender(ISceneNode * node)
    {
        throw "Abstract method";
    }
};


template <typename D>
void buildMetaClass_ILightManagerWrapper(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    
    _d.CPGF_MD_TEMPLATE _method("super_OnPreRender", (void (D::ClassType::*) (core::array< ISceneNode * > &))&D::ClassType::super_OnPreRender);
    _d.CPGF_MD_TEMPLATE _method("super_OnPostRender", (void (D::ClassType::*) ())&D::ClassType::super_OnPostRender);
    _d.CPGF_MD_TEMPLATE _method("super_OnRenderPassPreRender", (void (D::ClassType::*) (E_SCENE_NODE_RENDER_PASS))&D::ClassType::super_OnRenderPassPreRender);
    _d.CPGF_MD_TEMPLATE _method("super_OnRenderPassPostRender", (void (D::ClassType::*) (E_SCENE_NODE_RENDER_PASS))&D::ClassType::super_OnRenderPassPostRender);
    _d.CPGF_MD_TEMPLATE _method("super_OnNodePreRender", (void (D::ClassType::*) (ISceneNode *))&D::ClassType::super_OnNodePreRender);
    _d.CPGF_MD_TEMPLATE _method("super_OnNodePostRender", (void (D::ClassType::*) (ISceneNode *))&D::ClassType::super_OnNodePostRender);
    
    buildMetaClass_ILightManager<D>(config, _d);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
