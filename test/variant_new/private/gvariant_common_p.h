
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


