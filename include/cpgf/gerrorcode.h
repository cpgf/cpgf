#ifndef __GERRORCODE_H
#define __GERRORCODE_H


namespace cpgf {


const int Error_None						= 0;

const int Error_Variant_Begin				= 1;
const int Error_Variant_FailCast			= Error_Variant_Begin + 0;
const int Error_Variant_FailCopyObject		= Error_Variant_Begin + 1;
const int Error_Variant_CantReferenceToTemp = Error_Variant_Begin + 2;
const int Error_Variant_FailAdjustTypeSize	= Error_Variant_Begin + 3;
const int Error_Variant_End					= 100;

const int Error_Meta_Begin					= 101;
const int Error_Meta_ParamOutOfIndex		= Error_Meta_Begin + 0;
const int Error_Meta_ReadDenied				= Error_Meta_Begin + 1;
const int Error_Meta_WriteDenied			= Error_Meta_Begin + 2;
const int Error_Meta_CannotInitAbstractClass	= Error_Meta_Begin + 3;
const int Error_Meta_NoDefaultConstructor	= Error_Meta_Begin + 4;
const int Error_Meta_NoCopyConstructor		= Error_Meta_Begin + 5;
const int Error_Meta_NotUnaryOperator		= Error_Meta_Begin + 6;
const int Error_Meta_NotBinaryOperator		= Error_Meta_Begin + 7;
const int Error_Meta_NotFunctorOperator		= Error_Meta_Begin + 8;
const int Error_Meta_WrongArity				= Error_Meta_Begin + 9;
const int Error_Meta_End					= 200;

const int Error_ScriptBinding_Begin			= 201;
const int Error_ScriptBinding_InternalError_WrongFunctor= Error_ScriptBinding_Begin + 0;
const int Error_ScriptBinding_NotSupportedOperator		= Error_ScriptBinding_Begin + 1;
const int Error_ScriptBinding_FailVariantToScript		= Error_ScriptBinding_Begin + 2;
const int Error_ScriptBinding_CantFindMatchedMethod		= Error_ScriptBinding_Begin + 3;
const int Error_ScriptBinding_FailConstructObject		= Error_ScriptBinding_Begin + 4;
const int Error_ScriptBinding_CantFindEnumKey			= Error_ScriptBinding_Begin + 5;
const int Error_ScriptBinding_CantAssignToEnumMethodClass	= Error_ScriptBinding_Begin + 6;
const int Error_ScriptBinding_ScriptMethodParamMismatch	= Error_ScriptBinding_Begin + 7;
const int Error_ScriptBinding_CantReturnMultipleValue	= Error_ScriptBinding_Begin + 8;
const int Error_ScriptBinding_CantCallNonfunction		= Error_ScriptBinding_Begin + 9;
const int Error_ScriptBinding_CantBindFundamental		= Error_ScriptBinding_Begin + 10;
const int Error_ScriptBinding_ScriptFunctionReturnError	= Error_ScriptBinding_Begin + 11;
const int Error_ScriptBinding_CantWriteToConstObject	= Error_ScriptBinding_Begin + 12;
const int Error_ScriptBinding_CallMethodWithTooManyParameters	= Error_ScriptBinding_Begin + 13;
const int Error_ScriptBinding_AccessMemberWithWrongObject	= Error_ScriptBinding_Begin + 14;
const int Error_ScriptBinding_End			= 300;


} // namespace cpgf



#endif

