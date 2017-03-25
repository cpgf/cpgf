#include "cpgf/gexception.h"
#include "cpgf/gerrorcode.h"

#include <stdio.h>
#include <stdarg.h>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


namespace cpgf {

int Error_None						= 0;

int Error_Variant_Begin				= 1;
int Error_Variant_FailCast			= Error_Variant_Begin + 0;
int Error_Variant_FailCopyObject		= Error_Variant_Begin + 1;
int Error_Variant_CantReferenceToTemp = Error_Variant_Begin + 2;
int Error_Variant_FailAdjustTypeSize	= Error_Variant_Begin + 3;
int Error_Variant_End					= 100;

int Error_Meta_Begin					= 101;
int Error_Meta_ParamOutOfIndex		= Error_Meta_Begin + 0;
int Error_Meta_ReadDenied				= Error_Meta_Begin + 1;
int Error_Meta_WriteDenied			= Error_Meta_Begin + 2;
int Error_Meta_CannotInitAbstractClass	= Error_Meta_Begin + 3;
int Error_Meta_NoDefaultConstructor	= Error_Meta_Begin + 4;
int Error_Meta_NoCopyConstructor		= Error_Meta_Begin + 5;
int Error_Meta_NotUnaryOperator		= Error_Meta_Begin + 6;
int Error_Meta_NotBinaryOperator		= Error_Meta_Begin + 7;
int Error_Meta_NotFunctorOperator		= Error_Meta_Begin + 8;
int Error_Meta_WrongArity				= Error_Meta_Begin + 9;
int Error_Meta_End					= 200;

int Error_ScriptBinding_Begin			= 201;
int Error_ScriptBinding_InternalError_WrongFunctor= Error_ScriptBinding_Begin + 0;
int Error_ScriptBinding_NotSupportedOperator		= Error_ScriptBinding_Begin + 1;
int Error_ScriptBinding_FailVariantToScript		= Error_ScriptBinding_Begin + 2;
int Error_ScriptBinding_CantFindMatchedMethod		= Error_ScriptBinding_Begin + 3;
int Error_ScriptBinding_FailConstructObject		= Error_ScriptBinding_Begin + 4;
int Error_ScriptBinding_CantFindEnumKey			= Error_ScriptBinding_Begin + 5;
int Error_ScriptBinding_CantAssignToEnumMethodClass	= Error_ScriptBinding_Begin + 6;
int Error_ScriptBinding_ScriptMethodParamMismatch	= Error_ScriptBinding_Begin + 7;
int Error_ScriptBinding_CantReturnMultipleValue	= Error_ScriptBinding_Begin + 8;
int Error_ScriptBinding_CantCallNonfunction		= Error_ScriptBinding_Begin + 9;
int Error_ScriptBinding_CantBindFundamental		= Error_ScriptBinding_Begin + 10;
int Error_ScriptBinding_ScriptFunctionReturnError	= Error_ScriptBinding_Begin + 11;
int Error_ScriptBinding_CantWriteToConstObject	= Error_ScriptBinding_Begin + 12;
int Error_ScriptBinding_CallMethodWithTooManyParameters	= Error_ScriptBinding_Begin + 13;
int Error_ScriptBinding_AccessMemberWithWrongObject	= Error_ScriptBinding_Begin + 14;
int Error_ScriptBinding_CantBindRaw			= Error_ScriptBinding_Begin + 15;
int Error_ScriptBinding_NoContext				= Error_ScriptBinding_Begin + 16;
int Error_ScriptBinding_CantFindMatchedOperator		= Error_ScriptBinding_Begin + 17;
int Error_ScriptBinding_NotSupportedFeature		= Error_ScriptBinding_Begin + 18;
int Error_ScriptBinding_CantSetScriptValue		= Error_ScriptBinding_Begin + 19;
int Error_ScriptBinding_CantFindObject = Error_ScriptBinding_Begin + 21;
int Error_ScriptBinding_End			= 300;

int Error_Serialization_Begin = 301;
int Error_Serialization_TypeMismatch = Error_Serialization_Begin + 0;
int Error_Serialization_CannotFindObjectType = Error_Serialization_Begin + 1;
int Error_Serialization_MissingMetaClass = Error_Serialization_Begin + 2;
int Error_Serialization_UnknownType = Error_Serialization_Begin + 3;
int Error_Serialization_InvalidStorage = Error_Serialization_Begin + 4;
int Error_Serialization_End = 400;

int Error_Tween_Begin = 401;
int Error_Tween_TweenableNotOwnedByTimeline = Error_Tween_Begin + 0;
int Error_Tween_End = 500;

GException::GException(int errorCode, const char * message)
	: super(message), errorCode(errorCode)
{
}

int GException::getCode() const
{
	return this->errorCode;
}

const char * GException::getMessage() const
{
	return this->what();
}

void raiseException(int errorCode, const char *  message)
{
//	fprintf(stderr, "%s\n", message);

	throw GException(errorCode, message);
}

void raiseFormatException(int errorCode, const char * message, ...)
{
	char buffer[4096];

	va_list args;
	va_start(args, message);

	vsprintf(buffer, message, args);

	va_end (args);

	raiseException(errorCode, buffer);
}

namespace {
	struct ExceptionErrorInfo {
		int code;
		const char * message;
	};

	const ExceptionErrorInfo errorInfoList[] = {
		{ Error_Variant_FailCast, "GVariant: cast failure" },
		{ Error_Variant_CantReferenceToTemp, "GVariant: can't reference to temporary." },
		{ Error_Variant_FailCopyObject, "GVariant: can't create shadow object for noncopyable object." },
		{ Error_Variant_FailAdjustTypeSize, "GVariant: can't adjust size for inconsistent type." },

		{ Error_Meta_ReadDenied, "Can't read object. Read is forbidden." },
		{ Error_Meta_WriteDenied, "Can't write object. Write is forbidden." },
		{ Error_Meta_CannotInitAbstractClass, "Can't instantiate abstract class." },
		{ Error_Meta_NoDefaultConstructor, "Can't instantiate object that has no default constructor." },
		{ Error_Meta_NoCopyConstructor, "Can't copy object that has no copy constructor." },
		{ Error_Meta_ParamOutOfIndex, "Parameter out of index" },
		{ Error_Meta_NotUnaryOperator, "Can't invoke non-unary operator." },
		{ Error_Meta_NotBinaryOperator, "Can't invoke non-binary operator." },
		{ Error_Meta_NotFunctorOperator, "Can't invoke non-functor operator." },

		{ Error_ScriptBinding_FailVariantToScript,				"Can't convert variant to script object." },
		{ Error_ScriptBinding_CallMethodWithTooManyParameters,	"Too many parameters." },
		{ Error_ScriptBinding_CantFindMatchedMethod,			"Can't find matched method to invoke" },
		{ Error_ScriptBinding_FailConstructObject,				"Failed to construct an object." },
		{ Error_ScriptBinding_InternalError_WrongFunctor,		"Internal error: calling wrong functor." },
		{ Error_ScriptBinding_CantWriteToConstObject,			"Can't write to constant object." },
		{ Error_ScriptBinding_NotSupportedOperator,				"Failed to bind an operator that's not supported by the script." },
		{ Error_ScriptBinding_CantAssignToEnumMethodClass,		"Can't assign value to enumerator, method, or class." },
		{ Error_ScriptBinding_CantCallNonfunction,				"The script function being invoked is not a function." },
		{ Error_ScriptBinding_CantBindFundamental,				"Failed to bind fundamental variable" },
		{ Error_ScriptBinding_AccessMemberWithWrongObject,		"Access class member with wrong object." },
		{ Error_ScriptBinding_CantBindRaw, 						"Failed to bind raw data." },
		{ Error_ScriptBinding_NoContext,						"Script context doesn't exist." },
		{ Error_ScriptBinding_CantFindMatchedOperator,			"Can't find matched opereator to invoke." },
		{ Error_ScriptBinding_CantSetScriptValue,				"Can't set the script value to script engine." },
		{ Error_ScriptBinding_CantFindObject,	"Failed to find the requested object." },

		{ Error_Serialization_TypeMismatch,			"Type mismatch when reading serialized object." },
		{ Error_Serialization_CannotFindObjectType,	"Can't find object type." },
		{ Error_Serialization_MissingMetaClass,		"Meta class type is missing." },
		{ Error_Serialization_UnknownType,			"Unknow meta type." },
		{ Error_Serialization_InvalidStorage,			"Serializing storage is corrupted." },

		{ Error_Tween_TweenableNotOwnedByTimeline,		"Tweenable is not owned by the timeline it is adding to." },


		{ -1, nullptr },

		{ Error_Meta_WrongArity, "Wrong argument count. Expect: %d, but get: %d." },

		{ Error_ScriptBinding_CantFindMatchedMethod,			"Can't invoke meta method %s" },
		{ Error_ScriptBinding_CantFindEnumKey,					"Can't find enumerator key -- %s." },
		{ Error_ScriptBinding_ScriptMethodParamMismatch,		"Can't pass parameter at index %d in function %s" },
		{ Error_ScriptBinding_ScriptFunctionReturnError,		"Error when calling function %s, message: %s" },
		{ Error_ScriptBinding_CantReturnMultipleValue,			"Can't return multiple value when calling function %s" },
		{ Error_ScriptBinding_NotSupportedFeature,				"Feature %s it not supported by script binding for %s" },
	};

	const char * notFoundErrorMessage = "Can't find error message.";
}

void raiseCoreException(int errorCode, ...)
{
	const char * message = nullptr;
	bool hasArgs = false;

	for(size_t i = 0; i < sizeof(errorInfoList) / sizeof(errorInfoList[0]); ++i) {
		if(errorInfoList[i].code < 0) {
			hasArgs = true;
		}
		else {
			if(errorInfoList[i].code == errorCode) {
				message = errorInfoList[i].message;
				break;
			}
		}
	}

	if(message == nullptr) {
		message = notFoundErrorMessage;
		hasArgs = false;
	}

	if(hasArgs) {
		char buffer[4096];

		va_list args;
		va_start(args, errorCode);

		vsprintf(buffer, message, args);

		va_end (args);

		raiseException(errorCode, buffer);
	}
	else {
		raiseException(errorCode, message);
	}

}



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

