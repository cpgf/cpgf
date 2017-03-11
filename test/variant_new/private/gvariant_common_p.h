namespace variant_internal {

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


} //namespace variant_internal
