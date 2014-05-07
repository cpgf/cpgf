// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_SIMPLEOVERRIDABLEINCREMENTER_H
#define CPGF_META_TEST_SIMPLEOVERRIDABLEINCREMENTER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gscopedinterface.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_SimpleOverridableIncrementer(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("increment", &D::ClassType::increment);
}


class SimpleOverridableIncrementerWrapper : public SimpleOverridableIncrementer, public cpgf::GScriptWrapper {
public:
    
    int increment()
    {
        cpgf::GScopedInterface<cpgf::IScriptFunction> func(this->getScriptFunction("increment"));
        if(func)
        {
            return cpgf::fromVariant<int >(cpgf::invokeScriptFunctionOnObject(func.get(), this).getValue());
        }
        throw std::runtime_error("Abstract method");
    }
    int super_increment()
    {
        throw std::runtime_error("Abstract method");
    }
    template <typename D>
    static void cpgf__register(D _d)
    {
        (void)_d;
        using namespace cpgf;
        _d.CPGF_MD_TEMPLATE _method("super_increment", (int (D::ClassType::*) ())&D::ClassType::super_increment);
    }
};


template <typename D>
void buildMetaClass_SimpleOverridableIncrementerWrapper(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    
    SimpleOverridableIncrementerWrapper::cpgf__register(_d);
    
    buildMetaClass_SimpleOverridableIncrementer<D>(_d);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
