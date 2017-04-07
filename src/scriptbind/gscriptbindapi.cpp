#include "cpgf/gapiutil.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "gbindapiimpl.h"


namespace cpgf {


// This function is defined in gscriptvalue.cpp internally.
GScriptValue createScriptValueFromData(const GScriptValueData & data);

// This function is defined in gvariant.cpp, for internal use.
GVariant createVariantFromData(const GVariantData & data);

ImplScriptFunction::ImplScriptFunction(GScriptFunction * scriptFunction, bool freeFunction)
	: scriptFunction(scriptFunction), freeFunction(freeFunction)
{
}

ImplScriptFunction::~ImplScriptFunction()
{
	if(this->freeFunction) {
		delete this->scriptFunction;
	}
}

void G_API_CC ImplScriptFunction::invoke(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	const GVariantData * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramIndirect[i] = &params[i];
	}

	this->invokeIndirectly(outResult, paramIndirect, paramCount);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptFunction::invokeIndirectly(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	GVariant paramVariants[REF_MAX_ARITY];
	const GVariant * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramVariants[i] = createVariantFromData(*params[i]);
		paramIndirect[i] = &paramVariants[i];
	}

	GScriptValue result = this->scriptFunction->invokeIndirectly(paramIndirect, paramCount);
	if(outResult) {
		*outResult = result.takeData();
	}

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptFunction::invokeOnObject(GScriptValueData * outResult, const GVariantData * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	const GVariantData * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramIndirect[i] = &params[i];
	}

	this->invokeIndirectlyOnObject(outResult, paramIndirect, paramCount);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptFunction::invokeIndirectlyOnObject(GScriptValueData * outResult, GVariantData const * const * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	GVariant paramVariants[REF_MAX_ARITY];
	const GVariant * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramVariants[i] = createVariantFromData(*params[i]);
		paramIndirect[i] = &paramVariants[i];
	}

	GScriptValue result = this->scriptFunction->invokeIndirectlyOnObject(paramIndirect, paramCount);
	if(outResult) {
		*outResult = result.takeData();
	}

	LEAVE_BINDING_API()
}

// Internal use only!!!
void G_API_CC ImplScriptFunction::weaken()
{
	this->scriptFunction->weaken();
}

ImplScriptArray::ImplScriptArray(GScriptArray * scriptArray, bool freeArray)
	: scriptArray(scriptArray), freeArray(freeArray)
{
}

ImplScriptArray::~ImplScriptArray()
{
	if(this->freeArray) {
		delete this->scriptArray;
	}
}

uint32_t G_API_CC ImplScriptArray::getLength()
{
	return (uint32_t)(this->scriptArray->getLength());
}

void G_API_CC ImplScriptArray::getValue(GScriptValueData * outResult, uint32_t index)
{
	GScriptValue value(this->scriptArray->getValue(index));
	*outResult = value.takeData();
}

void G_API_CC ImplScriptArray::setValue(uint32_t index, const GScriptValueData * value)
{
	GScriptValue scriptValue(createScriptValueFromData(*value));
	this->scriptArray->setValue(index, scriptValue);
}

gapi_bool G_API_CC ImplScriptArray::maybeIsScriptArray(uint32_t index)
{
	return this->scriptArray->maybeIsScriptArray(index);
}
void G_API_CC ImplScriptArray::getAsScriptArray(GScriptValueData * outResult, uint32_t index)
{
	GScriptValue value(this->scriptArray->getAsScriptArray(index));
	*outResult = value.takeData();
}

void G_API_CC ImplScriptArray::createScriptArray(GScriptValueData * outResult, uint32_t index)
{
	GScriptValue value(this->scriptArray->createScriptArray(index));
	*outResult = value.takeData();
}


ImplScriptObject::ImplScriptObject(GScriptObject * scriptObject, bool freeObject)
	: scriptObject(scriptObject), freeObject(freeObject)
{
}

ImplScriptObject::~ImplScriptObject()
{
	if(this->freeObject) {
		delete this->scriptObject;
	}
}

IScriptContext * G_API_CC ImplScriptObject::getContext()
{
	ENTER_BINDING_API()

	return this->scriptObject->getContext();

	LEAVE_BINDING_API(return nullptr)
}

IScriptObject * G_API_CC ImplScriptObject::getOwner()
{
	ENTER_BINDING_API()

	GScriptObject * owner = this->scriptObject->getOwner();
	if(owner == nullptr) {
		return nullptr;
	}
	else {
		return new ImplScriptObject(owner, false);
	}

	LEAVE_BINDING_API(return nullptr)
}

gapi_bool G_API_CC ImplScriptObject::isGlobal()
{
	ENTER_BINDING_API()

	return this->scriptObject->isGlobal();

	LEAVE_BINDING_API(return false)
}

void G_API_CC ImplScriptObject::getValue(GScriptValueData * outResult, const char * name)
{
	GScriptValue value(this->scriptObject->getValue(name));
	*outResult = value.takeData();
}

void G_API_CC ImplScriptObject::setValue(const char * name, const GScriptValueData * value)
{
	GScriptValue scriptValue(createScriptValueFromData(*value));
	this->scriptObject->setValue(name, scriptValue);
}

void G_API_CC ImplScriptObject::createScriptObject(GScriptValueData * outResult, const char * name)
{
	ENTER_BINDING_API()

	*outResult = this->scriptObject->createScriptObject(name).takeData();

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::invoke(GScriptValueData * outResult, const char * name, const GVariantData * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	const GVariantData * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramIndirect[i] = &params[i];
	}

	this->invokeIndirectly(outResult, name, paramIndirect, paramCount);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::invokeIndirectly(GScriptValueData * outResult, const char * name, GVariantData const * const * params, uint32_t paramCount)
{
	ENTER_BINDING_API()

	GVariant paramVariants[REF_MAX_ARITY];
	const GVariant * paramIndirect[REF_MAX_ARITY];

	for(uint32_t i = 0; i < paramCount; ++i) {
		paramVariants[i] = createVariantFromData(*params[i]);
		paramIndirect[i] = &paramVariants[i];
	}

	GScriptValue result = this->scriptObject->invokeIndirectly(name, paramIndirect, paramCount);
	if(outResult) {
		*outResult = result.takeData();
	}

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::assignValue(const char * fromName, const char * toName)
{
	ENTER_BINDING_API()

	this->scriptObject->assignValue(fromName, toName);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader)
{
	ENTER_BINDING_API()

	this->scriptObject->bindCoreService(name, libraryLoader);

	LEAVE_BINDING_API()
}

void G_API_CC ImplScriptObject::holdObject(IObject * object)
{
	ENTER_BINDING_API()

	this->scriptObject->holdObject(object);

	LEAVE_BINDING_API()
}

gapi_bool G_API_CC G_API_CC ImplScriptObject::maybeIsScriptArray(const char * name)
{
	return !! this->scriptObject->maybeIsScriptArray(name);
}

void G_API_CC G_API_CC ImplScriptObject::getAsScriptArray(GScriptValueData * outResult, const char * name)
{
	ENTER_BINDING_API()

	*outResult = this->scriptObject->getAsScriptArray(name).takeData();

	LEAVE_BINDING_API()
}

void G_API_CC G_API_CC ImplScriptObject::createScriptArray(GScriptValueData * outResult, const char * name)
{
	ENTER_BINDING_API()

	*outResult = this->scriptObject->createScriptArray(name).takeData();

	LEAVE_BINDING_API()
}


} // namespace cpfg

