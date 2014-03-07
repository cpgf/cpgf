// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_OPERATOR_ARITHMETIC_ASSIGN_H
#define CPGF_META_TEST_METAGEN_OPERATOR_ARITHMETIC_ASSIGN_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opAddAssign(MetagenOperatorArithmeticAssign * self, int n) {
    return (*self) += n;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opAddAssign(MetagenOperatorArithmeticAssign * self, const MetagenOperatorArithmeticAssign & other) {
    return (*self) += other;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opAddAssign(MetagenOperatorArithmeticAssign * self, const std::string & s) {
    return (*self) += s;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opSubAssign(MetagenOperatorArithmeticAssign * self, int n) {
    return (*self) -= n;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opSubAssign(MetagenOperatorArithmeticAssign * self, const MetagenOperatorArithmeticAssign & other) {
    return (*self) -= other;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opSubAssign(MetagenOperatorArithmeticAssign * self, const std::string & s) {
    return (*self) -= s;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opMulAssign(MetagenOperatorArithmeticAssign * self, int n) {
    return (*self) *= n;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opMulAssign(MetagenOperatorArithmeticAssign * self, const MetagenOperatorArithmeticAssign & other) {
    return (*self) *= other;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opMulAssign(MetagenOperatorArithmeticAssign * self, const std::string & s) {
    return (*self) *= s;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opDivAssign(MetagenOperatorArithmeticAssign * self, int n) {
    return (*self) /= n;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opDivAssign(MetagenOperatorArithmeticAssign * self, const MetagenOperatorArithmeticAssign & other) {
    return (*self) /= other;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opDivAssign(MetagenOperatorArithmeticAssign * self, const std::string & s) {
    return (*self) /= s;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opModAssign(MetagenOperatorArithmeticAssign * self, int n) {
    return (*self) %= n;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opModAssign(MetagenOperatorArithmeticAssign * self, const MetagenOperatorArithmeticAssign & other) {
    return (*self) %= other;
}
inline MetagenOperatorArithmeticAssign & opErAToRWrapper_MetagenOperatorArithmeticAssign__opModAssign(MetagenOperatorArithmeticAssign * self, const std::string & s) {
    return (*self) %= s;
}


template <typename D>
void buildMetaClass_MetagenOperatorArithmeticAssign(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (int)>();
    _d.CPGF_MD_TEMPLATE _field("value", &D::ClassType::value);
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, int)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAddAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, int))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opAddAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, const MetagenOperatorArithmeticAssign &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAddAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, const MetagenOperatorArithmeticAssign &))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opAddAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAddAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, const std::string &))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opAddAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, int)>(mopHolder -= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opSubAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, int))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opSubAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, const MetagenOperatorArithmeticAssign &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opSubAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, const MetagenOperatorArithmeticAssign &))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opSubAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opSubAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, const std::string &))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opSubAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, int)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMulAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, int))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opMulAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, const MetagenOperatorArithmeticAssign &)>(mopHolder *= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMulAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, const MetagenOperatorArithmeticAssign &))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opMulAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder *= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMulAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, const std::string &))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opMulAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, int)>(mopHolder /= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opDivAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, int))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opDivAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, const MetagenOperatorArithmeticAssign &)>(mopHolder /= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opDivAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, const MetagenOperatorArithmeticAssign &))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opDivAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder /= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opDivAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, const std::string &))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opDivAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, int)>(mopHolder %= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opModAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, int))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opModAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, const MetagenOperatorArithmeticAssign &)>(mopHolder %= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opModAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, const MetagenOperatorArithmeticAssign &))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opModAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorArithmeticAssign & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder %= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opModAssign", (MetagenOperatorArithmeticAssign & (*) (MetagenOperatorArithmeticAssign *, const std::string &))&opErAToRWrapper_MetagenOperatorArithmeticAssign__opModAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
