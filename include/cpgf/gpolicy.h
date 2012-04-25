#ifndef __GPOLICY_H
#define __GPOLICY_H

#include "cpgf/gcompiler.h"
#include "cpgf/gtypelist.h"


#define POLICY_MAX_RULES GPP_MAX_LIMIT

namespace cpgf {

#define POLICTY_TYPENAME_PARAM_NULL(N, T) GPP_COMMA_IF(N) typename T ## N = GNullType

template <GPP_REPEAT(POLICY_MAX_RULES, POLICTY_TYPENAME_PARAM_NULL, T) >
struct MakePolicy
{
	typedef typename TypeList_Make<
		GPP_REPEAT_PARAMS(POLICY_MAX_RULES, T)
	>::Result
	Rules;
};

#undef POLICTY_TYPENAME_PARAM_NULL

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

template <typename Policy, template <int> class IndexedPolicyItem>
bool policyHasIndexedRule(int index)
{
	switch(index) {
		case -1:
			return PolicyHasRule<Policy, IndexedPolicyItem<-1> >::Result;

#define POLICY_CHECKINDEX_HELPER(N, unused) case N: return PolicyHasRule<Policy, IndexedPolicyItem<N> >::Result;
			
		GPP_REPEAT(POLICY_MAX_RULES, POLICY_CHECKINDEX_HELPER, GPP_EMPTY)

#undef POLICY_CHECKINDEX_HELPER

		default:
			return false;
	}
}



} // namespace cpgf



#undef POLICY_MAX_RULES


#endif

