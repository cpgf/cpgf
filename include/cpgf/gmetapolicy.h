#ifndef __GMETAPOLICY_H
#define __GMETAPOLICY_H

#include "cpgf/gtypelist.h"


namespace cpgf {

template <typename Policy, typename Item>
struct HasMetaPolicyItem
{
	enum { Result = (TypeList_IndexOf<typename Policy::Policy, Item>::Result >= 0) };
};

template <typename Policy, typename Item>
struct NotHasMetaPolicyItem
{
	enum { Result = (TypeList_IndexOf<typename Policy::Policy, Item>::Result < 0) };
};


// used by field and property
struct GMetaPolicyItemForbidRead {};
struct GMetaPolicyItemForbidWrite {};

// used by class
struct GMetaPolicyItemDefaultConstructorAbsent {};
struct GMetaPolicyItemCopyConstructorAbsent {};

// used by method, constructor, and operator
template <int N>
struct GMetaPolicyItemTransferOwnership {};


struct GMetaPolicyDefault
{
	typedef TypeList_Make<
	>::Result
	Policy;
};

// used by field and property
struct GMetaPolicyNoncopyable
{
	typedef TypeList_Make<
		GMetaPolicyItemForbidRead,
		GMetaPolicyItemForbidWrite
	>::Result
	Policy;
};

struct GMetaPolicyReadOnly
{
	typedef TypeList_Make<
		GMetaPolicyItemForbidWrite
	>::Result
	Policy;
};

struct GMetaPolicyWriteOnly
{
	typedef TypeList_Make<
		GMetaPolicyItemForbidRead
	>::Result
	Policy;
};

// used by class
struct GMetaPolicyNoDefaultConstructor
{
	typedef TypeList_Make<
		GMetaPolicyItemDefaultConstructorAbsent
	>::Result
	Policy;
};

struct GMetaPolicyNoCopyConstructor
{
	typedef TypeList_Make<
		GMetaPolicyItemCopyConstructorAbsent
	>::Result
	Policy;
};

struct GMetaPolicyNoDefaultAndCopyConstructor
{
	typedef TypeList_Make<
		GMetaPolicyItemDefaultConstructorAbsent,
		GMetaPolicyItemCopyConstructorAbsent
	>::Result
	Policy;
};


struct GMetaPolicyTransferResultOwnership
{
	typedef TypeList_Make<
		GMetaPolicyItemTransferOwnership<-1>
	>::Result
	Policy;
};


template <typename Policy, template <int> class IndexedPolicyItem>
bool hasIndexedPolicy(int index)
{
	switch(index) {
		case -1:
			return HasMetaPolicyItem<Policy, IndexedPolicyItem<-1> >::Result;

#define POLICY_CHECKINDEX_HELPER(N, unused) \
	case N: return HasMetaPolicyItem<Policy, IndexedPolicyItem<N> >::Result;
			
		GPP_REPEAT(REF_MAX_ARITY, POLICY_CHECKINDEX_HELPER, GPP_EMPTY)

#undef POLICY_CHECKINDEX_HELPER

		default:
			return false;
	}
}


} // namespace cpgf



#endif
