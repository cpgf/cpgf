#ifndef __GTYPELIST_H
#define __GTYPELIST_H

// code in this file is highly inspired from Loki library

#include "cpgf/gcompiler.h"
#include "cpgf/gpp.h"


#define TL_MAX_PARAM GPP_MAX_LIMIT


namespace cpgf {

struct GNullType {
};


template <typename H, typename T>
struct GTypeList {
	typedef H Head;
	typedef T Tail;
};


#define TL_TYPENAME_PARAM_NULL(N, T)			GPP_COMMA_IF(N) typename T ## N = GNullType
#define TL_DO_TYPE(N) T ## N GPP_COMMA()
#define TL_NOTDO_TYPE(N)
#define TL_TAIL_TYPE(N, T)			GPP_IF(N, TL_DO_TYPE, TL_NOTDO_TYPE)(N)
#define TL_LESS_PARAM GPP_DEC(TL_MAX_PARAM)

template <GPP_REPEAT(TL_MAX_PARAM, TL_TYPENAME_PARAM_NULL, T) >
struct TypeList_Make {
private:
	typedef typename TypeList_Make<GPP_REPEAT(TL_LESS_PARAM, TL_TAIL_TYPE, T) GPP_CONCAT(T, TL_LESS_PARAM) >::Result TailResult;

public:
	typedef GTypeList<T0, TailResult> Result;
};

template<>
struct TypeList_Make < > {
	typedef GNullType Result;
};

#undef TL_TYPENAME_PARAM_NULL
#undef TL_DO_TYPE
#undef TL_NOTDO_TYPE
#undef TL_TAIL_TYPE
#undef TL_LESS_PARAM


// get

template <typename TypeList, unsigned int index>
struct TypeList_Get;

template <typename H, typename T>
struct TypeList_Get <GTypeList<H, T>, 0 > {
    typedef H Result;
};

template <typename H, typename T, unsigned int index>
struct TypeList_Get <GTypeList<H, T>, index> {
    typedef typename TypeList_Get<T, index - 1>::Result Result;
};


// get with default

template <typename TypeList, unsigned int index, typename Default = void>
struct TypeList_GetWithDefault {
    typedef Default Result;
};

template <typename H, typename T, typename Default>
struct TypeList_GetWithDefault <GTypeList<H, T>, 0, Default> {
    typedef H Result;
};

template <typename H, typename T, unsigned int i, typename Default>
struct TypeList_GetWithDefault <GTypeList<H, T>, i, Default> {
    typedef typename TypeList_GetWithDefault<T, i - 1, Default>::Result Result;
};


// length

template <typename GTypeList>
struct TypeList_Length;

template <>
struct TypeList_Length <GNullType> {
	G_STATIC_CONSTANT(int, Result = 0);
};
        
template <typename H, typename T>
struct TypeList_Length<GTypeList<H, T> > {
	G_STATIC_CONSTANT(int, Result = 1 + TypeList_Length<T>::Result);
};


// index of

template <typename TypeList, typename T>
struct TypeList_IndexOf;

template <typename T>
struct TypeList_IndexOf<GNullType, T>
{
    G_STATIC_CONSTANT(int, Result = -1);
};

template <typename H, typename T>
struct TypeList_IndexOf<GTypeList<H, T>, H>
{
    G_STATIC_CONSTANT(int, Result = 0);
};

template <typename Head, typename Tail, typename T>
struct TypeList_IndexOf<GTypeList<Head, Tail>, T>
{
private:
    G_STATIC_CONSTANT(int, temp = (TypeList_IndexOf<Tail, T>::Result));
public:
    G_STATIC_CONSTANT(int, Result = (temp == -1 ? -1 : 1 + temp));
};


// append

template <typename TypeList, typename T>
struct TypeList_Append;

template <>
struct TypeList_Append <GNullType, GNullType>
{
    typedef GNullType Result;
};

template <typename T>
struct TypeList_Append<GNullType, T>
{
    typedef GTypeList<T, GNullType> Result;
};

template <typename Head, typename Tail>
struct TypeList_Append <GNullType, GTypeList<Head, Tail> >
{
    typedef GTypeList<Head, Tail> Result;
};

template <typename Head, typename Tail, typename T>
struct TypeList_Append <GTypeList<Head, Tail>, T>
{
    typedef GTypeList<Head, typename TypeList_Append<Tail, T>::Result> Result;
};


} // namespace cpgf



#undef TL_MAX_PARAM



#endif
