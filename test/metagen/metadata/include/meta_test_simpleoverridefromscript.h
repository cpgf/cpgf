// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_SIMPLEOVERRIDEFROMSCRIPT_H
#define CPGF_META_TEST_SIMPLEOVERRIDEFROMSCRIPT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gscopedinterface.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_SimpleOverride(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (int)>();
    _d.CPGF_MD_TEMPLATE _field("n", &D::ClassType::n);
    _d.CPGF_MD_TEMPLATE _method("getValue", &D::ClassType::getValue);
    _d.CPGF_MD_TEMPLATE _method("getAnother", &D::ClassType::getAnother);
    _d.CPGF_MD_TEMPLATE _method("createHelperData", &D::ClassType::createHelperData);
    _d.CPGF_MD_TEMPLATE _method("getName", &D::ClassType::getName);
    _d.CPGF_MD_TEMPLATE _method("consumeHelperData", &D::ClassType::consumeHelperData);
}


class SimpleOverrideWrapper : public SimpleOverride, public cpgf::GScriptWrapper {
public:
    
    SimpleOverrideWrapper(int n)
        : SimpleOverride(n) {}
    
    SimpleOverrideHelperData * createHelperData()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("createHelperData"));
        if(func)
        {
            cpgf::GScriptValue ret = cpgf::invokeScriptFunctionOnObject(func.get(), this);
            ret.discardOwnership();
            return cpgf::fromVariant<SimpleOverrideHelperData * >(ret.getValue());
        }
        return SimpleOverride::createHelperData();
    }
    SimpleOverrideHelperData * super_createHelperData()
    {
        return SimpleOverride::createHelperData();
    }
    
    int getValue()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getValue"));
        if(func)
        {
            return cpgf::fromVariant<int >(cpgf::invokeScriptFunctionOnObject(func.get(), this).getValue());
        }
        return SimpleOverride::getValue();
    }
    int super_getValue()
    {
        return SimpleOverride::getValue();
    }
    
    std::string getName()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getName"));
        if(func)
        {
            return cpgf::fromVariant<std::string >(cpgf::invokeScriptFunctionOnObject(func.get(), this).getValue());
        }
        return SimpleOverride::getName();
    }
    std::string super_getName()
    {
        return SimpleOverride::getName();
    }
    
    int getAnother()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("getAnother"));
        if(func)
        {
            return cpgf::fromVariant<int >(cpgf::invokeScriptFunctionOnObject(func.get(), this).getValue());
        }
        return SimpleOverride::getAnother();
    }
    int super_getAnother()
    {
        return SimpleOverride::getAnother();
    }
    template <typename D>
    static void cpgf__register(D _d)
    {
        (void)_d;
        using namespace cpgf;
        _d.CPGF_MD_TEMPLATE _method("super_createHelperData", (SimpleOverrideHelperData * (D::ClassType::*) ())&D::ClassType::super_createHelperData);
        _d.CPGF_MD_TEMPLATE _method("super_getValue", (int (D::ClassType::*) ())&D::ClassType::super_getValue);
        _d.CPGF_MD_TEMPLATE _method("super_getName", (std::string (D::ClassType::*) ())&D::ClassType::super_getName);
        _d.CPGF_MD_TEMPLATE _method("super_getAnother", (int (D::ClassType::*) ())&D::ClassType::super_getAnother);
    }
};


template <typename D>
void buildMetaClass_SimpleOverrideWrapper(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    
    SimpleOverrideWrapper::cpgf__register(_d);
    
    buildMetaClass_SimpleOverride<D>(_d);
}


template <typename D>
void buildMetaClass_SimpleOverrideBase(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("baseOnly", &D::ClassType::baseOnly);
    _d.CPGF_MD_TEMPLATE _method("getValue", &D::ClassType::getValue);
}


template <typename D>
void buildMetaClass_SimpleOverrideHelperData(D _d)
{
    (void)_d;
    using namespace cpgf;
    
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
