#ifndef __VMML__VMMLIB_LAPACK_LINEAR_LEAST_SQUARES__HPP__
#define __VMML__VMMLIB_LAPACK_LINEAR_LEAST_SQUARES__HPP__

#include <vmmlib/matrix.hpp>
#include <vmmlib/vector.hpp>
#include <vmmlib/exception.hpp>

#include <vmmlib/lapack_types.hpp>
#include <vmmlib/lapack_includes.hpp>

#include <string>

/**
*
* this is a wrapper for the following lapack routines: 
*   
* xGELS
*
*
*/ 


namespace vmml
{

// XYYZZZ 
// X    = data type: S - float, D - double
// YY   = matrix type, GE - general, TR - triangular
// ZZZ  = function name

namespace lapack
{

//
//
// SGELS/DGELS
//
//


// parameter struct
template< typename float_t >
struct llsq_params_xgels
{
    char            trans;  // 'N'->A, 'T'->Atransposed
    lapack_int      m;      // number of rows,      M >= 0
    lapack_int      n;      // number of columns,   N >= 0
    lapack_int      nrhs;   // number of columns of B/X
    float_t*        a;      // input A
    lapack_int      lda;    // leading dimension of A (number of rows)
    float_t*        b;      // input B, output X
    lapack_int      ldb;    // leading dimension of b 
    float_t*        work;   // workspace
    lapack_int      lwork;  // workspace size
    lapack_int      info;   // 'return' value
    
    friend std::ostream& operator << ( std::ostream& os, 
        const llsq_params_xgels< float_t >& p )
    {
        os 
            << " m "        << p.m
            << " n "        << p.n 
            << " nrhs "     << p.nrhs
            << " lda "      << p.lda
            << " ldb "      << p.ldb 
            << " lwork "    << p.lwork 
            << " info "     << p.info
            << std::endl;
        return os;
    }

};

// call wrappers

#if 0
void dgels_(const char *trans, const int *M, const int *N, const int *nrhs,
    double *A, const int *lda, double *b, const int *ldb, double *work,
    const int * lwork, int *info); 
#endif

template< typename float_t >
inline void
llsq_call_xgels( llsq_params_xgels< float_t >& p )
{
    VMMLIB_ERROR( "not implemented for this type.", VMMLIB_HERE );
}

template<>
inline void
llsq_call_xgels( llsq_params_xgels< float >& p )
{
    sgels_(
        &p.trans,
        &p.m,
        &p.n,
        &p.nrhs,
        p.a,
        &p.lda,
        p.b,
        &p.ldb,
        p.work,
        &p.lwork,
        &p.info
    );
}

template<>
inline void
llsq_call_xgels( llsq_params_xgels< double >& p )
{
    dgels_(
        &p.trans,
        &p.m,
        &p.n,
        &p.nrhs,
        p.a,
        &p.lda,
        p.b,
        &p.ldb,
        p.work,
        &p.lwork,
        &p.info
    );

}


template< size_t M, size_t N, typename float_t >
struct linear_least_squares_xgels
{
    bool compute( 
        const matrix< M, N, float_t >& A, 
        const vector< M, float_t >& B, 
        vector< N, float_t >& x );
    
    linear_least_squares_xgels();
    ~linear_least_squares_xgels();
       
    const lapack::llsq_params_xgels< float_t >& get_params(){ return p; };

    matrix< M, N, float_t >& get_factorized_A() { return _A; }
    
protected:
    matrix< M, N, float_t > _A;
    vector< M, float_t >    _b;

    llsq_params_xgels< float_t > p;

};



template< size_t M, size_t N, typename float_t >
bool
linear_least_squares_xgels< M, N, float_t >::compute( 
    const matrix< M, N, float_t >& A, 
    const vector< M, float_t >& B, 
    vector< N, float_t >& x )
{
    _A = A;
    _b = B;
    
    llsq_call_xgels( p );
    
    // success
    if ( p.info == 0 )
    {
        for( size_t index = 0; index < N; ++index )
        {
            x( index ) = _b( index );
        }
    
        return true;
    }    
    if ( p.info < 0 )
    {
        VMMLIB_ERROR( "xGELS - invalid argument.", VMMLIB_HERE );
    }
    else
    {
        std::cout << "A\n" << A << std::endl;
        std::cout << "B\n" << B << std::endl;

        VMMLIB_ERROR( "least squares solution could not be computed.", 
            VMMLIB_HERE );
    }
    return false;
}



template< size_t M, size_t N, typename float_t >
linear_least_squares_xgels< M, N, float_t >::
linear_least_squares_xgels()
{
    p.trans = 'N';
    p.m     = M;
    p.n     = N;
    p.nrhs  = 1;
    p.a     = _A.array;
    p.lda   = M;
    p.b     = _b.array;
    p.ldb   = M;
    p.work  = new float_t();
    p.lwork = -1;

    // workspace query
    llsq_call_xgels( p );

    p.lwork = static_cast< lapack_int > ( p.work[0] );
    delete p.work;

    p.work = new float_t[ p.lwork ];
}



template< size_t M, size_t N, typename float_t >
linear_least_squares_xgels< M, N, float_t >::
~linear_least_squares_xgels()
{
    delete[] p.work;
}



//
//
// SGESV/DGESV
//
//

template< typename float_t >
struct llsq_params_xgesv
{
    lapack_int      n; // order of matrix A = M * N
    lapack_int      nrhs; // number of columns of B
    float_t*        a;   // input A, output P*L*U
    lapack_int      lda; // leading dimension of A (for us: number of rows)
    lapack_int*     ipiv; // pivot indices, integer array of size N
    float_t*        b;  // input b, output X
    lapack_int      ldb; // leading dimension of b
    lapack_int      info;
    
    friend std::ostream& operator << ( std::ostream& os, 
        const llsq_params_xgesv< float_t >& p )
    {
        os 
            << "n "         << p.n 
            << " nrhs "     << p.nrhs
            << " lda "      << p.lda
            << " ldb "      << p.ldvt 
            << " info "     << p.info
            << std::endl;
        return os;
    }
    
};


#if 0
/* Subroutine */ int dgesv_(integer *n, integer *nrhs, doublereal *a, integer 
	*lda, integer *ipiv, doublereal *b, integer *ldb, integer *info);
#endif


template< typename float_t >
inline void
llsq_call_xgesv( llsq_params_xgesv< float_t >& p )
{
    VMMLIB_ERROR( "not implemented for this type.", VMMLIB_HERE );
}


template<>
inline void
llsq_call_xgesv( llsq_params_xgesv< float >& p )
{
    sgesv_( 
        &p.n,
        &p.nrhs,
        p.a,
        &p.lda,
        p.ipiv,
        p.b,
        &p.ldb,
        &p.info
    );

}


template<>
inline void
llsq_call_xgesv( llsq_params_xgesv< double >& p )
{
    dgesv_( 
        &p.n,
        &p.nrhs,
        p.a,
        &p.lda,
        p.ipiv,
        p.b,
        &p.ldb,
        &p.info
    );
}


template< size_t M, size_t N, typename float_t >
struct linear_least_squares_xgesv
{
    // computes x ( Ax = b ). x replaces b on output.
    void compute(
        matrix< N, N, float_t >& A, 
        matrix< N, M, float_t >& b 
        );

    linear_least_squares_xgesv();
    ~linear_least_squares_xgesv();

    const lapack::llsq_params_xgesv< float_t >& get_params() { return p; }

    lapack::llsq_params_xgesv< float_t > p;
    
}; // struct lapack_linear_least_squares


template< size_t M, size_t N, typename float_t >
void
linear_least_squares_xgesv< M, N, float_t >::
compute(
        matrix< N, N, float_t >& A, 
        matrix< N, M, float_t >& b 
        )
{
    p.a = A.array;
    p.b = b.array;
    
    lapack::llsq_call_xgesv( p );

    if ( p.info != 0 )
    {
        if ( p.info < 0 )
            VMMLIB_ERROR( "invalid value in input matrix", VMMLIB_HERE );
        else
            VMMLIB_ERROR( "factor U is exactly singular, solution could not be computed.", VMMLIB_HERE );
    }
}



template< size_t M, size_t N, typename float_t >
linear_least_squares_xgesv< M, N, float_t >::
linear_least_squares_xgesv()
{
    p.n     = N;
    p.nrhs  = M;
    p.lda   = N;
    p.ldb   = N;
    p.ipiv = new lapack_int[ N ];

}



template< size_t M, size_t N, typename float_t >
linear_least_squares_xgesv< M, N, float_t >::
~linear_least_squares_xgesv()
{
    delete[] p.ipiv;
}


} // namespace lapack

} // namespace vmml

#endif

