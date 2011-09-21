/* 
 * VMMLib - Tensor Classes
 *  
 * @author Susanne Suter
 * @author Jonas Boesch
 *
 * The tucker3 tensor class is consists of the same components (core tensor, basis matrices u1-u3) as the tucker3 model described in:
 * Tucker, “Some mathematical notes on three-mode factor analysis”, Psychometrika, vol. 31, no. 3, pp. 279–311, 1966 Sep.
 * 
 */

#ifndef __VMML__TUCKER3_TENSOR__HPP__
#define __VMML__TUCKER3_TENSOR__HPP__

#include <vmmlib/tensor3.hpp>
#include <vmmlib/svd.hpp>
#include <vmmlib/lapack_svd.hpp>

namespace vmml
{
	
	template< size_t J1, size_t J2, size_t J3, size_t I1, size_t I2, size_t I3, typename T = float >
	class tucker3_tensor
	{
public:    
    tucker3_tensor( tensor3< J1, J2, J3, T >& core, matrix< I1, J1, T >& U1, matrix< I2, J2, T >& U2, matrix< I3, J3, T >& U3 );

	void set_core( const tensor3< J1, J2, J3, T >& core )  { _core = core; } ;
	void set_u1( const matrix< I1, J1, T >& U1 ) { _u1 = U1; } ;
	void set_u2( const matrix< I2, J2, T >& U2 ) { _u2 = U2; } ;
	void set_u3( const matrix< I3, J3, T >& U3 ) { _u3 = U3; } ;
	
    tensor3< J1, J2, J3, T > get_core() const { return _core; } ;
	matrix< I1, J1, T > get_u1() const { return _u1; } ;
	matrix< I2, J2, T > get_u2() const { return _u2; } ;
	matrix< I3, J3, T > get_u3() const { return _u3; } ;
	
	void reconstruction( tensor3< I1, I2, I3, T >& tensor_ ) const;
	void decomposition( const tensor3< I1, I2, I3, T >& tensor_ ); 
	void tucker_method1( const tensor3< I1, I2, I3, T >& tensor_ );
    void derive_core( const tensor3< I1, I2, I3, T >& data_, tensor3< J1, J2, J3, T >& core_, const matrix< I1, J1, T >& U1_, const matrix< I2, J2, T >& U2_, const matrix< I3, J3, T >& U3_ );

	template< size_t K1, size_t K2, size_t K3>
	void rank_reduction( const tucker3_tensor< K1, K2, K3, I1, I2, I3, T>& other ); //call TuckerJI.rank_recuction(TuckerKI) K1 -> J1, K2 -> J2, K3 -> J3

	template< size_t K1, size_t K2, size_t K3>
	void subsampling( const tucker3_tensor< J1, J2, J3, K1, K2, K3, T>& other, const T& factor  );

	template< size_t K1, size_t K2, size_t K3>
	void region_of_interest( const tucker3_tensor< J1, J2, J3, K1, K2, K3, T>& other, 
							const T& start_index1, const T& end_index1, 
							const T& start_index2, const T& end_index2, 
							const T& start_index3, const T& end_index3);
	
	void tucker_method1();

	
	
	/*TODO:
	void tuck3als();
	void hosvd();
	void hooi();
	*/
	

private:
    tensor3< J1, J2, J3, T > _core ;
	matrix< I1, J1, T > _u1 ;
	matrix< I2, J2, T > _u2 ;
	matrix< I3, J3, T > _u3 ;
	
}; // class tucker3_tensor


#define VMML_TEMPLATE_STRING    	template< size_t J1, size_t J2, size_t J3, size_t I1, size_t I2, size_t I3, typename T >
#define VMML_TEMPLATE_CLASSNAME     tucker3_tensor< J1, J2, J3, I1, I2, I3,  T >


VMML_TEMPLATE_STRING
VMML_TEMPLATE_CLASSNAME::tucker3_tensor( tensor3< J1, J2, J3, T >& core, matrix< I1, J1, T >& U1, matrix< I2, J2, T >& U2, matrix< I3, J3, T >& U3 )
: _core(core), _u1(U1), _u2(U2), _u3(U3)
{
	//assert(J1 <= I1);
	//assert(J2 <= I2);
	//assert(J3 <= I3);	
}

VMML_TEMPLATE_STRING
void 
VMML_TEMPLATE_CLASSNAME::reconstruction( tensor3< I1, I2, I3, T >& tensor_ ) const
{
	tensor_.full_tensor3_matrix_multiplication( _core, _u1, _u2, _u3 );
}


VMML_TEMPLATE_STRING
void 
VMML_TEMPLATE_CLASSNAME::decomposition( const tensor3< I1, I2, I3, T >& tensor_ )
{
	tucker_method1( tensor_ ); //or tuckals3 or hooi
	
	//do ALS iterations
	
	
	//derive core
	derive_core( tensor_, _core, _u1, _u2, _u3 );
}


VMML_TEMPLATE_STRING
void 
VMML_TEMPLATE_CLASSNAME::tucker_method1( const tensor3< I1, I2, I3, T >& tensor_ )
{
	//matricization along each mode (backward matricization after Lathauwer et al. 2000a)
	matrix< I1, I2*I3, T> m_lateral; // -> u1
	matrix< I2, I1*I3, T> m_frontal; // -> u2
	matrix< I3, I1*I2, T> m_horizontal; //-> u3
	tensor_.lateral_matricization( m_lateral);
	tensor_.frontal_matricization(  m_frontal);
	tensor_.horizontal_matricization( m_horizontal);
	
	//std::cout << "tensor input for tucker, method1: " << std::endl << tensor_ << std::endl;
	
	//2-mode PCA for each matricization A_n: (1) covariance matrix, (2) SVD
	//covariance matrix S_n for each matrizitation A_n
	matrix< I1, I1, T > s1;
	matrix< I2, I2, T > s2;
	matrix< I3, I3, T > s3;
	
	s1.multiply(m_lateral, transpose(m_lateral));
	s2.multiply(m_frontal, transpose(m_frontal));
	s3.multiply(m_horizontal, transpose(m_horizontal));
	
	/*std::cout << "covariance matrix s1: " << std::endl << s1 << std::endl 
	<< "covariance matrix s2: " << s2 << std::endl
	<< "covariance matrix s3: " << s3 << std::endl;*/
	
	//SVD for each covariance matrix
	//svdecompose( matrix< M, N, T >& a, vector< N, T >& w, matrix< N, N, T >& v) 
	//[V,D] = EIGS(A) returns a diagonal matrix D of A's 6 largest magnitude eigenvalues and a matrix V whose columns are the corresponding eigenvectors.
	
	matrix< I1, I1, double > u1;
	matrix< I2, I2, double > u2;
	matrix< I3, I3, double > u3;
	u1 = s1;
	u2 = s2;
	u3 = s3;
	
	vector< I1, double > lambdas_u1;
	vector< I2, double > lambdas_u2;
	vector< I3, double > lambdas_u3;
	
	bool ok = false;
	    
	lapack_svd< I1, I1, double > svd1;
	ok = svd1.compute_and_overwrite_input( u1, lambdas_u1 ); 
	//std::cout << "u1 singular vectors: " << std::endl << u1 << std::endl << "u1 lambdas: " << lambdas_u1 << std::endl;
	_u1 = u1;
	
	lapack_svd< I2, I2, double > svd2;
	ok = svd2.compute_and_overwrite_input( u2, lambdas_u2 ); 
	//std::cout << "u2 singular vectors: " << std::endl << u2 << std::endl << "u2 lambdas: " << lambdas_u2 << std::endl;
	_u2 = u2;
	
	lapack_svd< I3, I3, double > svd3;
	ok = svd3.compute_and_overwrite_input( u3, lambdas_u3 ); 
	//std::cout << "u3 singular vectors: " << std::endl << u3 << std::endl << "u3 lambdas: " << lambdas_u3 << std::endl;
	_u3 = u3;
	
}


VMML_TEMPLATE_STRING
void 
VMML_TEMPLATE_CLASSNAME::derive_core( const tensor3< I1, I2, I3, T >& data_, tensor3< J1, J2, J3, T >& core_, const matrix< I1, J1, T >& U1_, const matrix< I2, J2, T >& U2_, const matrix< I3, J3, T >& U3_ )
{
	for( size_t j3 = 0; j3 < J3; ++j3 ) 
	{
		for( size_t j1 = 0; j1 < J1; ++j1 ) 
		{
			for( size_t j2 = 0; j2 < J2; ++j2 ) 
			{
				double sum_i1_i2_i3 = 0.0;
				for( size_t i3 = 0; i3 < I3; ++i3 ) 
				{
					for( size_t i1 = 0; i1 < I1; ++i1 ) 
					{
						for( size_t i2 = 0; i2 < I2; ++i2 ) 
						{
							sum_i1_i2_i3 += U1_.at( i1, j1 ) * U2_.at( i2, j2 ) * U3_.at( i3, j3 ) * data_.at( i1, i2, i3 );
						}
					}
				}
				core_.at( j1, j2, j3 ) = sum_i1_i2_i3;
			}
		}
	}
}



VMML_TEMPLATE_STRING
template< size_t K1, size_t K2, size_t K3>
void 
VMML_TEMPLATE_CLASSNAME::rank_reduction( const tucker3_tensor< K1, K2, K3, I1, I2, I3, T>& other )
//TuckerJI.rank_recuction(TuckerKI) K1 -> J1, K2 -> J2, K3 -> J3; I1, I2, I3 stay the same
{
	assert(J1 <= K1);
	assert(J2 <= K2);
	assert(J3 <= K3);	
		
	//reduce basis matrices
	matrix< I1, K1, T> u1 = other.get_u1();
	for( size_t j1 = 0; j1 < J1; ++j1 ) 
	{
		_u1.set_column( j1, u1.get_column( j1 ));
	}
	
	matrix< I2, K2, T> u2 = other.get_u2();
	for( size_t j2 = 0; j2 < J2; ++j2) 
	{
		_u2.set_column( j2, u2.get_column( j2 ));
	}
	
	matrix< I3, K3, T> u3 = other.get_u3();
	for( size_t j3 = 0; j3 < J3; ++j3) 
	{
		_u3.set_column( j3, u3.get_column( j3 ));
	}
	
	//reduce core
	tensor3<K1, K2, K3, T> other_core;
	other_core = other.get_core();

	for( size_t j3 = 0; j3 < J3; ++j3 ) 
	{
		for( size_t j1 = 0; j1 < J1; ++j1 ) 
		{
			for( size_t j2 = 0; j2 < J2; ++j2 ) 
			{
				_core.at( j1, j2, j3 ) = other_core.at( j1, j2, j3 );
			}
		}
	}
}



VMML_TEMPLATE_STRING
template< size_t K1, size_t K2, size_t K3>
void 
VMML_TEMPLATE_CLASSNAME::subsampling( const tucker3_tensor< J1, J2, J3, K1, K2, K3, T>& other, const T& factor )
{
	assert(I1 <= K1);
	assert(I1 <= K2);
	assert(I1 <= K3);	
	
	//subsample basis matrices
	matrix< K1, J1, T> u1 = other.get_u1();
	for( size_t i1 = 0, i = 0; i1 < K1; i1 += factor, ++i ) 
	{
		_u1.set_row( i, u1.get_row( i1 ));
	}
	
	matrix< K2, J2, T> u2 = other.get_u2();
	for( size_t i2 = 0,  i = 0; i2 < K2; i2 += factor, ++i) 
	{
		_u2.set_row( i, u2.get_row( i2 ));
	}
	
	matrix< K3, J3, T> u3 = other.get_u3();
	for( size_t i3 = 0,  i = 0; i3 < K3; i3 += factor, ++i) 
	{
		_u3.set_row( i, u3.get_row( i3 ));
	}
	
	_core = other.get_core();
}



VMML_TEMPLATE_STRING
template< size_t K1, size_t K2, size_t K3>
void 
VMML_TEMPLATE_CLASSNAME::region_of_interest( const tucker3_tensor< J1, J2, J3, K1, K2, K3, T>& other, 
											const T& start_index1, const T& end_index1, 
											const T& start_index2, const T& end_index2, 
											const T& start_index3, const T& end_index3)
{
	assert(I1 <= K1);
	assert(I1 <= K2);
	assert(I1 <= K3);
	assert(start_index1 < end_index1);
	assert(start_index2 < end_index2);
	assert(start_index3 < end_index3);
	assert(end_index1 < K1);
	assert(end_index2 < K2);
	assert(end_index3 < K3);
	
	//region_of_interes of basis matrices
	matrix< K1, J1, T> u1 = other.get_u1();
	for( size_t i1 = start_index1,  i = 0; i1 < end_index1; ++i1, ++i ) 
	{
		_u1.set_row( i, u1.get_row( i1 ));
	}
	
	matrix< K2, J2, T> u2 = other.get_u2();
	for( size_t i2 = start_index2,  i = 0; i2 < end_index2; ++i2, ++i) 
	{
		_u2.set_row( i, u2.get_row( i2 ));
	}
	
	matrix< K3, J3, T> u3 = other.get_u3();
	for( size_t i3 = start_index3,  i = 0; i3 < end_index3; ++i3, ++i) 
	{
		_u3.set_row( i, u3.get_row( i3 ));
	}
	
	_core = other.get_core();
	
}



#undef VMML_TEMPLATE_STRING
#undef VMML_TEMPLATE_CLASSNAME
	
} // namespace vmml

#endif
