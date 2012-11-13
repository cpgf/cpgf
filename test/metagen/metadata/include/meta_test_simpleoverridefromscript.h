// Auto generated file, don't modify.

#ifndef __META_TEST_SIMPLEOVERRIDEFROMSCRIPT_H
#define __META_TEST_SIMPLEOVERRIDEFROMSCRIPT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gscopedinterface.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_SimpleOverride(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (int)>();
    _d.CPGF_MD_TEMPLATE _field("n", &D::ClassType::n);
    _d.CPGF_MD_TEMPLATE _method("getValue", &D::ClassType::getValue);
    _d.CPGF_MD_TEMPLATE _method("getAnother", &D::ClassType::getAnother);
    _d.CPGF_MD_TEMPLATE _method("getName", &D::ClassType::getName);
}


class SimpleOverrideWrapper : public SimpleOverride, public cpgf::GScriptWrapper {
public:
    
    SimpleOverrideWrapper(int n)
        : SimpleOverride(n) {}
    
    int getValue()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getValue"));
        if(func)
        {
            return cpgf::fromVariant<int >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return SimpleOverride::getValue();
    }
    int super_getValue()
    {
        return SimpleOverride::getValue();
    }
    
    int getAnother()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getAnother"));
        if(func)
        {
            return cpgf::fromVariant<int >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return SimpleOverride::getAnother();
    }
    int super_getAnother()
    {
        return SimpleOverride::getAnother();
    }
    
    std::string getName()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getName"));
        if(func)
        {
            return cpgf::fromVariant<std::string >(cpgf::invokeScriptFunction(func.get(), this));
        }
        return SimpleOverride::getName();
    }
    std::string super_getName()
    {
        return SimpleOverride::getName();
    }
};


template <typename D>
void buildMetaClass_SimpleOverrideWrapper(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    
    _d.CPGF_MD_TEMPLATE _method("super_getValue", (int (D::ClassType::*) ())&D::ClassType::super_getValue);
    _d.CPGF_MD_TEMPLATE _method("super_getAnother", (int (D::ClassType::*) ())&D::ClassType::super_getAnother);
    _d.CPGF_MD_TEMPLATE _method("super_getName", (std::string (D::ClassType::*) ())&D::ClassType::super_getName);
    
    buildMetaClass_SimpleOverride<D>(config, _d);
}


template <typename D>
void buildMetaClass_SimpleOverrideBase(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("baseOnly", &D::ClassType::baseOnly);
    _d.CPGF_MD_TEMPLATE _method("getValue", &D::ClassType::getValue);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
