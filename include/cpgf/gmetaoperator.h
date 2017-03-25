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
	virtual GMetaExtendType getResultExtendType(uint32_t flags) const;
	virtual GMetaExtendType getParamExtendType(uint32_t flags, size_t index) const;
	virtual bool isVariadic() const;
	virtual bool isExplicitThis() const;

	virtual bool checkParam(const GVariant & param, size_t paramIndex) const;

	virtual bool isParamTransferOwnership(size_t paramIndex) const;
	virtual bool isResultTransferOwnership() const;
	
	virtual GVariant execute(void * instance, const GVariant * params, size_t paramCount) const;
	virtual GVariant executeByData(void * instance, const GVariantData * * params, size_t paramCount) const;

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


