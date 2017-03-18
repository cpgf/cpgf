#include "cpgf/gmetamethod.h"
#include "cpgf/gmetaclass.h"


#define REF_CALL_LOAD_PARAM(N, unused) params[N] = & p ## N;

#define REF_CALL_METHOD(N, unused) \
	GVariant GMetaMethod::invoke(void * instance GPP_COMMA_IF(N) GPP_REPEAT(N, GPP_COMMA_PARAM, const GVariant & p)) const { \
		if(this->isStatic() && ! this->baseData->isExplicitThis()) { instance = nullptr; } \
		const GVariant * params[REF_MAX_ARITY]; \
		GPP_REPEAT(N, REF_CALL_LOAD_PARAM, GPP_EMPTY); \
		size_t passedParamCount = N; \
		if(this->baseData->hasDefaultParam()) { \
			passedParamCount = this->baseData->getDefaultParamList()->loadDefaultParams(params, N, this->baseData->getParamCount()); \
		} \
		return this->baseData->invoke(instance, params, passedParamCount); \
	}

#define REF_NEW_INSTANCE(N, unused) \
	void * GMetaConstructor::invoke(GPP_REPEAT(N, GPP_COMMA_PARAM, const GVariant & p)) const { \
		const GVariant * params[REF_MAX_ARITY]; \
		GPP_REPEAT(N, REF_CALL_LOAD_PARAM, GPP_EMPTY); \
		size_t passedParamCount = N; \
		if(this->baseData->hasDefaultParam()) { \
			passedParamCount = this->baseData->getDefaultParamList()->loadDefaultParams(params, N, this->baseData->getParamCount()); \
		} \
		return objectAddressFromVariant(this->baseData->invoke(nullptr, params, passedParamCount)); \
	}

namespace cpgf {


namespace meta_internal {


void GMetaMethodDataBase::deleteSelf()
{
	this->virtualFunctions->deleteSelf(this);
}

size_t GMetaMethodDataBase::getParamCount() const
{
	return this->virtualFunctions->getParamCount();
}

bool GMetaMethodDataBase::hasResult() const
{
	return this->virtualFunctions->hasResult();
}

GMetaType GMetaMethodDataBase::getParamType(size_t index) const
{
	return this->virtualFunctions->getParamType(index);
}

GMetaType GMetaMethodDataBase::getResultType() const
{
	return this->virtualFunctions->getResultType();
}

bool GMetaMethodDataBase::isVariadic() const
{
	return this->virtualFunctions->isVariadic();
}

bool GMetaMethodDataBase::isExplicitThis() const
{
	return this->virtualFunctions->isExplicitThis();
}

GVariant GMetaMethodDataBase::invoke(void * instance, GVariant const * const * params, size_t paramCount) const
{
	return this->virtualFunctions->invoke(this, instance, params, paramCount);
}

GVariant GMetaMethodDataBase::invokeByData(void * instance, GVariantData const * const * params, size_t paramCount) const
{
	return this->virtualFunctions->invokeByData(this, instance, params, paramCount);
}

bool GMetaMethodDataBase::checkParam(const GVariant & param, size_t paramIndex) const
{
	return this->virtualFunctions->checkParam(param, paramIndex);
}

bool GMetaMethodDataBase::isParamTransferOwnership(size_t paramIndex) const
{
	return this->virtualFunctions->isParamTransferOwnership(paramIndex);
}

bool GMetaMethodDataBase::isResultTransferOwnership() const
{
	return this->virtualFunctions->isResultTransferOwnership();
}

GMetaDefaultParamList * GMetaMethodDataBase::getDefaultParamList() const
{
	if(! this->defaultParamList) {
		this->defaultParamList.reset(new GMetaDefaultParamList);
	}

	return this->defaultParamList.get();
}

bool GMetaMethodDataBase::hasDefaultParam() const
{
	return !! this->defaultParamList
		&& this->defaultParamList->getDefaultCount() > 0;
}


std::string arityToName(int arity)
{
	char buffer[10];
	int i = 0;

	if(arity < 10) {
		buffer[i++] = '0' + (char)arity;
	}
	else if(arity < 20) {
		buffer[i++] = '1';
		buffer[i++] = '0' + (char)(arity - 10);
	}
	else {
		buffer[i++] = 'X';
	}
	buffer[i++] = 0;

	return std::string(buffer);
}


} // namespace meta_internal


GMetaType GMetaMethod::getParamType(size_t index) const
{
	return this->baseData->getParamType(index);
}

size_t GMetaMethod::getParamCount() const
{
	return this->baseData->getParamCount();
}

size_t GMetaMethod::getDefaultParamCount() const
{
	return this->baseData->hasDefaultParam() ? this->baseData->getDefaultParamList()->getDefaultCount() : 0;
}

bool GMetaMethod::hasResult() const
{
	return this->baseData->hasResult();
}

GMetaType GMetaMethod::getResultType() const
{
	return this->baseData->getResultType();
}

GMetaExtendType GMetaMethod::getResultExtendType(uint32_t flags) const
{
	return this->baseData->getResultExtendType(flags);
}

GMetaExtendType GMetaMethod::getParamExtendType(uint32_t flags, size_t index) const
{
	return this->baseData->getParamExtendType(flags, index);
}

bool GMetaMethod::isVariadic() const
{
	return this->baseData->isVariadic();
}

bool GMetaMethod::isExplicitThis() const
{
	return this->baseData->isExplicitThis();
}

GVariant GMetaMethod::execute(void * instance, const GVariant * params, size_t paramCount) const
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	if(this->isStatic()) {
		instance = nullptr;
	}

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	if(this->baseData->hasDefaultParam()) {
		paramCount = this->baseData->getDefaultParamList()->loadDefaultParams(variantPointers, paramCount, this->baseData->getParamCount());
	}

	return this->baseData->invoke(instance, variantPointers, paramCount);
}

GVariant GMetaMethod::executeByData(void * instance, const GVariantData * * params, size_t paramCount) const
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	if(this->isStatic()) {
		instance = nullptr;
	}

	if(this->baseData->hasDefaultParam()) {
		paramCount = this->baseData->getDefaultParamList()->loadDefaultParamsByData(params, paramCount, this->baseData->getParamCount());
	}

	return this->baseData->invokeByData(instance, params, paramCount);
}

bool GMetaMethod::checkParam(const GVariant & param, size_t paramIndex) const
{
	return this->baseData->checkParam(param, paramIndex);
}

bool GMetaMethod::isParamTransferOwnership(size_t paramIndex) const
{
	return this->baseData->isParamTransferOwnership(paramIndex);
}

bool GMetaMethod::isResultTransferOwnership() const
{
	return this->baseData->isResultTransferOwnership();
}

GMetaExtendType GMetaMethod::getItemExtendType(uint32_t flags) const
{
	return this->baseData->getItemExtendType(flags, this);
}

void GMetaMethod::addDefaultParam(const GVariant & v)
{
	this->baseData->getDefaultParamList()->addDefault(v);
}

GPP_REPEAT_2(REF_MAX_ARITY, REF_CALL_METHOD, GPP_EMPTY)



GMetaType GMetaConstructor::getParamType(size_t index) const
{
	return this->baseData->getParamType(index);
}

size_t GMetaConstructor::getParamCount() const
{
	return this->baseData->getParamCount();
}

size_t GMetaConstructor::getDefaultParamCount() const
{
	return this->baseData->hasDefaultParam() ? this->baseData->getDefaultParamList()->getDefaultCount() : 0;
}

bool GMetaConstructor::hasResult() const
{
	return true;
}

GMetaType GMetaConstructor::getResultType() const
{
	GASSERT_MSG(metaIsClass(this->getOwnerItem()->getCategory()), "Constructor must be owned by class.");

	GMetaType type = static_cast<const GMetaClass *>(this->getOwnerItem())->getMetaType();

	type.addPointer();

	return type;
}

GMetaExtendType GMetaConstructor::getResultExtendType(uint32_t flags) const
{
	GASSERT_MSG(metaIsClass(this->getOwnerItem()->getCategory()), "Constructor must be owned by class.");

	return static_cast<const GMetaClass *>(this->getOwnerItem())->getItemExtendType(flags);
}

GMetaExtendType GMetaConstructor::getParamExtendType(uint32_t flags, size_t index) const
{
	return this->baseData->getParamExtendType(flags, index);
}

bool GMetaConstructor::isVariadic() const
{
	return this->baseData->isVariadic();
}

bool GMetaConstructor::isExplicitThis() const
{
	return false;
}

GVariant GMetaConstructor::execute(void * /*instance*/, const GVariant * params, size_t paramCount) const
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	if(this->baseData->hasDefaultParam()) {
		paramCount = this->baseData->getDefaultParamList()->loadDefaultParams(variantPointers, paramCount, this->baseData->getParamCount());
	}

	return this->baseData->invoke(nullptr, variantPointers, paramCount);
}

GVariant GMetaConstructor::executeByData(void * instance, const GVariantData * * params, size_t paramCount) const
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	if(this->baseData->hasDefaultParam()) {
		paramCount = this->baseData->getDefaultParamList()->loadDefaultParamsByData(params, paramCount, this->baseData->getParamCount());
	}

	return this->baseData->invokeByData(nullptr, params, paramCount);
}

bool GMetaConstructor::checkParam(const GVariant & param, size_t paramIndex) const
{
	return this->baseData->checkParam(param, paramIndex);
}

bool GMetaConstructor::isParamTransferOwnership(size_t paramIndex) const
{
	return this->baseData->isParamTransferOwnership(paramIndex);
}

bool GMetaConstructor::isResultTransferOwnership() const
{
	return this->baseData->isResultTransferOwnership();
}

GMetaExtendType GMetaConstructor::getItemExtendType(uint32_t flags) const
{
	return this->baseData->getItemExtendType(flags, this);
}

void GMetaConstructor::addDefaultParam(const GVariant & v)
{
	this->baseData->getDefaultParamList()->addDefault(v);
}

GPP_REPEAT_2(REF_MAX_ARITY, REF_NEW_INSTANCE, GPP_EMPTY)



} // namespace cpgf
