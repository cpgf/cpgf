#ifndef CPGF_GMETAOPERATOR_H
#define CPGF_GMETAOPERATOR_H

#include "cpgf/gtypeutil.h"
#include "cpgf/private/gmetainvoke_p.h"
#include "cpgf/private/gmetadefaultparam_p.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gassert.h"
#include "cpgf/gmetaoperatorop.h"

#include <string>
#include <stdio.h>


namespace cpgf {

extern int Error_Meta_WrongArity;
extern int Error_Meta_ParamOutOfIndex;

struct GMetaSelf {};


#include "cpgf/private/gmetaoperator_p.h"

#ifdef G_COMPILER_CPPBUILDER
#pragma warn -8058 //Cannot create pre-compiled header: initialized data in header
#endif

const meta_internal::GMetaOperatorHolder mopHolder = meta_internal::GMetaOperatorHolder();

#ifdef G_COMPILER_CPPBUILDER
#pragma warn .8058 //Cannot create pre-compiled header: initialized data in header
#endif


class GMetaOperator final : public GMetaCallable
{
private:
	typedef GMetaCallable super;

public:
	GMetaOperator(meta_internal::GMetaOperatorDataBase * baseData);

	GMetaOpType getOperator() const;

	virtual size_t getParamCount() const override;
	virtual size_t getDefaultParamCount() const override;
	virtual GMetaType getParamType(size_t index) const override;
	virtual bool hasResult() const override;
	virtual GMetaType getResultType() const override;
	virtual GMetaExtendType getResultExtendType(uint32_t flags) const override;
	virtual GMetaExtendType getParamExtendType(uint32_t flags, size_t index) const override;
	virtual bool isVariadic() const override;
	virtual bool isExplicitThis() const override;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const override;

	virtual bool isParamTransferOwnership(size_t paramIndex) const override;
	virtual bool isResultTransferOwnership() const override;
	
	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const override;
	virtual GVariant executeByData(void * instance, const GVariantData * * params, size_t paramCount) const override;

	virtual GMetaExtendType getItemExtendType(uint32_t flags) const override;

	void addDefaultParam(const GVariant & v);

	GVariant invokeUnary(const GVariant & p0) const;
	GVariant invokeBinary(const GVariant & p0, const GVariant & p1) const;

#define VAR_PARAM_DEFAULT(N, unused) GPP_COMMA_IF(N) const GVariant & p ## N = GVariant()
	GVariant invokeFunctor(const GVariant & instance, GPP_REPEAT(REF_MAX_ARITY, VAR_PARAM_DEFAULT, GPP_EMPTY)) const;
#undef VAR_PARAM_DEFAULT

private:
	std::unique_ptr<meta_internal::GMetaOperatorDataBase,
		meta_internal::GScopedPointerDeleter_BaseMeta<meta_internal::GMetaOperatorDataBase> >
		baseData;
};




} // namespace cpgf



#endif


