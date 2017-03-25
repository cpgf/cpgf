#ifndef CPGF_GMETAOPERATOR_P_H
#define CPGF_GMETAOPERATOR_P_H

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif

namespace meta_internal {

template <GMetaOpType op>
struct IsBinaryOperator
{
	G_STATIC_CONSTANT(bool, Result = (op >= mopBinaryFirst && op <= mopBinaryLast));
};

template <GMetaOpType op>
struct IsUnaryOperator
{
	G_STATIC_CONSTANT(bool, Result = (op >= mopUnaryFirst && op <= mopUnaryLast));
};

template <GMetaOpType op>
struct IsFunctorOperator
{
	G_STATIC_CONSTANT(bool, Result = (op == mopFunctor));
};

template <typename T>
struct CheckOperatorSelf
{
	G_STATIC_CONSTANT(bool, IsConst = (IsSameType<T, const GMetaSelf &>::Result));
	G_STATIC_CONSTANT(bool, IsSelf = (IsSameType<T, GMetaSelf>::Result || IsConst));
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
		return variantPointerToLvalueReference(param);
	}
	else {
		return param;
	}
}

template <typename OT, typename Executer, typename Policy, bool firstSelf, bool secondSelf>
struct MetaBinaryOperator
{
	GASSERT_STATIC(sizeof(OT) == 0);
};

template <typename OT, typename Executer, typename Policy>
struct MetaBinaryOperator <OT, Executer, Policy, true, false>
{
	static GVariant invoke(const GVariant & left, const GVariant & right) {
		return Executer::template invoke<typename ConvertReference<OT, Executer::IsConst0>::ObjectReference, typename Executer::FunctionTraits::ArgList::Arg1>
			(
				fromVariant<typename ConvertReference<OT, Executer::IsConst0>::SelfReference, typename SelectFromVariantPolicy<Policy, 0>::Result>(convertSelf(left)),
				fromVariant<typename Executer::FunctionTraits::ArgList::Arg1, typename SelectFromVariantPolicy<Policy, 1>::Result>(right)
			);
	}
};

template <typename OT, typename Executer, typename Policy>
struct MetaBinaryOperator <OT, Executer, Policy, false, true>
{
	static GVariant invoke(const GVariant & left, const GVariant & right) {
		return Executer::template invoke<typename Executer::FunctionTraits::ArgList::Arg0, typename ConvertReference<OT, Executer::IsConst1>::ObjectReference>
			(
				fromVariant<typename Executer::FunctionTraits::ArgList::Arg0, typename SelectFromVariantPolicy<Policy, 0>::Result>(left),
				fromVariant<typename ConvertReference<OT, Executer::IsConst1>::SelfReference, typename SelectFromVariantPolicy<Policy, 1>::Result>(convertSelf(right))
			);
	}
};

template <typename OT, typename Executer, typename Policy>
struct MetaBinaryOperator <OT, Executer, Policy, false, false>
{
	static GVariant invoke(const GVariant & left, const GVariant & right) {
		return Executer::template invoke<typename Executer::FunctionTraits::ArgList::Arg0, typename Executer::FunctionTraits::ArgList::Arg1>
			(
				fromVariant<typename Executer::FunctionTraits::ArgList::Arg0, typename SelectFromVariantPolicy<Policy, 0>::Result>(left),
				fromVariant<typename Executer::FunctionTraits::ArgList::Arg1, typename SelectFromVariantPolicy<Policy, 1>::Result>(right)
			);
	}
};


template <typename OT, typename Executer, typename Policy, bool isSelf>
struct MetaUnaryOperator;

template <typename OT, typename Executer, typename Policy>
struct MetaUnaryOperator <OT, Executer, Policy, true>
{
	static GVariant invoke(const GVariant & param) {
		return Executer::template invoke<typename ConvertReference<OT, Executer::IsConst0>::ObjectReference>
			(fromVariant<typename ConvertReference<OT, Executer::IsConst0>::SelfReference, typename SelectFromVariantPolicy<Policy, 0>::Result>(convertSelf(param)));
	}
};

template <typename OT, typename Executer, typename Policy>
struct MetaUnaryOperator <OT, Executer, Policy, false>
{
	static GVariant invoke(const GVariant & param) {
		return Executer::template invoke<typename Executer::FunctionTraits::ArgList::Arg0>
			(fromVariant<typename Executer::FunctionTraits::ArgList::Arg0, typename SelectFromVariantPolicy<Policy, 0>::Result>(param));
	}
};

template <typename FT>
struct MetaBinaryOperatorExecuter
{
	typedef FT FunctionTraits;

	G_STATIC_CONSTANT(bool, IsConst0 = (CheckOperatorSelf<typename FT::ArgList::Arg0>::IsConst));
	G_STATIC_CONSTANT(bool, IsConst1 = (CheckOperatorSelf<typename FT::ArgList::Arg1>::IsConst));
};

template <typename FT>
struct MetaUnaryOperatorExecuter
{
	typedef FT FunctionTraits;

	G_STATIC_CONSTANT(bool, IsConst0 = (CheckOperatorSelf<typename FT::ArgList::Arg0>::IsConst));
};

template <typename FT>
struct CheckHasResult
{
	G_STATIC_CONSTANT(bool, Result = FT::HasResult);
};

template <GMetaOpType Op, typename FT, typename Policy, typename EnableIf = void>
struct MetaOperatorExecuter;

#define DEF_BINARY_FULL(OP, EXP) \
	template <typename FT, typename Policy> struct MetaOperatorExecuter <OP, FT, Policy, typename GEnableIfResult<CheckHasResult<FT> >::Result> : public MetaBinaryOperatorExecuter<FT>	{ \
		template <typename P0, typename P1> static GVariant invoke(P0 p0, P1 p1) { \
			return createVariant<typename FT::ResultType>((const typename std::remove_reference<typename FT::ResultType>::type &)(EXP), ! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result); \
	} }; \
	template <typename FT, typename Policy> struct MetaOperatorExecuter <OP, FT, Policy, typename GDisableIfResult<CheckHasResult<FT> >::Result> : public MetaBinaryOperatorExecuter<FT> { \
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
	template <typename FT, typename Policy> struct MetaOperatorExecuter <OP, FT, Policy> : public MetaUnaryOperatorExecuter<FT> { \
		template <typename P0> static GVariant invoke(P0 p) { \
			return createVariant<typename FT::ResultType>((const typename std::remove_reference<typename FT::ResultType>::type &)(EXP), ! PolicyHasRule<Policy, GMetaRuleParamNoncopyable<metaPolicyResultIndex> >::Result); \
	} }; \
	template <typename FT, typename Policy> struct MetaOperatorExecuter <OP, FT, Policy, typename GDisableIfResult<CheckHasResult<FT> >::Result> : public MetaUnaryOperatorExecuter<FT> { \
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

DEF_UNARY(mopAddress, p.operator&())
DEF_UNARY(mopDerefer, *p)

DEF_UNARY(mopCast, (typename FT::ResultType)(p))

DEF_UNARY(mopMember, p.operator->())

#undef DEF_UNARY


struct GMetaOperatorDataVirtual
{
	void (*deleteSelf)(void * self);
	
	GVariant (*invoke)(const GVariant & p0);
	GVariant (*invoke2)(const GVariant & p0, const GVariant & p1);
	GVariant (*invokeFunctor)(void * instance, GVariant const * const * params, size_t paramCount);
	GVariant (*execute)(const void * self, void * instance, const GVariant * params, size_t paramCount);
	GVariant (*executeByData)(const void * self, void * instance, const GVariantData * * params, size_t paramCount);

	GMetaOpType (*getOperator)();
	size_t (*getParamCount)();
	bool (*hasResult)();
	bool (*isParamSelf)(size_t paramIndex);
	GMetaType (*getParamType)(size_t index);
	GMetaType (*getResultType)();
	GMetaExtendType (*getResultExtendType)(uint32_t flags);
	GMetaExtendType (*getParamExtendType)(uint32_t flags, size_t index);
	bool (*isVariadic)();
	bool (*isExplicitThis)();
	bool (*checkParam)(const GVariant & param, size_t paramIndex);
	bool (*isParamTransferOwnership)(size_t paramIndex);
	bool (*isResultTransferOwnership)();
	GMetaType (*createOperatorMetaType)();
	GMetaExtendType (*getItemExtendType)(uint32_t flags, const GMetaItem * metaItem);
};

class GMetaOperatorDataBase
{
public:
	void deleteSelf();

	GMetaOpType getOperator() const;

	size_t getParamCount() const;
	bool isParamSelf(size_t paramIndex) const;
	GMetaType getParamType(size_t paramIndex) const;

	bool hasResult() const;
	GMetaType getResultType() const;
	
	GMetaExtendType getResultExtendType(uint32_t flags) const {
		return this->virtualFunctions->getResultExtendType(flags);
	}

	GMetaExtendType getParamExtendType(uint32_t flags, size_t index) const {
		return this->virtualFunctions->getParamExtendType(flags, index);
	}

	bool isVariadic() const;
	bool isExplicitThis() const;

	bool checkParam(const GVariant & param, size_t paramIndex) const;

	GMetaType createOperatorMetaType() const;

	GVariant invoke(const GVariant & p0) const;
	GVariant invoke(const GVariant & p0, const GVariant & p1) const;
	GVariant invokeFunctor(void * instance, GVariant const * const * params, size_t paramCount) const;

	GVariant execute(void * instance, const GVariant * params, size_t paramCount) const;
	GVariant executeByData(void * instance, const GVariantData * * params, size_t paramCount) const;

	bool isParamTransferOwnership(size_t paramIndex) const;
	bool isResultTransferOwnership() const;

	// must be defined in header to make template function overloading happy.
	GMetaExtendType getItemExtendType(uint32_t flags, const GMetaItem * metaItem) const {
		return this->virtualFunctions->getItemExtendType(flags, metaItem);
	}
	
	GMetaDefaultParamList * getDefaultParamList() const;
	bool hasDefaultParam() const;

protected:
	GMetaOperatorDataVirtual * virtualFunctions;

private:
	mutable GScopedPointer<GMetaDefaultParamList> defaultParamList;
};


void operatorIndexOutOfBound(size_t index, size_t maxIndex);

template <typename OT, GMetaOpType Op, typename Signature, typename Policy, typename Enabled = void>
class GMetaOperatorData;

template <typename OT, GMetaOpType Op, typename Signature, typename Policy>
class GMetaOperatorData <OT, Op, Signature, Policy, typename GEnableIfResult<IsBinaryOperator<Op> >::Result>
	: public GMetaOperatorDataBase
{
private:
	typedef GMetaOperatorData <OT, Op, Signature, Policy, typename GEnableIfResult<IsBinaryOperator<Op> >::Result> ThisType;
	typedef GFunctionTraits<Signature> FT;

	GASSERT_STATIC(FT::Arity == 2);

private:
	static GMetaOpType virtualGetOperator() {
		return Op;
	}

	static size_t virtualGetParamCount() {
		return 2;
	}

	static bool virtualIsParamSelf(size_t paramIndex) {
		switch(paramIndex) {
		case 0:
			return CheckOperatorSelf<typename FT::ArgList::Arg0>::IsSelf;

		case 1:
			return CheckOperatorSelf<typename FT::ArgList::Arg1>::IsSelf;
		}

		operatorIndexOutOfBound(paramIndex, 2);

		return false;
	}

	static GMetaType virtualGetParamType(size_t index) {
		switch(index) {
		case 0:
			return createMetaType<typename FT::ArgList::Arg0>();

		case 1:
			return createMetaType<typename FT::ArgList::Arg1>();
		}

		operatorIndexOutOfBound(index, 2);

		return GMetaType();
	}

	static bool virtualHasResult() {
		return ! IsVoid<typename FT::ResultType>::Result;
	}

	static GMetaType virtualGetResultType() {
		return createMetaType<typename FT::ResultType>();
	}

	static GMetaExtendType virtualGetResultExtendType(uint32_t flags) {
		return createMetaExtendType<typename FT::ResultType>(flags);
	}
	
	static GMetaExtendType virtualGetParamExtendType(uint32_t flags, size_t index) {
		switch(index) {
		case 0:
			return createMetaExtendType<typename FT::ArgList::Arg0>(flags);

		case 1:
			return createMetaExtendType<typename FT::ArgList::Arg1>(flags);
		}

		operatorIndexOutOfBound(index, 2);

		return GMetaExtendType();
	}

	static bool virtualIsVariadic() {
		return false;
	}

	static bool virtualIsExplicitThis() {
		return false;
	}

	static bool virtualCheckParam(const GVariant & param, size_t paramIndex) {
		switch(paramIndex) {
		case 0:
			return canFromVariant<typename FT::ArgList::Arg0>(param);

		case 1:
			return canFromVariant<typename FT::ArgList::Arg1>(param);

		default:
			return false;
		}
	}

	static GMetaType virtualCreateOperatorMetaType() {
		return createMetaType<typename GFunctionTraits<Signature>::FullType>();
	}

	static GVariant virtualInvoke2(const GVariant & p0, const GVariant & p1) {
		return MetaBinaryOperator<OT,
			MetaOperatorExecuter<Op, FT, Policy>,
			Policy,
			CheckOperatorSelf<typename FT::ArgList::Arg0>::IsSelf,
			CheckOperatorSelf<typename FT::ArgList::Arg1>::IsSelf
		>::invoke(p0, p1);
	}

	static GVariant virtualExecute(const void * /*self*/, void * /*instance*/, const GVariant * params, size_t paramCount) {
		if(paramCount != virtualGetParamCount()) {
			raiseCoreException(Error_Meta_WrongArity, virtualGetParamCount(), paramCount);
		}

		return virtualInvoke2(params[0], params[1]);
	}

	static GVariant virtualExecuteByData(const void * /*self*/, void * /*instance*/, const GVariantData * * params, size_t paramCount) {
		if(paramCount != virtualGetParamCount()) {
			raiseCoreException(Error_Meta_WrongArity, virtualGetParamCount(), paramCount);
		}

		return virtualInvoke2(createVariantFromData(*params[0]), createVariantFromData(*params[1]));
	}

	static bool virtualIsParamTransferOwnership(size_t paramIndex) {
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(static_cast<int>(paramIndex));
	}

	static bool virtualIsResultTransferOwnership() {
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(metaPolicyResultIndex);
	}

	static GMetaExtendType virtualGetItemExtendType(uint32_t flags, const GMetaItem * metaItem)
	{
		return createMetaExtendType<FT>(flags, metaItem);
	}

public:
	GMetaOperatorData() {
		static GMetaOperatorDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<ThisType>,
			
			nullptr,
			&virtualInvoke2,
			nullptr,
			&virtualExecute,
			&virtualExecuteByData,

			&virtualGetOperator,
			&virtualGetParamCount,
			&virtualHasResult,
			&virtualIsParamSelf,
			&virtualGetParamType,
			&virtualGetResultType,
			&virtualGetResultExtendType,
			&virtualGetParamExtendType,
			&virtualIsVariadic,
			&virtualIsExplicitThis,
			&virtualCheckParam,
			&virtualIsParamTransferOwnership,
			&virtualIsResultTransferOwnership,
			&virtualCreateOperatorMetaType,
			&virtualGetItemExtendType
		};

		this->virtualFunctions = &thisFunctions;
	}
};


template <typename OT, GMetaOpType Op, typename Signature, typename Policy>
class GMetaOperatorData <OT, Op, Signature, Policy, typename GEnableIfResult<IsUnaryOperator<Op> >::Result>
	: public GMetaOperatorDataBase
{
private:
	typedef GMetaOperatorData <OT, Op, Signature, Policy, typename GEnableIfResult<IsUnaryOperator<Op> >::Result> ThisType;
	typedef GFunctionTraits<Signature> FT;

	GASSERT_STATIC(FT::Arity == 1);

private:
	static GMetaOpType virtualGetOperator() {
		return Op;
	}

	static size_t virtualGetParamCount() {
		return 1;
	}

	static bool virtualIsParamSelf(size_t paramIndex) {
		switch(paramIndex) {
		case 0:
			return CheckOperatorSelf<typename FT::ArgList::Arg0>::IsSelf;
		}

		operatorIndexOutOfBound(paramIndex, 1);

		return false;
	}

	static GMetaType virtualGetParamType(size_t index) {
		switch(index) {
		case 0:

			return createMetaType<typename FT::ArgList::Arg0>();
		}

		operatorIndexOutOfBound(index, 1);

		return GMetaType();
	}

	static bool virtualHasResult() {
		return ! IsVoid<typename FT::ResultType>::Result;
	}

	static GMetaType virtualGetResultType() {
		return createMetaType<typename FT::ResultType>();
	}

	static GMetaExtendType virtualGetResultExtendType(uint32_t flags) {
		return createMetaExtendType<typename FT::ResultType>(flags);
	}
	
	static GMetaExtendType virtualGetParamExtendType(uint32_t flags, size_t index) {
		switch(index) {
		case 0:
			return createMetaExtendType<typename FT::ArgList::Arg0>(flags);
		}

		operatorIndexOutOfBound(index, 1);

		return GMetaExtendType();
	}

	static bool virtualIsVariadic() {
		return false;
	}

	static bool virtualIsExplicitThis() {
		return false;
	}

	static bool virtualCheckParam(const GVariant & param, size_t paramIndex) {
		switch(paramIndex) {
		case 0:
			return canFromVariant<typename FT::ArgList::Arg0>(param);

		default:
			return false;
		}
	}

	static GMetaType virtualCreateOperatorMetaType() {
		return createMetaType<typename GFunctionTraits<Signature>::FullType>();
	}

	static GVariant virtualInvoke(const GVariant & p0) {
		return MetaUnaryOperator<OT,
			MetaOperatorExecuter<Op, FT, Policy>,
			Policy,
			CheckOperatorSelf<typename FT::ArgList::Arg0>::IsSelf
		>::invoke(p0);
	}

	static GVariant virtualExecute(const void * /*self*/, void * /*instance*/, const GVariant * params, size_t paramCount) {
		if(paramCount != virtualGetParamCount()) {
			raiseCoreException(Error_Meta_WrongArity, virtualGetParamCount(), paramCount);
		}

		return virtualInvoke(params[0]);
	}

	static GVariant virtualExecuteByData(const void * /*self*/, void * /*instance*/, const GVariantData * * params, size_t paramCount) {
		if(paramCount != virtualGetParamCount()) {
			raiseCoreException(Error_Meta_WrongArity, virtualGetParamCount(), paramCount);
		}

		return virtualInvoke(createVariantFromData(*params[0]));
	}

	static bool virtualIsParamTransferOwnership(size_t paramIndex) {
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(static_cast<int>(paramIndex));
	}

	static bool virtualIsResultTransferOwnership() {
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(metaPolicyResultIndex);
	}

	static GMetaExtendType virtualGetItemExtendType(uint32_t flags, const GMetaItem * metaItem)
	{
		return createMetaExtendType<FT>(flags, metaItem);
	}

public:
	GMetaOperatorData() {
		static GMetaOperatorDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<ThisType>,
			
			&virtualInvoke,
			nullptr,
			nullptr,
			&virtualExecute,
			&virtualExecuteByData,

			&virtualGetOperator,
			&virtualGetParamCount,
			&virtualHasResult,
			&virtualIsParamSelf,
			&virtualGetParamType,
			&virtualGetResultType,
			&virtualGetResultExtendType,
			&virtualGetParamExtendType,
			&virtualIsVariadic,
			&virtualIsExplicitThis,
			&virtualCheckParam,
			&virtualIsParamTransferOwnership,
			&virtualIsResultTransferOwnership,
			&virtualCreateOperatorMetaType,
			&virtualGetItemExtendType
		};

		this->virtualFunctions = &thisFunctions;
	}
};

template <typename OT, GMetaOpType Op, typename Signature, typename Policy>
class GMetaOperatorData <OT, Op, Signature, Policy, typename GEnableIfResult<IsFunctorOperator<Op> >::Result>
	: public GMetaOperatorDataBase
{
private:
	typedef GMetaOperatorData <OT, Op, Signature, Policy, typename GEnableIfResult<IsFunctorOperator<Op> >::Result> ThisType;
	typedef GFunctionTraits<Signature> FT;

private:
	static GMetaOpType virtualGetOperator() {
		return Op;
	}

	static size_t virtualGetParamCount() {
		size_t paramCount = FT::Arity;
		if(paramCount > 0 && virtualIsVariadic()) {
			--paramCount;
		}
		return paramCount;
	}

	static bool virtualIsParamSelf(size_t /*paramIndex*/) {
		return false;
	}

	template <unsigned int N>
	struct GetParamTypeSelector
	{
		template <typename TypeList>
		GMetaType operator()(const TypeList & /*typeList*/)
		{
			return createMetaType<typename TypeList_GetWithDefault<TypeList, N>::Result>();
		}
	};

	static GMetaType virtualGetParamType(size_t index) {
		if((int)index < static_cast<int>(FT::Arity)) {
			return GTypeSelector<FT::Arity>::template select<GMetaType, GetParamTypeSelector>(
				index,
				typename FT::ArgTypeList()
			);
		}

		operatorIndexOutOfBound(index, FT::Arity);
		return GMetaType();
	}

	static bool virtualHasResult() {
		return ! IsVoid<typename FT::ResultType>::Result;
	}

	static GMetaType virtualGetResultType() {
		return createMetaType<typename FT::ResultType>();
	}

	static GMetaExtendType virtualGetResultExtendType(uint32_t flags) {
		return createMetaExtendType<typename FT::ResultType>(flags);
	}

	template <typename TypeList>
	struct GetParamExtendTypeSelectorParam
	{
		uint32_t flags;
	};
	
	template <unsigned int N>
	struct GetParamExtendTypeSelector
	{
		template <typename TypeList>
		GMetaExtendType operator()(const GetParamExtendTypeSelectorParam<TypeList> & param)
		{
			return createMetaExtendType<typename TypeList_GetWithDefault<TypeList, N>::Result>(param.flags);
		}
	};

	static GMetaExtendType virtualGetParamExtendType(uint32_t flags, size_t index) {
		meta_internal::adjustParamIndex(index, PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result);
		return GTypeSelector<FT::Arity>::template select<GMetaExtendType, GetParamExtendTypeSelector>(
			index,
			GetParamExtendTypeSelectorParam<typename FT::ArgTypeList>{ flags }
		);
	}
	

	static bool virtualIsVariadic() {
		return IsVariadicFunction<FT>::Result;
	}

	static bool virtualIsExplicitThis() {
		return PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result;
	}

	template <typename TypeList>
	struct CheckParamSelectorParam
	{
		const GVariant & param;
	};
	
	template <unsigned int N>
	struct CheckParamSelector
	{
		template <typename TypeList>
		bool operator()(const CheckParamSelectorParam<TypeList> & param)
		{
			return canFromVariant<typename TypeList_GetWithDefault<TypeList, N>::Result>(param.param);
		}
	};

	static bool virtualCheckParam(const GVariant & param, size_t paramIndex) {
		if(virtualIsVariadic() && paramIndex >= virtualGetParamCount()) {
			return true;
		}
		else {
			if(paramIndex >= virtualGetParamCount()) {
				return false;
			}
		}

		return GTypeSelector<FT::Arity>::template select<bool, CheckParamSelector>(
			paramIndex,
			CheckParamSelectorParam<typename FT::ArgTypeList>{ param }
		);
	}

	static GMetaType virtualCreateOperatorMetaType() {
		return createMetaType<typename GFunctionTraits<Signature>::FullType>();
	}

	static GVariant virtualInvoke(const GVariant & p0) {
		return virtualInvokeFunctor(objectAddressFromVariant(p0), nullptr, 0);
	}

	static GVariant virtualInvoke2(const GVariant & p0, const GVariant & p1) {
		const GVariant * params = &p1;

		return virtualInvokeFunctor(objectAddressFromVariant(p0), &params, 1);
	}

	static GVariant virtualInvokeFunctor(void * instance, GVariant const * const * params, size_t paramCount) {
		checkInvokingArity(paramCount, virtualGetParamCount(), virtualIsVariadic());

		return GMetaInvokeHelper<OT,
			FT,
			FT::Arity,
			typename FT::ResultType,
			Policy,
			IsVariadicFunction<FT>::Result,
			PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result
		>::invoke(instance, *static_cast<OT *>(instance), params, paramCount);
	}

	static GVariant virtualInvokeFunctorByData(void * instance, GVariantData const * const * params, size_t paramCount) {
		checkInvokingArity(paramCount, virtualGetParamCount(), virtualIsVariadic());

		return GMetaInvokeHelper<OT,
			FT,
			FT::Arity,
			typename FT::ResultType,
			Policy,
			IsVariadicFunction<FT>::Result,
			PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result
		>::invokeByData(instance, *static_cast<OT *>(instance), params, paramCount);
	}

	static GVariant virtualExecute(const void * self, void * instance, const GVariant * params, size_t paramCount) {
		GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

		checkInvokingArity(paramCount, virtualGetParamCount(), virtualIsVariadic());

		const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

		for(size_t i = 0; i < paramCount; ++i) {
			variantPointers[i] = &params[i];
		}
		
		if(static_cast<const ThisType *>(self)->hasDefaultParam()) {
			static_cast<const ThisType *>(self)->getDefaultParamList()->loadDefaultParams(variantPointers, paramCount, virtualGetParamCount());
		}

		return virtualInvokeFunctor(instance, variantPointers, paramCount);
	}

	static GVariant virtualExecuteByData(const void * self, void * instance, const GVariantData * * params, size_t paramCount) {
		GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

		checkInvokingArity(paramCount, virtualGetParamCount(), virtualIsVariadic());

		if(static_cast<const ThisType *>(self)->hasDefaultParam()) {
			static_cast<const ThisType *>(self)->getDefaultParamList()->loadDefaultParamsByData(params, paramCount, virtualGetParamCount());
		}

		return virtualInvokeFunctorByData(instance, params, paramCount);
	}

	static bool virtualIsParamTransferOwnership(size_t paramIndex) {
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(static_cast<int>(paramIndex));
	}

	static bool virtualIsResultTransferOwnership() {
		return policyHasIndexedRule<Policy, GMetaRuleTransferOwnership>(metaPolicyResultIndex);
	}

	static GMetaExtendType virtualGetItemExtendType(uint32_t flags, const GMetaItem * metaItem)
	{
		return createMetaExtendType<FT>(flags, metaItem);
	}

public:
	GMetaOperatorData() {
		static GMetaOperatorDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<ThisType>,
			
			&virtualInvoke,
			&virtualInvoke2,
			&virtualInvokeFunctor,
			&virtualExecute,
			&virtualExecuteByData,

			&virtualGetOperator,
			&virtualGetParamCount,
			&virtualHasResult,
			&virtualIsParamSelf,
			&virtualGetParamType,
			&virtualGetResultType,
			&virtualGetResultExtendType,
			&virtualGetParamExtendType,
			&virtualIsVariadic,
			&virtualIsExplicitThis,
			&virtualCheckParam,
			&virtualIsParamTransferOwnership,
			&virtualIsResultTransferOwnership,
			&virtualCreateOperatorMetaType,
			&virtualGetItemExtendType
		};

		this->virtualFunctions = &thisFunctions;
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



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
