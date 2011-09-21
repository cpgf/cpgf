#ifndef __VMML__QR_DECOMPOSITION__HPP__
#define __VMML__QR_DECOMPOSITION__HPP__

#include <vmmlib/matrix.hpp>
#include <vmmlib/vector.hpp>
#include <vmmlib/exception.hpp>

#include <cmath>
#include <vector>

/*
* QR decomposition using stabilized gram-schmidt
* A  -> matrix to be factorized
* Q  -> orthonormal
* Rn -> upper triangular 
*/

namespace vmml
{

template< size_t M, size_t N, typename T >
void qr_decompose_gram_schmidt( 
    const matrix< M, N, T >& A_, 
    matrix< M, M, T >& Q, 
    matrix< N, N, T >& R 
    )
{
    Q   = 0.0; 
    R   = 0.0;

    // create a copy of A_ since we will change it in the algorithm
    matrix< M, N, T > A( A_ );

    vector< M, T > a_column, q_column;
    
    #if 0
    // for each column
    for( size_t k = 0; k < N; ++k )
    {
        // compute norm of A's column k
        A.get_column( k, a_column );
        const T a_norm = a_column.length();

        R.at( k, k ) = a_norm;
               
        if ( a_norm == static_cast< T >( 0.0 ) )
            break;

        Q.set_column( k, a_column / a_norm );
    
        for( size_t j = k+1; j < N; ++j )
        {
            Q.get_column( k, q_column );
            A.get_column( j, a_column );
            
            R.at( k, j ) = a_column.dot( q_column );
            
            for( size_t i = 0; i < M; ++i )
            {
                A( i, j ) = A( i, j ) - R( k, j ) * Q( i, k );
            }
        }
    }
    #else
    vector< M, T >  v;
    for( int j = 0; j < N; ++j )
    {
        A.get_column( j, v );

        for( int i = 0; i < j; ++i )
        {
            Q.get_column( i, q_column );
            A.get_column( j, a_column );

            R( i, j ) = dot( q_column, a_column );
            v -= q_column * R(i,j);
        }
        R(j,j) = v.length();
        Q.set_column( j, v / R(j,j) );
    }
    
    
    #endif

}


} // namespace vmml

#endif

