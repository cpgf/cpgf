#include "cpgf/metautility/gmetadebug.h"
#include "cpgf/gcompiler.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/scriptbind/gscriptbindapi.h"

#include <stdio.h>


namespace cpgf {


void GMetaDebug::debugBreak(const GMetaVariadicParam * params)
{
#if defined(__i386) || defined(_M_IX86)
	#if defined(G_COMPILER_VC)
		__asm int 3;
	#elif defined(G_COMPILER_GCC)
		__asm__("int $0x03");
	#endif
#else
#endif

	if(params != nullptr) {
		// Nothing to do. Inspect params in the debugger if you need.
		GVariant value;
		for(size_t i = 0; i < params->paramCount; i++) {
			value = *(params->params[i]);
		}
	}
}

void GMetaDebug::trace(const char * message)
{
	fprintf(stdout, "%s", message);
}

void GMetaDebug::traceError(const char * message)
{
	fprintf(stderr, "%s", message);
}

void GMetaDebug::traceLine(const char * message)
{
	fprintf(stdout, "%s\n", message);
}

void GMetaDebug::traceErrorLine(const char * message)
{
	fprintf(stderr, "%s\n", message);
}

GVariant GMetaDebug::inspect(const GVariant & value)
{
	std::string result = "";
	const GVariantType vt = value.getType();
	if(vtIsBoolean(vt)) {
		result = "Bool: ";
		result += (fromVariant<bool>(value) ? "true" : "false");
	}
	else if(vtIsInteger(vt)) {
		result = "int: ";
		result += std::to_string(fromVariant<long long>(value));
	}
	else if(vtIsReal(vt)) {
		result = "real: ";
		result += std::to_string(fromVariant<long double>(value));
	}
	else if(variantIsString(value)) {
		result = "string: ";
		result += fromVariant<std::string>(value);
	}
	else if(variantIsWideString(value)) {
		std::wstring ws = L"wide string: ";
		ws += fromVariant<std::wstring>(value);
		return createWideStringVariant(ws.c_str());
	}
	else if(vtIsTypedVar(vt)) {
		result = "Typed variant";
		//return inspect(getVariantRealValue(value));
	}
	else if(vtIsInterface(vt)) {
		IObject * obj = fromVariant<IObject *>(value);
		if(obj == nullptr) {
			result = "Null interface";
		}
		else if(IMetaClass * metaClass = dynamic_cast<IMetaClass *>(obj)) {
			result = "Class: ";
			result += metaClass->getName();
		}
		else if(dynamic_cast<IScriptFunction *>(obj)) {
			result = "Script function";
		}
	}
	else {
		result = "Unknown value: type ";
		result += std::to_string((int)vt);
	}
	return createStringVariant(result.c_str());
}


} // namespace cpgf


