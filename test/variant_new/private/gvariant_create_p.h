namespace variant_internal {

template <typename T>
struct GVariantDeducer_Type
{
	static constexpr int fundamentalIndex = cpgf::TypeList_IndexOf<FundamentalTypeList, T>::Result;
	static constexpr int Type =
		(fundamentalIndex >= 0 ? fundamentalIndex + FundamentalTypeStartType
			: (std::is_void<T>::value ? (int)GVariantType::vtVoid : (int)GVariantType::vtObject)
		)
	;

	template <typename V>
	static void assign(GVariantData * data, const V & value,
		typename std::enable_if<! std::is_arithmetic<V>::value>::type * = 0)
	{
		data->pointer = (void *)(std::addressof(value));
	}

	template <typename V>
	static void assign(GVariantData * data, const V & value,
		typename std::enable_if<std::is_integral<V>::value>::type * = 0)
	{
		data->valueInt = (GVariantInteger)value;
	}

	template <typename V>
	static void assign(GVariantData * data, const V & value,
		typename std::enable_if<std::is_floating_point<V>::value>::type * = 0)
	{
		data->valueReal = (GVariantReal)value;
	}
};

template <typename T, int typeMask, bool isPointerOrReference>
struct GVariantDeducer_Value_Helper;

template <typename T, int typeMask>
struct GVariantDeducer_Value_Helper <T, typeMask, false>
{
	typedef GVariantDeducer_Type<T> TypeDeducer;

	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		data->typeData.vt = TypeDeducer::Type;
		TypeDeducer::assign(data, value);
	}
};

template <typename T, int typeMask>
struct GVariantDeducer_Value_Helper <T, typeMask, true>
{
	typedef GVariantDeducer_Type<T> TypeDeducer;

	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		data->typeData.vt = TypeDeducer::Type | typeMask;
		data->pointer = (void *)(value);
	}
};

template <typename T, bool isPointer, bool isLvalueReference, bool isRvalueReference>
struct GVariantDeducer_Value
{
	static constexpr int typeMask = (
		(isPointer ? (int)GVariantType::byPointer : 0)
		| (isLvalueReference ? (int)GVariantType::byLvalueReference : 0)
		| (isRvalueReference ? (int)GVariantType::byRvalueReference : 0)
	);
	static constexpr bool isPointerOrReference = (isPointer || isLvalueReference || isRvalueReference);

	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value_Helper<T, typeMask, isPointerOrReference>::deduce(data, value);
	}
};

template <typename T>
struct GVariantDeducer_Pointer
{
	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<T, true, false, false>::deduce(data, value);
	}
};

template <typename T, bool isPointer>
struct GVariantDeducer_LvalueReference;

template <typename T>
struct GVariantDeducer_LvalueReference <T, false>
{
	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<
			T,
			false,
			true,
			false
		>::deduce(data, std::addressof(value));
	}
};

template <typename T>
struct GVariantDeducer_LvalueReference <T, true>
{
	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<
			typename std::remove_cv<typename std::remove_pointer<T>::type >::type,
			true,
			true,
			false
		>::deduce(data, std::addressof(value));
	}
};

template <typename T, bool isPointer>
struct GVariantDeducer_RvalueReference;

template <typename T>
struct GVariantDeducer_RvalueReference <T, false>
{
	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<
			T,
			false,
			false,
			true
		>::deduce(data, std::addressof(value));
	}
};

template <typename T>
struct GVariantDeducer_RvalueReference <T, true>
{
	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<
			typename std::remove_cv<typename std::remove_pointer<T>::type >::type,
			true,
			false,
			true
		>::deduce(data, std::addressof(value));
	}
};

template <typename T, bool isPointer, bool isLvalueReference, bool isRvalueReference>
struct GVariantDeducer;

template <typename T>
struct GVariantDeducer <T, false, false, false>
{
	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<T, false, false, false>::deduce(data, value);
	}
};

template <typename T>
struct GVariantDeducer <T, true, false, false>
{
	typedef typename std::remove_cv<typename std::remove_pointer<T>::type >::type U;

	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		GVariantDeducer_Pointer<U>::deduce(data, value);
	}
};

template <typename T>
struct GVariantDeducer <T, false, true, false>
{
	typedef typename std::remove_cv<typename std::remove_reference<T>::type >::type U;

	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		GVariantDeducer_LvalueReference<U, std::is_pointer<U>::value>::deduce(data, value);
	}
};

template <typename T>
struct GVariantDeducer <T, false, false, true>
{
	typedef typename std::remove_cv<typename std::remove_reference<T>::type >::type U;

	template <typename V>
	static void deduce(GVariantData * data, const V & value)
	{
		GVariantDeducer_RvalueReference<U, std::is_pointer<U>::value>::deduce(data, value);
	}
};


template <typename T>
void deduceVariantType(
	GVariantData * data,
	const T & value,
	typename std::enable_if<! std::is_convertible<T, const volatile cpgf::IObject *>::value>::type * = 0
)
{
	typedef typename std::remove_cv<typename std::decay<T>::type>::type U;
	GVariantDeducer<
		U,
		std::is_pointer<U>::value,
		std::is_lvalue_reference<U>::value,
		std::is_rvalue_reference<U>::value
	>::deduce(data, value);

	const uint16_t size = 0;
	constexpr uint16_t pointers = cpgf::PointerDimension<T>::Result;
	data->typeData.sizeAndPointers = (size << 4) | pointers;
}

template <typename T>
void deduceVariantType(
	GVariantData * data,
	const T & value,
	typename std::enable_if<std::is_convertible<T, const volatile cpgf::IObject *>::value>::type * = 0
)
{
	data->typeData.vt = (uint16_t)GVariantType::vtInterface;
	data->pointer = (void *)value;
	constexpr uint16_t size = sizeof(cpgf::IObject *);
	constexpr uint16_t pointers = cpgf::PointerDimension<T>::Result;
	data->typeData.sizeAndPointers = (size << 4) | pointers;
}


} //namespace variant_internal
