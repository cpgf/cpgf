// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISHADERCONSTANTSETCALLBACK_H
#define __META_IRRLICHT_ISHADERCONSTANTSETCALLBACK_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gscopedinterface.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IShaderConstantSetCallBack(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("OnSetMaterial", &D::ClassType::OnSetMaterial);
    _d.CPGF_MD_TEMPLATE _method("OnSetConstants", &D::ClassType::OnSetConstants);
}


class IShaderConstantSetCallBackWrapper : public irr::video::IShaderConstantSetCallBack, public cpgf::GScriptWrapper {
public:
    
    void OnSetMaterial(const SMaterial & material)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnSetMaterial"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, material);
            return;
        }
        irr::video::IShaderConstantSetCallBack::OnSetMaterial(material);
    }
    void super_OnSetMaterial(const SMaterial & material)
    {
        irr::video::IShaderConstantSetCallBack::OnSetMaterial(material);
    }
    
    void OnSetConstants(IMaterialRendererServices * services, s32 userData)
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("OnSetConstants"));
        if(func)
        {
            cpgf::invokeScriptFunction(func.get(), this, services, userData);
            return;
        }
        throw "Abstract method";
    }
    void super_OnSetConstants(IMaterialRendererServices * services, s32 userData)
    {
        throw "Abstract method";
    }
};


template <typename D>
void buildMetaClass_IShaderConstantSetCallBackWrapper(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    
    _d.CPGF_MD_TEMPLATE _method("super_OnSetMaterial", (void (D::ClassType::*) (const SMaterial &))&D::ClassType::super_OnSetMaterial);
    _d.CPGF_MD_TEMPLATE _method("super_OnSetConstants", (void (D::ClassType::*) (IMaterialRendererServices *, s32))&D::ClassType::super_OnSetConstants);
    
    buildMetaClass_IShaderConstantSetCallBack<D>(config, _d);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
