#ifndef __VMML__VMMLIB_LAPACK_SVD__HPP__
#define __VMML__VMMLIB_LAPACK_SVD__HPP__

#include <vmmlib/matrix.hpp>
#include <vmmlib/vector.hpp>
#include <vmmlib/exception.hpp>

#include <vmmlib/lapack_types.hpp>
#include <vmmlib/lapack_includes.hpp>

#include <string>

/** 
*
*   a wrapper for lapack's DGESVD routine.
*   
*   returns a boolean to indicate success of the operation. 
*   if the return value is false, you can get the parameters using
*   get_params(). 
*   error states:
*
*  INFO    (output) INTEGER
*          = 0:  successful exit.
*          < 0:  if INFO = -i, the i-th argument had an illegal value.
*          > 0:  if DBDSQR did not converge, INFO specifies how many
*                superdiagonals of an intermediate bidiagonal form B
*                did not converge to zero. See the description of WORK
*                above for details.
*
*   more information in: http://www.netlib.org/lapack/double/dgesvd.f
**
*/


namespace vmml
{

namespace lapack
{

// XYYZZZ 
// X    = data type: S - float, D - double
// YY   = matrix type, GE - general, TR - triangular
// ZZZ  = function name


template< typename float_t >
struct svd_params
{
    char            jobu;
    char            jobvt;
    lapack_int      m;
    lapack_int      n;
    float_t*        a;
    lapack_int      lda;
    float_t*        s;
    float_t*        u;
    lapack_int      ldu;
    float_t*        vt;
    lapack_int      ldvt;
    float_t*        work;
    lapack_int      lwork;
    lapack_int      info;
    
    friend std::ostream& operator << ( std::ostream& os, 
        const svd_params< float_t >& p )
    {
        os 
            << "jobu "      << p.jobu 
            << " jobvt "    << p.jobvt
            << " m "        << p.m
            << " n "        << p.n 
            << " lda "      << p.lda
            << " ldu "      << p.ldu 
            << " ldvt "     << p.ldvt 
            << " lwork "    << p.lwork
            << " info "     << p.info
            << std::endl;
        return os;
    }
    
};


#if 0
/* Subroutine */ int dgesvd_(char *jobu, char *jobvt, integer *m, integer *n, 
	doublereal *a, integer *lda, doublereal *s, doublereal *u, integer *
	ldu, doublereal *vt, integer *ldvt, doublereal *work, integer *lwork, 
	integer *info);
#endif


template< typename float_t >
inline void
svd_call( svd_params< float_t >& p )
{
    VMMLIB_ERROR( "not implemented for this type.", VMMLIB_HERE );
}


template<>
inline void
svd_call( svd_params< float >& p )
{
    //std::cout << "calling lapack svd (single precision) " << std::endl;
    sgesvd_( 
        &p.jobu,
        &p.jobvt,
        &p.m,
        &p.n,
        p.a,
        &p.lda,
        p.s,
        p.u,
        &p.ldu,
        p.vt,
        &p.ldvt,
        p.work,
        &p.lwork,
        &p.info
        );

}


template<>
inline void
svd_call( svd_params< double >& p )
{
    //std::cout << "calling lapack svd (double precision) " << std::endl;
    dgesvd_( 
        &p.jobu,
        &p.jobvt,
        &p.m,
        &p.n,
        p.a,
        &p.lda,
        p.s,
        p.u,
        &p.ldu,
        p.vt,
        &p.ldvt,
        p.work,
        &p.lwork,
        &p.info
        );
}

} // namespace lapack



template< size_t M, size_t N, typename float_t >
struct lapack_svd
{
    lapack_svd();
    ~lapack_svd();

    // slow version, use if U and Vt are needed
    bool compute(
        const matrix< M, N, float_t >& A,
        matrix< M, N, float_t >& U,
        vector< N, float_t >& sigma,
        matrix< N, N, float_t >& Vt
        );

    // overwrites A with the result U, 
    bool compute_and_overwrite_input( 
        matrix< M, N, float_t >& A_U,
        vector< N, float_t >& sigma
        );

    // fast version, use if only sigma is needed.
    bool compute( 
        const matrix< M, N, float_t >& A,
        vector< N, float_t >& sigma
        );
        
    inline bool test_success( lapack::lapack_int info );
    
    lapack::svd_params< float_t > p;

    const lapack::svd_params< float_t >& get_params(){ return p; };
    
}; // struct lapack_svd


template< size_t M, size_t N, typename float_t >
lapack_svd< M, N, float_t >::lapack_svd()
{
    p.jobu      = 'N';
    p.jobvt     = 'N';
    p.m         = M;
    p.n         = N;
    p.a         = 0;
    p.lda       = M;
    p.s         = 0;
    p.u         = 0;
    p.ldu       = M;
    p.vt        = 0;
    p.ldvt      = 1;
    p.work      = new float_t;
    p.lwork     = -1;

    // workspace query
    lapack::svd_call( p );

    p.lwork = static_cast< lapack::lapack_int >( p.work[0] );
    delete p.work;

    p.work = new float_t[ p.lwork ];

}



template< size_t M, size_t N, typename float_t >
lapack_svd< M, N, float_t >::~lapack_svd()
{
    delete[] p.work; 
}



template< size_t M, size_t N, typename float_t >
bool
lapack_svd< M, N, float_t >::compute(
    const matrix< M, N, float_t >& A,
    matrix< M, N, float_t >& U,
    vector< N, float_t >& S,
    matrix< N, N, float_t >& Vt
    )
{
    // lapack destroys the contents of the input matrix
    matrix< M, N, float_t > AA( A );

    p.jobu      = 'A';
    p.jobvt     = 'A';
    p.a         = AA.array;
    p.u         = U.array;
    p.s         = S.array;
    p.vt        = Vt.array;
    p.ldvt      = N;

    lapack::svd_call< float_t >( p );
    
    return p.info == 0;
}



template< size_t M, size_t N, typename float_t >
bool
lapack_svd< M, N, float_t >::compute_and_overwrite_input(
    matrix< M, N, float_t >& A_U,
    vector< N, float_t >& S
    )
{
    p.jobu      = 'O';
    p.jobvt     = 'N';
    p.a         = A_U.array;
    p.s         = S.array;
    p.ldvt      = N;

    lapack::svd_call< float_t >( p );

    return p.info == 0;
}



template< size_t M, size_t N, typename float_t >
bool
lapack_svd< M, N, float_t >::compute( 
    const matrix< M, N, float_t >& A,
    vector< N, float_t >& S
    )
{
    // lapack destroys the contents of the input matrix
    matrix< M, N, float_t > AA( A ); 
    
    p.jobu      = 'N';
    p.jobvt     = 'N';
    p.a         = AA.array;
    p.u         = 0;
    p.s         = S.array;
    p.vt        = 0;

    lapack::svd_call< float_t >( p );

    return p.info == 0;
}


} // namespace vmml

#endif

