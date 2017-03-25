#include "cpgf/gmetaoperator.h"
#include "cpgf/gerrorcode.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


namespace cpgf {


namespace meta_internal {


void GMetaOperatorDataBase::deleteSelf()
{
	this->virtualFunctions->deleteSelf(this);
}

GMetaOpType GMetaOperatorDataBase::getOperator() const
{
	return this->virtualFunctions->getOperator();
}

size_t GMetaOperatorDataBase::getParamCount() const
{
	return this->virtualFunctions->getParamCount();
}

bool GMetaOperatorDataBase::isParamSelf(size_t paramIndex) const
{
	return this->virtualFunctions->isParamSelf(paramIndex);
}

GMetaType GMetaOperatorDataBase::getParamType(size_t paramIndex) const
{
	return this->virtualFunctions->getParamType(paramIndex);
}

bool GMetaOperatorDataBase::hasResult() const
{
	return this->virtualFunctions->hasResult();
}

GMetaType GMetaOperatorDataBase::getResultType() const
{
	return this->virtualFunctions->getResultType();
}

bool GMetaOperatorDataBase::isVariadic() const
{
	return this->virtualFunctions->isVariadic();
}

bool GMetaOperatorDataBase::isExplicitThis() const
{
	return this->virtualFunctions->isExplicitThis();
}

bool GMetaOperatorDataBase::checkParam(const GVariant & param, size_t paramIndex) const
{
	return this->virtualFunctions->checkParam(param, paramIndex);
}

GMetaType GMetaOperatorDataBase::createOperatorMetaType() const
{
	return this->virtualFunctions->createOperatorMetaType();
}

bool GMetaOperatorDataBase::isParamTransferOwnership(size_t paramIndex) const
{
	return this->virtualFunctions->isParamTransferOwnership(paramIndex);
}

bool GMetaOperatorDataBase::isResultTransferOwnership() const
{
	return this->virtualFunctions->isResultTransferOwnership();
}

GVariant GMetaOperatorDataBase::invoke(const GVariant & p0) const
{
	if(this->virtualFunctions->invoke == nullptr) {
		raiseCoreException(Error_Meta_NotUnaryOperator);
		return GVariant();
	}
	else {
		return this->virtualFunctions->invoke(p0);
	}
}

GVariant GMetaOperatorDataBase::invoke(const GVariant & p0, const GVariant & p1) const
{
	if(this->virtualFunctions->invoke2 == nullptr) {
		raiseCoreException(Error_Meta_NotBinaryOperator);
		return GVariant();
	}
	else {
		return this->virtualFunctions->invoke2(p0, p1);
	}
}

GVariant GMetaOperatorDataBase::invokeFunctor(void * instance, GVariant const * const * params, size_t paramCount) const
{
	if(this->virtualFunctions->invokeFunctor == nullptr) {
		raiseCoreException(Error_Meta_NotFunctorOperator);
		return GVariant();
	}
	else {
		return this->virtualFunctions->invokeFunctor(instance, params, paramCount);
	}
}

GVariant GMetaOperatorDataBase::execute(void * instance, const GVariant * params, size_t paramCount) const
{
	if(this->virtualFunctions->execute == nullptr) {
		raiseCoreException(Error_Meta_NotFunctorOperator);
		return GVariant();
	}
	else {
		return this->virtualFunctions->execute(this, instance, params, paramCount);
	}
}

GVariant GMetaOperatorDataBase::executeByData(void * instance, const GVariantData * * params, size_t paramCount) const
{
	if(this->virtualFunctions->executeByData == nullptr) {
		raiseCoreException(Error_Meta_NotFunctorOperator);
		return GVariant();
	}
	else {
		return this->virtualFunctions->executeByData(this, instance, params, paramCount);
	}
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


void operatorIndexOutOfBound(size_t /*index*/, size_t /*maxIndex*/)
{
	raiseCoreException(Error_Meta_ParamOutOfIndex);
}

std::string operatorToName(GMetaOpType op) {
	char buffer[10];

	sprintf(buffer, "%d", op);

	return std::string(buffer);
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

GMetaExtendType GMetaOperator::getResultExtendType(uint32_t flags) const
{
	return this->baseData->getResultExtendType(flags);
}

GMetaExtendType GMetaOperator::getParamExtendType(uint32_t flags, size_t index) const
{
	return this->baseData->getParamExtendType(flags, index);
}

bool GMetaOperator::isVariadic() const
{
	return this->baseData->isVariadic();
}

bool GMetaOperator::isExplicitThis() const
{
	return this->baseData->isExplicitThis();
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

GVariant GMetaOperator::executeByData(void * instance, const GVariantData * * params, size_t paramCount) const
{
	return this->baseData->executeByData(instance, params, paramCount);
}

GMetaExtendType GMetaOperator::getItemExtendType(uint32_t flags) const
{
	return this->baseData->getItemExtendType(flags, this);
}

#define VAR_PARAM_DEFAULT(N, unused) GPP_COMMA_IF(N) const GVariant & p ## N
#define FUNCTOR_LOAD_PARAM(N, unused) params[index++] = & p ## N;

GVariant GMetaOperator::invokeFunctor(const GVariant & instance, GPP_REPEAT(REF_MAX_ARITY, VAR_PARAM_DEFAULT, GPP_EMPTY)) const
{
	const GVariant * params[REF_MAX_ARITY];
	int index = 0;

	GPP_REPEAT(REF_MAX_ARITY, FUNCTOR_LOAD_PARAM, GPP_EMPTY);

	size_t paramCount = 0;

	while(paramCount < REF_MAX_ARITY) {
		if(params[paramCount]->isEmpty()) {
			break;
		}

		++paramCount;
	}
	
	if(this->baseData->hasDefaultParam()) {
		paramCount = this->baseData->getDefaultParamList()->loadDefaultParams(params, paramCount, this->baseData->getParamCount());
	}

	return this->baseData->invokeFunctor(objectAddressFromVariant(instance), params, paramCount);
}



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif
