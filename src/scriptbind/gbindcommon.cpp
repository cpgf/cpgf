#include "gbindcommon.h"

namespace cpgf {


ObjectPointerCV metaTypeToCV(const GMetaType & type)
{
	if(type.isPointer()) {
		if(type.isPointerToConst()) {
			return opcvConst;
		}
		else if(type.isPointerToVolatile()) {
			return opcvVolatile;
		}
		else if(type.isPointerToConstVolatile()) {
			return opcvConstVolatile;
		}
	}
	else {
		if(type.isConst()) {
			return opcvConst;
		}
		else if(type.isVolatile()) {
			return opcvVolatile;
		}
		else if(type.isConstVolatile()) {
			return opcvConstVolatile;
		}
	}
	
	return opcvNone;
}

void cvToFilters(ObjectPointerCV cv, GFlags<GMetaFilters> * filters)
{
	switch(cv) {
		case opcvConst:
			filters->set(metaFilterConstMethod);
			break;
			
		case opcvVolatile:
			filters->set(metaFilterVolatileMethod);
			break;
			
		case opcvConstVolatile:
			filters->set(metaFilterConstVolatileMethod);
			break;
			
		default:
			break;
	}
}

int rankCallable(IMetaCallable * callable, GVariantData * paramsData, GScriptDataType * paramsType, size_t paramCount)
{
	if(!! callable->isVariadic()) {
		return 0;
	}

	if(callable->getParamCount() != paramCount) {
		return -1;
	}

	for(size_t i = 0; i < paramCount; ++i) {
		if(!callable->checkParam(&paramsData[i], i)) {
			return -1;
		}
	}

	int rank = static_cast<int>(paramCount) + 1;

	for(size_t i = 0; i < paramCount; ++i) {
		GMetaType proto = metaGetParamType(callable, i);
		GScriptDataType sdt = paramsType[i];
		if(sdt == sdtNull) {
			continue;
		}
		if(proto.isFundamental() && sdt == sdtFundamental) {
			continue;
		}
		
		--rank;
	}

	return rank;
}

bool checkCallable(IMetaCallable * callable, GVariantData * paramsData, size_t paramCount)
{
	if(!! callable->isVariadic()) {
		return true;
	}

	if(callable->getParamCount() != paramCount) {
		return false;
	}

	for(size_t i = 0; i < paramCount; ++i) {
		if(!callable->checkParam(&paramsData[i], i)) {
			return false;
		}
	}

	return true;
}

IMetaAccessible * findAccessible(IMetaClass * metaClass, const char * name, bool checkGet, bool checkSet, void ** instance)
{
	GScopedInterface<IMetaAccessible> data;

	data.reset(metaClass->getField(name));
	
	if(!data || (checkGet && !data->canGet()) || (checkSet && !data->canSet())) {
		data.reset(metaClass->getProperty(name));
	}
	
	if(!data || (checkGet && !data->canGet()) || (checkSet && !data->canSet())) {
		size_t baseCount = metaClass->getBaseCount();
		void ** self = instance;
		for(size_t i = 0; i < baseCount; ++i) {
			*instance = metaClass->castToBase(*self, i);

			GScopedInterface<IMetaClass> baseClass(metaClass->getBaseClass(i));
			data.reset(findAccessible(metaClass, name, checkGet, checkSet, instance));
			if(data) {
				break;
			}
		}
	}

	return data.take();
}



} // namespace cpgf


