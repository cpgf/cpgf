#include "cpgf/gmetaoperator.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


namespace cpgf {

namespace meta_internal {

std::string operatorToName(GMetaOpType op) {
	char buffer[10];

	sprintf(buffer, "%d", op);

	return std::string(buffer);
}

} // namespace meta_internal


size_t GMetaOperator::getParamCount() const
{
	return this->baseData->getParamCount();
}

GMetaType GMetaOperator::getParamType(size_t index) const
{
	return this->baseData->getParamType(index);
}

bool GMetaOperator::hasResult() const
{
	return this->baseData->hasResult();
}

GMetaType GMetaOperator::getResultType() const
{
	return this->baseData->getResultType();
}

bool GMetaOperator::isVariadic() const
{
	return this->baseData->isVariadic();
}

bool GMetaOperator::checkParam(const GVariant & param, size_t paramIndex) const
{
	return this->baseData->checkParam(param, paramIndex);
}

bool GMetaOperator::isParamTransferOwnership(size_t paramIndex) const
{
	return this->baseData->isParamTransferOwnership(paramIndex);
}

bool GMetaOperator::isResultTransferOwnership() const
{
	return this->baseData->isResultTransferOwnership();
}

GMetaConverter * GMetaOperator::createResultConverter() const
{
	return this->baseData->createResultConverter();
}

GVariant GMetaOperator::invokeUnary(const GVariant & p0) const
{
	return this->baseData->invoke(p0);
}

GVariant GMetaOperator::invokeBinary(const GVariant & p0, const GVariant & p1) const
{
	return this->baseData->invoke(p0, p1);
}

GVariant GMetaOperator::execute(void * instance, const GVariant * params, size_t paramCount) const
{
	return this->baseData->execute(instance, params, paramCount);
}

#define VAR_PARAM_DEFAULT(N, unused) GPP_COMMA_IF(N) const GVariant & p ## N
#define FUNCTOR_LOAD_PARAM(N, unused) params[index++] = & p ## N;

#define LESS_MAX_ARITY_1 GPP_DEC(REF_MAX_ARITY)
#define LESS_MAX_ARITY GPP_DEC(LESS_MAX_ARITY_1)

GVariant GMetaOperator::invokeFunctor(const GVariant & instance, const GVariant & pa, const GVariant & pb, GPP_REPEAT(LESS_MAX_ARITY, VAR_PARAM_DEFAULT, GPP_EMPTY)) const
{
	const GVariant * params[REF_MAX_ARITY];
	int index = 0;

	params[index++] = &pa;
	params[index++] = &pb;

	GPP_REPEAT(LESS_MAX_ARITY, FUNCTOR_LOAD_PARAM, GPP_EMPTY);

	int paramCount = 0;

	while(paramCount < REF_MAX_ARITY) {
		if(params[paramCount]->isEmpty()) {
			break;
		}

		++paramCount;
	}

	return this->baseData->invokeFunctor(fromVariant<void *>(instance), params, paramCount);
}



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif
