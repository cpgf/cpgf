#ifndef CPGF_GERRORCODE_H
#define CPGF_GERRORCODE_H


namespace cpgf {


extern int Error_None;

extern int Error_Variant_Begin;
extern int Error_Variant_FailCast;
extern int Error_Variant_FailCopyObject;
extern int Error_Variant_CantReferenceToTemp;
extern int Error_Variant_FailAdjustTypeSize;
extern int Error_Variant_End;

extern int Error_Meta_Begin;
extern int Error_Meta_ParamOutOfIndex;
extern int Error_Meta_ReadDenied;
extern int Error_Meta_WriteDenied;
extern int Error_Meta_CannotInitAbstractClass;
extern int Error_Meta_NoDefaultexternructor;
extern int Error_Meta_NoCopyConstructor;
extern int Error_Meta_NotUnaryOperator;
extern int Error_Meta_NotBinaryOperator;
extern int Error_Meta_NotFunctorOperator;
extern int Error_Meta_WrongArity;
extern int Error_Meta_End;

extern int Error_ScriptBinding_Begin;
extern int Error_ScriptBinding_InternalError_WrongFunctor;
extern int Error_ScriptBinding_NotSupportedOperator;
extern int Error_ScriptBinding_FailVariantToScript;
extern int Error_ScriptBinding_CantFindMatchedMethod;
extern int Error_ScriptBinding_FailConstructObject;
extern int Error_ScriptBinding_CantFindEnumKey;
extern int Error_ScriptBinding_CantAssignToEnumMethodClass;
extern int Error_ScriptBinding_ScriptMethodParamMismatch;
extern int Error_ScriptBinding_CantReturnMultipleValue;
extern int Error_ScriptBinding_CantCallNonfunction;
extern int Error_ScriptBinding_CantBindFundamental;
extern int Error_ScriptBinding_ScriptFunctionReturnError;
extern int Error_ScriptBinding_CantWriteToConstObject;
extern int Error_ScriptBinding_CallMethodWithTooManyParameters;
extern int Error_ScriptBinding_AccessMemberWithWrongObject;
extern int Error_ScriptBinding_CantBindRaw;
extern int Error_ScriptBinding_NoContext;
extern int Error_ScriptBinding_CantFindMatchedOperator;
extern int Error_ScriptBinding_NotSupportedFeature;
extern int Error_ScriptBinding_CantSetScriptValue;
extern int Error_ScriptBinding_CantFindObject;
extern int Error_ScriptBinding_End;

extern int Error_Serialization_Begin;
extern int Error_Serialization_TypeMismatch;
extern int Error_Serialization_CannotFindObjectType;
extern int Error_Serialization_MissingMetaClass;
extern int Error_Serialization_UnknownType;
extern int Error_Serialization_InvalidStorage;
extern int Error_Serialization_End;

extern int Error_Tween_Begin;
extern int Error_Tween_TweenableNotOwnedByTimeline;
extern int Error_Tween_End ;

} // namespace cpgf



#endif

