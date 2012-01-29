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


GMetaOperatorDataBase::~GMetaOperatorDataBase()
{
}

GVariant GMetaOperatorDataBase::invoke(const GVariant & p0) const
{
	(void)p0;

	raiseCoreException(Error_Meta_NotUnaryOperator);

	return GVariant();
}

GVariant GMetaOperatorDataBase::invoke(const GVariant & p0, const GVariant & p1) const
{
	(void)p0; (void)p1;

	raiseCoreException(Error_Meta_NotBinaryOperator);

	return GVariant();
}

GVariant GMetaOperatorDataBase::invokeFunctor(void * instance, GVariant const * const * params, size_t paramCount) const
{
	(void)instance; (void)params; (void)paramCount;

	raiseCoreException(Error_Meta_NotFunctorOperator);

	return GVariant();
}

GVariant GMetaOperatorDataBase::execute(void * instance, const GVariant * params, size_t paramCount) const
{
	(void)instance; (void)params; (void)paramCount;

	raiseCoreException(Error_Meta_NotFunctorOperator);

	return GVariant();
}

GMetaDefaultParamList * GMetaOperatorDataBase::getDefaultParamList() const
{
	if(! this->defaultParamList) {
		this->defaultParamList.reset(new GMetaDefaultParamList);
	}

	return this->defaultParamList.get();
}

bool GMetaOperatorDataBase::hasDefaultParam() const
{
	return !! this->defaultParamList
		&& this->defaultParamList->getDefaultCount() > 0;
}

} // namespace meta_internal


GMetaOperator::GMetaOperator(meta_internal::GMetaOperatorDataBase * baseData)
	: super(meta_internal::operatorToName(baseData->getOperator()).c_str(), baseData->createOperatorMetaType(), mcatOperator), baseData(baseData)
{
}

GMetaOpType GMetaOperator::getOperator() const
{
	return this->baseData->getOperator();
}

size_t GMetaOperator::getParamCount() const
{
	return this->baseData->getParamCount();
}

GMetaType GMetaOperator::getParamType(size_t index) const
{
	return this->baseData->getParamType(index);
}

size_t GMetaOperator::getDefaultParamCount() const
{
	return this->baseData->hasDefaultParam() ? this->baseData->getDefaultParamList()->getDefaultCount() : 0;
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

void GMetaOperator::addDefaultParam(const GVariant & v)
{
	this->baseData->getDefaultParamList()->addDefault(v);
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

GVariant GMetaOperator::invokeFunctor(const GVariant & instance, GPP_REPEAT(REF_MAX_ARITY, VAR_PARAM_DEFAULT, GPP_EMPTY)) const
{
	const GVariant * params[REF_MAX_ARITY];
	int index = 0;

	GPP_REPEAT(REF_MAX_ARITY, FUNCTOR_LOAD_PARAM, GPP_EMPTY);

	int paramCount = 0;

	while(paramCount < REF_MAX_ARITY) {
		if(params[paramCount]->isEmpty()) {
			break;
		}

		++paramCount;
	}
	
	if(this->baseData->hasDefaultParam()) {
		paramCount = this->baseData->getDefaultParamList()->loadDefaultParams(params, paramCount, this->baseData->getParamCount());
	}

	return this->baseData->invokeFunctor(fromVariant<void *>(instance), params, paramCount);
}



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif
