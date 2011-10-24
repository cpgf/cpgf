#ifndef __GMETAPOLICY_H
#define __GMETAPOLICY_H

#include "cpgf/gpp.h"
#include "cpgf/gpolicy.h"
#include "cpgf/gmetacommon.h"


#define POLICY_ALL_INDEX_HELPER(N, P) GPP_COMMA_IF(N) P<N>


namespace cpgf {

const int metaPolicyResultIndex = -1;


// used by field and property
struct GMetaRuleForbidRead {};
struct GMetaRuleForbidWrite {};

// used by class
struct GMetaRuleDefaultConstructorAbsent {};
struct GMetaRuleCopyConstructorAbsent {};

// used by method, constructor, and operator
template <int N>
struct GMetaRuleTransferOwnership {};

// used by method
template <int N>
struct GMetaRuleKeepConstReference {};


// policies

typedef MakePolicy<
	>
	GMetaPolicyDefault;


// used by field and property
typedef MakePolicy<
		GMetaRuleForbidRead,
		GMetaRuleForbidWrite
	>
	GMetaPolicyNoncopyable;


typedef MakePolicy<
		GMetaRuleForbidWrite
	>
	GMetaPolicyReadOnly;


typedef MakePolicy<
		GMetaRuleForbidRead
	>
	GMetaPolicyWriteOnly;


// used by class
typedef MakePolicy<
		GMetaRuleDefaultConstructorAbsent
	>
	GMetaPolicyNoDefaultConstructor;


typedef MakePolicy<
		GMetaRuleCopyConstructorAbsent
	>
	GMetaPolicyNoCopyConstructor;


typedef MakePolicy<
		GMetaRuleDefaultConstructorAbsent,
		GMetaRuleCopyConstructorAbsent
	>
	GMetaPolicyNoDefaultAndCopyConstructor;


typedef MakePolicy<
		GMetaRuleTransferOwnership <metaPolicyResultIndex>
	>
	GMetaPolicyTransferResultOwnership;


typedef MakePolicy<
		GMetaRuleKeepConstReference <metaPolicyResultIndex>,
		GPP_REPEAT(REF_MAX_ARITY, POLICY_ALL_INDEX_HELPER, GMetaRuleKeepConstReference)
	>
	GMetaPolicyKeepAllConstReference;



} // namespace cpgf


#undef POLICY_ALL_INDEX_HELPER



#endif

