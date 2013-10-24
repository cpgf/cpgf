#ifndef __GSCRIPTUSERCONVERTER_H
#define __GSCRIPTUSERCONVERTER_H

#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/gmetatype.h"
#include "cpgf/ginterface.h"
#include "cpgf/gvariant.h"
#include "cpgf/gapiutil.h"


namespace cpgf {

struct IMetaCallable;

#pragma pack(push, 1)
#pragma pack(1)
struct GScriptUserConverterParamData
{
	IMetaCallable * callable;
	uint32_t paramIndex;

	GScriptValueData sourceValue;
	GMetaTypeData sourceType;
};
#pragma pack(pop)


struct IScriptUserConverter : public IObject
{
	virtual gapi_bool G_API_CC canConvert(const GScriptUserConverterParamData * paramData) = 0;
	virtual void G_API_CC convert(GVariantData * outputValue, const GScriptUserConverterParamData * paramData) = 0;
};


class GScriptUserConverterBase : public IScriptUserConverter
{
	G_INTERFACE_IMPL_OBJECT
};


} // namespace cpgf



#endif
