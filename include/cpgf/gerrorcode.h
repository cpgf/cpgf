#ifndef __GERRORCODE_H
#define __GERRORCODE_H


namespace cpgf {


const int Error_None						= 0;

const int Error_Variant_Begin				= 1;
const int Error_Variant_FailCast			= Error_Variant_Begin + 0;
const int Error_Variant_FailCopyObject		= Error_Variant_Begin + 1;
const int Error_Variant_FailAdjustTypeSize	= Error_Variant_Begin + 2;
const int Error_Variant_End					= 100;

const int Error_Meta_Begin					= 101;
const int Error_Meta_WrongArity				= Error_Meta_Begin + 0;
const int Error_Meta_ParamOutOfIndex		= Error_Meta_Begin + 1;
const int Error_Meta_PropertyCannotGet		= Error_Meta_Begin + 2;
const int Error_Meta_PropertyCannotSet		= Error_Meta_Begin + 3;
const int Error_Meta_PropertyCannotAccess	= Error_Meta_Begin + 4;
const int Error_Meta_CannotInitAbstractClass	= Error_Meta_Begin + 5;
const int Error_Meta_AccessNoncopable		= Error_Meta_Begin + 6;
const int Error_Meta_End					= 200;

const int Error_Lua_Begin					= 201;
const int Error_Lua_InternalError_CantFindMethodListName	= Error_Lua_Begin + 0;
const int Error_Lua_InternalError_WrongFunctor				= Error_Lua_Begin + 1;
const int Error_Lua_FailVariantToLua		= Error_Lua_Begin + 2;
const int Error_Lua_FailInvokeMetaMethod	= Error_Lua_Begin + 3;
const int Error_Lua_CantFindMatchedMethod	= Error_Lua_Begin + 4;
const int Error_Lua_FailConstructObject		= Error_Lua_Begin + 5;
const int Error_Lua_NotSupportedOperator	= Error_Lua_Begin + 6;
const int Error_Lua_CantFindEnumKey			= Error_Lua_Begin + 7;
const int Error_Lua_CantAssignToEnum		= Error_Lua_Begin + 8;
const int Error_Lua_MethodParamMismatch		= Error_Lua_Begin + 9;
const int Error_Lua_CantReturnMultipleValue	= Error_Lua_Begin + 10;
const int Error_Lua_CantCallNonfunction		= Error_Lua_Begin + 11;
const int Error_Lua_CantBindFundamental		= Error_Lua_Begin + 12;
const int Error_Lua_ScriptFunctionReturnError				= Error_Lua_Begin + 13;
const int Error_Lua_End						= 300;


} // namespace cpgf



#endif

