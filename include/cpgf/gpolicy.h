#ifndef CPGF_GPOLICY_H
#define CPGF_GPOLICY_H

#include "cpgf/gcompiler.h"
#include "cpgf/gtypelist.h"
#include "cpgf/gtypeutil.h"

#define POLICY_MAX_RULES GPP_MAX_LIMIT

namespace cpgf {

template <typename... RuleTypes>
struct MakePolicy
{
	typedef typename TypeList_Make<RuleTypes...>::Result Rules;
};

template <typename PolicyA, typename PolicyB>
struct MergePolicy
{
	typedef typename TypeList_Append<
		typename PolicyA::Rules, typename PolicyB::Rules
	>::Result
	Rules;
};

template <typename Policy, typename Rule>
struct PolicyHasRule
{
	G_STATIC_CONSTANT(bool, Result = ((TypeList_IndexOf<typename Policy::Rules, Rule>::Result) >= 0));
};

template <typename Policy, typename Rule>
struct PolicyNotHasRule
{
	G_STATIC_CONSTANT(bool, Result = ((TypeList_IndexOf<typename Policy::Rules, Rule>::Result) < 0));
};

namespace policy_internal {

template <unsigned int N>
struct PolicyHasIndexedRuleTypeSelector
{
	template <typename Policy, template <int> class IndexedPolicyItem>
	bool operator()(const Policy &, const IndexedPolicyItem<0> &)
	{
		return PolicyHasRule<Policy, IndexedPolicyItem<N> >::Result;
	}
};

} //namespace policy_internal

template <typename Policy, template <int> class IndexedPolicyItem>
bool policyHasIndexedRule(int index)
{
	if(index  == -1) {
		return PolicyHasRule<Policy, IndexedPolicyItem<-1> >::Result;
	}

	return GTypeSelector<POLICY_MAX_RULES>::template select<bool, policy_internal::PolicyHasIndexedRuleTypeSelector>(
		index,
		Policy(),
		IndexedPolicyItem<0>()
	);
}


} // namespace cpgf


#undef POLICY_MAX_RULES


#endif

