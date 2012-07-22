#include "cpgf/gexception.h"
#include "cpgf/gerrorcode.h"

#include <stdio.h>
#include <stdarg.h>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


namespace cpgf {

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
	struct ErrorInfo {
		int code;
		const char * message;
	};

	const ErrorInfo errorInfoList[] = {
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

		{ Error_Serialization_TypeMismatch,			"Type mismatch when reading serialized object." },
		{ Error_Serialization_CannotFindObjectType,	"Can't find object type." },
		{ Error_Serialization_MissingMetaClass,		"Meta class type is missing." },
		{ Error_Serialization_UnknownType,			"Unknow meta type." },
		{ Error_Serialization_InvalidStorage,			"Serializing storage is corrupted." },


		{ -1, NULL },

		{ Error_Meta_WrongArity, "Wrong argument count. Expect: %d, but get: %d." },
		
		{ Error_ScriptBinding_CantFindMatchedMethod,			"Can't invoke meta method %s" },
		{ Error_ScriptBinding_CantFindEnumKey,					"Can't find enumerator key -- %s." },
		{ Error_ScriptBinding_ScriptMethodParamMismatch,		"Can't pass parameter at index %d in function %s" },
		{ Error_ScriptBinding_ScriptFunctionReturnError,		"Error when calling function %s, message: %s" },
		{ Error_ScriptBinding_CantReturnMultipleValue,			"Can't return multiple value when calling function %s" },
	};

	const char * notFoundErrorMessage = "Can't find error message.";
}
	
void raiseCoreException(int errorCode, ...)
{
	const char * message = NULL;
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

	if(message == NULL) {
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

