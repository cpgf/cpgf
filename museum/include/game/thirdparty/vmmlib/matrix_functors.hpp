#ifndef __VMML__MATRIX_FUNCTORS__HPP__
#define __VMML__MATRIX_FUNCTORS__HPP__

#include <vmmlib/enable_if.hpp>

#include <cstddef>
#include <functional>

namespace vmml
{

template< typename T >
struct set_to_zero_functor
{
    inline void operator()( T& matrix_ ) const
    {
        matrix_ = static_cast< typename T::value_type >( 0.0 );
    }
    
}; // struct set_to_zero


template< typename T >
struct set_to_identity_functor
{
    inline
    typename enable_if< T::ROWS == T::COLS >::type*
    operator()( T& matrix_ )
    {
        set_to_zero_functor< T >()( matrix_ );
        for( size_t index = 0; index < T::ROWS; ++index )
        {
            matrix_( index, index ) = static_cast< typename T::value_type >( 1.0 );
        }
        return 0; // for sfinae
    }
}; // struct set_to_identity


// this functor compares to matrices, and also returns true/equal if 
// the matrices have the same values but some rows/columns are inverted
template< typename T >
struct matrix_equals_allow_inverted_rows : std::binary_function< const T&, const T&, bool >
{

    bool operator()( const T& matrix0, const T& matrix1 )
    {
        const size_t r = matrix0.get_number_of_rows();
        bool ok = true;
        for( size_t index = 0; ok && index < r; ++index )
        {
            if ( matrix0.get_row( index ) != matrix1.get_row( index ) 
                && matrix0.get_row( index ) != - matrix1.get_row( index ) )
            {
                ok = false;
            }
        
        }
        return ok;
    }

    bool operator()( const T& matrix0, const T& matrix1, typename T::value_type tolerance )
    {
        const size_t r = matrix0.get_number_of_rows();
        bool ok = true;
        for( size_t index = 0; ok && index < r; ++index )
        {
            if ( 
                ! matrix0.get_row( index ).equals( matrix1.get_row( index ), tolerance ) 
                && ! matrix0.get_row( index ).equals( - matrix1.get_row( index ), tolerance )
                )
            {
                ok = false;
            }
        
        }
        return ok;
    }


}; // struct matrix_equals_allow_inverted_rows

template< typename T >
struct matrix_equals_allow_inverted_columns : std::binary_function< const T&, const T&, bool >
{

    bool operator()( const T& matrix0, const T& matrix1 )
    {
        const size_t r = matrix0.get_number_of_columns();
        bool ok = true;
        for( size_t index = 0; ok && index < r; ++index )
        {
            if ( matrix0.get_column( index ) != matrix1.get_column( index ) 
                && matrix0.get_column( index ) != - matrix1.get_column( index ) )
            {
                ok = false;
            }
        
        }
        return ok;
    }

    bool operator()( const T& matrix0, const T& matrix1, typename T::value_type tolerance )
    {
        const size_t r = matrix0.get_number_of_columns();
        bool ok = true;
        for( size_t index = 0; ok && index < r; ++index )
        {
            if ( 
                ! matrix0.get_column( index ).equals( matrix1.get_column( index ), tolerance ) 
                && ! matrix0.get_column( index ).equals( - matrix1.get_column( index ), tolerance )
                )
            {
                ok = false;
            }
        
        }
        return ok;
    }


}; // struct matrix_equals_allow_inverted_columns


} // namespace vmml

#endif

