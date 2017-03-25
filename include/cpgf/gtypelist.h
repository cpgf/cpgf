#ifndef CPGF_GTYPELIST_H
#define CPGF_GTYPELIST_H

namespace cpgf {

struct GNullType {};

template <typename... Types>
struct GTypeList
{
};

using GNullTypeList = GTypeList<>;

template <typename... Types>
struct TypeList_Make
{
	typedef GTypeList<Types...> Result;
};

template <typename... Types>
struct TypeList_Concat
{
	typedef GTypeList<Types...> Result;
};

template <typename... TypesA, typename... TypesB>
struct TypeList_Concat <GTypeList<TypesA...>, GTypeList<TypesB...> >
{
	typedef GTypeList<TypesA..., TypesB...> Result;
};

template <typename TypeList, unsigned int index>
struct TypeList_Get;

template <typename T, typename... Types, unsigned int index>
struct TypeList_Get <GTypeList<T, Types...>, index>
{
	typedef typename TypeList_Get<GTypeList<Types...>, index - 1>::Result Result;
};

template <typename T, typename... Types>
struct TypeList_Get <GTypeList<T, Types...>, 0>
{
	typedef T Result;
};

template <unsigned int index>
struct TypeList_Get <GNullTypeList, index>
{
	typedef void Result;
};

// should be enhanced or removed
template <typename TypeList, unsigned int index, typename Default = void>
using TypeList_GetWithDefault = TypeList_Get <TypeList, index>;

template <typename GTypeList>
struct TypeList_Length;

template <typename... Types>
struct TypeList_Length <GTypeList<Types...> >
{
	static constexpr unsigned int Result = sizeof...(Types);
};

template <typename TypeList, typename T>
struct TypeList_IndexOf;

template <typename... Types, typename H, typename T>
struct TypeList_IndexOf <GTypeList<H, Types...>, T>
{
private:
	static constexpr int PreviousIndex = TypeList_IndexOf<GTypeList<Types...>, T>::Result;
	
public:
	static constexpr int Result = (PreviousIndex < 0 ? PreviousIndex : PreviousIndex + 1);
};

template <typename... Types, typename T>
struct TypeList_IndexOf <GTypeList<T, Types...>, T>
{
	static constexpr int Result = 0;
};

template <typename T>
struct TypeList_IndexOf <GNullTypeList, T>
{
	static constexpr int Result = -1;
};

template <typename TypeList, typename T>
struct TypeList_Append;

template <typename... Types, typename T>
struct TypeList_Append <GTypeList<Types...>, T>
{
	typedef GTypeList<Types..., T> Result;
};

template <typename... TypesA, typename... TypesB>
struct TypeList_Append <GTypeList<TypesA...>, GTypeList<TypesB...> >
{
	typedef GTypeList<TypesA..., TypesB...> Result;
};


} // namespace cpgf



#endif
