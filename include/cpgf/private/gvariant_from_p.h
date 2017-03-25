// This file implements the function "fromVariant" and "canFromVariant"

#ifdef G_COMPILER_GCC
#pragma GCC diagnostic push
// ignore warning: returning reference to temporary
// those "problematical" code is not executed on those types so it's safe to ignore the warning.
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#endif

#ifdef G_COMPILER_VC
#pragma warning(push)
#pragma warning(disable:4172) // warning C4172: returning address of local variable or temporar
#pragma warning(disable:4244) // warning C4244: 'argument': conversion from 'U' to 'int', possible loss of data
#pragma warning(disable:4800) // warning C4800: 'U': forcing value to bool 'true' or 'false' (performance warning)
#endif


namespace variant_internal {

template <typename T, bool IsEnum>
struct HelperValueTypeSelector;

template <typename T>
struct HelperValueTypeSelector <T, true>
{
	typedef typename std::underlying_type<T>::type type;
};

template <typename T>
struct HelperValueTypeSelector <T, false>
{
	typedef T type;
};

template <typename T>
struct HelperValueType
{
	typedef typename HelperValueTypeSelector<T, std::is_enum<typename std::remove_reference<T>::type>::value>::type type;
};

template <typename T>
T helperReturnEmptyValue(typename std::enable_if<std::is_lvalue_reference<T>::value || std::is_rvalue_reference<T>::value>::type * = 0)
{
	typedef typename std::remove_reference<T>::type NoRef;
	//static NoRef x = NoRef();
	//return (T)x;
	return (T)*(NoRef *)0;
}

template <typename T>
T helperReturnEmptyValue(typename std::enable_if<! std::is_lvalue_reference<T>::value && ! std::is_rvalue_reference<T>::value>::type * = 0)
{
	return T();
}

template <typename T, typename FromTypeList, typename V>
T helperFromVariant(const V & from, typename std::enable_if<TypeListConvertible<FromTypeList, T>::convertible>::type * = 0)
{
	typedef typename std::remove_reference<typename TypeListConvertible<FromTypeList, T>::type>::type U;
	return (T)*(U *)(&from);
}

template <typename T, typename FromTypeList, typename V>
T helperFromVariant(const V & /*from*/, typename std::enable_if<! TypeListConvertible<FromTypeList, T>::convertible>::type * = 0)
{
	failedCast();
	return helperReturnEmptyValue<T>();
}

template <typename T, typename PointerTypeList, typename ReferenceTypeList, typename V>
T helperFromPointerOrReference(const V & from, typename std::enable_if<TypeListConvertible<PointerTypeList, T>::convertible>::type * = 0)
{
	typedef typename std::remove_reference<typename TypeListConvertible<PointerTypeList, T>::type>::type U;
	return (T)(U)(&from);
}

template <typename T, typename PointerTypeList, typename ReferenceTypeList, typename V>
T helperFromPointerOrReference(const V & from, typename std::enable_if<! TypeListConvertible<PointerTypeList, T>::convertible && TypeListConvertible<ReferenceTypeList, T>::convertible>::type * = 0)
{
	typedef typename std::remove_reference<typename TypeListConvertible<ReferenceTypeList, T>::type>::type U;
	return (T)*(U *)(&from);
}

template <typename T, typename PointerTypeList, typename ReferenceTypeList, typename V>
T helperFromPointerOrReference(const V & /*from*/, typename std::enable_if<! TypeListConvertible<PointerTypeList, T>::convertible && ! TypeListConvertible<ReferenceTypeList, T>::convertible>::type * = 0)
{
	failedCast();
	return helperReturnEmptyValue<T>();
}

template <typename T>
struct FromObjectType
{
	typedef typename std::remove_cv<
		typename std::remove_reference<
			typename std::remove_cv<T>::type
		>::type
	>::type type;
};

template <typename T>
T helperFromObject(void * object, typename std::enable_if<std::is_pointer<typename FromObjectType<T>::type>::value>::type * = 0)
{
	typedef typename FromObjectType<T>::type U;
	return (T)*(U *)(&object);
}

template <typename T>
T helperFromObject(void * object, typename std::enable_if<! std::is_pointer<typename FromObjectType<T>::type>::value>::type * = 0)
{
	typedef typename FromObjectType<T>::type U;
	return (T)*(U *)(object);
}

template <typename T>
T helperFromNullPointer(typename std::enable_if<std::is_pointer<T>::value>::type * = 0)
{
	return (T)0;
}

template <typename T>
T helperFromNullPointer(typename std::enable_if<! std::is_pointer<T>::value>::type * = 0)
{
	failedCast();
	return helperReturnEmptyValue<T>();
}

template <typename T, typename Policy>
struct CastVariant_Value
{
	typedef typename VariantCastResult<T, Policy>::Result ResultType;

	typedef cpgf::GTypeList<const char *, char *, const volatile char *, volatile char *> StringCharTypeList;
	typedef cpgf::GTypeList<const std::string &, std::string, std::string &, const volatile std::string &, volatile std::string &> StringStringTypeList;
	typedef cpgf::GTypeList<const wchar_t *, wchar_t *, const volatile wchar_t *, volatile wchar_t *> WideStringCharTypeList;
	typedef cpgf::GTypeList<const std::wstring &, std::wstring, std::wstring &, const volatile std::wstring &, volatile std::wstring &> WideStringStringTypeList;

	static ResultType cast(const GVariantData & data)
	{
		// 0 can always be converted to pointer, similar as we do in C++.
		if(std::is_pointer<ResultType>::value) {
			if(data.valueInt == 0 && vtIsFundamental(vtGetType(data.typeData))) {
				return helperFromNullPointer<ResultType>();
			}
		}

		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<bool> >(data.valueInt); 
		case GVariantType::vtChar: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<char> >(data.valueInt); 
		case GVariantType::vtWchar: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<wchar_t> >(data.valueInt); 
		case GVariantType::vtSignedChar: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<signed char> >(data.valueInt); 
		case GVariantType::vtUnsignedChar: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<unsigned char> >(data.valueInt); 
		case GVariantType::vtSignedShort: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<signed short> >(data.valueInt); 
		case GVariantType::vtUnsignedShort: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<unsigned short> >(data.valueInt); 
		case GVariantType::vtSignedInt: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<signed int> >(data.valueInt); 
		case GVariantType::vtUnsignedInt: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<unsigned int> >(data.valueInt); 
		case GVariantType::vtSignedLong: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<signed long> >(data.valueInt); 
		case GVariantType::vtUnsignedLong: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<unsigned long> >(data.valueInt); 
		case GVariantType::vtSignedLongLong: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<signed long long> >(data.valueInt); 
		case GVariantType::vtUnsignedLongLong: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<unsigned long long> >(data.valueInt); 
		case GVariantType::vtFloat: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<float> >(data.valueFloat); 
		case GVariantType::vtDouble: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<double> >(data.valueDouble); 
		case GVariantType::vtLongDouble: return (ResultType)helperFromVariant<typename HelperValueType<ResultType>::type, cpgf::GTypeList<long double> >(data.valueLongDouble); 

		case GVariantType::vtObject:
			return helperFromObject<ResultType>(data.pointer);

		case GVariantType::vtShadow:
			if(data.valueInterface != nullptr) {
				return helperFromObject<ResultType>(((IVariantShadowObject *)data.valueInterface)->getObject());
			}
			else {
				return helperReturnEmptyValue<T>();
			}

		case GVariantType::vtInterface:
			return helperFromObject<ResultType>((cpgf::IObject *)data.valueInterface);

		case GVariantType::vtString:
			if(TypeListConvertible<StringStringTypeList, ResultType>::convertible) {
				return (ResultType)helperFromVariant<ResultType, StringStringTypeList>(
					(std::string &)*static_cast<std::string *>(((IVariantShadowObject *)data.valueInterface)->getObject())
					); 
			}
			else if(TypeListConvertible<StringCharTypeList, ResultType>::convertible) {
				return (ResultType)helperFromVariant<ResultType, StringCharTypeList>(
					static_cast<std::string *>(((IVariantShadowObject *)data.valueInterface)->getObject())->c_str()
				); 
			}
			break;

		case GVariantType::vtWideString:
			if(TypeListConvertible<WideStringStringTypeList, ResultType>::convertible) {
				return (ResultType)helperFromVariant<ResultType, WideStringStringTypeList>(
					(std::wstring &)*static_cast<std::wstring *>(((IVariantShadowObject *)data.valueInterface)->getObject())
					); 
			}
			else if(TypeListConvertible<WideStringCharTypeList, ResultType>::convertible) {
				return (ResultType)helperFromVariant<ResultType, WideStringCharTypeList>(
					static_cast<std::wstring *>(((IVariantShadowObject *)data.valueInterface)->getObject())->c_str()
					); 
			}
			break;
			
		case GVariantType::vtTypedVar:
			return fromVariantData<T, Policy>(getVariantRealValue(GVariant(data)).refData());

		default:
			break;
		}

		failedCast();
		return helperReturnEmptyValue<T>();
	}
	
	static bool canCast(const GVariantData & data)
	{
		// 0 can always be converted to pointer, similar as we do in C++.
		if(std::is_pointer<ResultType>::value) {
			if(data.valueInt == 0 && vtIsFundamental(vtGetType(data.typeData))) {
				return true;
			}
		}

		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return TypeListConvertible<cpgf::GTypeList<bool>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtChar: return TypeListConvertible<cpgf::GTypeList<char>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtWchar: return TypeListConvertible<cpgf::GTypeList<wchar_t>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtSignedChar: return TypeListConvertible<cpgf::GTypeList<signed char>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible<cpgf::GTypeList<unsigned char>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtSignedShort: return TypeListConvertible<cpgf::GTypeList<signed short>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible<cpgf::GTypeList<unsigned short>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtSignedInt: return TypeListConvertible<cpgf::GTypeList<signed int>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible<cpgf::GTypeList<unsigned int>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtSignedLong: return TypeListConvertible<cpgf::GTypeList<signed long>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible<cpgf::GTypeList<unsigned long>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible<cpgf::GTypeList<signed long long>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible<cpgf::GTypeList<unsigned long long>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtFloat: return TypeListConvertible<cpgf::GTypeList<float>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtDouble: return TypeListConvertible<cpgf::GTypeList<double>, typename HelperValueType<ResultType>::type>::convertible; 
		case GVariantType::vtLongDouble: return TypeListConvertible<cpgf::GTypeList<long double>, typename HelperValueType<ResultType>::type>::convertible; 

		case GVariantType::vtObject:
			return true;

		case GVariantType::vtShadow:
			return true;

		case GVariantType::vtInterface:
			// If ResultType can be casted to IObject * implicitly, then IObject * can be casted to ResultType explicitly.
			return std::is_convertible<ResultType, cpgf::IObject *>::value;

		case GVariantType::vtString:
			return TypeListConvertible<cpgf::TypeList_Concat<StringCharTypeList, StringStringTypeList>::Result, ResultType>::convertible;

		case GVariantType::vtWideString:
			return TypeListConvertible<cpgf::TypeList_Concat<WideStringCharTypeList, WideStringStringTypeList>::Result, ResultType>::convertible;

		case GVariantType::vtTypedVar:
			return canFromVariantData<T, Policy>(getVariantRealValue(GVariant(data)).refData());

		default:
			break;
		}
		
		return false;
	}

};

template <typename T, typename Policy>
struct CastVariant_Pointer
{
	typedef typename VariantCastResult<T, Policy>::Result ResultType;

	static ResultType cast(const GVariantData & data)
	{
		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const bool *, bool *, const volatile bool *, volatile bool *> >(data.pointer); 
		case GVariantType::vtChar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const char *, char *, const volatile char *, volatile char *> >(data.pointer); 
		case GVariantType::vtWchar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const wchar_t *, wchar_t *, const volatile wchar_t *, volatile wchar_t *> >(data.pointer); 
		case GVariantType::vtSignedChar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const signed char *, signed char *, const volatile signed char *, volatile signed char *> >(data.pointer); 
		case GVariantType::vtUnsignedChar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const unsigned char *, unsigned char *, const volatile unsigned char *, volatile unsigned char *> >(data.pointer); 
		case GVariantType::vtSignedShort: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const signed short *, signed short *, const volatile signed short *, volatile signed short *> >(data.pointer); 
		case GVariantType::vtUnsignedShort: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const unsigned short *, unsigned short *, const volatile unsigned short *, volatile unsigned short *> >(data.pointer); 
		case GVariantType::vtSignedInt: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const signed int *, signed int *, const volatile signed int *, volatile signed int *> >(data.pointer); 
		case GVariantType::vtUnsignedInt: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const unsigned int *, unsigned int *, const volatile unsigned int *, volatile unsigned int *> >(data.pointer); 
		case GVariantType::vtSignedLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const signed long *, signed long *, const volatile signed long *, volatile signed long *> >(data.pointer); 
		case GVariantType::vtUnsignedLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const unsigned long *, unsigned long *, const volatile unsigned long *, volatile unsigned long *> >(data.pointer); 
		case GVariantType::vtSignedLongLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const signed long long *, signed long long *, const volatile signed long long *, volatile signed long long *> >(data.pointer); 
		case GVariantType::vtUnsignedLongLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const unsigned long long *, unsigned long long *, const volatile unsigned long long *, volatile unsigned long long *> >(data.pointer); 
		case GVariantType::vtFloat: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const float *, float *, const volatile float *, volatile float *> >(data.pointer); 
		case GVariantType::vtDouble: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const double *, double *, const volatile double *, volatile double *> >(data.pointer); 
		case GVariantType::vtLongDouble: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList<const long double *, long double *, const volatile long double *, volatile long double *> >(data.pointer); 

		case GVariantType::vtObject: return helperFromObject<ResultType>(data.pointer);
		case GVariantType::vtVoid: return helperFromObject<ResultType>(data.pointer);

		case GVariantType::vtShadow:
			if(data.valueInterface != nullptr) {
				return helperFromObject<ResultType>(((IVariantShadowObject *)data.valueInterface)->getObject());
			}
			else {
				return helperReturnEmptyValue<T>();
			}

		case GVariantType::vtInterface:
			return helperFromObject<ResultType>((cpgf::IObject *)data.valueInterface);

		default:
			break;
		}
		
		failedCast();
		return helperReturnEmptyValue<T>();
	}
	
	static bool canCast(const GVariantData & data)
	{
		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return TypeListConvertible<cpgf::GTypeList<const bool *, bool *, const volatile bool *, volatile bool *>, ResultType>::convertible; 
		case GVariantType::vtChar: return TypeListConvertible<cpgf::GTypeList<const char *, char *, const volatile char *, volatile char *>, ResultType>::convertible; 
		case GVariantType::vtWchar: return TypeListConvertible<cpgf::GTypeList<const wchar_t *, wchar_t *, const volatile wchar_t *, volatile wchar_t *>, ResultType>::convertible; 
		case GVariantType::vtSignedChar: return TypeListConvertible<cpgf::GTypeList<const signed char *, signed char *, const volatile signed char *, volatile signed char *>, ResultType>::convertible; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible<cpgf::GTypeList<const unsigned char *, unsigned char *, const volatile unsigned char *, volatile unsigned char *>, ResultType>::convertible; 
		case GVariantType::vtSignedShort: return TypeListConvertible<cpgf::GTypeList<const signed short *, signed short *, const volatile signed short *, volatile signed short *>, ResultType>::convertible; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible<cpgf::GTypeList<const unsigned short *, unsigned short *, const volatile unsigned short *, volatile unsigned short *>, ResultType>::convertible; 
		case GVariantType::vtSignedInt: return TypeListConvertible<cpgf::GTypeList<const signed int *, signed int *, const volatile signed int *, volatile signed int *>, ResultType>::convertible; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible<cpgf::GTypeList<const unsigned int *, unsigned int *, const volatile unsigned int *, volatile unsigned int *>, ResultType>::convertible; 
		case GVariantType::vtSignedLong: return TypeListConvertible<cpgf::GTypeList<const signed long *, signed long *, const volatile signed long *, volatile signed long *>, ResultType>::convertible; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible<cpgf::GTypeList<const unsigned long *, unsigned long *, const volatile unsigned long *, volatile unsigned long *>, ResultType>::convertible; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible<cpgf::GTypeList<const signed long long *, signed long long *, const volatile signed long long *, volatile signed long long *>, ResultType>::convertible; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible<cpgf::GTypeList<const unsigned long long *, unsigned long long *, const volatile unsigned long long *, volatile unsigned long long *>, ResultType>::convertible; 
		case GVariantType::vtFloat: return TypeListConvertible<cpgf::GTypeList<const float *, float *, const volatile float *, volatile float *>, ResultType>::convertible; 
		case GVariantType::vtDouble: return TypeListConvertible<cpgf::GTypeList<const double *, double *, const volatile double *, volatile double *>, ResultType>::convertible; 
		case GVariantType::vtLongDouble: return TypeListConvertible<cpgf::GTypeList<const long double *, long double *, const volatile long double *, volatile long double *>, ResultType>::convertible; 

		case GVariantType::vtObject: return true;
		case GVariantType::vtVoid: return true;

		case GVariantType::vtShadow:
			return true;

		case GVariantType::vtInterface:
			// If ResultType can be casted to IObject * implicitly, then IObject * can be casted to ResultType explicitly.
			return std::is_convertible<ResultType, cpgf::IObject *>::value;

		default:
			break;
		}
		
		return false;
	}

};

template <typename T, typename Policy>
struct CastVariant_LvalueReference
{
	typedef typename VariantCastResult<T, Policy>::Result ResultType;

	static ResultType cast(const GVariantData & data)
	{
		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const bool *, bool *, const volatile bool *, volatile bool *>, cpgf::GTypeList<const bool &, bool &, const volatile bool &, volatile bool &> >((bool &) * (bool *)data.pointer); 
		case GVariantType::vtChar: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const char *, char *, const volatile char *, volatile char *>, cpgf::GTypeList<const char &, char &, const volatile char &, volatile char &> >((char &) * (char *)data.pointer); 
		case GVariantType::vtWchar: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const wchar_t *, wchar_t *, const volatile wchar_t *, volatile wchar_t *>, cpgf::GTypeList<const wchar_t &, wchar_t &, const volatile wchar_t &, volatile wchar_t &> >((wchar_t &) * (wchar_t *)data.pointer); 
		case GVariantType::vtSignedChar: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const signed char *, signed char *, const volatile signed char *, volatile signed char *>, cpgf::GTypeList<const signed char &, signed char &, const volatile signed char &, volatile signed char &> >((signed char &) * (signed char *)data.pointer); 
		case GVariantType::vtUnsignedChar: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const unsigned char *, unsigned char *, const volatile unsigned char *, volatile unsigned char *>, cpgf::GTypeList<const unsigned char &, unsigned char &, const volatile unsigned char &, volatile unsigned char &> >((unsigned char &) * (unsigned char *)data.pointer); 
		case GVariantType::vtSignedShort: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const signed short *, signed short *, const volatile signed short *, volatile signed short *>, cpgf::GTypeList<const signed short &, signed short &, const volatile signed short &, volatile signed short &> >((signed short &) * (signed short *)data.pointer); 
		case GVariantType::vtUnsignedShort: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const unsigned short *, unsigned short *, const volatile unsigned short *, volatile unsigned short *>, cpgf::GTypeList<const unsigned short &, unsigned short &, const volatile unsigned short &, volatile unsigned short &> >((unsigned short &) * (unsigned short *)data.pointer); 
		case GVariantType::vtSignedInt: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const signed int *, signed int *, const volatile signed int *, volatile signed int *>, cpgf::GTypeList<const signed int &, signed int &, const volatile signed int &, volatile signed int &> >((signed int &) * (signed int *)data.pointer); 
		case GVariantType::vtUnsignedInt: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const unsigned int *, unsigned int *, const volatile unsigned int *, volatile unsigned int *>, cpgf::GTypeList<const unsigned int &, unsigned int &, const volatile unsigned int &, volatile unsigned int &> >((unsigned int &) * (unsigned int *)data.pointer); 
		case GVariantType::vtSignedLong: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const signed long *, signed long *, const volatile signed long *, volatile signed long *>, cpgf::GTypeList<const signed long &, signed long &, const volatile signed long &, volatile signed long &> >((signed long &) * (signed long *)data.pointer); 
		case GVariantType::vtUnsignedLong: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const unsigned long *, unsigned long *, const volatile unsigned long *, volatile unsigned long *>, cpgf::GTypeList<const unsigned long &, unsigned long &, const volatile unsigned long &, volatile unsigned long &> >((unsigned long &) * (unsigned long *)data.pointer); 
		case GVariantType::vtSignedLongLong: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const signed long long *, signed long long *, const volatile signed long long *, volatile signed long long *>, cpgf::GTypeList<const signed long long &, signed long long &, const volatile signed long long &, volatile signed long long &> >((signed long long &) * (signed long long *)data.pointer); 
		case GVariantType::vtUnsignedLongLong: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const unsigned long long *, unsigned long long *, const volatile unsigned long long *, volatile unsigned long long *>, cpgf::GTypeList<const unsigned long long &, unsigned long long &, const volatile unsigned long long &, volatile unsigned long long &> >((unsigned long long &) * (unsigned long long *)data.pointer); 
		case GVariantType::vtFloat: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const float *, float *, const volatile float *, volatile float *>, cpgf::GTypeList<const float &, float &, const volatile float &, volatile float &> >((float &) * (float *)data.pointer); 
		case GVariantType::vtDouble: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const double *, double *, const volatile double *, volatile double *>, cpgf::GTypeList<const double &, double &, const volatile double &, volatile double &> >((double &) * (double *)data.pointer); 
		case GVariantType::vtLongDouble: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const long double *, long double *, const volatile long double *, volatile long double *>, cpgf::GTypeList<const long double &, long double &, const volatile long double &, volatile long double &> >((long double &) * (long double *)data.pointer); 

		case GVariantType::vtObject: return helperFromObject<ResultType>(data.pointer);
		case GVariantType::vtVoid: return helperFromObject<ResultType>(data.pointer);

		default:
			break;
		}
		
		failedCast();
		return helperReturnEmptyValue<ResultType>();
	}
	
	static bool canCast(const GVariantData & data)
	{
		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return TypeListConvertible<cpgf::GTypeList<const bool *, bool *, const volatile bool *, volatile bool *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const bool &, bool &, const volatile bool &, volatile bool &>, ResultType>::convertible ; 
		case GVariantType::vtChar: return TypeListConvertible<cpgf::GTypeList<const char *, char *, const volatile char *, volatile char *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const char &, char &, const volatile char &, volatile char &>, ResultType>::convertible ; 
		case GVariantType::vtWchar: return TypeListConvertible<cpgf::GTypeList<const wchar_t *, wchar_t *, const volatile wchar_t *, volatile wchar_t *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const wchar_t &, wchar_t &, const volatile wchar_t &, volatile wchar_t &>, ResultType>::convertible ; 
		case GVariantType::vtSignedChar: return TypeListConvertible<cpgf::GTypeList<const signed char *, signed char *, const volatile signed char *, volatile signed char *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const signed char &, signed char &, const volatile signed char &, volatile signed char &>, ResultType>::convertible ; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible<cpgf::GTypeList<const unsigned char *, unsigned char *, const volatile unsigned char *, volatile unsigned char *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const unsigned char &, unsigned char &, const volatile unsigned char &, volatile unsigned char &>, ResultType>::convertible ; 
		case GVariantType::vtSignedShort: return TypeListConvertible<cpgf::GTypeList<const signed short *, signed short *, const volatile signed short *, volatile signed short *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const signed short &, signed short &, const volatile signed short &, volatile signed short &>, ResultType>::convertible ; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible<cpgf::GTypeList<const unsigned short *, unsigned short *, const volatile unsigned short *, volatile unsigned short *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const unsigned short &, unsigned short &, const volatile unsigned short &, volatile unsigned short &>, ResultType>::convertible ; 
		case GVariantType::vtSignedInt: return TypeListConvertible<cpgf::GTypeList<const signed int *, signed int *, const volatile signed int *, volatile signed int *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const signed int &, signed int &, const volatile signed int &, volatile signed int &>, ResultType>::convertible ; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible<cpgf::GTypeList<const unsigned int *, unsigned int *, const volatile unsigned int *, volatile unsigned int *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const unsigned int &, unsigned int &, const volatile unsigned int &, volatile unsigned int &>, ResultType>::convertible ; 
		case GVariantType::vtSignedLong: return TypeListConvertible<cpgf::GTypeList<const signed long *, signed long *, const volatile signed long *, volatile signed long *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const signed long &, signed long &, const volatile signed long &, volatile signed long &>, ResultType>::convertible ; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible<cpgf::GTypeList<const unsigned long *, unsigned long *, const volatile unsigned long *, volatile unsigned long *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const unsigned long &, unsigned long &, const volatile unsigned long &, volatile unsigned long &>, ResultType>::convertible ; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible<cpgf::GTypeList<const signed long long *, signed long long *, const volatile signed long long *, volatile signed long long *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const signed long long &, signed long long &, const volatile signed long long &, volatile signed long long &>, ResultType>::convertible ; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible<cpgf::GTypeList<const unsigned long long *, unsigned long long *, const volatile unsigned long long *, volatile unsigned long long *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const unsigned long long &, unsigned long long &, const volatile unsigned long long &, volatile unsigned long long &>, ResultType>::convertible ; 
		case GVariantType::vtFloat: return TypeListConvertible<cpgf::GTypeList<const float *, float *, const volatile float *, volatile float *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const float &, float &, const volatile float &, volatile float &>, ResultType>::convertible ; 
		case GVariantType::vtDouble: return TypeListConvertible<cpgf::GTypeList<const double *, double *, const volatile double *, volatile double *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const double &, double &, const volatile double &, volatile double &>, ResultType>::convertible ; 
		case GVariantType::vtLongDouble: return TypeListConvertible<cpgf::GTypeList<const long double *, long double *, const volatile long double *, volatile long double *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const long double &, long double &, const volatile long double &, volatile long double &>, ResultType>::convertible ; 

		case GVariantType::vtObject: return true;
		case GVariantType::vtVoid: return true;

		default:
			break;
		}
		
		return false;
	}

};

template <typename T, typename Policy>
struct CastVariant_Pointer_LvalueReference
{
	typedef typename VariantCastResult<T, Policy>::Result ResultType;

	static ResultType cast(const GVariantData & data)
	{
		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const bool * &, bool * &, const volatile bool * &, volatile bool * &, const bool * const &, bool * const &, const volatile bool * const &, volatile bool * const &, const bool * volatile &, bool * volatile &, const volatile bool * volatile &, volatile bool * volatile &, const bool * const volatile &, bool * const volatile &, const volatile bool * const volatile &, volatile bool * const volatile & > >((bool * &) * (bool **)data.pointer); 
		case GVariantType::vtChar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const char * &, char * &, const volatile char * &, volatile char * &, const char * const &, char * const &, const volatile char * const &, volatile char * const &, const char * volatile &, char * volatile &, const volatile char * volatile &, volatile char * volatile &, const char * const volatile &, char * const volatile &, const volatile char * const volatile &, volatile char * const volatile & > >((char * &) * (char **)data.pointer); 
		case GVariantType::vtWchar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const wchar_t * &, wchar_t * &, const volatile wchar_t * &, volatile wchar_t * &, const wchar_t * const &, wchar_t * const &, const volatile wchar_t * const &, volatile wchar_t * const &, const wchar_t * volatile &, wchar_t * volatile &, const volatile wchar_t * volatile &, volatile wchar_t * volatile &, const wchar_t * const volatile &, wchar_t * const volatile &, const volatile wchar_t * const volatile &, volatile wchar_t * const volatile & > >((wchar_t * &) * (wchar_t **)data.pointer); 
		case GVariantType::vtSignedChar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const signed char * &, signed char * &, const volatile signed char * &, volatile signed char * &, const signed char * const &, signed char * const &, const volatile signed char * const &, volatile signed char * const &, const signed char * volatile &, signed char * volatile &, const volatile signed char * volatile &, volatile signed char * volatile &, const signed char * const volatile &, signed char * const volatile &, const volatile signed char * const volatile &, volatile signed char * const volatile & > >((signed char * &) * (signed char **)data.pointer); 
		case GVariantType::vtUnsignedChar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const unsigned char * &, unsigned char * &, const volatile unsigned char * &, volatile unsigned char * &, const unsigned char * const &, unsigned char * const &, const volatile unsigned char * const &, volatile unsigned char * const &, const unsigned char * volatile &, unsigned char * volatile &, const volatile unsigned char * volatile &, volatile unsigned char * volatile &, const unsigned char * const volatile &, unsigned char * const volatile &, const volatile unsigned char * const volatile &, volatile unsigned char * const volatile & > >((unsigned char * &) * (unsigned char **)data.pointer); 
		case GVariantType::vtSignedShort: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const signed short * &, signed short * &, const volatile signed short * &, volatile signed short * &, const signed short * const &, signed short * const &, const volatile signed short * const &, volatile signed short * const &, const signed short * volatile &, signed short * volatile &, const volatile signed short * volatile &, volatile signed short * volatile &, const signed short * const volatile &, signed short * const volatile &, const volatile signed short * const volatile &, volatile signed short * const volatile & > >((signed short * &) * (signed short **)data.pointer); 
		case GVariantType::vtUnsignedShort: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const unsigned short * &, unsigned short * &, const volatile unsigned short * &, volatile unsigned short * &, const unsigned short * const &, unsigned short * const &, const volatile unsigned short * const &, volatile unsigned short * const &, const unsigned short * volatile &, unsigned short * volatile &, const volatile unsigned short * volatile &, volatile unsigned short * volatile &, const unsigned short * const volatile &, unsigned short * const volatile &, const volatile unsigned short * const volatile &, volatile unsigned short * const volatile & > >((unsigned short * &) * (unsigned short **)data.pointer); 
		case GVariantType::vtSignedInt: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const signed int * &, signed int * &, const volatile signed int * &, volatile signed int * &, const signed int * const &, signed int * const &, const volatile signed int * const &, volatile signed int * const &, const signed int * volatile &, signed int * volatile &, const volatile signed int * volatile &, volatile signed int * volatile &, const signed int * const volatile &, signed int * const volatile &, const volatile signed int * const volatile &, volatile signed int * const volatile & > >((signed int * &) * (signed int **)data.pointer); 
		case GVariantType::vtUnsignedInt: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const unsigned int * &, unsigned int * &, const volatile unsigned int * &, volatile unsigned int * &, const unsigned int * const &, unsigned int * const &, const volatile unsigned int * const &, volatile unsigned int * const &, const unsigned int * volatile &, unsigned int * volatile &, const volatile unsigned int * volatile &, volatile unsigned int * volatile &, const unsigned int * const volatile &, unsigned int * const volatile &, const volatile unsigned int * const volatile &, volatile unsigned int * const volatile & > >((unsigned int * &) * (unsigned int **)data.pointer); 
		case GVariantType::vtSignedLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const signed long * &, signed long * &, const volatile signed long * &, volatile signed long * &, const signed long * const &, signed long * const &, const volatile signed long * const &, volatile signed long * const &, const signed long * volatile &, signed long * volatile &, const volatile signed long * volatile &, volatile signed long * volatile &, const signed long * const volatile &, signed long * const volatile &, const volatile signed long * const volatile &, volatile signed long * const volatile & > >((signed long * &) * (signed long **)data.pointer); 
		case GVariantType::vtUnsignedLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const unsigned long * &, unsigned long * &, const volatile unsigned long * &, volatile unsigned long * &, const unsigned long * const &, unsigned long * const &, const volatile unsigned long * const &, volatile unsigned long * const &, const unsigned long * volatile &, unsigned long * volatile &, const volatile unsigned long * volatile &, volatile unsigned long * volatile &, const unsigned long * const volatile &, unsigned long * const volatile &, const volatile unsigned long * const volatile &, volatile unsigned long * const volatile & > >((unsigned long * &) * (unsigned long **)data.pointer); 
		case GVariantType::vtSignedLongLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const signed long long * &, signed long long * &, const volatile signed long long * &, volatile signed long long * &, const signed long long * const &, signed long long * const &, const volatile signed long long * const &, volatile signed long long * const &, const signed long long * volatile &, signed long long * volatile &, const volatile signed long long * volatile &, volatile signed long long * volatile &, const signed long long * const volatile &, signed long long * const volatile &, const volatile signed long long * const volatile &, volatile signed long long * const volatile & > >((signed long long * &) * (signed long long **)data.pointer); 
		case GVariantType::vtUnsignedLongLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const unsigned long long * &, unsigned long long * &, const volatile unsigned long long * &, volatile unsigned long long * &, const unsigned long long * const &, unsigned long long * const &, const volatile unsigned long long * const &, volatile unsigned long long * const &, const unsigned long long * volatile &, unsigned long long * volatile &, const volatile unsigned long long * volatile &, volatile unsigned long long * volatile &, const unsigned long long * const volatile &, unsigned long long * const volatile &, const volatile unsigned long long * const volatile &, volatile unsigned long long * const volatile & > >((unsigned long long * &) * (unsigned long long **)data.pointer); 
		case GVariantType::vtFloat: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const float * &, float * &, const volatile float * &, volatile float * &, const float * const &, float * const &, const volatile float * const &, volatile float * const &, const float * volatile &, float * volatile &, const volatile float * volatile &, volatile float * volatile &, const float * const volatile &, float * const volatile &, const volatile float * const volatile &, volatile float * const volatile & > >((float * &) * (float **)data.pointer); 
		case GVariantType::vtDouble: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const double * &, double * &, const volatile double * &, volatile double * &, const double * const &, double * const &, const volatile double * const &, volatile double * const &, const double * volatile &, double * volatile &, const volatile double * volatile &, volatile double * volatile &, const double * const volatile &, double * const volatile &, const volatile double * const volatile &, volatile double * const volatile & > >((double * &) * (double **)data.pointer); 
		case GVariantType::vtLongDouble: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const long double * &, long double * &, const volatile long double * &, volatile long double * &, const long double * const &, long double * const &, const volatile long double * const &, volatile long double * const &, const long double * volatile &, long double * volatile &, const volatile long double * volatile &, volatile long double * volatile &, const long double * const volatile &, long double * const volatile &, const volatile long double * const volatile &, volatile long double * const volatile & > >((long double * &) * (long double **)data.pointer); 

		case GVariantType::vtObject: return helperFromObject<ResultType>(*(void **)data.pointer);
		case GVariantType::vtVoid: return helperFromObject<ResultType>(*(void **)data.pointer);

		case GVariantType::vtShadow:
			if(data.valueInterface != nullptr) {
				return helperFromObject<ResultType>((*(IVariantShadowObject **)data.valueInterface)->getObject());
			}
			else {
				return helperReturnEmptyValue<T>();
			}

		case GVariantType::vtInterface:
			return helperFromObject<ResultType>(*(cpgf::IObject **)data.valueInterface);

		default:
			break;
		}
		
		failedCast();
		return helperReturnEmptyValue<ResultType>();
	}
	
	static bool canCast(const GVariantData & data)
	{
		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return TypeListConvertible< cpgf::GTypeList< const bool * &, bool * &, const volatile bool * &, volatile bool * &, const bool * const &, bool * const &, const volatile bool * const &, volatile bool * const &, const bool * volatile &, bool * volatile &, const volatile bool * volatile &, volatile bool * volatile &, const bool * const volatile &, bool * const volatile &, const volatile bool * const volatile &, volatile bool * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtChar: return TypeListConvertible< cpgf::GTypeList< const char * &, char * &, const volatile char * &, volatile char * &, const char * const &, char * const &, const volatile char * const &, volatile char * const &, const char * volatile &, char * volatile &, const volatile char * volatile &, volatile char * volatile &, const char * const volatile &, char * const volatile &, const volatile char * const volatile &, volatile char * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtWchar: return TypeListConvertible< cpgf::GTypeList< const wchar_t * &, wchar_t * &, const volatile wchar_t * &, volatile wchar_t * &, const wchar_t * const &, wchar_t * const &, const volatile wchar_t * const &, volatile wchar_t * const &, const wchar_t * volatile &, wchar_t * volatile &, const volatile wchar_t * volatile &, volatile wchar_t * volatile &, const wchar_t * const volatile &, wchar_t * const volatile &, const volatile wchar_t * const volatile &, volatile wchar_t * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtSignedChar: return TypeListConvertible< cpgf::GTypeList< const signed char * &, signed char * &, const volatile signed char * &, volatile signed char * &, const signed char * const &, signed char * const &, const volatile signed char * const &, volatile signed char * const &, const signed char * volatile &, signed char * volatile &, const volatile signed char * volatile &, volatile signed char * volatile &, const signed char * const volatile &, signed char * const volatile &, const volatile signed char * const volatile &, volatile signed char * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible< cpgf::GTypeList< const unsigned char * &, unsigned char * &, const volatile unsigned char * &, volatile unsigned char * &, const unsigned char * const &, unsigned char * const &, const volatile unsigned char * const &, volatile unsigned char * const &, const unsigned char * volatile &, unsigned char * volatile &, const volatile unsigned char * volatile &, volatile unsigned char * volatile &, const unsigned char * const volatile &, unsigned char * const volatile &, const volatile unsigned char * const volatile &, volatile unsigned char * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtSignedShort: return TypeListConvertible< cpgf::GTypeList< const signed short * &, signed short * &, const volatile signed short * &, volatile signed short * &, const signed short * const &, signed short * const &, const volatile signed short * const &, volatile signed short * const &, const signed short * volatile &, signed short * volatile &, const volatile signed short * volatile &, volatile signed short * volatile &, const signed short * const volatile &, signed short * const volatile &, const volatile signed short * const volatile &, volatile signed short * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible< cpgf::GTypeList< const unsigned short * &, unsigned short * &, const volatile unsigned short * &, volatile unsigned short * &, const unsigned short * const &, unsigned short * const &, const volatile unsigned short * const &, volatile unsigned short * const &, const unsigned short * volatile &, unsigned short * volatile &, const volatile unsigned short * volatile &, volatile unsigned short * volatile &, const unsigned short * const volatile &, unsigned short * const volatile &, const volatile unsigned short * const volatile &, volatile unsigned short * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtSignedInt: return TypeListConvertible< cpgf::GTypeList< const signed int * &, signed int * &, const volatile signed int * &, volatile signed int * &, const signed int * const &, signed int * const &, const volatile signed int * const &, volatile signed int * const &, const signed int * volatile &, signed int * volatile &, const volatile signed int * volatile &, volatile signed int * volatile &, const signed int * const volatile &, signed int * const volatile &, const volatile signed int * const volatile &, volatile signed int * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible< cpgf::GTypeList< const unsigned int * &, unsigned int * &, const volatile unsigned int * &, volatile unsigned int * &, const unsigned int * const &, unsigned int * const &, const volatile unsigned int * const &, volatile unsigned int * const &, const unsigned int * volatile &, unsigned int * volatile &, const volatile unsigned int * volatile &, volatile unsigned int * volatile &, const unsigned int * const volatile &, unsigned int * const volatile &, const volatile unsigned int * const volatile &, volatile unsigned int * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtSignedLong: return TypeListConvertible< cpgf::GTypeList< const signed long * &, signed long * &, const volatile signed long * &, volatile signed long * &, const signed long * const &, signed long * const &, const volatile signed long * const &, volatile signed long * const &, const signed long * volatile &, signed long * volatile &, const volatile signed long * volatile &, volatile signed long * volatile &, const signed long * const volatile &, signed long * const volatile &, const volatile signed long * const volatile &, volatile signed long * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible< cpgf::GTypeList< const unsigned long * &, unsigned long * &, const volatile unsigned long * &, volatile unsigned long * &, const unsigned long * const &, unsigned long * const &, const volatile unsigned long * const &, volatile unsigned long * const &, const unsigned long * volatile &, unsigned long * volatile &, const volatile unsigned long * volatile &, volatile unsigned long * volatile &, const unsigned long * const volatile &, unsigned long * const volatile &, const volatile unsigned long * const volatile &, volatile unsigned long * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible< cpgf::GTypeList< const signed long long * &, signed long long * &, const volatile signed long long * &, volatile signed long long * &, const signed long long * const &, signed long long * const &, const volatile signed long long * const &, volatile signed long long * const &, const signed long long * volatile &, signed long long * volatile &, const volatile signed long long * volatile &, volatile signed long long * volatile &, const signed long long * const volatile &, signed long long * const volatile &, const volatile signed long long * const volatile &, volatile signed long long * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible< cpgf::GTypeList< const unsigned long long * &, unsigned long long * &, const volatile unsigned long long * &, volatile unsigned long long * &, const unsigned long long * const &, unsigned long long * const &, const volatile unsigned long long * const &, volatile unsigned long long * const &, const unsigned long long * volatile &, unsigned long long * volatile &, const volatile unsigned long long * volatile &, volatile unsigned long long * volatile &, const unsigned long long * const volatile &, unsigned long long * const volatile &, const volatile unsigned long long * const volatile &, volatile unsigned long long * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtFloat: return TypeListConvertible< cpgf::GTypeList< const float * &, float * &, const volatile float * &, volatile float * &, const float * const &, float * const &, const volatile float * const &, volatile float * const &, const float * volatile &, float * volatile &, const volatile float * volatile &, volatile float * volatile &, const float * const volatile &, float * const volatile &, const volatile float * const volatile &, volatile float * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtDouble: return TypeListConvertible< cpgf::GTypeList< const double * &, double * &, const volatile double * &, volatile double * &, const double * const &, double * const &, const volatile double * const &, volatile double * const &, const double * volatile &, double * volatile &, const volatile double * volatile &, volatile double * volatile &, const double * const volatile &, double * const volatile &, const volatile double * const volatile &, volatile double * const volatile & > , ResultType>::convertible; 
		case GVariantType::vtLongDouble: return TypeListConvertible< cpgf::GTypeList< const long double * &, long double * &, const volatile long double * &, volatile long double * &, const long double * const &, long double * const &, const volatile long double * const &, volatile long double * const &, const long double * volatile &, long double * volatile &, const volatile long double * volatile &, volatile long double * volatile &, const long double * const volatile &, long double * const volatile &, const volatile long double * const volatile &, volatile long double * const volatile & > , ResultType>::convertible; 

		case GVariantType::vtObject: return true;
		case GVariantType::vtVoid: return true;

		case GVariantType::vtShadow:
			return true;

		case GVariantType::vtInterface:
			// If ResultType can be casted to IObject * implicitly, then IObject * can be casted to ResultType explicitly.
			return std::is_convertible<ResultType, cpgf::IObject *>::value;

		default:
			break;
		}
		
		return false;
	}

};

template <typename T, typename Policy>
struct CastVariant_RvalueReference
{
	typedef typename VariantCastResult<T, Policy>::Result ResultType;

	static ResultType cast(const GVariantData & data)
	{
		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const bool *, bool *, const volatile bool *, volatile bool *>, cpgf::GTypeList<const bool &&, bool &&, const volatile bool &&, volatile bool &&> >((bool &&) * (bool *)data.pointer); 
		case GVariantType::vtChar: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const char *, char *, const volatile char *, volatile char *>, cpgf::GTypeList<const char &&, char &&, const volatile char &&, volatile char &&> >((char &&) * (char *)data.pointer); 
		case GVariantType::vtWchar: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const wchar_t *, wchar_t *, const volatile wchar_t *, volatile wchar_t *>, cpgf::GTypeList<const wchar_t &&, wchar_t &&, const volatile wchar_t &&, volatile wchar_t &&> >((wchar_t &&) * (wchar_t *)data.pointer); 
		case GVariantType::vtSignedChar: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const signed char *, signed char *, const volatile signed char *, volatile signed char *>, cpgf::GTypeList<const signed char &&, signed char &&, const volatile signed char &&, volatile signed char &&> >((signed char &&) * (signed char *)data.pointer); 
		case GVariantType::vtUnsignedChar: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const unsigned char *, unsigned char *, const volatile unsigned char *, volatile unsigned char *>, cpgf::GTypeList<const unsigned char &&, unsigned char &&, const volatile unsigned char &&, volatile unsigned char &&> >((unsigned char &&) * (unsigned char *)data.pointer); 
		case GVariantType::vtSignedShort: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const signed short *, signed short *, const volatile signed short *, volatile signed short *>, cpgf::GTypeList<const signed short &&, signed short &&, const volatile signed short &&, volatile signed short &&> >((signed short &&) * (signed short *)data.pointer); 
		case GVariantType::vtUnsignedShort: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const unsigned short *, unsigned short *, const volatile unsigned short *, volatile unsigned short *>, cpgf::GTypeList<const unsigned short &&, unsigned short &&, const volatile unsigned short &&, volatile unsigned short &&> >((unsigned short &&) * (unsigned short *)data.pointer); 
		case GVariantType::vtSignedInt: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const signed int *, signed int *, const volatile signed int *, volatile signed int *>, cpgf::GTypeList<const signed int &&, signed int &&, const volatile signed int &&, volatile signed int &&> >((signed int &&) * (signed int *)data.pointer); 
		case GVariantType::vtUnsignedInt: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const unsigned int *, unsigned int *, const volatile unsigned int *, volatile unsigned int *>, cpgf::GTypeList<const unsigned int &&, unsigned int &&, const volatile unsigned int &&, volatile unsigned int &&> >((unsigned int &&) * (unsigned int *)data.pointer); 
		case GVariantType::vtSignedLong: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const signed long *, signed long *, const volatile signed long *, volatile signed long *>, cpgf::GTypeList<const signed long &&, signed long &&, const volatile signed long &&, volatile signed long &&> >((signed long &&) * (signed long *)data.pointer); 
		case GVariantType::vtUnsignedLong: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const unsigned long *, unsigned long *, const volatile unsigned long *, volatile unsigned long *>, cpgf::GTypeList<const unsigned long &&, unsigned long &&, const volatile unsigned long &&, volatile unsigned long &&> >((unsigned long &&) * (unsigned long *)data.pointer); 
		case GVariantType::vtSignedLongLong: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const signed long long *, signed long long *, const volatile signed long long *, volatile signed long long *>, cpgf::GTypeList<const signed long long &&, signed long long &&, const volatile signed long long &&, volatile signed long long &&> >((signed long long &&) * (signed long long *)data.pointer); 
		case GVariantType::vtUnsignedLongLong: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const unsigned long long *, unsigned long long *, const volatile unsigned long long *, volatile unsigned long long *>, cpgf::GTypeList<const unsigned long long &&, unsigned long long &&, const volatile unsigned long long &&, volatile unsigned long long &&> >((unsigned long long &&) * (unsigned long long *)data.pointer); 
		case GVariantType::vtFloat: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const float *, float *, const volatile float *, volatile float *>, cpgf::GTypeList<const float &&, float &&, const volatile float &&, volatile float &&> >((float &&) * (float *)data.pointer); 
		case GVariantType::vtDouble: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const double *, double *, const volatile double *, volatile double *>, cpgf::GTypeList<const double &&, double &&, const volatile double &&, volatile double &&> >((double &&) * (double *)data.pointer); 
		case GVariantType::vtLongDouble: return (ResultType)helperFromPointerOrReference< ResultType, cpgf::GTypeList<const long double *, long double *, const volatile long double *, volatile long double *>, cpgf::GTypeList<const long double &&, long double &&, const volatile long double &&, volatile long double &&> >((long double &&) * (long double *)data.pointer); 

		case GVariantType::vtObject: return helperFromObject<ResultType>(data.pointer);
		case GVariantType::vtVoid: return helperFromObject<ResultType>(data.pointer);

		default:
			break;
		}
		
		failedCast();
		return helperReturnEmptyValue<ResultType>();
	}
	
	static bool canCast(const GVariantData & data)
	{
		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return TypeListConvertible<cpgf::GTypeList<const bool *, bool *, const volatile bool *, volatile bool *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const bool &&, bool &&, const volatile bool &&, volatile bool &&>, ResultType>::convertible ; 
		case GVariantType::vtChar: return TypeListConvertible<cpgf::GTypeList<const char *, char *, const volatile char *, volatile char *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const char &&, char &&, const volatile char &&, volatile char &&>, ResultType>::convertible ; 
		case GVariantType::vtWchar: return TypeListConvertible<cpgf::GTypeList<const wchar_t *, wchar_t *, const volatile wchar_t *, volatile wchar_t *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const wchar_t &&, wchar_t &&, const volatile wchar_t &&, volatile wchar_t &&>, ResultType>::convertible ; 
		case GVariantType::vtSignedChar: return TypeListConvertible<cpgf::GTypeList<const signed char *, signed char *, const volatile signed char *, volatile signed char *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const signed char &&, signed char &&, const volatile signed char &&, volatile signed char &&>, ResultType>::convertible ; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible<cpgf::GTypeList<const unsigned char *, unsigned char *, const volatile unsigned char *, volatile unsigned char *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const unsigned char &&, unsigned char &&, const volatile unsigned char &&, volatile unsigned char &&>, ResultType>::convertible ; 
		case GVariantType::vtSignedShort: return TypeListConvertible<cpgf::GTypeList<const signed short *, signed short *, const volatile signed short *, volatile signed short *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const signed short &&, signed short &&, const volatile signed short &&, volatile signed short &&>, ResultType>::convertible ; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible<cpgf::GTypeList<const unsigned short *, unsigned short *, const volatile unsigned short *, volatile unsigned short *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const unsigned short &&, unsigned short &&, const volatile unsigned short &&, volatile unsigned short &&>, ResultType>::convertible ; 
		case GVariantType::vtSignedInt: return TypeListConvertible<cpgf::GTypeList<const signed int *, signed int *, const volatile signed int *, volatile signed int *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const signed int &&, signed int &&, const volatile signed int &&, volatile signed int &&>, ResultType>::convertible ; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible<cpgf::GTypeList<const unsigned int *, unsigned int *, const volatile unsigned int *, volatile unsigned int *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const unsigned int &&, unsigned int &&, const volatile unsigned int &&, volatile unsigned int &&>, ResultType>::convertible ; 
		case GVariantType::vtSignedLong: return TypeListConvertible<cpgf::GTypeList<const signed long *, signed long *, const volatile signed long *, volatile signed long *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const signed long &&, signed long &&, const volatile signed long &&, volatile signed long &&>, ResultType>::convertible ; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible<cpgf::GTypeList<const unsigned long *, unsigned long *, const volatile unsigned long *, volatile unsigned long *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const unsigned long &&, unsigned long &&, const volatile unsigned long &&, volatile unsigned long &&>, ResultType>::convertible ; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible<cpgf::GTypeList<const signed long long *, signed long long *, const volatile signed long long *, volatile signed long long *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const signed long long &&, signed long long &&, const volatile signed long long &&, volatile signed long long &&>, ResultType>::convertible ; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible<cpgf::GTypeList<const unsigned long long *, unsigned long long *, const volatile unsigned long long *, volatile unsigned long long *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const unsigned long long &&, unsigned long long &&, const volatile unsigned long long &&, volatile unsigned long long &&>, ResultType>::convertible ; 
		case GVariantType::vtFloat: return TypeListConvertible<cpgf::GTypeList<const float *, float *, const volatile float *, volatile float *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const float &&, float &&, const volatile float &&, volatile float &&>, ResultType>::convertible ; 
		case GVariantType::vtDouble: return TypeListConvertible<cpgf::GTypeList<const double *, double *, const volatile double *, volatile double *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const double &&, double &&, const volatile double &&, volatile double &&>, ResultType>::convertible ; 
		case GVariantType::vtLongDouble: return TypeListConvertible<cpgf::GTypeList<const long double *, long double *, const volatile long double *, volatile long double *>, ResultType>::convertible || TypeListConvertible<cpgf::GTypeList<const long double &&, long double &&, const volatile long double &&, volatile long double &&>, ResultType>::convertible ; 

		case GVariantType::vtObject: return true;
		case GVariantType::vtVoid: return true;

		default:
			break;
		}
		
		return false;
	}

};

template <typename T, typename Policy>
struct CastVariant_Pointer_RvalueReference
{
	typedef typename VariantCastResult<T, Policy>::Result ResultType;

	static ResultType cast(const GVariantData & data)
	{
		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const bool * &&, bool * &&, const volatile bool * &&, volatile bool * &&, const bool * const &&, bool * const &&, const volatile bool * const &&, volatile bool * const &&, const bool * volatile &&, bool * volatile &&, const volatile bool * volatile &&, volatile bool * volatile &&, const bool * const volatile &&, bool * const volatile &&, const volatile bool * const volatile &&, volatile bool * const volatile && > >((bool * &&) * (bool **)data.pointer); 
		case GVariantType::vtChar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const char * &&, char * &&, const volatile char * &&, volatile char * &&, const char * const &&, char * const &&, const volatile char * const &&, volatile char * const &&, const char * volatile &&, char * volatile &&, const volatile char * volatile &&, volatile char * volatile &&, const char * const volatile &&, char * const volatile &&, const volatile char * const volatile &&, volatile char * const volatile && > >((char * &&) * (char **)data.pointer); 
		case GVariantType::vtWchar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const wchar_t * &&, wchar_t * &&, const volatile wchar_t * &&, volatile wchar_t * &&, const wchar_t * const &&, wchar_t * const &&, const volatile wchar_t * const &&, volatile wchar_t * const &&, const wchar_t * volatile &&, wchar_t * volatile &&, const volatile wchar_t * volatile &&, volatile wchar_t * volatile &&, const wchar_t * const volatile &&, wchar_t * const volatile &&, const volatile wchar_t * const volatile &&, volatile wchar_t * const volatile && > >((wchar_t * &&) * (wchar_t **)data.pointer); 
		case GVariantType::vtSignedChar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const signed char * &&, signed char * &&, const volatile signed char * &&, volatile signed char * &&, const signed char * const &&, signed char * const &&, const volatile signed char * const &&, volatile signed char * const &&, const signed char * volatile &&, signed char * volatile &&, const volatile signed char * volatile &&, volatile signed char * volatile &&, const signed char * const volatile &&, signed char * const volatile &&, const volatile signed char * const volatile &&, volatile signed char * const volatile && > >((signed char * &&) * (signed char **)data.pointer); 
		case GVariantType::vtUnsignedChar: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const unsigned char * &&, unsigned char * &&, const volatile unsigned char * &&, volatile unsigned char * &&, const unsigned char * const &&, unsigned char * const &&, const volatile unsigned char * const &&, volatile unsigned char * const &&, const unsigned char * volatile &&, unsigned char * volatile &&, const volatile unsigned char * volatile &&, volatile unsigned char * volatile &&, const unsigned char * const volatile &&, unsigned char * const volatile &&, const volatile unsigned char * const volatile &&, volatile unsigned char * const volatile && > >((unsigned char * &&) * (unsigned char **)data.pointer); 
		case GVariantType::vtSignedShort: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const signed short * &&, signed short * &&, const volatile signed short * &&, volatile signed short * &&, const signed short * const &&, signed short * const &&, const volatile signed short * const &&, volatile signed short * const &&, const signed short * volatile &&, signed short * volatile &&, const volatile signed short * volatile &&, volatile signed short * volatile &&, const signed short * const volatile &&, signed short * const volatile &&, const volatile signed short * const volatile &&, volatile signed short * const volatile && > >((signed short * &&) * (signed short **)data.pointer); 
		case GVariantType::vtUnsignedShort: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const unsigned short * &&, unsigned short * &&, const volatile unsigned short * &&, volatile unsigned short * &&, const unsigned short * const &&, unsigned short * const &&, const volatile unsigned short * const &&, volatile unsigned short * const &&, const unsigned short * volatile &&, unsigned short * volatile &&, const volatile unsigned short * volatile &&, volatile unsigned short * volatile &&, const unsigned short * const volatile &&, unsigned short * const volatile &&, const volatile unsigned short * const volatile &&, volatile unsigned short * const volatile && > >((unsigned short * &&) * (unsigned short **)data.pointer); 
		case GVariantType::vtSignedInt: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const signed int * &&, signed int * &&, const volatile signed int * &&, volatile signed int * &&, const signed int * const &&, signed int * const &&, const volatile signed int * const &&, volatile signed int * const &&, const signed int * volatile &&, signed int * volatile &&, const volatile signed int * volatile &&, volatile signed int * volatile &&, const signed int * const volatile &&, signed int * const volatile &&, const volatile signed int * const volatile &&, volatile signed int * const volatile && > >((signed int * &&) * (signed int **)data.pointer); 
		case GVariantType::vtUnsignedInt: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const unsigned int * &&, unsigned int * &&, const volatile unsigned int * &&, volatile unsigned int * &&, const unsigned int * const &&, unsigned int * const &&, const volatile unsigned int * const &&, volatile unsigned int * const &&, const unsigned int * volatile &&, unsigned int * volatile &&, const volatile unsigned int * volatile &&, volatile unsigned int * volatile &&, const unsigned int * const volatile &&, unsigned int * const volatile &&, const volatile unsigned int * const volatile &&, volatile unsigned int * const volatile && > >((unsigned int * &&) * (unsigned int **)data.pointer); 
		case GVariantType::vtSignedLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const signed long * &&, signed long * &&, const volatile signed long * &&, volatile signed long * &&, const signed long * const &&, signed long * const &&, const volatile signed long * const &&, volatile signed long * const &&, const signed long * volatile &&, signed long * volatile &&, const volatile signed long * volatile &&, volatile signed long * volatile &&, const signed long * const volatile &&, signed long * const volatile &&, const volatile signed long * const volatile &&, volatile signed long * const volatile && > >((signed long * &&) * (signed long **)data.pointer); 
		case GVariantType::vtUnsignedLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const unsigned long * &&, unsigned long * &&, const volatile unsigned long * &&, volatile unsigned long * &&, const unsigned long * const &&, unsigned long * const &&, const volatile unsigned long * const &&, volatile unsigned long * const &&, const unsigned long * volatile &&, unsigned long * volatile &&, const volatile unsigned long * volatile &&, volatile unsigned long * volatile &&, const unsigned long * const volatile &&, unsigned long * const volatile &&, const volatile unsigned long * const volatile &&, volatile unsigned long * const volatile && > >((unsigned long * &&) * (unsigned long **)data.pointer); 
		case GVariantType::vtSignedLongLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const signed long long * &&, signed long long * &&, const volatile signed long long * &&, volatile signed long long * &&, const signed long long * const &&, signed long long * const &&, const volatile signed long long * const &&, volatile signed long long * const &&, const signed long long * volatile &&, signed long long * volatile &&, const volatile signed long long * volatile &&, volatile signed long long * volatile &&, const signed long long * const volatile &&, signed long long * const volatile &&, const volatile signed long long * const volatile &&, volatile signed long long * const volatile && > >((signed long long * &&) * (signed long long **)data.pointer); 
		case GVariantType::vtUnsignedLongLong: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const unsigned long long * &&, unsigned long long * &&, const volatile unsigned long long * &&, volatile unsigned long long * &&, const unsigned long long * const &&, unsigned long long * const &&, const volatile unsigned long long * const &&, volatile unsigned long long * const &&, const unsigned long long * volatile &&, unsigned long long * volatile &&, const volatile unsigned long long * volatile &&, volatile unsigned long long * volatile &&, const unsigned long long * const volatile &&, unsigned long long * const volatile &&, const volatile unsigned long long * const volatile &&, volatile unsigned long long * const volatile && > >((unsigned long long * &&) * (unsigned long long **)data.pointer); 
		case GVariantType::vtFloat: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const float * &&, float * &&, const volatile float * &&, volatile float * &&, const float * const &&, float * const &&, const volatile float * const &&, volatile float * const &&, const float * volatile &&, float * volatile &&, const volatile float * volatile &&, volatile float * volatile &&, const float * const volatile &&, float * const volatile &&, const volatile float * const volatile &&, volatile float * const volatile && > >((float * &&) * (float **)data.pointer); 
		case GVariantType::vtDouble: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const double * &&, double * &&, const volatile double * &&, volatile double * &&, const double * const &&, double * const &&, const volatile double * const &&, volatile double * const &&, const double * volatile &&, double * volatile &&, const volatile double * volatile &&, volatile double * volatile &&, const double * const volatile &&, double * const volatile &&, const volatile double * const volatile &&, volatile double * const volatile && > >((double * &&) * (double **)data.pointer); 
		case GVariantType::vtLongDouble: return (ResultType)helperFromVariant< ResultType, cpgf::GTypeList< const long double * &&, long double * &&, const volatile long double * &&, volatile long double * &&, const long double * const &&, long double * const &&, const volatile long double * const &&, volatile long double * const &&, const long double * volatile &&, long double * volatile &&, const volatile long double * volatile &&, volatile long double * volatile &&, const long double * const volatile &&, long double * const volatile &&, const volatile long double * const volatile &&, volatile long double * const volatile && > >((long double * &&) * (long double **)data.pointer); 

		case GVariantType::vtObject: return helperFromObject<ResultType>(*(void **)data.pointer);
		case GVariantType::vtVoid: return helperFromObject<ResultType>(*(void **)data.pointer);

		case GVariantType::vtShadow:
			if(data.valueInterface != nullptr) {
				return helperFromObject<ResultType>((*(IVariantShadowObject **)data.valueInterface)->getObject());
			}
			else {
				return helperReturnEmptyValue<T>();
			}

		case GVariantType::vtInterface:
			return helperFromObject<ResultType>(*(cpgf::IObject **)data.valueInterface);

		default:
			break;
		}
		
		failedCast();
		return helperReturnEmptyValue<ResultType>();
	}
	
	static bool canCast(const GVariantData & data)
	{
		switch(vtGetBaseType(data.typeData)) {

		case GVariantType::vtBool: return TypeListConvertible< cpgf::GTypeList< const bool * &&, bool * &&, const volatile bool * &&, volatile bool * &&, const bool * const &&, bool * const &&, const volatile bool * const &&, volatile bool * const &&, const bool * volatile &&, bool * volatile &&, const volatile bool * volatile &&, volatile bool * volatile &&, const bool * const volatile &&, bool * const volatile &&, const volatile bool * const volatile &&, volatile bool * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtChar: return TypeListConvertible< cpgf::GTypeList< const char * &&, char * &&, const volatile char * &&, volatile char * &&, const char * const &&, char * const &&, const volatile char * const &&, volatile char * const &&, const char * volatile &&, char * volatile &&, const volatile char * volatile &&, volatile char * volatile &&, const char * const volatile &&, char * const volatile &&, const volatile char * const volatile &&, volatile char * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtWchar: return TypeListConvertible< cpgf::GTypeList< const wchar_t * &&, wchar_t * &&, const volatile wchar_t * &&, volatile wchar_t * &&, const wchar_t * const &&, wchar_t * const &&, const volatile wchar_t * const &&, volatile wchar_t * const &&, const wchar_t * volatile &&, wchar_t * volatile &&, const volatile wchar_t * volatile &&, volatile wchar_t * volatile &&, const wchar_t * const volatile &&, wchar_t * const volatile &&, const volatile wchar_t * const volatile &&, volatile wchar_t * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtSignedChar: return TypeListConvertible< cpgf::GTypeList< const signed char * &&, signed char * &&, const volatile signed char * &&, volatile signed char * &&, const signed char * const &&, signed char * const &&, const volatile signed char * const &&, volatile signed char * const &&, const signed char * volatile &&, signed char * volatile &&, const volatile signed char * volatile &&, volatile signed char * volatile &&, const signed char * const volatile &&, signed char * const volatile &&, const volatile signed char * const volatile &&, volatile signed char * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible< cpgf::GTypeList< const unsigned char * &&, unsigned char * &&, const volatile unsigned char * &&, volatile unsigned char * &&, const unsigned char * const &&, unsigned char * const &&, const volatile unsigned char * const &&, volatile unsigned char * const &&, const unsigned char * volatile &&, unsigned char * volatile &&, const volatile unsigned char * volatile &&, volatile unsigned char * volatile &&, const unsigned char * const volatile &&, unsigned char * const volatile &&, const volatile unsigned char * const volatile &&, volatile unsigned char * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtSignedShort: return TypeListConvertible< cpgf::GTypeList< const signed short * &&, signed short * &&, const volatile signed short * &&, volatile signed short * &&, const signed short * const &&, signed short * const &&, const volatile signed short * const &&, volatile signed short * const &&, const signed short * volatile &&, signed short * volatile &&, const volatile signed short * volatile &&, volatile signed short * volatile &&, const signed short * const volatile &&, signed short * const volatile &&, const volatile signed short * const volatile &&, volatile signed short * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible< cpgf::GTypeList< const unsigned short * &&, unsigned short * &&, const volatile unsigned short * &&, volatile unsigned short * &&, const unsigned short * const &&, unsigned short * const &&, const volatile unsigned short * const &&, volatile unsigned short * const &&, const unsigned short * volatile &&, unsigned short * volatile &&, const volatile unsigned short * volatile &&, volatile unsigned short * volatile &&, const unsigned short * const volatile &&, unsigned short * const volatile &&, const volatile unsigned short * const volatile &&, volatile unsigned short * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtSignedInt: return TypeListConvertible< cpgf::GTypeList< const signed int * &&, signed int * &&, const volatile signed int * &&, volatile signed int * &&, const signed int * const &&, signed int * const &&, const volatile signed int * const &&, volatile signed int * const &&, const signed int * volatile &&, signed int * volatile &&, const volatile signed int * volatile &&, volatile signed int * volatile &&, const signed int * const volatile &&, signed int * const volatile &&, const volatile signed int * const volatile &&, volatile signed int * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible< cpgf::GTypeList< const unsigned int * &&, unsigned int * &&, const volatile unsigned int * &&, volatile unsigned int * &&, const unsigned int * const &&, unsigned int * const &&, const volatile unsigned int * const &&, volatile unsigned int * const &&, const unsigned int * volatile &&, unsigned int * volatile &&, const volatile unsigned int * volatile &&, volatile unsigned int * volatile &&, const unsigned int * const volatile &&, unsigned int * const volatile &&, const volatile unsigned int * const volatile &&, volatile unsigned int * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtSignedLong: return TypeListConvertible< cpgf::GTypeList< const signed long * &&, signed long * &&, const volatile signed long * &&, volatile signed long * &&, const signed long * const &&, signed long * const &&, const volatile signed long * const &&, volatile signed long * const &&, const signed long * volatile &&, signed long * volatile &&, const volatile signed long * volatile &&, volatile signed long * volatile &&, const signed long * const volatile &&, signed long * const volatile &&, const volatile signed long * const volatile &&, volatile signed long * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible< cpgf::GTypeList< const unsigned long * &&, unsigned long * &&, const volatile unsigned long * &&, volatile unsigned long * &&, const unsigned long * const &&, unsigned long * const &&, const volatile unsigned long * const &&, volatile unsigned long * const &&, const unsigned long * volatile &&, unsigned long * volatile &&, const volatile unsigned long * volatile &&, volatile unsigned long * volatile &&, const unsigned long * const volatile &&, unsigned long * const volatile &&, const volatile unsigned long * const volatile &&, volatile unsigned long * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible< cpgf::GTypeList< const signed long long * &&, signed long long * &&, const volatile signed long long * &&, volatile signed long long * &&, const signed long long * const &&, signed long long * const &&, const volatile signed long long * const &&, volatile signed long long * const &&, const signed long long * volatile &&, signed long long * volatile &&, const volatile signed long long * volatile &&, volatile signed long long * volatile &&, const signed long long * const volatile &&, signed long long * const volatile &&, const volatile signed long long * const volatile &&, volatile signed long long * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible< cpgf::GTypeList< const unsigned long long * &&, unsigned long long * &&, const volatile unsigned long long * &&, volatile unsigned long long * &&, const unsigned long long * const &&, unsigned long long * const &&, const volatile unsigned long long * const &&, volatile unsigned long long * const &&, const unsigned long long * volatile &&, unsigned long long * volatile &&, const volatile unsigned long long * volatile &&, volatile unsigned long long * volatile &&, const unsigned long long * const volatile &&, unsigned long long * const volatile &&, const volatile unsigned long long * const volatile &&, volatile unsigned long long * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtFloat: return TypeListConvertible< cpgf::GTypeList< const float * &&, float * &&, const volatile float * &&, volatile float * &&, const float * const &&, float * const &&, const volatile float * const &&, volatile float * const &&, const float * volatile &&, float * volatile &&, const volatile float * volatile &&, volatile float * volatile &&, const float * const volatile &&, float * const volatile &&, const volatile float * const volatile &&, volatile float * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtDouble: return TypeListConvertible< cpgf::GTypeList< const double * &&, double * &&, const volatile double * &&, volatile double * &&, const double * const &&, double * const &&, const volatile double * const &&, volatile double * const &&, const double * volatile &&, double * volatile &&, const volatile double * volatile &&, volatile double * volatile &&, const double * const volatile &&, double * const volatile &&, const volatile double * const volatile &&, volatile double * const volatile && > , ResultType>::convertible; 
		case GVariantType::vtLongDouble: return TypeListConvertible< cpgf::GTypeList< const long double * &&, long double * &&, const volatile long double * &&, volatile long double * &&, const long double * const &&, long double * const &&, const volatile long double * const &&, volatile long double * const &&, const long double * volatile &&, long double * volatile &&, const volatile long double * volatile &&, volatile long double * volatile &&, const long double * const volatile &&, long double * const volatile &&, const volatile long double * const volatile &&, volatile long double * const volatile && > , ResultType>::convertible; 

		case GVariantType::vtObject: return true;
		case GVariantType::vtVoid: return true;

		case GVariantType::vtShadow:
			return true;

		case GVariantType::vtInterface:
			// If ResultType can be casted to IObject * implicitly, then IObject * can be casted to ResultType explicitly.
			return std::is_convertible<ResultType, cpgf::IObject *>::value;

		default:
			break;
		}
		
		return false;
	}

};


} //namespace variant_internal


#ifdef G_COMPILER_VC
#pragma warning(pop)
#endif

#ifdef G_COMPILER_GCC
#pragma GCC diagnostic pop
#endif

