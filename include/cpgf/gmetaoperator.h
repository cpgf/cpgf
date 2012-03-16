#ifndef __GMETAOPERATOR_H
#define __GMETAOPERATOR_H


#include "cpgf/private/gmetainvoke_p.h"
#include "cpgf/private/gmetadefaultparam_p.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gassert.h"

#include <string>
#include <stdio.h>


namespace cpgf {


struct GMetaSelf {};

enum GMetaOpType {
	mopBinaryFirst = 1,

	mopAdd = mopBinaryFirst + 0, // a + b
	mopSub = mopBinaryFirst + 1, // a - b
	mopMul = mopBinaryFirst + 2, // a * b
	mopDiv = mopBinaryFirst + 3, // a / b
	mopMod = mopBinaryFirst + 4, // a % b

	mopAddAssign = mopBinaryFirst + 5, // a += b
	mopSubAssign = mopBinaryFirst + 6, // a -= b
	mopMulAssign = mopBinaryFirst + 7, // a *= b
	mopDivAssign = mopBinaryFirst + 8, // a /= b
	mopModAssign = mopBinaryFirst + 9, // a %= b

	mopAssign = mopBinaryFirst + 10, // a = b

	mopEqual = mopBinaryFirst + 11, // a == b
	mopNotEqual = mopBinaryFirst + 12, // a != b
	mopGreater = mopBinaryFirst + 13, // a > b
	mopLessor = mopBinaryFirst + 14, // a < b
	mopGreaterEqual = mopBinaryFirst + 15, // a >= b
	mopLessorEqual = mopBinaryFirst + 16, // a <= b

	mopLogicAnd = mopBinaryFirst + 17, // a && b
	mopLogicOr = mopBinaryFirst + 18, // a || b

	mopBitAnd = mopBinaryFirst + 19, // a & b
	mopBitOr = mopBinaryFirst + 20, // a | b
	mopBitXor = mopBinaryFirst + 21, // a ^ b
	mopBitLeftShift = mopBinaryFirst + 22, // a << b
	mopBitRightShift = mopBinaryFirst + 23, // a >> b

	mopBitAndAssign = mopBinaryFirst + 24, // a &= b
	mopBitOrAssign = mopBinaryFirst + 25, // a |= b
	mopBitXorAssign = mopBinaryFirst + 26, // a ^= b
	mopBitLeftShiftAssign = mopBinaryFirst + 27, // a <<= b
	mopBitRightShiftAssign = mopBinaryFirst + 28, // a >>= b

	mopComma = mopBinaryFirst + 29, // a , b

	mopSubscript = mopBinaryFirst + 30, // a[b]

	mopPointerMember = mopBinaryFirst + 31, // a->*b

	mopBinaryLast = mopBinaryFirst + 31,


	mopUnaryFirst = mopBinaryLast + 1,

	mopPlus = mopUnaryFirst + 0, // +a
	mopNeg = mopUnaryFirst + 1, // -a

	mopPreInc = mopUnaryFirst + 2, // ++a
	mopPostInc = mopUnaryFirst + 3, // a++
	mopPreDec = mopUnaryFirst + 4, // --a
	mopPostDec = mopUnaryFirst + 5, // a--

	mopLogicNot = mopUnaryFirst + 6, // !a

	mopBitNot = mopUnaryFirst + 7, // ~a

	mopAddress = mopUnaryFirst + 8, // &a
	mopDerefer = mopUnaryFirst + 9, // *a

	mopCast = mopUnaryFirst + 10, // T(a)

	mopMember = mopUnaryFirst + 11, // a->b

	mopUnaryLast = mopUnaryFirst + 11,

	mopFunctor = mopUnaryLast + 1, // a(p0, p1...pn)
};


#include "cpgf/private/gmetaoperator_p.h"

#ifdef G_COMPILER_CPPBUILDER
#pragma warn -8058 //Cannot create pre-compiled header: initialized data in header
#endif

const meta_internal::GMetaOperatorHolder mopHolder = meta_internal::GMetaOperatorHolder();

#ifdef G_COMPILER_CPPBUILDER
#pragma warn .8058 //Cannot create pre-compiled header: initialized data in header
#endif


GMAKE_FINAL(GMetaOperator)

class GMetaOperator : public GMetaCallable, GFINAL_BASE(GMetaOperator)
{
private:
	typedef GMetaCallable super;

public:
	GMetaOperator(meta_internal::GMetaOperatorDataBase * baseData);

	GMetaOpType getOperator() const;

	virtual size_t getParamCount() const;
	virtual size_t getDefaultParamCount() const;
	virtual GMetaType getParamType(size_t index) const;
	virtual bool hasResult() const;
	virtual GMetaType getResultType() const;
	virtual bool isVariadic() const;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const;

	virtual bool isParamTransferOwnership(size_t paramIndex) const;
	virtual bool isResultTransferOwnership() const;
	
	virtual GMetaConverter * createResultConverter() const;

	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const;

	void addDefaultParam(const GVariant & v);

	GVariant invokeUnary(const GVariant & p0) const;
	GVariant invokeBinary(const GVariant & p0, const GVariant & p1) const;

#define VAR_PARAM_DEFAULT(N, unused) GPP_COMMA_IF(N) const GVariant & p ## N = GVariant()
	GVariant invokeFunctor(const GVariant & instance, GPP_REPEAT(REF_MAX_ARITY, VAR_PARAM_DEFAULT, GPP_EMPTY)) const;
#undef VAR_PARAM_DEFAULT

private:
	GScopedPointer<meta_internal::GMetaOperatorDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaOperatorDataBase> >
		baseData;
};




} // namespace cpgf



#endif


