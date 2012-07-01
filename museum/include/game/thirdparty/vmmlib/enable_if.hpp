#ifndef __VMML__SFINAE__HPP__
#define __VMML__SFINAE__HPP__

#include <cstddef>

/**
 *   heavily inspired by boost::enable_if
 *   http://www.boost.org, file: boost/utility/enable_if.hpp,
 *   Copyright 2003 Jaakko Järvi, Jeremiah Willcock, Andrew Lumsdaine
 */
namespace vmml
{

template < bool condition, typename T = void >
struct enable_if { typedef T type; };

template< typename T >
struct enable_if< false, T > {};

} // namespace vmml


#endif

