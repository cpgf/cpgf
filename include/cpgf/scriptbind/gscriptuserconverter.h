#ifndef CPGF_GSCRIPTUSERCONVERTER_H
#define CPGF_GSCRIPTUSERCONVERTER_H

#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gmetatype.h"
#include "cpgf/ginterface.h"
#include "cpgf/gvariant.h"
#include "cpgf/gapiutil.h"


namespace cpgf {

#pragma pack(push, 1)
#pragma pack(1)
struct GScriptUserConverterParamData
{
	IMetaCallable * callable;
	uint32_t paramIndex;

	GScriptValueData * sourceValue;
};
#pragma pack(pop)


struct IScriptUserConverter : public IObject
{
	virtual uint32_t G_API_CC canConvert(const GScriptUserConverterParamData * paramData) = 0;
	virtual void G_API_CC convert(GVariantData * outputValue, const GScriptUserConverterParamData * paramData, uint32_t tag) = 0;
};


class GScriptUserConverterBase : public IScriptUserConverter
{
	G_INTERFACE_IMPL_OBJECT
public:
    virtual ~GScriptUserConverterBase() {}
};


} // namespace cpgf



#endif
