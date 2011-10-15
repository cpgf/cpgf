#ifndef __GMETAOPERATOR_H
#define __GMETAOPERATOR_H


#include "cpgf/gmetacommon.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gmetamethod.h"
#include "cpgf/gassert.h"

#include <string>
#include <stdio.h>


namespace cpgf {


template <typename Signature, typename Creator>
void reflectOperator(const Creator & creator);


struct GMetaSelf {};

namespace meta_internal {

template <GMetaOpType op>
struct IsBinaryOperator
{
	enum { Result = (op >= mopBinaryFirst && op <= mopBinaryLast) };
};

template <GMetaOpType op>
struct IsUnaryOperator
{
	enum { Result = (op >= mopUnaryFirst && op <= mopUnaryLast) };
};

template <GMetaOpType op>
struct IsFunctorOperator
{
	enum { Result = (op == mopFunctor) };
};

template <typename T>
struct CheckOperatorSelf
{
	enum { IsConst =  IsSameType<T, const GMetaSelf &>::Result };
	enum { IsSelf =  IsSameType<T, GMetaSelf>::Result || IsConst };
};


template <typename T, bool isConst>
struct ConvertReference
{
	typedef T & ObjectReference;

	typedef T & SelfReference;
};

template <typename T>
struct ConvertReference <T, true>
{
	typedef const T & ObjectReference;

	// can't use const T &, because converting from GVariant to const T & will cause object copy and pass wrong "this" object.
	typedef T & SelfReference;
};

inline GVariant convertSelf(const GVariant & param)
{
	if(vtIsByPointer(param.getType())) {
		return pointerToRefVariant(param);
	}
	else {
		return param;
	}
}

template <typename OT, typename Executer, bool firstSelf, bool secondSelf>
struct MetaBinaryOperator
{
	GASSERT_STATIC(sizeof(OT) == 0);
};

template <typename OT, typename Executer>
struct MetaBinaryOperator <OT, Executer, true, false>
{
	static GVariant invoke(const GVariant & left, const GVariant & right) {
		return Executer::template invoke<typename ConvertReference<OT, Executer::IsConst0>::ObjectReference, typename Executer::FunctionTraits::ArgList::Arg1>
			(fromVariant<typename ConvertReference<OT, Executer::IsConst0>::SelfReference>(convertSelf(left)), fromVariant<typename Executer::FunctionTraits::ArgList::Arg1>(right));
	}
};

template <typename OT, typename Executer>
struct MetaBinaryOperator <OT, Executer, false, true>
{
	static GVariant invoke(const GVariant & left, const GVariant & right) {
		return Executer::template invoke<typename Executer::FunctionTraits::ArgList::Arg0, typename ConvertReference<OT, Executer::IsConst1>::ObjectReference>
			(fromVariant<typename Executer::FunctionTraits::ArgList::Arg0>(left), fromVariant<typename ConvertReference<OT, Executer::IsConst1>::SelfReference>(convertSelf(right)));
	}
};

template <typename OT, typename Executer>
struct MetaBinaryOperator <OT, Executer, false, false>
{
	static GVariant invoke(const GVariant & left, const GVariant & right) {
		return Executer::template invoke<typename Executer::FunctionTraits::ArgList::Arg0, typename Executer::FunctionTraits::ArgList::Arg1>
			(fromVariant<typename Executer::FunctionTraits::ArgList::Arg0>(left), fromVariant<typename Executer::FunctionTraits::ArgList::Arg1>(right));
	}
};


template <typename OT, typename Executer, bool isSelf>
struct MetaUnaryOperator;

template <typename OT, typename Executer>
struct MetaUnaryOperator <OT, Executer, true>
{
	static GVariant invoke(const GVariant & param) {
		return Executer::template invoke<typename ConvertReference<OT, Executer::IsConst0>::ObjectReference>
			(fromVariant<typename ConvertReference<OT, Executer::IsConst0>::SelfReference>(convertSelf(param)));
	}
};

template <typename OT, typename Executer>
struct MetaUnaryOperator <OT, Executer, false>
{
	static GVariant invoke(const GVariant & param) {
		return Executer::template invoke<typename Executer::FunctionTraits::ArgList::Arg0>
			(fromVariant<typename Executer::FunctionTraits::ArgList::Arg0>(param));
	}
};

template <typename FT>
struct MetaBinaryOperatorExecuter
{
	typedef FT FunctionTraits;
	enum {
		IsConst0 = CheckOperatorSelf<typename FT::ArgList::Arg0>::IsConst,
		IsConst1 = CheckOperatorSelf<typename FT::ArgList::Arg1>::IsConst
	};
};

template <typename FT>
struct MetaUnaryOperatorExecuter
{
	typedef FT FunctionTraits;
	enum {
		IsConst0 = CheckOperatorSelf<typename FT::ArgList::Arg0>::IsConst
	};
};

template <GMetaOpType Op, typename FT, typename EnableIf = void>
struct MetaOperatorExecuter;

#define DEF_BINARY_FULL(OP, EXP) \
	template <typename FT> struct MetaOperatorExecuter <OP, FT, typename GEnableIf<FT::HasResult>::Result> : public MetaBinaryOperatorExecuter<FT>	{ \
		template <typename P0, typename P1> static GVariant invoke(P0 p0, P1 p1) { \
			GVarTypeData data; \
			deduceVariantType<typename FT::ResultType>(data, true); \
			return GVariant(data, EXP); \
	} }; \
	template <typename FT> struct MetaOperatorExecuter <OP, FT, typename GEnableIf<! FT::HasResult>::Result> : public MetaBinaryOperatorExecuter<FT> { \
		template <typename P0, typename P1> static GVariant invoke(P0 p0, P1 p1) { \
			EXP; return GVariant(); \
	} };

#define DEF_BINARY(OP, SYM) DEF_BINARY_FULL(OP, p0 SYM p1)

DEF_BINARY(mopAdd, +)
DEF_BINARY(mopSub, -)
DEF_BINARY(mopMul, *)
DEF_BINARY(mopDiv, /)
DEF_BINARY(mopMod, %)

DEF_BINARY(mopAddAssign, +=)
DEF_BINARY(mopSubAssign, -=)
DEF_BINARY(mopMulAssign, *=)
DEF_BINARY(mopDivAssign, /=)
DEF_BINARY(mopModAssign, %=)

DEF_BINARY(mopAssign, =)

DEF_BINARY(mopEqual, ==)
DEF_BINARY(mopNotEqual, !=)
DEF_BINARY(mopGreater, >)
DEF_BINARY(mopLessor, <)
DEF_BINARY(mopGreaterEqual, >=)
DEF_BINARY(mopLessorEqual, <=)

DEF_BINARY(mopLogicAnd, &&)
DEF_BINARY(mopLogicOr, ||)

DEF_BINARY(mopBitAnd, &)
DEF_BINARY(mopBitOr, |)
DEF_BINARY(mopBitXor, ^)
DEF_BINARY(mopBitLeftShift, <<)
DEF_BINARY(mopBitRightShift, >>)

DEF_BINARY(mopBitAndAssign, &=)
DEF_BINARY(mopBitOrAssign, |=)
DEF_BINARY(mopBitXorAssign, ^=)
DEF_BINARY(mopBitLeftShiftAssign, <<=)
DEF_BINARY(mopBitRightShiftAssign, >>=)

DEF_BINARY_FULL(mopComma, (p0, p1))

DEF_BINARY_FULL(mopSubscript, p0[p1])

//DEF_BINARY(mopMember, ->)
DEF_BINARY(mopPointerMember, ->*)

#undef DEF_BINARY
#undef DEF_BINARY_FULL


#define DEF_UNARY(OP, EXP) \
	template <typename FT> struct MetaOperatorExecuter <OP, FT> : public MetaUnaryOperatorExecuter<FT> { \
		template <typename P0> static GVariant invoke(P0 p) { \
			GVarTypeData data; \
			deduceVariantType<typename FT::ResultType>(data, true); \
			return GVariant(data, EXP); \
	} }; \
	template <typename FT> struct MetaOperatorExecuter <OP, FT, typename GEnableIf<! FT::HasResult>::Result> : public MetaUnaryOperatorExecuter<FT> { \
		template <typename P0> static GVariant invoke(P0 p) { \
			EXP; return GVariant(); \
	} };

DEF_UNARY(mopPlus, +p)
DEF_UNARY(mopNeg, -p)

DEF_UNARY(mopPreInc, ++p)
DEF_UNARY(mopPostInc, p++)
DEF_UNARY(mopPreDec, --p)
DEF_UNARY(mopPostDec, p--)

DEF_UNARY(mopLogicNot, !p)

DEF_UNARY(mopBitNot, ~p)

DEF_UNARY(mopAddress, &p)
DEF_UNARY(mopDerefer, *p)

DEF_UNARY(mopCast, (typename FT::ResultType)(p))

DEF_UNARY(mopMember, p.operator->())

#undef DEF_UNARY


class GMetaOperatorDataBase
{
public:
	virtual ~GMetaOperatorDataBase() {
	}

	virtual GMetaOpType getOperator() const = 0;
	virtual size_t getParamCount() const = 0;
	
	virtual bool isParamSelf(size_t paramIndex) const = 0;
	virtual GMetaType getParamType(size_t index) const = 0;
	virtual bool hasResult() const = 0;
	virtual GMetaType getResultType() const = 0;
	virtual bool isVariadic() const = 0;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const = 0;

	virtual GMetaType createOperatorMetaType() const = 0;
	
	virtual GVariant invoke(const GVariant & p0) const {
		(void)p0;

		meta_internal::handleError(metaError_WrongArity, meta_internal::formatString("Wrong argument count. Expect: 2.")); // yes, expect 2

		return GVariant();
	}

	virtual GVariant invoke(const GVariant & p0, const GVariant & p1) const {
		(void)p0; (void)p1;

		meta_internal::handleError(metaError_WrongArity, meta_internal::formatString("Wrong argument count. Expect: 1.")); // yes, expect 1

		return GVariant();
	}

	virtual GVariant invokeFunctor(void * instance, GVariant const * const * params, size_t paramCount) const {
		(void)instance; (void)params; (void)paramCount;

		meta_internal::handleError(metaError_WrongArity, meta_internal::formatString("Invoke functor on a non-functor operator."));

		return GVariant();
	}

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const {
		(void)instance; (void)params; (void)paramCount;

		meta_internal::handleError(metaError_WrongArity, meta_internal::formatString("Execute on a non-functor operator."));

		return GVariant();
	}

	virtual bool isParamTransferOwnership(size_t paramIndex) const = 0;
	virtual bool isResultTransferOwnership() const = 0;
	virtual GMetaConverter * createResultConverter() const = 0;
};

inline void operatorIndexOutOfBound(size_t index, size_t maxIndex)
{
	(void)index;
	(void)maxIndex;

	meta_internal::handleError(metaError_ParamOutOfIndex, meta_internal::formatString("Index out of bound."));
}

template <typename OT, GMetaOpType Op, typename Signature, typename Policy, typename Enabled = void>
class GMetaOperatorData;

template <typename OT, GMetaOpType Op, typename Signature, typename Policy>
class GMetaOperatorData <OT, Op, Signature, Policy, typename GEnableIf<IsBinaryOperator<Op>::Result>::Result>
	: public GMetaOperatorDataBase
{
private:
	typedef GFunctionTraits<Signature> FT;

	GASSERT_STATIC(FT::Arity == 2);

public:
	virtual GMetaOpType getOperator() const {
		return Op;
	}

	virtual size_t getParamCount() const {
		return 2;
	}

	virtual bool isParamSelf(size_t paramIndex) const {
		switch(paramIndex) {
		case 0:
			return CheckOperatorSelf<typename FT::ArgList::Arg0>::IsSelf;

		case 1:
			return CheckOperatorSelf<typename FT::ArgList::Arg1>::IsSelf;
		}

		operatorIndexOutOfBound(paramIndex, 2);

		return false;
	}

	virtual GMetaType getParamType(size_t index) const {
		switch(index) {
		case 0:
			return createMetaType<typename FT::ArgList::Arg0>();

		case 1:
			return createMetaType<typename FT::ArgList::Arg1>();
		}

		operatorIndexOutOfBound(index, 2);

		return GMetaType();
	}

	virtual bool hasResult() const {
		return ! IsVoid<typename FT::ResultType>::Result;
	}

	virtual GMetaType getResultType() const {
		return createMetaType<typename FT::ResultType>();
	}

	virtual bool isVariadic() const {
		return false;
	}

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const {
		switch(paramIndex) {
		case 0:
			return canFromVariant<typename FT::ArgList::Arg0>(param);

		case 1:
			return canFromVariant<typename FT::ArgList::Arg1>(param);

		default:
			return false;
		}
	}

	virtual GMetaType createOperatorMetaType() const {
		return createMetaType<typename GFunctionTraits<Signature>::FullType>();
	}

	virtual GVariant invoke(const GVariant & p0, const GVariant & p1) const {
		return MetaBinaryOperator<OT,
			MetaOperatorExecuter<Op, FT>,
			CheckOperatorSelf<typename FT::ArgList::Arg0>::IsSelf,
			CheckOperatorSelf<typename FT::ArgList::Arg1>::IsSelf
		>::invoke(p0, p1);
	}

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const {
		(void)instance;

		if(paramCount != this->getParamCount()) {
			meta_internal::handleError(metaError_WrongArity, meta_internal::formatString("Wrong argument count. Expect: %d, but get: %d.", this->getParamCount(), paramCount));
		}

		return this->invoke(params[0], params[1]);
	}

	virtual bool isParamTransferOwnership(size_t paramIndex) const {
		return hasIndexedPolicy<Policy, GMetaPolicyItemTransferOwnership>(static_cast<int>(paramIndex));
	}

	virtual bool isResultTransferOwnership() const {
		return hasIndexedPolicy<Policy, GMetaPolicyItemTransferOwnership>(-1);
	}

	virtual GMetaConverter * createResultConverter() const {
		return GMetaConverterTraits<typename FT::ResultType>::createConverter();
	}
};


template <typename OT, GMetaOpType Op, typename Signature, typename Policy>
class GMetaOperatorData <OT, Op, Signature, Policy, typename GEnableIf<IsUnaryOperator<Op>::Result>::Result>
	: public GMetaOperatorDataBase
{
private:
	typedef GFunctionTraits<Signature> FT;

	GASSERT_STATIC(FT::Arity == 1);

public:
	virtual GMetaOpType getOperator() const {
		return Op;
	}

	virtual size_t getParamCount() const {
		return 1;
	}

	virtual bool isParamSelf(size_t paramIndex) const {
		switch(paramIndex) {
		case 0:
			return CheckOperatorSelf<typename FT::ArgList::Arg0>::IsSelf;
		}

		operatorIndexOutOfBound(paramIndex, 1);

		return false;
	}

	virtual GMetaType getParamType(size_t index) const {
		switch(index) {
		case 0:
			return createMetaType<typename FT::ArgList::Arg0>();
		}

		operatorIndexOutOfBound(index, 1);

		return GMetaType();
	}

	virtual bool hasResult() const {
		return ! IsVoid<typename FT::ResultType>::Result;
	}

	virtual GMetaType getResultType() const {
		return createMetaType<typename FT::ResultType>();
	}

	virtual bool isVariadic() const {
		return false;
	}

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const {
		switch(paramIndex) {
		case 0:
			return canFromVariant<typename FT::ArgList::Arg0>(param);

		default:
			return false;
		}
	}

	virtual GMetaType createOperatorMetaType() const {
		return createMetaType<typename GFunctionTraits<Signature>::FullType>();
	}

	virtual GVariant invoke(const GVariant & p0) const {
		return MetaUnaryOperator<OT,
			MetaOperatorExecuter<Op, FT>,
			CheckOperatorSelf<typename FT::ArgList::Arg0>::IsSelf
		>::invoke(p0);
	}

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const {
		(void)instance;

		if(paramCount != this->getParamCount()) {
			meta_internal::handleError(metaError_WrongArity, meta_internal::formatString("Wrong argument count. Expect: %d, but get: %d.", this->getParamCount(), paramCount));
		}

		return this->invoke(params[0]);
	}

	virtual bool isParamTransferOwnership(size_t paramIndex) const {
		return hasIndexedPolicy<Policy, GMetaPolicyItemTransferOwnership>(static_cast<int>(paramIndex));
	}

	virtual bool isResultTransferOwnership() const {
		return hasIndexedPolicy<Policy, GMetaPolicyItemTransferOwnership>(-1);
	}

	virtual GMetaConverter * createResultConverter() const {
		return GMetaConverterTraits<typename FT::ResultType>::createConverter();
	}

};

template <typename OT, GMetaOpType Op, typename Signature, typename Policy>
class GMetaOperatorData <OT, Op, Signature, Policy, typename GEnableIf<IsFunctorOperator<Op>::Result>::Result>
	: public GMetaOperatorDataBase
{
private:
	typedef GFunctionTraits<Signature> FT;

public:
	virtual GMetaOpType getOperator() const {
		return Op;
	}

	virtual size_t getParamCount() const {
		return FT::Arity;
	}

	virtual bool isParamSelf(size_t paramIndex) const {
		(void)paramIndex;

		return false;
	}

	virtual GMetaType getParamType(size_t index) const {
		if(index < FT::Arity) {
			switch(index) {
#define REF_GETPARAM_HELPER(N, unused) \
	case N: return createMetaType<typename TypeList_GetWithDefault<typename FT::ArgTypeList, N>::Result>();

			GPP_REPEAT(REF_MAX_ARITY, REF_GETPARAM_HELPER, GPP_EMPTY)

#undef REF_GETPARAM_HELPER
			}
		}
		
		operatorIndexOutOfBound(index, FT::Arity);
		return GMetaType();
	}

	virtual bool hasResult() const {
		return ! IsVoid<typename FT::ResultType>::Result;
	}

	virtual GMetaType getResultType() const {
		return createMetaType<typename FT::ResultType>();
	}

	virtual bool isVariadic() const {
		return IsVariadicFunction<FT>::Result;
	}

#define REF_CHECKPARAM_HELPER(N, unused) \
	case N: return canFromVariant<typename TypeList_GetWithDefault<typename FT::ArgTypeList, N>::Result>(param);

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const {
		if(paramIndex >= this->getParamCount()) {
			return false;
		}

		switch(paramIndex) {
			GPP_REPEAT(REF_MAX_ARITY, REF_CHECKPARAM_HELPER, GPP_EMPTY)

			default:
				handleError(metaError_ParamOutOfIndex, "Parameter out of index");
				return false;
		}
	}
#undef REF_CHECKPARAM_HELPER

	virtual GMetaType createOperatorMetaType() const {
		return createMetaType<typename GFunctionTraits<Signature>::FullType>();
	}

	virtual GVariant invoke(const GVariant & p0) const {
		const GVariant * params[1];

		return this->invokeFunctor(fromVariant<void *>(p0), params, 0);
	}

	virtual GVariant invoke(const GVariant & p0, const GVariant & p1) const {
		const GVariant * params[1];

		params[0] = &p1;

		return this->invokeFunctor(fromVariant<void *>(p0), params, 1);
	}

	virtual GVariant invokeFunctor(void * instance, GVariant const * const * params, size_t paramCount) const {
		if(!this->isVariadic() && paramCount != this->getParamCount()) {
			meta_internal::handleError(metaError_WrongArity, meta_internal::formatString("Wrong argument count. Expect: %d, but get: %d.", this->getParamCount(), paramCount));
		}

		return GMetaMethodCallHelper<OT, FT, FT::Arity, typename FT::ResultType, Policy, IsVariadicFunction<FT>::Result>::invoke(*static_cast<OT *>(instance), params, paramCount);
	}

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const {
		GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");
		
		if(!this->isVariadic() && paramCount != this->getParamCount()) {
			meta_internal::handleError(metaError_WrongArity, meta_internal::formatString("Wrong argument count. Expect: %d, but get: %d.", this->getParamCount(), paramCount));
		}

		const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

		for(size_t i = 0; i < paramCount; ++i) {
			variantPointers[i] = &params[i];
		}

		return this->invokeFunctor(instance, variantPointers, paramCount);
	}

	virtual bool isParamTransferOwnership(size_t paramIndex) const {
		return hasIndexedPolicy<Policy, GMetaPolicyItemTransferOwnership>(static_cast<int>(paramIndex));
	}

	virtual bool isResultTransferOwnership() const {
		return hasIndexedPolicy<Policy, GMetaPolicyItemTransferOwnership>(-1);
	}

	virtual GMetaConverter * createResultConverter() const {
		return GMetaConverterTraits<typename FT::ResultType>::createConverter();
	}

};


template <GMetaOpType Op>
struct GMetaOperatorCreator
{
	template <typename OT, typename Signature, typename Policy>
	GMetaOperatorDataBase * create(const Policy &) const {
		return new GMetaOperatorData<OT, Op, Signature, Policy>();
	}

	GMetaOpType getOperator() const {
		return Op;
	}

	operator GMetaOpType() const {
		return Op;
	}
};

struct GMetaOperatorHolder
{
#define HOLD_BINARY(OP, SYM) GMetaOperatorCreator<OP> operator SYM (const GMetaOperatorHolder &) const { return GMetaOperatorCreator<OP>(); }

	HOLD_BINARY(mopAdd, +)
	HOLD_BINARY(mopSub, -)
	HOLD_BINARY(mopMul, *)
	HOLD_BINARY(mopDiv, /)
	HOLD_BINARY(mopMod, %)

	HOLD_BINARY(mopAddAssign, +=)
	HOLD_BINARY(mopSubAssign, -=)
	HOLD_BINARY(mopMulAssign, *=)
	HOLD_BINARY(mopDivAssign, /=)
	HOLD_BINARY(mopModAssign, %=)

	HOLD_BINARY(mopAssign, =)

	HOLD_BINARY(mopEqual, ==)
	HOLD_BINARY(mopNotEqual, !=)
	HOLD_BINARY(mopGreater, >)
	HOLD_BINARY(mopLessor, <)
	HOLD_BINARY(mopGreaterEqual, >=)
	HOLD_BINARY(mopLessorEqual, <=)

	HOLD_BINARY(mopLogicAnd, &&)
	HOLD_BINARY(mopLogicOr, ||)

	HOLD_BINARY(mopBitAnd, &)
	HOLD_BINARY(mopBitOr, |)
	HOLD_BINARY(mopBitXor, ^)
	HOLD_BINARY(mopBitLeftShift, <<)
	HOLD_BINARY(mopBitRightShift, >>)

	HOLD_BINARY(mopBitAndAssign, &=)
	HOLD_BINARY(mopBitOrAssign, |=)
	HOLD_BINARY(mopBitXorAssign, ^=)
	HOLD_BINARY(mopBitLeftShiftAssign, <<=)
	HOLD_BINARY(mopBitRightShiftAssign, >>=)

	HOLD_BINARY(mopComma, GPP_COMMA())

	HOLD_BINARY(mopSubscript, [])
	GMetaOperatorCreator<mopSubscript> operator [](int) const { return GMetaOperatorCreator<mopSubscript>(); }

	GMetaOperatorCreator<mopMember> mopHolder;
	const GMetaOperatorHolder * operator ->() const { return this; }

	GMetaOperatorCreator<mopPointerMember> operator ->*(const GMetaOperatorHolder &) const { return GMetaOperatorCreator<mopPointerMember>(); }

#undef HOLD_BINARY

#define HOLD_UNARY_FULL(OP, SYM, PARAM) GMetaOperatorCreator<OP> operator SYM (PARAM) const { return GMetaOperatorCreator<OP>(); }
#define HOLD_UNARY(OP, SYM) HOLD_UNARY_FULL(OP, SYM, GPP_EMPTY())

	HOLD_UNARY(mopPlus, +)
	HOLD_UNARY(mopNeg, -)

	HOLD_UNARY(mopPreInc, ++)
	HOLD_UNARY_FULL(mopPostInc, ++, int)
	HOLD_UNARY(mopPreDec, --)
	HOLD_UNARY_FULL(mopPostDec, --, int)

	HOLD_UNARY(mopLogicNot, !)

	HOLD_UNARY(mopBitNot, ~)

	HOLD_UNARY(mopAddress, &)
	HOLD_UNARY(mopDerefer, *)

	GMetaOperatorCreator<mopCast> operator () () const { return GMetaOperatorCreator<mopCast>(); }

#undef HOLD_UNARY
#undef HOLD_UNARY_FULL

	GMetaOperatorCreator<mopFunctor> operator () (const GMetaOperatorHolder &) const { return GMetaOperatorCreator<mopFunctor>(); }

};

std::string operatorToName(GMetaOpType op);

} // namespace meta_internal


const meta_internal::GMetaOperatorHolder mopHolder = meta_internal::GMetaOperatorHolder();


#define VAR_PARAM_DEFAULT(N, unused) GPP_COMMA_IF(N) const GVariant & p ## N = GVariant()
#define FUNCTOR_LOAD_PARAM(N, unused) params[index++] = & p ## N;

class GMetaOperator : public GMetaCallable
{
private:
	typedef GMetaCallable super;

public:
	GMetaOperator(meta_internal::GMetaOperatorDataBase * baseData)
		: super(meta_internal::operatorToName(baseData->getOperator()).c_str(), baseData->createOperatorMetaType(), mcatOperator), baseData(baseData) {
	}

	GMetaOpType getOperator() const;

	virtual size_t getParamCount() const;
	virtual GMetaType getParamType(size_t index) const;
	virtual bool hasResult() const;
	virtual GMetaType getResultType() const;
	virtual bool isVariadic() const;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const;

	virtual bool isParamTransferOwnership(size_t paramIndex) const;
	virtual bool isResultTransferOwnership() const;
	
	virtual GMetaConverter * createResultConverter() const;

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const;

	GVariant invokeUnary(const GVariant & p0) const;
	GVariant invokeBinary(const GVariant & p0, const GVariant & p1) const;
	GVariant invokeFunctor(const GVariant & instance, GPP_REPEAT(REF_MAX_ARITY, VAR_PARAM_DEFAULT, GPP_EMPTY)) const;

private:
	GScopedPointer<meta_internal::GMetaOperatorDataBase> baseData;
};

#undef VAR_PARAM_DEFAULT
#undef FUNCTOR_LOAD_PARAM


void globalAddOperator(GMetaOperator * metaOperator);

template <typename Signature, typename Creator>
void reflectOperator(const Creator & creator)
{
	globalAddOperator(new cpgf::GMetaOperator(creator.template create<void, Signature>(GMetaPolicyDefault())));
}

template <typename Signature, typename Creator, typename Policy>
void reflectOperator(const Creator & creator, const Policy & policy)
{
	globalAddOperator(new cpgf::GMetaOperator(creator.template create<void, Signature>(policy)));
}



} // namespace cpgf



#endif
