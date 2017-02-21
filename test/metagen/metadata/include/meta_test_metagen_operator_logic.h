// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_OPERATOR_LOGIC_H
#define CPGF_META_TEST_METAGEN_OPERATOR_LOGIC_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


inline bool opErAToRWrapper_MetagenOperatorLogic__opGreater(const MetagenOperatorLogic * self, unsigned int n) {
    return (*self) > n;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opGreater(const MetagenOperatorLogic * self, const MetagenOperatorLogic & other) {
    return (*self) > other;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opGreater(const MetagenOperatorLogic * self, const std::string & s) {
    return (*self) > s;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opGreaterEqual(const MetagenOperatorLogic * self, unsigned int n) {
    return (*self) >= n;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opGreaterEqual(const MetagenOperatorLogic * self, const MetagenOperatorLogic & other) {
    return (*self) >= other;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opGreaterEqual(const MetagenOperatorLogic * self, const std::string & s) {
    return (*self) >= s;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opLess(const MetagenOperatorLogic * self, unsigned int n) {
    return (*self) < n;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opLess(const MetagenOperatorLogic * self, const MetagenOperatorLogic & other) {
    return (*self) < other;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opLess(const MetagenOperatorLogic * self, const std::string & s) {
    return (*self) < s;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opLessEqual(const MetagenOperatorLogic * self, unsigned int n) {
    return (*self) <= n;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opLessEqual(const MetagenOperatorLogic * self, const MetagenOperatorLogic & other) {
    return (*self) <= other;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opLessEqual(const MetagenOperatorLogic * self, const std::string & s) {
    return (*self) <= s;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opAnd(const MetagenOperatorLogic * self, unsigned int n) {
    return (*self) && n;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opAnd(const MetagenOperatorLogic * self, const MetagenOperatorLogic & other) {
    return (*self) && other;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opAnd(const MetagenOperatorLogic * self, const std::string & s) {
    return (*self) && s;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opOr(const MetagenOperatorLogic * self, unsigned int n) {
    return (*self) || n;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opOr(const MetagenOperatorLogic * self, const MetagenOperatorLogic & other) {
    return (*self) || other;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opOr(const MetagenOperatorLogic * self, const std::string & s) {
    return (*self) || s;
}
inline bool opErAToRWrapper_MetagenOperatorLogic__opNot(const MetagenOperatorLogic * self) {
    return !(*self);
}


template <typename D>
void buildMetaClass_MetagenOperatorLogic(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned int)>();
    _d.CPGF_MD_TEMPLATE _field("value", &D::ClassType::value);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, unsigned int)>(mopHolder > mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opGreater", (bool (*) (const MetagenOperatorLogic *, unsigned int))&opErAToRWrapper_MetagenOperatorLogic__opGreater, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const MetagenOperatorLogic &)>(mopHolder > mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opGreater", (bool (*) (const MetagenOperatorLogic *, const MetagenOperatorLogic &))&opErAToRWrapper_MetagenOperatorLogic__opGreater, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const std::string &)>(mopHolder > mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opGreater", (bool (*) (const MetagenOperatorLogic *, const std::string &))&opErAToRWrapper_MetagenOperatorLogic__opGreater, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, unsigned int)>(mopHolder >= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opGreaterEqual", (bool (*) (const MetagenOperatorLogic *, unsigned int))&opErAToRWrapper_MetagenOperatorLogic__opGreaterEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const MetagenOperatorLogic &)>(mopHolder >= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opGreaterEqual", (bool (*) (const MetagenOperatorLogic *, const MetagenOperatorLogic &))&opErAToRWrapper_MetagenOperatorLogic__opGreaterEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const std::string &)>(mopHolder >= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opGreaterEqual", (bool (*) (const MetagenOperatorLogic *, const std::string &))&opErAToRWrapper_MetagenOperatorLogic__opGreaterEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, unsigned int)>(mopHolder < mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLess", (bool (*) (const MetagenOperatorLogic *, unsigned int))&opErAToRWrapper_MetagenOperatorLogic__opLess, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const MetagenOperatorLogic &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLess", (bool (*) (const MetagenOperatorLogic *, const MetagenOperatorLogic &))&opErAToRWrapper_MetagenOperatorLogic__opLess, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const std::string &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLess", (bool (*) (const MetagenOperatorLogic *, const std::string &))&opErAToRWrapper_MetagenOperatorLogic__opLess, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, unsigned int)>(mopHolder <= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLessEqual", (bool (*) (const MetagenOperatorLogic *, unsigned int))&opErAToRWrapper_MetagenOperatorLogic__opLessEqual, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const MetagenOperatorLogic &)>(mopHolder <= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLessEqual", (bool (*) (const MetagenOperatorLogic *, const MetagenOperatorLogic &))&opErAToRWrapper_MetagenOperatorLogic__opLessEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const std::string &)>(mopHolder <= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLessEqual", (bool (*) (const MetagenOperatorLogic *, const std::string &))&opErAToRWrapper_MetagenOperatorLogic__opLessEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, unsigned int)>(mopHolder && mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAnd", (bool (*) (const MetagenOperatorLogic *, unsigned int))&opErAToRWrapper_MetagenOperatorLogic__opAnd, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const MetagenOperatorLogic &)>(mopHolder && mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAnd", (bool (*) (const MetagenOperatorLogic *, const MetagenOperatorLogic &))&opErAToRWrapper_MetagenOperatorLogic__opAnd, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const std::string &)>(mopHolder && mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAnd", (bool (*) (const MetagenOperatorLogic *, const std::string &))&opErAToRWrapper_MetagenOperatorLogic__opAnd, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, unsigned int)>(mopHolder || mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opOr", (bool (*) (const MetagenOperatorLogic *, unsigned int))&opErAToRWrapper_MetagenOperatorLogic__opOr, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const MetagenOperatorLogic &)>(mopHolder || mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opOr", (bool (*) (const MetagenOperatorLogic *, const MetagenOperatorLogic &))&opErAToRWrapper_MetagenOperatorLogic__opOr, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const std::string &)>(mopHolder || mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opOr", (bool (*) (const MetagenOperatorLogic *, const std::string &))&opErAToRWrapper_MetagenOperatorLogic__opOr, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &)>(!mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opNot", (bool (*) (const MetagenOperatorLogic *))&opErAToRWrapper_MetagenOperatorLogic__opNot, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
