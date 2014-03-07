// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_OPERATOR_INNER_CLASS_H
#define CPGF_META_TEST_METAGEN_OPERATOR_INNER_CLASS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opAdd(const MetagenOperatorInnerClass::MetagenOperatorInner * self, int n) {
    return (*self) + n;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opAdd(const MetagenOperatorInnerClass::MetagenOperatorInner * self, const MetagenOperatorInnerClass::MetagenOperatorInner& other) {
    return (*self) + other;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opAdd(const MetagenOperatorInnerClass::MetagenOperatorInner * self, const std::string & s) {
    return (*self) + s;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opSub(MetagenOperatorInnerClass::MetagenOperatorInner * self, int n) {
    return (*self) - n;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opSub(MetagenOperatorInnerClass::MetagenOperatorInner * self, const MetagenOperatorInnerClass::MetagenOperatorInner& other) {
    return (*self) - other;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opSub(MetagenOperatorInnerClass::MetagenOperatorInner * self, const std::string & s) {
    return (*self) - s;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMul(MetagenOperatorInnerClass::MetagenOperatorInner * self, int n) {
    return (*self) * n;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMul(MetagenOperatorInnerClass::MetagenOperatorInner * self, const MetagenOperatorInnerClass::MetagenOperatorInner& other) {
    return (*self) * other;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMul(MetagenOperatorInnerClass::MetagenOperatorInner * self, const std::string & s) {
    return (*self) * s;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opDiv(MetagenOperatorInnerClass::MetagenOperatorInner * self, int n) {
    return (*self) / n;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opDiv(MetagenOperatorInnerClass::MetagenOperatorInner * self, const MetagenOperatorInnerClass::MetagenOperatorInner& other) {
    return (*self) / other;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opDiv(MetagenOperatorInnerClass::MetagenOperatorInner * self, const std::string & s) {
    return (*self) / s;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMod(MetagenOperatorInnerClass::MetagenOperatorInner * self, int n) {
    return (*self) % n;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMod(MetagenOperatorInnerClass::MetagenOperatorInner * self, const MetagenOperatorInnerClass::MetagenOperatorInner& other) {
    return (*self) % other;
}
inline MetagenOperatorInnerClass::MetagenOperatorInner opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMod(MetagenOperatorInnerClass::MetagenOperatorInner * self, const std::string & s) {
    return (*self) % s;
}


template <typename D>
void buildMetaClass_MetagenOperatorInnerClass(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    {
        GDefineMetaClass<MetagenOperatorInnerClass::MetagenOperatorInner> _nd = GDefineMetaClass<MetagenOperatorInnerClass::MetagenOperatorInner>::declare("MetagenOperatorInner");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (int)>();
        _nd.CPGF_MD_TEMPLATE _field("value", &MetagenOperatorInnerClass::MetagenOperatorInner::value);
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(const cpgf::GMetaSelf &, int)>(mopHolder + mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opAdd", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (const MetagenOperatorInnerClass::MetagenOperatorInner *, int))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opAdd, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(const cpgf::GMetaSelf &, const MetagenOperatorInnerClass::MetagenOperatorInner&)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opAdd", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (const MetagenOperatorInnerClass::MetagenOperatorInner *, const MetagenOperatorInnerClass::MetagenOperatorInner&))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opAdd, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(const cpgf::GMetaSelf &, const std::string &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opAdd", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (const MetagenOperatorInnerClass::MetagenOperatorInner *, const std::string &))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opAdd, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, int)>(mopHolder - mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opSub", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, int))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opSub, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, const MetagenOperatorInnerClass::MetagenOperatorInner&)>(mopHolder - mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opSub", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, const MetagenOperatorInnerClass::MetagenOperatorInner&))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opSub, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder - mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opSub", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, const std::string &))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opSub, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, int)>(mopHolder * mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opMul", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, int))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMul, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, const MetagenOperatorInnerClass::MetagenOperatorInner&)>(mopHolder * mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opMul", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, const MetagenOperatorInnerClass::MetagenOperatorInner&))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMul, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder * mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opMul", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, const std::string &))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMul, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, int)>(mopHolder / mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opDiv", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, int))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opDiv, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, const MetagenOperatorInnerClass::MetagenOperatorInner&)>(mopHolder / mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opDiv", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, const MetagenOperatorInnerClass::MetagenOperatorInner&))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opDiv, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder / mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opDiv", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, const std::string &))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opDiv, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, int)>(mopHolder % mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opMod", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, int))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMod, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, const MetagenOperatorInnerClass::MetagenOperatorInner&)>(mopHolder % mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opMod", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, const MetagenOperatorInnerClass::MetagenOperatorInner&))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMod, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
        _nd.CPGF_MD_TEMPLATE _operator<MetagenOperatorInnerClass::MetagenOperatorInner (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder % mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opMod", (MetagenOperatorInnerClass::MetagenOperatorInner (*) (MetagenOperatorInnerClass::MetagenOperatorInner *, const std::string &))&opErAToRWrapper_MetagenOperatorInnerClass_MetagenOperatorInner__opMod, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
