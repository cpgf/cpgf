#ifndef CPGF_GIFELSE_H
#define CPGF_GIFELSE_H

namespace cpgf {

template <bool P, typename T, typename F>
struct GIfElse;

template <typename T, typename F>
struct GIfElse <true, T, F>
{
	typedef T Result;
};

template <typename T, typename F>
struct GIfElse <false, T, F>
{
	typedef F Result;
};

template <typename C, typename T, typename F>
struct GIfElseResult : public GIfElse <C::Result, T, F>
{
};


} // namespace cpgf


#endif
