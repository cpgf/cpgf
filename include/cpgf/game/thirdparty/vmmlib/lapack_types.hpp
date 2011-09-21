#ifndef __VMML__LAPACK_TYPES__HPP__
#define __VMML__LAPACK_TYPES__HPP__

#include "lapack_includes.hpp"


namespace vmml
{

namespace lapack
{

#ifdef __APPLE__

typedef __CLPK_integer  lapack_int;

#else

typedef ssize_t         lapack_int;

#endif


} // namespace lapack


} // namespace vmml

#endif

