#ifndef CPGF_GSCRIPTBINDUTIL_H
#define CPGF_GSCRIPTBINDUTIL_H


#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gtypeutil.h"

namespace cpgf {

template <typename T>
GVariant createTypedVariant(const T & value)
{
	return createTypedVariant(GVariant(value), createMetaType<T>());
}

template <typename Tuple>
struct LoadVariantListParam
{
	GVariant * variantList;
	const Tuple & tuple;
};

template <unsigned int N>
struct LoadVariantListFunc
{
	template <typename Param>
	void operator()(Param & param)
	{
		param.variantList[N] = createTypedVariant(std::get<N>(param.tuple));
	}
};

template <typename... Parameters>
void loadVariantList(GVariant * variantList, const Parameters & ... parameters)
{
	const auto tuple = std::forward_as_tuple(parameters...);
	LoadVariantListParam<decltype(tuple)> param { variantList, tuple };
	GTypeForEach<sizeof...(Parameters)>::template forEach<LoadVariantListFunc>(param);
}


template <typename Tuple>
struct LoadVariantDataListParam
{
	GVariant * variantList;
	GVariantData ** variantDataList;
	const Tuple & tuple;
};

template <unsigned int N>
struct LoadVariantDataListFunc
{
	template <typename Param>
	void operator()(Param & param)
	{
		param.variantList[N] = createTypedVariant(std::get<N>(param.tuple));
		param.variantDataList[N] = &param.variantList[N].refData();
	}
};

template <typename... Parameters>
void loadVariantDataList(GVariant * variantList, GVariantData ** variantDataList, const Parameters & ... parameters)
{
	const auto tuple = std::forward_as_tuple(parameters...);
	LoadVariantDataListParam<decltype(tuple)> param { variantList, variantDataList, tuple };
	GTypeForEach<sizeof...(Parameters)>::template forEach<LoadVariantDataListFunc>(param);
}

// This function is defined in gscriptvalue.cpp internally.
GScriptValue createScriptValueFromData(const GScriptValueData & data);


template <typename... Parameters>
GScriptValue invokeScriptFunction(GScriptObject * scriptObject, const char * functionName, const Parameters & ... parameters)
{
	constexpr size_t paramCount = sizeof...(Parameters);

	GVariant variantList[paramCount == 0 ? 1 : paramCount];
	loadVariantList(variantList, parameters...);
	
	return scriptObject->invoke(functionName, variantList, paramCount);
}

template <typename... Parameters>
GScriptValue invokeScriptFunction(IScriptObject * scriptObject, const char * functionName, const Parameters & ... parameters)
{
	constexpr size_t paramCount = sizeof...(Parameters);

	GScriptValueData data;
	// Hold the object so metaCheckError won't crash if scriptObject is freed in invoke
	GSharedInterface<IScriptObject> holder(scriptObject);

	GVariant variantList[paramCount == 0 ? 1 : paramCount];
	GVariantData * variantDataList[paramCount == 0 ? 1 : paramCount];
	loadVariantDataList(variantList, variantDataList, parameters...);
	
	scriptObject->invokeIndirectly(&data, functionName, variantDataList, paramCount);
	metaCheckError(scriptObject);

	return createScriptValueFromData(data);
}

template <typename... Parameters>
GScriptValue invokeScriptFunction(IScriptFunction * scriptFunction, const Parameters & ... parameters)
{
	constexpr size_t paramCount = sizeof...(Parameters);

	GScriptValueData data;
	// Hold the object so metaCheckError won't crash if scriptFunction is freed in invoke
	GSharedInterface<IScriptFunction> holder(scriptFunction);

	GVariant variantList[paramCount == 0 ? 1 : paramCount];
	GVariantData * variantDataList[paramCount == 0 ? 1 : paramCount];
	loadVariantDataList(variantList, variantDataList, parameters...);
	
	scriptFunction->invokeIndirectly(&data, variantDataList, paramCount);
	metaCheckError(scriptFunction);

	return createScriptValueFromData(data);
}

template <typename... Parameters>
GScriptValue invokeScriptFunctionOnObject(IScriptFunction * scriptFunction, const Parameters & ... parameters)
{
	constexpr size_t paramCount = sizeof...(Parameters);

	GScriptValueData data;
	// Hold the object so metaCheckError won't crash if scriptFunction is freed in invoke
	GSharedInterface<IScriptFunction> holder(scriptFunction);

	GVariant variantList[paramCount == 0 ? 1 : paramCount];
	GVariantData * variantDataList[paramCount == 0 ? 1 : paramCount];
	loadVariantDataList(variantList, variantDataList, parameters...);
	
	scriptFunction->invokeIndirectlyOnObject(&data, variantDataList, paramCount);
	metaCheckError(scriptFunction);

	return createScriptValueFromData(data);
}


GScriptValue scriptGetValue(GScriptObject * scriptObject, const char * name);
GScriptValue scriptGetValue(IScriptObject * scriptObject, const char * name);
void scriptSetValue(GScriptObject * scriptObject, const char * name, const GScriptValue & value);
void scriptSetValue(IScriptObject * scriptObject, const char * name, const GScriptValue & value);

GScriptValue scriptCreateScriptObject(GScriptObject * scriptObject, const char * name);
GScriptValue scriptCreateScriptObject(IScriptObject * scriptObject, const char * name);

GScriptValue scriptGetScriptFunction(GScriptObject * scriptObject, const char * name);
GScriptValue scriptGetScriptFunction(IScriptObject * scriptObject, const char * name);

GScriptValue scriptGetAsScriptArray(GScriptObject * scriptObject, const char * name);
GScriptValue scriptGetAsScriptArray(IScriptObject * scriptObject, const char * name);

GScriptValue scriptCreateScriptArray(GScriptObject * scriptObject, const char * name);
GScriptValue scriptCreateScriptArray(IScriptObject * scriptObject, const char * name);

GScriptValue scriptGetScriptArrayValue(IScriptArray * scriptArray, size_t index);
void scriptSetScriptArrayValue(IScriptArray * scriptArray, size_t index, const GScriptValue & value);
GScriptValue scriptGetAsScriptArray(IScriptArray * scriptArray, size_t index);
GScriptValue scriptCreateScriptArray(IScriptArray * scriptArray, size_t index);

IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject, bool freeObject);
IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject);

void injectObjectToScript(IScriptObject * scriptObject, IMetaClass * metaClass, void * instance, const char * namespaceName = nullptr);
void injectObjectToScript(GScriptObject * scriptObject, IMetaClass * metaClass, void * instance, const char * namespaceName = nullptr);
void injectObjectToScript(IScriptObject * scriptObject, GMetaClass * metaClass, void * instance, const char * namespaceName = nullptr);
void injectObjectToScript(GScriptObject * scriptObject, GMetaClass * metaClass, void * instance, const char * namespaceName = nullptr);

// We can't do this for GScriptObject because if namespaces is nullptr, we can't return owner of GScriptObject (we can't share the ownership)
IScriptObject * createScriptObject(IScriptObject * owner, const char * namespaces);


} // namespace cpgf



#endif

