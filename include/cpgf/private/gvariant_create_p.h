namespace variant_internal {

constexpr int FundamentalTypeStartType = (int)GVariantType::vtFundamentalBegin;

template <typename T, bool IsEnum>
struct GVariantEnumSelector;

template <typename T>
struct GVariantEnumSelector <T, true>
{
	typedef typename std::underlying_type<T>::type Result;
};

template <typename T>
struct GVariantEnumSelector <T, false>
{
	typedef T Result;
};

template <typename T>
struct GVariantCreatingType
{
	typedef typename std::remove_cv<T>::type A;
	
	// std::decay removes referene, let's backup it somewhere.
	static constexpr bool lvalueRef = std::is_lvalue_reference<A>::value;
	static constexpr bool rvalueRef = std::is_rvalue_reference<A>::value;
	
	typedef typename std::decay<
		typename ArrayToPointer<
			A
		>::Result
	>::type B;

	typedef typename std::conditional<
		lvalueRef,
		B &,
		typename std::conditional<
			rvalueRef,
			B &&,
			B
		>::type
	>::type U;
	
	typedef typename GVariantEnumSelector<U, std::is_enum<U>::value>::Result Result;
};

template <typename T>
struct GVariantDeducer_Type
{
	static constexpr int fundamentalIndex = cpgf::TypeList_IndexOf<FundamentalTypeList, T>::Result;
	static constexpr int Type =
		(fundamentalIndex >= 0 ? (fundamentalIndex + FundamentalTypeStartType)
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
	static void doAssignReal(GVariantData * data, const V & value,
		typename std::enable_if<std::is_same<V, float>::value>::type * = 0)
	{
		data->valueFloat = (float)value;
	}

	template <typename V>
	static void doAssignReal(GVariantData * data, const V & value,
		typename std::enable_if<std::is_same<V, double>::value>::type * = 0)
	{
		data->valueDouble = (double)value;
	}

	template <typename V>
	static void doAssignReal(GVariantData * data, const V & value,
		typename std::enable_if<std::is_same<V, long double>::value>::type * = 0)
	{
		data->valueLongDouble = (long double)value;
	}

	template <typename V>
	static void assign(GVariantData * data, const V & value,
		typename std::enable_if<std::is_floating_point<V>::value>::type * = 0)
	{
		doAssignReal(data, value);
	}
};

template <typename T, int typeMask, bool isPointerOrReference>
struct GVariantDeducer_Value_Helper;

template <typename T, int typeMask>
struct GVariantDeducer_Value_Helper <T, typeMask, false>
{
	typedef GVariantDeducer_Type<T> TypeDeducer;

	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		data->typeData.vt = TypeDeducer::Type;
		vtSetSize(data->typeData, (TypeDeducer::fundamentalIndex >= 0 ? variantTypeInfo[TypeDeducer::fundamentalIndex].size : 0));
		TypeDeducer::assign(data, value);
	}

	static void deduce(GVariantData * data)
	{
		data->typeData.vt = TypeDeducer::Type;
		vtSetSize(data->typeData, (TypeDeducer::fundamentalIndex >= 0 ? variantTypeInfo[TypeDeducer::fundamentalIndex].size : 0));
	}
};

template <typename T, int typeMask>
struct GVariantDeducer_Value_Helper <T, typeMask, true>
{
	typedef GVariantDeducer_Type<T> TypeDeducer;

	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		data->typeData.vt = TypeDeducer::Type | typeMask;
		vtSetSize(data->typeData, sizeof(void *));
		data->pointer = (void *)(value);
	}

	static void deduce(GVariantData * data)
	{
		data->typeData.vt = TypeDeducer::Type | typeMask;
		vtSetSize(data->typeData, sizeof(void *));
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
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value_Helper<T, typeMask, isPointerOrReference>::deduceAndSet(data, value);
	}

	static void deduce(GVariantData * data)
	{
		GVariantDeducer_Value_Helper<T, typeMask, isPointerOrReference>::deduce(data);
	}
};

template <typename T>
struct GVariantDeducer_Pointer
{
	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<T, true, false, false>::deduceAndSet(data, value);
	}

	static void deduce(GVariantData * data)
	{
		GVariantDeducer_Value<T, true, false, false>::deduce(data);
	}
};

template <typename T, bool isPointer>
struct GVariantDeducer_LvalueReference;

template <typename T>
struct GVariantDeducer_LvalueReference <T, false>
{
	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<
			T,
			false,
			true,
			false
		>::deduceAndSet(data, std::addressof(value));
	}

	static void deduce(GVariantData * data)
	{
		GVariantDeducer_Value<
			T,
			false,
			true,
			false
		>::deduce(data);
	}
};

template <typename T>
struct GVariantDeducer_LvalueReference <T, true>
{
	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<
			typename std::remove_cv<typename std::remove_pointer<T>::type >::type,
			true,
			true,
			false
		>::deduceAndSet(data, std::addressof(value));
	}

	static void deduce(GVariantData * data)
	{
		GVariantDeducer_Value<
			typename std::remove_cv<typename std::remove_pointer<T>::type >::type,
			true,
			true,
			false
		>::deduce(data);
	}
};

template <typename T, bool isPointer>
struct GVariantDeducer_RvalueReference;

template <typename T>
struct GVariantDeducer_RvalueReference <T, false>
{
	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<
			T,
			false,
			false,
			true
		>::deduceAndSet(data, std::addressof(value));
	}

	static void deduce(GVariantData * data)
	{
		GVariantDeducer_Value<
			T,
			false,
			false,
			true
		>::deduce(data);
	}
};

template <typename T>
struct GVariantDeducer_RvalueReference <T, true>
{
	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<
			typename std::remove_cv<typename std::remove_pointer<T>::type >::type,
			true,
			false,
			true
		>::deduceAndSet(data, std::addressof(value));
	}

	static void deduce(GVariantData * data)
	{
		GVariantDeducer_Value<
			typename std::remove_cv<typename std::remove_pointer<T>::type >::type,
			true,
			false,
			true
		>::deduce(data);
	}
};

template <typename T, bool isPointer, bool isLvalueReference, bool isRvalueReference>
struct GVariantDeducer;

template <typename T>
struct GVariantDeducer <T, false, false, false>
{
	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		GVariantDeducer_Value<T, false, false, false>::deduceAndSet(data, value);
	}

	static void deduce(GVariantData * data)
	{
		GVariantDeducer_Value<T, false, false, false>::deduce(data);
	}
};

template <typename T>
struct GVariantDeducer <T, true, false, false>
{
	typedef typename std::remove_cv<typename std::remove_pointer<T>::type >::type U;

	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		GVariantDeducer_Pointer<U>::deduceAndSet(data, value);
	}

	static void deduce(GVariantData * data)
	{
		GVariantDeducer_Pointer<U>::deduce(data);
	}
};

template <typename T>
struct GVariantDeducer <T, false, true, false>
{
	typedef typename std::remove_cv<typename std::remove_reference<T>::type >::type U;

	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		GVariantDeducer_LvalueReference<U, std::is_pointer<U>::value>::deduceAndSet(data, value);
	}

	static void deduce(GVariantData * data)
	{
		GVariantDeducer_LvalueReference<U, std::is_pointer<U>::value>::deduce(data);
	}
};

template <typename T>
struct GVariantDeducer <T, false, false, true>
{
	typedef typename std::remove_cv<typename std::remove_reference<T>::type >::type U;

	template <typename V>
	static void deduceAndSet(GVariantData * data, const V & value)
	{
		GVariantDeducer_RvalueReference<U, std::is_pointer<U>::value>::deduceAndSet(data, value);
	}

	static void deduce(GVariantData * data)
	{
		GVariantDeducer_RvalueReference<U, std::is_pointer<U>::value>::deduce(data);
	}
};


template <typename T, typename V>
void variantDeduceAndSet(
	GVariantData * data,
	const V & value,
	typename std::enable_if<! std::is_convertible<T, const volatile cpgf::IObject *>::value>::type * = 0
)
{
	typedef typename GVariantCreatingType<T>::Result U;
	GVariantDeducer<
		U,
		std::is_pointer<U>::value,
		std::is_lvalue_reference<U>::value,
		std::is_rvalue_reference<U>::value
	>::template deduceAndSet(data, (const U &)value);

	constexpr uint16_t pointers = cpgf::PointerDimension<U>::Result;
	vtSetPointers(data->typeData, pointers);
}

template <typename T, typename V>
void variantDeduceAndSet(
	GVariantData * data,
	const V & value,
	typename std::enable_if<std::is_convertible<T, const volatile cpgf::IObject *>::value>::type * = 0
)
{
	data->typeData.vt = (uint16_t)GVariantType::vtInterface;
	constexpr uint16_t size = sizeof(cpgf::IObject *);
	constexpr uint16_t pointers = cpgf::PointerDimension<T>::Result;
	vtSetSizeAndPointers(data->typeData, size, pointers);

	data->valueInterface = (cpgf::IObject *)value;
	if(data->valueInterface != nullptr) {
		data->valueInterface->addReference();
	}
}


template <typename T>
void variantDeduceType(
	GVariantData * data,
	typename std::enable_if<! std::is_convertible<T, const volatile cpgf::IObject *>::value>::type * = 0
)
{
	typedef typename GVariantCreatingType<T>::Result U;
	GVariantDeducer<
		U,
		std::is_pointer<U>::value,
		std::is_lvalue_reference<U>::value,
		std::is_rvalue_reference<U>::value
	>::deduce(data);

	constexpr uint16_t pointers = cpgf::PointerDimension<U>::Result;
	vtSetPointers(data->typeData, pointers);
}

template <typename T>
void variantDeduceType(
	GVariantData * data,
	typename std::enable_if<std::is_convertible<T, const volatile cpgf::IObject *>::value>::type * = 0
)
{
	data->typeData.vt = (uint16_t)GVariantType::vtInterface;
	constexpr uint16_t size = sizeof(cpgf::IObject *);
	constexpr uint16_t pointers = cpgf::PointerDimension<T>::Result;
	vtSetSizeAndPointers(data->typeData, size, pointers);
}


} //namespace variant_internal
