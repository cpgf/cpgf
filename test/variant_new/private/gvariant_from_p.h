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
#endif


template <typename T>
T helperReturnEmptyValue(typename std::enable_if<std::is_lvalue_reference<T>::value || std::is_rvalue_reference<T>::value>::type * = 0)
{
	typedef typename std::remove_reference<T>::type NoRef;
	static NoRef x = NoRef();
	return (T)x;
}

template <typename T>
T helperReturnEmptyValue(typename std::enable_if<! std::is_lvalue_reference<T>::value && ! std::is_rvalue_reference<T>::value>::type * = 0)
{
	return T();
}

template <typename T, typename FromTypeList, typename V>
T helperFromVariant(const V & from, typename std::enable_if<TypeListConvertible<FromTypeList, T>::convertible>::type * = 0)
{
	return (T)(typename TypeListConvertible<FromTypeList, T>::type)(from);
}

template <typename T, typename FromTypeList, typename V>
T helperFromVariant(const V & /*from*/, typename std::enable_if<! TypeListConvertible<FromTypeList, T>::convertible>::type * = 0)
{
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
	return (T)(U)(object);
}

template <typename T>
T helperFromObject(void * object, typename std::enable_if<! std::is_pointer<typename FromObjectType<T>::type>::value>::type * = 0)
{
	typedef typename FromObjectType<T>::type U;
	return (T)*(U *)(object);
}

template <typename T>
struct CastVariant_Value
{
	static T cast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return (T)helperFromVariant<T, GTypeList<bool> >(!! data.valueInt); 
		case GVariantType::vtChar: return (T)helperFromVariant<T, GTypeList<char> >(data.valueInt); 
		case GVariantType::vtWchar: return (T)helperFromVariant<T, GTypeList<wchar_t> >(data.valueInt); 
		case GVariantType::vtSignedChar: return (T)helperFromVariant<T, GTypeList<signed char> >(data.valueInt); 
		case GVariantType::vtUnsignedChar: return (T)helperFromVariant<T, GTypeList<unsigned char> >(data.valueInt); 
		case GVariantType::vtSignedShort: return (T)helperFromVariant<T, GTypeList<signed short> >(data.valueInt); 
		case GVariantType::vtUnsignedShort: return (T)helperFromVariant<T, GTypeList<unsigned short> >(data.valueInt); 
		case GVariantType::vtSignedInt: return (T)helperFromVariant<T, GTypeList<signed int> >(data.valueInt); 
		case GVariantType::vtUnsignedInt: return (T)helperFromVariant<T, GTypeList<unsigned int> >(data.valueInt); 
		case GVariantType::vtSignedLong: return (T)helperFromVariant<T, GTypeList<signed long> >(data.valueInt); 
		case GVariantType::vtUnsignedLong: return (T)helperFromVariant<T, GTypeList<unsigned long> >(data.valueInt); 
		case GVariantType::vtSignedLongLong: return (T)helperFromVariant<T, GTypeList<signed long long> >(data.valueInt); 
		case GVariantType::vtUnsignedLongLong: return (T)helperFromVariant<T, GTypeList<unsigned long long> >(data.valueInt); 
		case GVariantType::vtFloat: return (T)helperFromVariant<T, GTypeList<float> >(data.valueFloat); 
		case GVariantType::vtDouble: return (T)helperFromVariant<T, GTypeList<double> >(data.valueDouble); 
		case GVariantType::vtLongDouble: return (T)helperFromVariant<T, GTypeList<long double> >(data.valueLongDouble); 

		case GVariantType::vtObject: return helperFromObject<T>(data.pointer);

		default:
			break;
		}
		
		return helperReturnEmptyValue<T>();
	}
	
	static bool canCast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return TypeListConvertible<GTypeList<bool>, T>::convertible; 
		case GVariantType::vtChar: return TypeListConvertible<GTypeList<char>, T>::convertible; 
		case GVariantType::vtWchar: return TypeListConvertible<GTypeList<wchar_t>, T>::convertible; 
		case GVariantType::vtSignedChar: return TypeListConvertible<GTypeList<signed char>, T>::convertible; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible<GTypeList<unsigned char>, T>::convertible; 
		case GVariantType::vtSignedShort: return TypeListConvertible<GTypeList<signed short>, T>::convertible; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible<GTypeList<unsigned short>, T>::convertible; 
		case GVariantType::vtSignedInt: return TypeListConvertible<GTypeList<signed int>, T>::convertible; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible<GTypeList<unsigned int>, T>::convertible; 
		case GVariantType::vtSignedLong: return TypeListConvertible<GTypeList<signed long>, T>::convertible; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible<GTypeList<unsigned long>, T>::convertible; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible<GTypeList<signed long long>, T>::convertible; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible<GTypeList<unsigned long long>, T>::convertible; 
		case GVariantType::vtFloat: return TypeListConvertible<GTypeList<float>, T>::convertible; 
		case GVariantType::vtDouble: return TypeListConvertible<GTypeList<double>, T>::convertible; 
		case GVariantType::vtLongDouble: return TypeListConvertible<GTypeList<long double>, T>::convertible; 

		case GVariantType::vtObject: return true;

		default:
			break;
		}
		
		return false;
	}

};

template <typename T>
struct CastVariant_Pointer
{
	static T cast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return (T)helperFromVariant< T, GTypeList<const bool *, bool *, const volatile bool *, volatile bool *> >(data.pointer); 
		case GVariantType::vtChar: return (T)helperFromVariant< T, GTypeList<const char *, char *, const volatile char *, volatile char *> >(data.pointer); 
		case GVariantType::vtWchar: return (T)helperFromVariant< T, GTypeList<const wchar_t *, wchar_t *, const volatile wchar_t *, volatile wchar_t *> >(data.pointer); 
		case GVariantType::vtSignedChar: return (T)helperFromVariant< T, GTypeList<const signed char *, signed char *, const volatile signed char *, volatile signed char *> >(data.pointer); 
		case GVariantType::vtUnsignedChar: return (T)helperFromVariant< T, GTypeList<const unsigned char *, unsigned char *, const volatile unsigned char *, volatile unsigned char *> >(data.pointer); 
		case GVariantType::vtSignedShort: return (T)helperFromVariant< T, GTypeList<const signed short *, signed short *, const volatile signed short *, volatile signed short *> >(data.pointer); 
		case GVariantType::vtUnsignedShort: return (T)helperFromVariant< T, GTypeList<const unsigned short *, unsigned short *, const volatile unsigned short *, volatile unsigned short *> >(data.pointer); 
		case GVariantType::vtSignedInt: return (T)helperFromVariant< T, GTypeList<const signed int *, signed int *, const volatile signed int *, volatile signed int *> >(data.pointer); 
		case GVariantType::vtUnsignedInt: return (T)helperFromVariant< T, GTypeList<const unsigned int *, unsigned int *, const volatile unsigned int *, volatile unsigned int *> >(data.pointer); 
		case GVariantType::vtSignedLong: return (T)helperFromVariant< T, GTypeList<const signed long *, signed long *, const volatile signed long *, volatile signed long *> >(data.pointer); 
		case GVariantType::vtUnsignedLong: return (T)helperFromVariant< T, GTypeList<const unsigned long *, unsigned long *, const volatile unsigned long *, volatile unsigned long *> >(data.pointer); 
		case GVariantType::vtSignedLongLong: return (T)helperFromVariant< T, GTypeList<const signed long long *, signed long long *, const volatile signed long long *, volatile signed long long *> >(data.pointer); 
		case GVariantType::vtUnsignedLongLong: return (T)helperFromVariant< T, GTypeList<const unsigned long long *, unsigned long long *, const volatile unsigned long long *, volatile unsigned long long *> >(data.pointer); 
		case GVariantType::vtFloat: return (T)helperFromVariant< T, GTypeList<const float *, float *, const volatile float *, volatile float *> >(data.pointer); 
		case GVariantType::vtDouble: return (T)helperFromVariant< T, GTypeList<const double *, double *, const volatile double *, volatile double *> >(data.pointer); 
		case GVariantType::vtLongDouble: return (T)helperFromVariant< T, GTypeList<const long double *, long double *, const volatile long double *, volatile long double *> >(data.pointer); 

		case GVariantType::vtObject: return helperFromObject<T>(data.pointer);

		default:
			break;
		}
		
		return helperReturnEmptyValue<T>();
	}
	
	static bool canCast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return TypeListConvertible<GTypeList<const bool *, bool *, const volatile bool *, volatile bool *>, T>::convertible; 
		case GVariantType::vtChar: return TypeListConvertible<GTypeList<const char *, char *, const volatile char *, volatile char *>, T>::convertible; 
		case GVariantType::vtWchar: return TypeListConvertible<GTypeList<const wchar_t *, wchar_t *, const volatile wchar_t *, volatile wchar_t *>, T>::convertible; 
		case GVariantType::vtSignedChar: return TypeListConvertible<GTypeList<const signed char *, signed char *, const volatile signed char *, volatile signed char *>, T>::convertible; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible<GTypeList<const unsigned char *, unsigned char *, const volatile unsigned char *, volatile unsigned char *>, T>::convertible; 
		case GVariantType::vtSignedShort: return TypeListConvertible<GTypeList<const signed short *, signed short *, const volatile signed short *, volatile signed short *>, T>::convertible; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible<GTypeList<const unsigned short *, unsigned short *, const volatile unsigned short *, volatile unsigned short *>, T>::convertible; 
		case GVariantType::vtSignedInt: return TypeListConvertible<GTypeList<const signed int *, signed int *, const volatile signed int *, volatile signed int *>, T>::convertible; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible<GTypeList<const unsigned int *, unsigned int *, const volatile unsigned int *, volatile unsigned int *>, T>::convertible; 
		case GVariantType::vtSignedLong: return TypeListConvertible<GTypeList<const signed long *, signed long *, const volatile signed long *, volatile signed long *>, T>::convertible; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible<GTypeList<const unsigned long *, unsigned long *, const volatile unsigned long *, volatile unsigned long *>, T>::convertible; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible<GTypeList<const signed long long *, signed long long *, const volatile signed long long *, volatile signed long long *>, T>::convertible; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible<GTypeList<const unsigned long long *, unsigned long long *, const volatile unsigned long long *, volatile unsigned long long *>, T>::convertible; 
		case GVariantType::vtFloat: return TypeListConvertible<GTypeList<const float *, float *, const volatile float *, volatile float *>, T>::convertible; 
		case GVariantType::vtDouble: return TypeListConvertible<GTypeList<const double *, double *, const volatile double *, volatile double *>, T>::convertible; 
		case GVariantType::vtLongDouble: return TypeListConvertible<GTypeList<const long double *, long double *, const volatile long double *, volatile long double *>, T>::convertible; 

		case GVariantType::vtObject: return true;

		default:
			break;
		}
		
		return false;
	}

};

template <typename T>
struct CastVariant_LvalueReference
{
	static T cast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return (T)helperFromVariant< T, GTypeList<const bool &, bool &, const volatile bool &, volatile bool &> >((bool &) * (bool *)data.pointer); 
		case GVariantType::vtChar: return (T)helperFromVariant< T, GTypeList<const char &, char &, const volatile char &, volatile char &> >((char &) * (char *)data.pointer); 
		case GVariantType::vtWchar: return (T)helperFromVariant< T, GTypeList<const wchar_t &, wchar_t &, const volatile wchar_t &, volatile wchar_t &> >((wchar_t &) * (wchar_t *)data.pointer); 
		case GVariantType::vtSignedChar: return (T)helperFromVariant< T, GTypeList<const signed char &, signed char &, const volatile signed char &, volatile signed char &> >((signed char &) * (signed char *)data.pointer); 
		case GVariantType::vtUnsignedChar: return (T)helperFromVariant< T, GTypeList<const unsigned char &, unsigned char &, const volatile unsigned char &, volatile unsigned char &> >((unsigned char &) * (unsigned char *)data.pointer); 
		case GVariantType::vtSignedShort: return (T)helperFromVariant< T, GTypeList<const signed short &, signed short &, const volatile signed short &, volatile signed short &> >((signed short &) * (signed short *)data.pointer); 
		case GVariantType::vtUnsignedShort: return (T)helperFromVariant< T, GTypeList<const unsigned short &, unsigned short &, const volatile unsigned short &, volatile unsigned short &> >((unsigned short &) * (unsigned short *)data.pointer); 
		case GVariantType::vtSignedInt: return (T)helperFromVariant< T, GTypeList<const signed int &, signed int &, const volatile signed int &, volatile signed int &> >((signed int &) * (signed int *)data.pointer); 
		case GVariantType::vtUnsignedInt: return (T)helperFromVariant< T, GTypeList<const unsigned int &, unsigned int &, const volatile unsigned int &, volatile unsigned int &> >((unsigned int &) * (unsigned int *)data.pointer); 
		case GVariantType::vtSignedLong: return (T)helperFromVariant< T, GTypeList<const signed long &, signed long &, const volatile signed long &, volatile signed long &> >((signed long &) * (signed long *)data.pointer); 
		case GVariantType::vtUnsignedLong: return (T)helperFromVariant< T, GTypeList<const unsigned long &, unsigned long &, const volatile unsigned long &, volatile unsigned long &> >((unsigned long &) * (unsigned long *)data.pointer); 
		case GVariantType::vtSignedLongLong: return (T)helperFromVariant< T, GTypeList<const signed long long &, signed long long &, const volatile signed long long &, volatile signed long long &> >((signed long long &) * (signed long long *)data.pointer); 
		case GVariantType::vtUnsignedLongLong: return (T)helperFromVariant< T, GTypeList<const unsigned long long &, unsigned long long &, const volatile unsigned long long &, volatile unsigned long long &> >((unsigned long long &) * (unsigned long long *)data.pointer); 
		case GVariantType::vtFloat: return (T)helperFromVariant< T, GTypeList<const float &, float &, const volatile float &, volatile float &> >((float &) * (float *)data.pointer); 
		case GVariantType::vtDouble: return (T)helperFromVariant< T, GTypeList<const double &, double &, const volatile double &, volatile double &> >((double &) * (double *)data.pointer); 
		case GVariantType::vtLongDouble: return (T)helperFromVariant< T, GTypeList<const long double &, long double &, const volatile long double &, volatile long double &> >((long double &) * (long double *)data.pointer); 

		case GVariantType::vtObject: return helperFromObject<T>(data.pointer);

		default:
			break;
		}
		
		return helperReturnEmptyValue<T>();
	}
	
	static bool canCast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return TypeListConvertible<GTypeList<const bool &, bool &, const volatile bool &, volatile bool &>, T>::convertible; 
		case GVariantType::vtChar: return TypeListConvertible<GTypeList<const char &, char &, const volatile char &, volatile char &>, T>::convertible; 
		case GVariantType::vtWchar: return TypeListConvertible<GTypeList<const wchar_t &, wchar_t &, const volatile wchar_t &, volatile wchar_t &>, T>::convertible; 
		case GVariantType::vtSignedChar: return TypeListConvertible<GTypeList<const signed char &, signed char &, const volatile signed char &, volatile signed char &>, T>::convertible; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible<GTypeList<const unsigned char &, unsigned char &, const volatile unsigned char &, volatile unsigned char &>, T>::convertible; 
		case GVariantType::vtSignedShort: return TypeListConvertible<GTypeList<const signed short &, signed short &, const volatile signed short &, volatile signed short &>, T>::convertible; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible<GTypeList<const unsigned short &, unsigned short &, const volatile unsigned short &, volatile unsigned short &>, T>::convertible; 
		case GVariantType::vtSignedInt: return TypeListConvertible<GTypeList<const signed int &, signed int &, const volatile signed int &, volatile signed int &>, T>::convertible; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible<GTypeList<const unsigned int &, unsigned int &, const volatile unsigned int &, volatile unsigned int &>, T>::convertible; 
		case GVariantType::vtSignedLong: return TypeListConvertible<GTypeList<const signed long &, signed long &, const volatile signed long &, volatile signed long &>, T>::convertible; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible<GTypeList<const unsigned long &, unsigned long &, const volatile unsigned long &, volatile unsigned long &>, T>::convertible; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible<GTypeList<const signed long long &, signed long long &, const volatile signed long long &, volatile signed long long &>, T>::convertible; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible<GTypeList<const unsigned long long &, unsigned long long &, const volatile unsigned long long &, volatile unsigned long long &>, T>::convertible; 
		case GVariantType::vtFloat: return TypeListConvertible<GTypeList<const float &, float &, const volatile float &, volatile float &>, T>::convertible; 
		case GVariantType::vtDouble: return TypeListConvertible<GTypeList<const double &, double &, const volatile double &, volatile double &>, T>::convertible; 
		case GVariantType::vtLongDouble: return TypeListConvertible<GTypeList<const long double &, long double &, const volatile long double &, volatile long double &>, T>::convertible; 

		case GVariantType::vtObject: return true;

		default:
			break;
		}
		
		return false;
	}

};

template <typename T>
struct CastVariant_Pointer_LvalueReference
{
	static T cast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return (T)helperFromVariant< T, GTypeList< const bool * &, bool * &, const volatile bool * &, volatile bool * &, const bool * const &, bool * const &, const volatile bool * const &, volatile bool * const &, const bool * volatile &, bool * volatile &, const volatile bool * volatile &, volatile bool * volatile &, const bool * const volatile &, bool * const volatile &, const volatile bool * const volatile &, volatile bool * const volatile & > >((bool * &) * (bool **)data.pointer); 
		case GVariantType::vtChar: return (T)helperFromVariant< T, GTypeList< const char * &, char * &, const volatile char * &, volatile char * &, const char * const &, char * const &, const volatile char * const &, volatile char * const &, const char * volatile &, char * volatile &, const volatile char * volatile &, volatile char * volatile &, const char * const volatile &, char * const volatile &, const volatile char * const volatile &, volatile char * const volatile & > >((char * &) * (char **)data.pointer); 
		case GVariantType::vtWchar: return (T)helperFromVariant< T, GTypeList< const wchar_t * &, wchar_t * &, const volatile wchar_t * &, volatile wchar_t * &, const wchar_t * const &, wchar_t * const &, const volatile wchar_t * const &, volatile wchar_t * const &, const wchar_t * volatile &, wchar_t * volatile &, const volatile wchar_t * volatile &, volatile wchar_t * volatile &, const wchar_t * const volatile &, wchar_t * const volatile &, const volatile wchar_t * const volatile &, volatile wchar_t * const volatile & > >((wchar_t * &) * (wchar_t **)data.pointer); 
		case GVariantType::vtSignedChar: return (T)helperFromVariant< T, GTypeList< const signed char * &, signed char * &, const volatile signed char * &, volatile signed char * &, const signed char * const &, signed char * const &, const volatile signed char * const &, volatile signed char * const &, const signed char * volatile &, signed char * volatile &, const volatile signed char * volatile &, volatile signed char * volatile &, const signed char * const volatile &, signed char * const volatile &, const volatile signed char * const volatile &, volatile signed char * const volatile & > >((signed char * &) * (signed char **)data.pointer); 
		case GVariantType::vtUnsignedChar: return (T)helperFromVariant< T, GTypeList< const unsigned char * &, unsigned char * &, const volatile unsigned char * &, volatile unsigned char * &, const unsigned char * const &, unsigned char * const &, const volatile unsigned char * const &, volatile unsigned char * const &, const unsigned char * volatile &, unsigned char * volatile &, const volatile unsigned char * volatile &, volatile unsigned char * volatile &, const unsigned char * const volatile &, unsigned char * const volatile &, const volatile unsigned char * const volatile &, volatile unsigned char * const volatile & > >((unsigned char * &) * (unsigned char **)data.pointer); 
		case GVariantType::vtSignedShort: return (T)helperFromVariant< T, GTypeList< const signed short * &, signed short * &, const volatile signed short * &, volatile signed short * &, const signed short * const &, signed short * const &, const volatile signed short * const &, volatile signed short * const &, const signed short * volatile &, signed short * volatile &, const volatile signed short * volatile &, volatile signed short * volatile &, const signed short * const volatile &, signed short * const volatile &, const volatile signed short * const volatile &, volatile signed short * const volatile & > >((signed short * &) * (signed short **)data.pointer); 
		case GVariantType::vtUnsignedShort: return (T)helperFromVariant< T, GTypeList< const unsigned short * &, unsigned short * &, const volatile unsigned short * &, volatile unsigned short * &, const unsigned short * const &, unsigned short * const &, const volatile unsigned short * const &, volatile unsigned short * const &, const unsigned short * volatile &, unsigned short * volatile &, const volatile unsigned short * volatile &, volatile unsigned short * volatile &, const unsigned short * const volatile &, unsigned short * const volatile &, const volatile unsigned short * const volatile &, volatile unsigned short * const volatile & > >((unsigned short * &) * (unsigned short **)data.pointer); 
		case GVariantType::vtSignedInt: return (T)helperFromVariant< T, GTypeList< const signed int * &, signed int * &, const volatile signed int * &, volatile signed int * &, const signed int * const &, signed int * const &, const volatile signed int * const &, volatile signed int * const &, const signed int * volatile &, signed int * volatile &, const volatile signed int * volatile &, volatile signed int * volatile &, const signed int * const volatile &, signed int * const volatile &, const volatile signed int * const volatile &, volatile signed int * const volatile & > >((signed int * &) * (signed int **)data.pointer); 
		case GVariantType::vtUnsignedInt: return (T)helperFromVariant< T, GTypeList< const unsigned int * &, unsigned int * &, const volatile unsigned int * &, volatile unsigned int * &, const unsigned int * const &, unsigned int * const &, const volatile unsigned int * const &, volatile unsigned int * const &, const unsigned int * volatile &, unsigned int * volatile &, const volatile unsigned int * volatile &, volatile unsigned int * volatile &, const unsigned int * const volatile &, unsigned int * const volatile &, const volatile unsigned int * const volatile &, volatile unsigned int * const volatile & > >((unsigned int * &) * (unsigned int **)data.pointer); 
		case GVariantType::vtSignedLong: return (T)helperFromVariant< T, GTypeList< const signed long * &, signed long * &, const volatile signed long * &, volatile signed long * &, const signed long * const &, signed long * const &, const volatile signed long * const &, volatile signed long * const &, const signed long * volatile &, signed long * volatile &, const volatile signed long * volatile &, volatile signed long * volatile &, const signed long * const volatile &, signed long * const volatile &, const volatile signed long * const volatile &, volatile signed long * const volatile & > >((signed long * &) * (signed long **)data.pointer); 
		case GVariantType::vtUnsignedLong: return (T)helperFromVariant< T, GTypeList< const unsigned long * &, unsigned long * &, const volatile unsigned long * &, volatile unsigned long * &, const unsigned long * const &, unsigned long * const &, const volatile unsigned long * const &, volatile unsigned long * const &, const unsigned long * volatile &, unsigned long * volatile &, const volatile unsigned long * volatile &, volatile unsigned long * volatile &, const unsigned long * const volatile &, unsigned long * const volatile &, const volatile unsigned long * const volatile &, volatile unsigned long * const volatile & > >((unsigned long * &) * (unsigned long **)data.pointer); 
		case GVariantType::vtSignedLongLong: return (T)helperFromVariant< T, GTypeList< const signed long long * &, signed long long * &, const volatile signed long long * &, volatile signed long long * &, const signed long long * const &, signed long long * const &, const volatile signed long long * const &, volatile signed long long * const &, const signed long long * volatile &, signed long long * volatile &, const volatile signed long long * volatile &, volatile signed long long * volatile &, const signed long long * const volatile &, signed long long * const volatile &, const volatile signed long long * const volatile &, volatile signed long long * const volatile & > >((signed long long * &) * (signed long long **)data.pointer); 
		case GVariantType::vtUnsignedLongLong: return (T)helperFromVariant< T, GTypeList< const unsigned long long * &, unsigned long long * &, const volatile unsigned long long * &, volatile unsigned long long * &, const unsigned long long * const &, unsigned long long * const &, const volatile unsigned long long * const &, volatile unsigned long long * const &, const unsigned long long * volatile &, unsigned long long * volatile &, const volatile unsigned long long * volatile &, volatile unsigned long long * volatile &, const unsigned long long * const volatile &, unsigned long long * const volatile &, const volatile unsigned long long * const volatile &, volatile unsigned long long * const volatile & > >((unsigned long long * &) * (unsigned long long **)data.pointer); 
		case GVariantType::vtFloat: return (T)helperFromVariant< T, GTypeList< const float * &, float * &, const volatile float * &, volatile float * &, const float * const &, float * const &, const volatile float * const &, volatile float * const &, const float * volatile &, float * volatile &, const volatile float * volatile &, volatile float * volatile &, const float * const volatile &, float * const volatile &, const volatile float * const volatile &, volatile float * const volatile & > >((float * &) * (float **)data.pointer); 
		case GVariantType::vtDouble: return (T)helperFromVariant< T, GTypeList< const double * &, double * &, const volatile double * &, volatile double * &, const double * const &, double * const &, const volatile double * const &, volatile double * const &, const double * volatile &, double * volatile &, const volatile double * volatile &, volatile double * volatile &, const double * const volatile &, double * const volatile &, const volatile double * const volatile &, volatile double * const volatile & > >((double * &) * (double **)data.pointer); 
		case GVariantType::vtLongDouble: return (T)helperFromVariant< T, GTypeList< const long double * &, long double * &, const volatile long double * &, volatile long double * &, const long double * const &, long double * const &, const volatile long double * const &, volatile long double * const &, const long double * volatile &, long double * volatile &, const volatile long double * volatile &, volatile long double * volatile &, const long double * const volatile &, long double * const volatile &, const volatile long double * const volatile &, volatile long double * const volatile & > >((long double * &) * (long double **)data.pointer); 

		case GVariantType::vtObject: return helperFromObject<T>(data.pointer);

		default:
			break;
		}
		
		return helperReturnEmptyValue<T>();
	}
	
	static bool canCast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return TypeListConvertible< GTypeList< const bool * &, bool * &, const volatile bool * &, volatile bool * &, const bool * const &, bool * const &, const volatile bool * const &, volatile bool * const &, const bool * volatile &, bool * volatile &, const volatile bool * volatile &, volatile bool * volatile &, const bool * const volatile &, bool * const volatile &, const volatile bool * const volatile &, volatile bool * const volatile & > , T>::convertible; 
		case GVariantType::vtChar: return TypeListConvertible< GTypeList< const char * &, char * &, const volatile char * &, volatile char * &, const char * const &, char * const &, const volatile char * const &, volatile char * const &, const char * volatile &, char * volatile &, const volatile char * volatile &, volatile char * volatile &, const char * const volatile &, char * const volatile &, const volatile char * const volatile &, volatile char * const volatile & > , T>::convertible; 
		case GVariantType::vtWchar: return TypeListConvertible< GTypeList< const wchar_t * &, wchar_t * &, const volatile wchar_t * &, volatile wchar_t * &, const wchar_t * const &, wchar_t * const &, const volatile wchar_t * const &, volatile wchar_t * const &, const wchar_t * volatile &, wchar_t * volatile &, const volatile wchar_t * volatile &, volatile wchar_t * volatile &, const wchar_t * const volatile &, wchar_t * const volatile &, const volatile wchar_t * const volatile &, volatile wchar_t * const volatile & > , T>::convertible; 
		case GVariantType::vtSignedChar: return TypeListConvertible< GTypeList< const signed char * &, signed char * &, const volatile signed char * &, volatile signed char * &, const signed char * const &, signed char * const &, const volatile signed char * const &, volatile signed char * const &, const signed char * volatile &, signed char * volatile &, const volatile signed char * volatile &, volatile signed char * volatile &, const signed char * const volatile &, signed char * const volatile &, const volatile signed char * const volatile &, volatile signed char * const volatile & > , T>::convertible; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible< GTypeList< const unsigned char * &, unsigned char * &, const volatile unsigned char * &, volatile unsigned char * &, const unsigned char * const &, unsigned char * const &, const volatile unsigned char * const &, volatile unsigned char * const &, const unsigned char * volatile &, unsigned char * volatile &, const volatile unsigned char * volatile &, volatile unsigned char * volatile &, const unsigned char * const volatile &, unsigned char * const volatile &, const volatile unsigned char * const volatile &, volatile unsigned char * const volatile & > , T>::convertible; 
		case GVariantType::vtSignedShort: return TypeListConvertible< GTypeList< const signed short * &, signed short * &, const volatile signed short * &, volatile signed short * &, const signed short * const &, signed short * const &, const volatile signed short * const &, volatile signed short * const &, const signed short * volatile &, signed short * volatile &, const volatile signed short * volatile &, volatile signed short * volatile &, const signed short * const volatile &, signed short * const volatile &, const volatile signed short * const volatile &, volatile signed short * const volatile & > , T>::convertible; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible< GTypeList< const unsigned short * &, unsigned short * &, const volatile unsigned short * &, volatile unsigned short * &, const unsigned short * const &, unsigned short * const &, const volatile unsigned short * const &, volatile unsigned short * const &, const unsigned short * volatile &, unsigned short * volatile &, const volatile unsigned short * volatile &, volatile unsigned short * volatile &, const unsigned short * const volatile &, unsigned short * const volatile &, const volatile unsigned short * const volatile &, volatile unsigned short * const volatile & > , T>::convertible; 
		case GVariantType::vtSignedInt: return TypeListConvertible< GTypeList< const signed int * &, signed int * &, const volatile signed int * &, volatile signed int * &, const signed int * const &, signed int * const &, const volatile signed int * const &, volatile signed int * const &, const signed int * volatile &, signed int * volatile &, const volatile signed int * volatile &, volatile signed int * volatile &, const signed int * const volatile &, signed int * const volatile &, const volatile signed int * const volatile &, volatile signed int * const volatile & > , T>::convertible; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible< GTypeList< const unsigned int * &, unsigned int * &, const volatile unsigned int * &, volatile unsigned int * &, const unsigned int * const &, unsigned int * const &, const volatile unsigned int * const &, volatile unsigned int * const &, const unsigned int * volatile &, unsigned int * volatile &, const volatile unsigned int * volatile &, volatile unsigned int * volatile &, const unsigned int * const volatile &, unsigned int * const volatile &, const volatile unsigned int * const volatile &, volatile unsigned int * const volatile & > , T>::convertible; 
		case GVariantType::vtSignedLong: return TypeListConvertible< GTypeList< const signed long * &, signed long * &, const volatile signed long * &, volatile signed long * &, const signed long * const &, signed long * const &, const volatile signed long * const &, volatile signed long * const &, const signed long * volatile &, signed long * volatile &, const volatile signed long * volatile &, volatile signed long * volatile &, const signed long * const volatile &, signed long * const volatile &, const volatile signed long * const volatile &, volatile signed long * const volatile & > , T>::convertible; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible< GTypeList< const unsigned long * &, unsigned long * &, const volatile unsigned long * &, volatile unsigned long * &, const unsigned long * const &, unsigned long * const &, const volatile unsigned long * const &, volatile unsigned long * const &, const unsigned long * volatile &, unsigned long * volatile &, const volatile unsigned long * volatile &, volatile unsigned long * volatile &, const unsigned long * const volatile &, unsigned long * const volatile &, const volatile unsigned long * const volatile &, volatile unsigned long * const volatile & > , T>::convertible; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible< GTypeList< const signed long long * &, signed long long * &, const volatile signed long long * &, volatile signed long long * &, const signed long long * const &, signed long long * const &, const volatile signed long long * const &, volatile signed long long * const &, const signed long long * volatile &, signed long long * volatile &, const volatile signed long long * volatile &, volatile signed long long * volatile &, const signed long long * const volatile &, signed long long * const volatile &, const volatile signed long long * const volatile &, volatile signed long long * const volatile & > , T>::convertible; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible< GTypeList< const unsigned long long * &, unsigned long long * &, const volatile unsigned long long * &, volatile unsigned long long * &, const unsigned long long * const &, unsigned long long * const &, const volatile unsigned long long * const &, volatile unsigned long long * const &, const unsigned long long * volatile &, unsigned long long * volatile &, const volatile unsigned long long * volatile &, volatile unsigned long long * volatile &, const unsigned long long * const volatile &, unsigned long long * const volatile &, const volatile unsigned long long * const volatile &, volatile unsigned long long * const volatile & > , T>::convertible; 
		case GVariantType::vtFloat: return TypeListConvertible< GTypeList< const float * &, float * &, const volatile float * &, volatile float * &, const float * const &, float * const &, const volatile float * const &, volatile float * const &, const float * volatile &, float * volatile &, const volatile float * volatile &, volatile float * volatile &, const float * const volatile &, float * const volatile &, const volatile float * const volatile &, volatile float * const volatile & > , T>::convertible; 
		case GVariantType::vtDouble: return TypeListConvertible< GTypeList< const double * &, double * &, const volatile double * &, volatile double * &, const double * const &, double * const &, const volatile double * const &, volatile double * const &, const double * volatile &, double * volatile &, const volatile double * volatile &, volatile double * volatile &, const double * const volatile &, double * const volatile &, const volatile double * const volatile &, volatile double * const volatile & > , T>::convertible; 
		case GVariantType::vtLongDouble: return TypeListConvertible< GTypeList< const long double * &, long double * &, const volatile long double * &, volatile long double * &, const long double * const &, long double * const &, const volatile long double * const &, volatile long double * const &, const long double * volatile &, long double * volatile &, const volatile long double * volatile &, volatile long double * volatile &, const long double * const volatile &, long double * const volatile &, const volatile long double * const volatile &, volatile long double * const volatile & > , T>::convertible; 

		case GVariantType::vtObject: return true;

		default:
			break;
		}
		
		return false;
	}

};

template <typename T>
struct CastVariant_RvalueReference
{
	static T cast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return (T)helperFromVariant< T, GTypeList<const bool &&, bool &&, const volatile bool &&, volatile bool &&> >((bool &&) * (bool *)data.pointer); 
		case GVariantType::vtChar: return (T)helperFromVariant< T, GTypeList<const char &&, char &&, const volatile char &&, volatile char &&> >((char &&) * (char *)data.pointer); 
		case GVariantType::vtWchar: return (T)helperFromVariant< T, GTypeList<const wchar_t &&, wchar_t &&, const volatile wchar_t &&, volatile wchar_t &&> >((wchar_t &&) * (wchar_t *)data.pointer); 
		case GVariantType::vtSignedChar: return (T)helperFromVariant< T, GTypeList<const signed char &&, signed char &&, const volatile signed char &&, volatile signed char &&> >((signed char &&) * (signed char *)data.pointer); 
		case GVariantType::vtUnsignedChar: return (T)helperFromVariant< T, GTypeList<const unsigned char &&, unsigned char &&, const volatile unsigned char &&, volatile unsigned char &&> >((unsigned char &&) * (unsigned char *)data.pointer); 
		case GVariantType::vtSignedShort: return (T)helperFromVariant< T, GTypeList<const signed short &&, signed short &&, const volatile signed short &&, volatile signed short &&> >((signed short &&) * (signed short *)data.pointer); 
		case GVariantType::vtUnsignedShort: return (T)helperFromVariant< T, GTypeList<const unsigned short &&, unsigned short &&, const volatile unsigned short &&, volatile unsigned short &&> >((unsigned short &&) * (unsigned short *)data.pointer); 
		case GVariantType::vtSignedInt: return (T)helperFromVariant< T, GTypeList<const signed int &&, signed int &&, const volatile signed int &&, volatile signed int &&> >((signed int &&) * (signed int *)data.pointer); 
		case GVariantType::vtUnsignedInt: return (T)helperFromVariant< T, GTypeList<const unsigned int &&, unsigned int &&, const volatile unsigned int &&, volatile unsigned int &&> >((unsigned int &&) * (unsigned int *)data.pointer); 
		case GVariantType::vtSignedLong: return (T)helperFromVariant< T, GTypeList<const signed long &&, signed long &&, const volatile signed long &&, volatile signed long &&> >((signed long &&) * (signed long *)data.pointer); 
		case GVariantType::vtUnsignedLong: return (T)helperFromVariant< T, GTypeList<const unsigned long &&, unsigned long &&, const volatile unsigned long &&, volatile unsigned long &&> >((unsigned long &&) * (unsigned long *)data.pointer); 
		case GVariantType::vtSignedLongLong: return (T)helperFromVariant< T, GTypeList<const signed long long &&, signed long long &&, const volatile signed long long &&, volatile signed long long &&> >((signed long long &&) * (signed long long *)data.pointer); 
		case GVariantType::vtUnsignedLongLong: return (T)helperFromVariant< T, GTypeList<const unsigned long long &&, unsigned long long &&, const volatile unsigned long long &&, volatile unsigned long long &&> >((unsigned long long &&) * (unsigned long long *)data.pointer); 
		case GVariantType::vtFloat: return (T)helperFromVariant< T, GTypeList<const float &&, float &&, const volatile float &&, volatile float &&> >((float &&) * (float *)data.pointer); 
		case GVariantType::vtDouble: return (T)helperFromVariant< T, GTypeList<const double &&, double &&, const volatile double &&, volatile double &&> >((double &&) * (double *)data.pointer); 
		case GVariantType::vtLongDouble: return (T)helperFromVariant< T, GTypeList<const long double &&, long double &&, const volatile long double &&, volatile long double &&> >((long double &&) * (long double *)data.pointer); 

		case GVariantType::vtObject: return helperFromObject<T>(data.pointer);

		default:
			break;
		}
		
		return helperReturnEmptyValue<T>();
	}
	
	static bool canCast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return TypeListConvertible<GTypeList<const bool &&, bool &&, const volatile bool &&, volatile bool &&>, T>::convertible; 
		case GVariantType::vtChar: return TypeListConvertible<GTypeList<const char &&, char &&, const volatile char &&, volatile char &&>, T>::convertible; 
		case GVariantType::vtWchar: return TypeListConvertible<GTypeList<const wchar_t &&, wchar_t &&, const volatile wchar_t &&, volatile wchar_t &&>, T>::convertible; 
		case GVariantType::vtSignedChar: return TypeListConvertible<GTypeList<const signed char &&, signed char &&, const volatile signed char &&, volatile signed char &&>, T>::convertible; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible<GTypeList<const unsigned char &&, unsigned char &&, const volatile unsigned char &&, volatile unsigned char &&>, T>::convertible; 
		case GVariantType::vtSignedShort: return TypeListConvertible<GTypeList<const signed short &&, signed short &&, const volatile signed short &&, volatile signed short &&>, T>::convertible; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible<GTypeList<const unsigned short &&, unsigned short &&, const volatile unsigned short &&, volatile unsigned short &&>, T>::convertible; 
		case GVariantType::vtSignedInt: return TypeListConvertible<GTypeList<const signed int &&, signed int &&, const volatile signed int &&, volatile signed int &&>, T>::convertible; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible<GTypeList<const unsigned int &&, unsigned int &&, const volatile unsigned int &&, volatile unsigned int &&>, T>::convertible; 
		case GVariantType::vtSignedLong: return TypeListConvertible<GTypeList<const signed long &&, signed long &&, const volatile signed long &&, volatile signed long &&>, T>::convertible; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible<GTypeList<const unsigned long &&, unsigned long &&, const volatile unsigned long &&, volatile unsigned long &&>, T>::convertible; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible<GTypeList<const signed long long &&, signed long long &&, const volatile signed long long &&, volatile signed long long &&>, T>::convertible; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible<GTypeList<const unsigned long long &&, unsigned long long &&, const volatile unsigned long long &&, volatile unsigned long long &&>, T>::convertible; 
		case GVariantType::vtFloat: return TypeListConvertible<GTypeList<const float &&, float &&, const volatile float &&, volatile float &&>, T>::convertible; 
		case GVariantType::vtDouble: return TypeListConvertible<GTypeList<const double &&, double &&, const volatile double &&, volatile double &&>, T>::convertible; 
		case GVariantType::vtLongDouble: return TypeListConvertible<GTypeList<const long double &&, long double &&, const volatile long double &&, volatile long double &&>, T>::convertible; 

		case GVariantType::vtObject: return true;

		default:
			break;
		}
		
		return false;
	}

};

template <typename T>
struct CastVariant_Pointer_RvalueReference
{
	static T cast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return (T)helperFromVariant< T, GTypeList< const bool * &&, bool * &&, const volatile bool * &&, volatile bool * &&, const bool * const &&, bool * const &&, const volatile bool * const &&, volatile bool * const &&, const bool * volatile &&, bool * volatile &&, const volatile bool * volatile &&, volatile bool * volatile &&, const bool * const volatile &&, bool * const volatile &&, const volatile bool * const volatile &&, volatile bool * const volatile && > >((bool * &&) * (bool **)data.pointer); 
		case GVariantType::vtChar: return (T)helperFromVariant< T, GTypeList< const char * &&, char * &&, const volatile char * &&, volatile char * &&, const char * const &&, char * const &&, const volatile char * const &&, volatile char * const &&, const char * volatile &&, char * volatile &&, const volatile char * volatile &&, volatile char * volatile &&, const char * const volatile &&, char * const volatile &&, const volatile char * const volatile &&, volatile char * const volatile && > >((char * &&) * (char **)data.pointer); 
		case GVariantType::vtWchar: return (T)helperFromVariant< T, GTypeList< const wchar_t * &&, wchar_t * &&, const volatile wchar_t * &&, volatile wchar_t * &&, const wchar_t * const &&, wchar_t * const &&, const volatile wchar_t * const &&, volatile wchar_t * const &&, const wchar_t * volatile &&, wchar_t * volatile &&, const volatile wchar_t * volatile &&, volatile wchar_t * volatile &&, const wchar_t * const volatile &&, wchar_t * const volatile &&, const volatile wchar_t * const volatile &&, volatile wchar_t * const volatile && > >((wchar_t * &&) * (wchar_t **)data.pointer); 
		case GVariantType::vtSignedChar: return (T)helperFromVariant< T, GTypeList< const signed char * &&, signed char * &&, const volatile signed char * &&, volatile signed char * &&, const signed char * const &&, signed char * const &&, const volatile signed char * const &&, volatile signed char * const &&, const signed char * volatile &&, signed char * volatile &&, const volatile signed char * volatile &&, volatile signed char * volatile &&, const signed char * const volatile &&, signed char * const volatile &&, const volatile signed char * const volatile &&, volatile signed char * const volatile && > >((signed char * &&) * (signed char **)data.pointer); 
		case GVariantType::vtUnsignedChar: return (T)helperFromVariant< T, GTypeList< const unsigned char * &&, unsigned char * &&, const volatile unsigned char * &&, volatile unsigned char * &&, const unsigned char * const &&, unsigned char * const &&, const volatile unsigned char * const &&, volatile unsigned char * const &&, const unsigned char * volatile &&, unsigned char * volatile &&, const volatile unsigned char * volatile &&, volatile unsigned char * volatile &&, const unsigned char * const volatile &&, unsigned char * const volatile &&, const volatile unsigned char * const volatile &&, volatile unsigned char * const volatile && > >((unsigned char * &&) * (unsigned char **)data.pointer); 
		case GVariantType::vtSignedShort: return (T)helperFromVariant< T, GTypeList< const signed short * &&, signed short * &&, const volatile signed short * &&, volatile signed short * &&, const signed short * const &&, signed short * const &&, const volatile signed short * const &&, volatile signed short * const &&, const signed short * volatile &&, signed short * volatile &&, const volatile signed short * volatile &&, volatile signed short * volatile &&, const signed short * const volatile &&, signed short * const volatile &&, const volatile signed short * const volatile &&, volatile signed short * const volatile && > >((signed short * &&) * (signed short **)data.pointer); 
		case GVariantType::vtUnsignedShort: return (T)helperFromVariant< T, GTypeList< const unsigned short * &&, unsigned short * &&, const volatile unsigned short * &&, volatile unsigned short * &&, const unsigned short * const &&, unsigned short * const &&, const volatile unsigned short * const &&, volatile unsigned short * const &&, const unsigned short * volatile &&, unsigned short * volatile &&, const volatile unsigned short * volatile &&, volatile unsigned short * volatile &&, const unsigned short * const volatile &&, unsigned short * const volatile &&, const volatile unsigned short * const volatile &&, volatile unsigned short * const volatile && > >((unsigned short * &&) * (unsigned short **)data.pointer); 
		case GVariantType::vtSignedInt: return (T)helperFromVariant< T, GTypeList< const signed int * &&, signed int * &&, const volatile signed int * &&, volatile signed int * &&, const signed int * const &&, signed int * const &&, const volatile signed int * const &&, volatile signed int * const &&, const signed int * volatile &&, signed int * volatile &&, const volatile signed int * volatile &&, volatile signed int * volatile &&, const signed int * const volatile &&, signed int * const volatile &&, const volatile signed int * const volatile &&, volatile signed int * const volatile && > >((signed int * &&) * (signed int **)data.pointer); 
		case GVariantType::vtUnsignedInt: return (T)helperFromVariant< T, GTypeList< const unsigned int * &&, unsigned int * &&, const volatile unsigned int * &&, volatile unsigned int * &&, const unsigned int * const &&, unsigned int * const &&, const volatile unsigned int * const &&, volatile unsigned int * const &&, const unsigned int * volatile &&, unsigned int * volatile &&, const volatile unsigned int * volatile &&, volatile unsigned int * volatile &&, const unsigned int * const volatile &&, unsigned int * const volatile &&, const volatile unsigned int * const volatile &&, volatile unsigned int * const volatile && > >((unsigned int * &&) * (unsigned int **)data.pointer); 
		case GVariantType::vtSignedLong: return (T)helperFromVariant< T, GTypeList< const signed long * &&, signed long * &&, const volatile signed long * &&, volatile signed long * &&, const signed long * const &&, signed long * const &&, const volatile signed long * const &&, volatile signed long * const &&, const signed long * volatile &&, signed long * volatile &&, const volatile signed long * volatile &&, volatile signed long * volatile &&, const signed long * const volatile &&, signed long * const volatile &&, const volatile signed long * const volatile &&, volatile signed long * const volatile && > >((signed long * &&) * (signed long **)data.pointer); 
		case GVariantType::vtUnsignedLong: return (T)helperFromVariant< T, GTypeList< const unsigned long * &&, unsigned long * &&, const volatile unsigned long * &&, volatile unsigned long * &&, const unsigned long * const &&, unsigned long * const &&, const volatile unsigned long * const &&, volatile unsigned long * const &&, const unsigned long * volatile &&, unsigned long * volatile &&, const volatile unsigned long * volatile &&, volatile unsigned long * volatile &&, const unsigned long * const volatile &&, unsigned long * const volatile &&, const volatile unsigned long * const volatile &&, volatile unsigned long * const volatile && > >((unsigned long * &&) * (unsigned long **)data.pointer); 
		case GVariantType::vtSignedLongLong: return (T)helperFromVariant< T, GTypeList< const signed long long * &&, signed long long * &&, const volatile signed long long * &&, volatile signed long long * &&, const signed long long * const &&, signed long long * const &&, const volatile signed long long * const &&, volatile signed long long * const &&, const signed long long * volatile &&, signed long long * volatile &&, const volatile signed long long * volatile &&, volatile signed long long * volatile &&, const signed long long * const volatile &&, signed long long * const volatile &&, const volatile signed long long * const volatile &&, volatile signed long long * const volatile && > >((signed long long * &&) * (signed long long **)data.pointer); 
		case GVariantType::vtUnsignedLongLong: return (T)helperFromVariant< T, GTypeList< const unsigned long long * &&, unsigned long long * &&, const volatile unsigned long long * &&, volatile unsigned long long * &&, const unsigned long long * const &&, unsigned long long * const &&, const volatile unsigned long long * const &&, volatile unsigned long long * const &&, const unsigned long long * volatile &&, unsigned long long * volatile &&, const volatile unsigned long long * volatile &&, volatile unsigned long long * volatile &&, const unsigned long long * const volatile &&, unsigned long long * const volatile &&, const volatile unsigned long long * const volatile &&, volatile unsigned long long * const volatile && > >((unsigned long long * &&) * (unsigned long long **)data.pointer); 
		case GVariantType::vtFloat: return (T)helperFromVariant< T, GTypeList< const float * &&, float * &&, const volatile float * &&, volatile float * &&, const float * const &&, float * const &&, const volatile float * const &&, volatile float * const &&, const float * volatile &&, float * volatile &&, const volatile float * volatile &&, volatile float * volatile &&, const float * const volatile &&, float * const volatile &&, const volatile float * const volatile &&, volatile float * const volatile && > >((float * &&) * (float **)data.pointer); 
		case GVariantType::vtDouble: return (T)helperFromVariant< T, GTypeList< const double * &&, double * &&, const volatile double * &&, volatile double * &&, const double * const &&, double * const &&, const volatile double * const &&, volatile double * const &&, const double * volatile &&, double * volatile &&, const volatile double * volatile &&, volatile double * volatile &&, const double * const volatile &&, double * const volatile &&, const volatile double * const volatile &&, volatile double * const volatile && > >((double * &&) * (double **)data.pointer); 
		case GVariantType::vtLongDouble: return (T)helperFromVariant< T, GTypeList< const long double * &&, long double * &&, const volatile long double * &&, volatile long double * &&, const long double * const &&, long double * const &&, const volatile long double * const &&, volatile long double * const &&, const long double * volatile &&, long double * volatile &&, const volatile long double * volatile &&, volatile long double * volatile &&, const long double * const volatile &&, long double * const volatile &&, const volatile long double * const volatile &&, volatile long double * const volatile && > >((long double * &&) * (long double **)data.pointer); 

		case GVariantType::vtObject: return helperFromObject<T>(data.pointer);

		default:
			break;
		}
		
		return helperReturnEmptyValue<T>();
	}
	
	static bool canCast(const GVariant & value)
	{
		const GVariantData & data = value.refData();
		switch((GVariantType)vtGetBaseType(data.typeData.vt)) {

		case GVariantType::vtBool: return TypeListConvertible< GTypeList< const bool * &&, bool * &&, const volatile bool * &&, volatile bool * &&, const bool * const &&, bool * const &&, const volatile bool * const &&, volatile bool * const &&, const bool * volatile &&, bool * volatile &&, const volatile bool * volatile &&, volatile bool * volatile &&, const bool * const volatile &&, bool * const volatile &&, const volatile bool * const volatile &&, volatile bool * const volatile && > , T>::convertible; 
		case GVariantType::vtChar: return TypeListConvertible< GTypeList< const char * &&, char * &&, const volatile char * &&, volatile char * &&, const char * const &&, char * const &&, const volatile char * const &&, volatile char * const &&, const char * volatile &&, char * volatile &&, const volatile char * volatile &&, volatile char * volatile &&, const char * const volatile &&, char * const volatile &&, const volatile char * const volatile &&, volatile char * const volatile && > , T>::convertible; 
		case GVariantType::vtWchar: return TypeListConvertible< GTypeList< const wchar_t * &&, wchar_t * &&, const volatile wchar_t * &&, volatile wchar_t * &&, const wchar_t * const &&, wchar_t * const &&, const volatile wchar_t * const &&, volatile wchar_t * const &&, const wchar_t * volatile &&, wchar_t * volatile &&, const volatile wchar_t * volatile &&, volatile wchar_t * volatile &&, const wchar_t * const volatile &&, wchar_t * const volatile &&, const volatile wchar_t * const volatile &&, volatile wchar_t * const volatile && > , T>::convertible; 
		case GVariantType::vtSignedChar: return TypeListConvertible< GTypeList< const signed char * &&, signed char * &&, const volatile signed char * &&, volatile signed char * &&, const signed char * const &&, signed char * const &&, const volatile signed char * const &&, volatile signed char * const &&, const signed char * volatile &&, signed char * volatile &&, const volatile signed char * volatile &&, volatile signed char * volatile &&, const signed char * const volatile &&, signed char * const volatile &&, const volatile signed char * const volatile &&, volatile signed char * const volatile && > , T>::convertible; 
		case GVariantType::vtUnsignedChar: return TypeListConvertible< GTypeList< const unsigned char * &&, unsigned char * &&, const volatile unsigned char * &&, volatile unsigned char * &&, const unsigned char * const &&, unsigned char * const &&, const volatile unsigned char * const &&, volatile unsigned char * const &&, const unsigned char * volatile &&, unsigned char * volatile &&, const volatile unsigned char * volatile &&, volatile unsigned char * volatile &&, const unsigned char * const volatile &&, unsigned char * const volatile &&, const volatile unsigned char * const volatile &&, volatile unsigned char * const volatile && > , T>::convertible; 
		case GVariantType::vtSignedShort: return TypeListConvertible< GTypeList< const signed short * &&, signed short * &&, const volatile signed short * &&, volatile signed short * &&, const signed short * const &&, signed short * const &&, const volatile signed short * const &&, volatile signed short * const &&, const signed short * volatile &&, signed short * volatile &&, const volatile signed short * volatile &&, volatile signed short * volatile &&, const signed short * const volatile &&, signed short * const volatile &&, const volatile signed short * const volatile &&, volatile signed short * const volatile && > , T>::convertible; 
		case GVariantType::vtUnsignedShort: return TypeListConvertible< GTypeList< const unsigned short * &&, unsigned short * &&, const volatile unsigned short * &&, volatile unsigned short * &&, const unsigned short * const &&, unsigned short * const &&, const volatile unsigned short * const &&, volatile unsigned short * const &&, const unsigned short * volatile &&, unsigned short * volatile &&, const volatile unsigned short * volatile &&, volatile unsigned short * volatile &&, const unsigned short * const volatile &&, unsigned short * const volatile &&, const volatile unsigned short * const volatile &&, volatile unsigned short * const volatile && > , T>::convertible; 
		case GVariantType::vtSignedInt: return TypeListConvertible< GTypeList< const signed int * &&, signed int * &&, const volatile signed int * &&, volatile signed int * &&, const signed int * const &&, signed int * const &&, const volatile signed int * const &&, volatile signed int * const &&, const signed int * volatile &&, signed int * volatile &&, const volatile signed int * volatile &&, volatile signed int * volatile &&, const signed int * const volatile &&, signed int * const volatile &&, const volatile signed int * const volatile &&, volatile signed int * const volatile && > , T>::convertible; 
		case GVariantType::vtUnsignedInt: return TypeListConvertible< GTypeList< const unsigned int * &&, unsigned int * &&, const volatile unsigned int * &&, volatile unsigned int * &&, const unsigned int * const &&, unsigned int * const &&, const volatile unsigned int * const &&, volatile unsigned int * const &&, const unsigned int * volatile &&, unsigned int * volatile &&, const volatile unsigned int * volatile &&, volatile unsigned int * volatile &&, const unsigned int * const volatile &&, unsigned int * const volatile &&, const volatile unsigned int * const volatile &&, volatile unsigned int * const volatile && > , T>::convertible; 
		case GVariantType::vtSignedLong: return TypeListConvertible< GTypeList< const signed long * &&, signed long * &&, const volatile signed long * &&, volatile signed long * &&, const signed long * const &&, signed long * const &&, const volatile signed long * const &&, volatile signed long * const &&, const signed long * volatile &&, signed long * volatile &&, const volatile signed long * volatile &&, volatile signed long * volatile &&, const signed long * const volatile &&, signed long * const volatile &&, const volatile signed long * const volatile &&, volatile signed long * const volatile && > , T>::convertible; 
		case GVariantType::vtUnsignedLong: return TypeListConvertible< GTypeList< const unsigned long * &&, unsigned long * &&, const volatile unsigned long * &&, volatile unsigned long * &&, const unsigned long * const &&, unsigned long * const &&, const volatile unsigned long * const &&, volatile unsigned long * const &&, const unsigned long * volatile &&, unsigned long * volatile &&, const volatile unsigned long * volatile &&, volatile unsigned long * volatile &&, const unsigned long * const volatile &&, unsigned long * const volatile &&, const volatile unsigned long * const volatile &&, volatile unsigned long * const volatile && > , T>::convertible; 
		case GVariantType::vtSignedLongLong: return TypeListConvertible< GTypeList< const signed long long * &&, signed long long * &&, const volatile signed long long * &&, volatile signed long long * &&, const signed long long * const &&, signed long long * const &&, const volatile signed long long * const &&, volatile signed long long * const &&, const signed long long * volatile &&, signed long long * volatile &&, const volatile signed long long * volatile &&, volatile signed long long * volatile &&, const signed long long * const volatile &&, signed long long * const volatile &&, const volatile signed long long * const volatile &&, volatile signed long long * const volatile && > , T>::convertible; 
		case GVariantType::vtUnsignedLongLong: return TypeListConvertible< GTypeList< const unsigned long long * &&, unsigned long long * &&, const volatile unsigned long long * &&, volatile unsigned long long * &&, const unsigned long long * const &&, unsigned long long * const &&, const volatile unsigned long long * const &&, volatile unsigned long long * const &&, const unsigned long long * volatile &&, unsigned long long * volatile &&, const volatile unsigned long long * volatile &&, volatile unsigned long long * volatile &&, const unsigned long long * const volatile &&, unsigned long long * const volatile &&, const volatile unsigned long long * const volatile &&, volatile unsigned long long * const volatile && > , T>::convertible; 
		case GVariantType::vtFloat: return TypeListConvertible< GTypeList< const float * &&, float * &&, const volatile float * &&, volatile float * &&, const float * const &&, float * const &&, const volatile float * const &&, volatile float * const &&, const float * volatile &&, float * volatile &&, const volatile float * volatile &&, volatile float * volatile &&, const float * const volatile &&, float * const volatile &&, const volatile float * const volatile &&, volatile float * const volatile && > , T>::convertible; 
		case GVariantType::vtDouble: return TypeListConvertible< GTypeList< const double * &&, double * &&, const volatile double * &&, volatile double * &&, const double * const &&, double * const &&, const volatile double * const &&, volatile double * const &&, const double * volatile &&, double * volatile &&, const volatile double * volatile &&, volatile double * volatile &&, const double * const volatile &&, double * const volatile &&, const volatile double * const volatile &&, volatile double * const volatile && > , T>::convertible; 
		case GVariantType::vtLongDouble: return TypeListConvertible< GTypeList< const long double * &&, long double * &&, const volatile long double * &&, volatile long double * &&, const long double * const &&, long double * const &&, const volatile long double * const &&, volatile long double * const &&, const long double * volatile &&, long double * volatile &&, const volatile long double * volatile &&, volatile long double * volatile &&, const long double * const volatile &&, long double * const volatile &&, const volatile long double * const volatile &&, volatile long double * const volatile && > , T>::convertible; 

		case GVariantType::vtObject: return true;

		default:
			break;
		}
		
		return false;
	}

};

#ifdef G_COMPILER_VC
#pragma warning(pop)
#endif

#ifdef G_COMPILER_GCC
#pragma GCC diagnostic pop
#endif
