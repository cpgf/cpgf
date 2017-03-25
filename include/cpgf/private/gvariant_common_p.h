namespace variant_internal {

typedef cpgf::GTypeList<
	bool,
	char, wchar_t,
	signed char, unsigned char,
	signed short, unsigned short,
	signed int, unsigned int,
	signed long, unsigned long,
	signed long long, unsigned long long,
	float, double, long double
> FundamentalTypeList;

struct VariantTypeInfo
{
	int size;
};

extern VariantTypeInfo variantTypeInfo[];

inline void failedCast()
{
	cpgf::raiseCoreException(cpgf::Error_Variant_FailCast);
}

inline void checkFailCast(bool success)
{
	if(!success) {
		failedCast();
	}
}

template <typename FromTypeList, typename To>
struct TypeListConvertible;

template <typename H, typename... Types, typename To>
struct TypeListConvertible <cpgf::GTypeList<H, Types...>, To>
{
	static constexpr bool myConvertible = std::is_convertible<H, To>::value;

	typedef typename std::conditional<
		myConvertible,
		H,
		typename TypeListConvertible<cpgf::GTypeList<Types...>, To>::type
	>::type type;
	static constexpr bool convertible = myConvertible || TypeListConvertible<cpgf::GTypeList<Types...>, To>::convertible;
};

template <typename To>
struct TypeListConvertible <cpgf::GTypeList<>, To>
{
	typedef void type;
	static constexpr bool convertible = false;
};


template <typename FromTypeList, typename To>
struct TypeListSame;

template <typename H, typename... Types, typename To>
struct TypeListSame <cpgf::GTypeList<H, Types...>, To>
{
	static constexpr bool mySame = std::is_same<H, To>::value;

	typedef typename std::conditional<
		mySame,
		H,
		typename TypeListSame<cpgf::GTypeList<Types...>, To>::type
	>::type type;
	static constexpr bool same = mySame || TypeListSame<cpgf::GTypeList<Types...>, To>::same;
};

template <typename To>
struct TypeListSame <cpgf::GTypeList<>, To>
{
	typedef void type;
	static constexpr bool same = false;
};


struct IVariantShadowObject : public cpgf::IObject
{
public:
	virtual void * G_API_CC getObject() = 0;
};

template <typename T>
class GVariantShadowObject : public IVariantShadowObject
{
private:
	typedef GVariantShadowObject<T> ThisType;

	G_INTERFACE_IMPL_OBJECT

public:
	virtual ~GVariantShadowObject() {}

protected:
	virtual void * G_API_CC getObject() {
		return (void *)(std::addressof(this->obj));
	}

public:
	GVariantShadowObject(const T & obj) : obj(obj) {
	}

private:
	T obj;
};

template <typename T>
struct ArrayToPointer
{
	typedef T Result;

	G_STATIC_CONSTANT(bool, IsArray = false);
};

#define DEF_ARRAY_TO_PTR(CV) \
	template <typename T> struct ArrayToPointer <CV T[]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; G_STATIC_CONSTANT(bool, IsArray = true); }; \
	template <typename T> struct ArrayToPointer <CV T (*) []> { typedef const volatile typename ArrayToPointer<T>::Result * Result; G_STATIC_CONSTANT(bool, IsArray = true); };

DEF_ARRAY_TO_PTR(GPP_EMPTY())
DEF_ARRAY_TO_PTR(const)
DEF_ARRAY_TO_PTR(volatile)
DEF_ARRAY_TO_PTR(const volatile)

#undef DEF_ARRAY_TO_PTR

#ifndef G_COMPILER_CPPBUILDER
#define DEF_ARRAY_TO_PTR(CV) \
	template <typename T, unsigned int N> struct ArrayToPointer <CV T[N]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; G_STATIC_CONSTANT(bool, IsArray = true); }; \
	template <typename T, unsigned int N> struct ArrayToPointer <CV T (*) [N]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; G_STATIC_CONSTANT(bool, IsArray = true); }; \
	template <typename T, unsigned int N> struct ArrayToPointer <CV T (&) [N]> { typedef const volatile typename ArrayToPointer<T>::Result * Result; G_STATIC_CONSTANT(bool, IsArray = true); };

DEF_ARRAY_TO_PTR(GPP_EMPTY())
DEF_ARRAY_TO_PTR(const)
DEF_ARRAY_TO_PTR(volatile)
DEF_ARRAY_TO_PTR(const volatile)

#undef DEF_ARRAY_TO_PTR
#endif

void retainVariantData(GVariantData & data);
void releaseVariantData(GVariantData & data);

template <typename T, typename Policy>
struct VariantCastResult
{
	typedef T Result;
};

template <typename T>
struct VariantCastResult <const T &, VarantCastKeepConstRef>
{
	typedef typename std::conditional<
		std::is_fundamental<T>::value,
		T,
		typename std::conditional<
			std::is_pointer<T>::value,
			const T,
			const T &
		>::type
	>::type Result;
};

template <typename T>
struct VariantCastResult <const T &, VarantCastCopyConstRef>
{
	typedef T Result;
};


} //namespace variant_internal
