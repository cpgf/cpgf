#ifndef CPGF_GBINDCOMMON_H
#define CPGF_GBINDCOMMON_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/scriptbind/gscriptuserconverter.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gglobal.h"
#include "cpgf/gmetaoperatorop.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gsharedinterface.h"

#include "gbindmetamap.h"
#include "gbindgluedata.h"
#include "gbindobject.h"
#include "gbindcontext.h"
#include "gbindconverttoscript.h"


namespace cpgf {

namespace bind_internal {

class ConvertRank
{
public:
	void resetRank() {
		this->weight = 0; //ValueMatchRank_Unknown;
/* it's safe to not reset the pointers because they are used according to weight.
		this->sourceClass = nullptr;
		this->targetClass.reset();
		this->userConverter = nullptr;
		this->userConverterTag = 0;
*/
	}

public:
	int weight;
	IMetaClass * sourceClass;
	// We have to use GSharedInterface to hold targetClass
	// because the targetClass may be created dynamically from the global repository.
	// sourceClass doesn't have this problem, it's always hold by the caller.
	GSharedInterface<IMetaClass> targetClass;
	IScriptUserConverter * userConverter;
	uint32_t userConverterTag;
};

class CallableParamData
{
public:
	GScriptValue value;
	GGlueDataPointer paramGlueData;
};

class InvokeCallableParam
{
public:
	InvokeCallableParam(size_t paramCount, IScriptContext * scriptContext);
	~InvokeCallableParam();

public:
	CallableParamData * params;
	char paramsBuffer[sizeof(CallableParamData) * REF_MAX_ARITY];
	size_t paramCount;
	ConvertRank * paramRanks;
	char paramRanksBuffer[sizeof(ConvertRank) * REF_MAX_ARITY];
	ConvertRank * backParamRanks;
	char paramRanksBackBuffer[sizeof(ConvertRank) * REF_MAX_ARITY];
	GSharedInterface<IScriptContext> scriptContext;
};

int rankCallable(
	IMetaService * service,
	const GObjectGlueDataPointer & objectData,
	IMetaCallable * callable,
	const InvokeCallableParam * callbackParam,
	ConvertRank * paramRanks
);

void * doInvokeConstructor(
	const GContextPointer & context,
	IMetaService * service,
	IMetaClass * metaClass,
	InvokeCallableParam * callableParam
);

InvokeCallableResult doInvokeMethodList(
	const GContextPointer & context,
	const GObjectGlueDataPointer & objectData,
	const GMethodGlueDataPointer & methodData,
	InvokeCallableParam * callableParam
);

GScriptValue glueDataToScriptValue(const GGlueDataPointer & glueData);

IMetaClass * selectBoundClass(IMetaClass * currentClass, IMetaClass * derived);

bool setValueOnNamedMember(
	const GGlueDataPointer & glueData,
	const char * name,
	const GScriptValue & value,
	const GGlueDataPointer & valueGlueData
);

InvokeCallableResult doInvokeOperator(
	const GContextPointer & context,
	const GObjectGlueDataPointer & objectData,
	IMetaClass * metaClass,
	GMetaOpType op,
	InvokeCallableParam * callableParam
);

std::string getMethodNameFromMethodList(IMetaList * methodList);

struct ConvertRankBuffer
{
	explicit ConvertRankBuffer(const size_t paramCount)
		: paramCount(paramCount), paramRanks((ConvertRank *)paramRanksBuffer)
	{
		memset(this->paramRanksBuffer, 0, sizeof(ConvertRank) * paramCount);
	};
	
	~ConvertRankBuffer()
	{
	for(size_t i = 0; i < this->paramCount; ++i) {
		this->paramRanks[i].~ConvertRank();
	}
	}

	size_t paramCount;
	ConvertRank * paramRanks;
	char paramRanksBuffer[sizeof(ConvertRank) * REF_MAX_ARITY];
};

template <typename Getter, typename Predict>
int findAppropriateCallable(
	IMetaService * service,
	const GObjectGlueDataPointer & objectData,
	const Getter & getter,
	const size_t callableCount,
	InvokeCallableParam * callableParam,
	const Predict & predict
)
{
	int maxRank = -1;
	int maxRankIndex = -1;

	for(size_t i = 0; i < callableCount; ++i) {
		GScopedInterface<IMetaCallable> meta(gdynamic_cast<IMetaCallable *>(getter(static_cast<uint32_t>(i))));
		if(predict(meta.get())) {
			const int weight = rankCallable(service, objectData, meta.get(), callableParam, callableParam->backParamRanks);
			if(weight > maxRank) {
				maxRank = weight;
				maxRankIndex = static_cast<int>(i);
				std::swap(callableParam->paramRanks, callableParam->backParamRanks);
			}
			if(callableCount > 1) {
				for(size_t i = 0; i < callableParam->paramCount; ++i) {
					callableParam->backParamRanks[i].resetRank();
				}
			}
		}
	}

	return maxRankIndex;
}



} // namespace bind_internal


} // namespace cpgf



#endif
