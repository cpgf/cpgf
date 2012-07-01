#ifndef __VMML__SINGULAR_VALUE_DECOMPOSITION__HPP__
#define __VMML__SINGULAR_VALUE_DECOMPOSITION__HPP__

#include <vmmlib/matrix.hpp>
#include <vmmlib/vector.hpp>

#include <cmath>

namespace vmml
{

/*
 * Given a matrix a[1..m][1..n], this routine computes its singular value
 * decomposition, A = U·W·V T. The matrix U replaces a on output. The diagonal
 * matrix of singular values W is output as a vector w[1..n]. The transpose V T 
 * (not the matrix V ! ) is output as v[1..n][1..n].
 */ 
//static double rv1[16]; 

template < size_t M, size_t N, typename T >
void svdecompose( 
    matrix< M, N, T >&    a, 
    vector< N, T >&       w, 
    matrix< N, N, T >&    v 
    )
{
    int m = M;
    int n = N;
    int flag, i, its, j, jj, k, l, nm;
    T anorm, c, f, g, h, s, scale, x, y, z;

    //T* rv1 = (T*)calloc( n, sizeof(T));   // vector(1,n);
    vector< N, T > rv1;

    g = scale = anorm = 0.0;					// Householder reduction to bidiagonal form.
    for ( i = 0; i < n; ++i ) 
    {
        l = i + 1;
        rv1[i] = scale * g;
        g = s = scale = 0.0;
        if ( i < m ) 
        {
            for ( k = i; k < m; ++k )
                scale += fabs(a[k][i]);
            if ( scale ) 
            {
                for ( k = i; k < m; ++k ) 
                {
                    a[k][i] /= scale;
                    s += a[k][i] * a[k][i];
                }
                f = a[i][i];
                g = -math::sign( static_cast< T >(sqrt(s)), f);
                h = f * g - s;
                a[i][i] = f - g;
                for ( j = l; j < n; ++j ) 
                {
                    for ( s = 0.0, k = i; k < m; ++k )
                        s += a[k][i] * a[k][j];
                    f = s / h;
                    for ( k = i; k < m; ++k )
                        a[k][j] += f * a[k][i];
                }
                for ( k = i; k < m; ++k )
                    a[k][i] *= scale;
            }
        }
        w[i] = scale * g;
        g = s = scale = 0.0;
        if ( i < m && i != n-1 ) 
        {
            for ( k = l; k < n; ++k )
                scale += fabs(a[i][k]);
            if ( scale ) 
            {
                for ( k = l; k < n; ++k ) 
                {
                    a[i][k] /= scale;
                    s += a[i][k] * a[i][k];
                }
                f = a[i][l];
                g = -math::sign( static_cast< T >( sqrt(s)), f);
                h = f * g - s;
                a[i][l] = f - g;
                for ( k = l; k < n; ++k )
                    rv1[k] = a[i][k] / h;
                for ( j = l; j < m; ++j ) 
                {
                    for ( s = 0.0, k = l; k < n; ++k )
                        s += a[j][k] * a[i][k];
                    for ( k = l; k < n; ++k )
                        a[j][k] += s * rv1[k];
                }
                for ( k = l; k < n; ++k )
                    a[i][k] *= scale;
            }
        }
        anorm = std::max( anorm, static_cast< T >( ( fabs( w[i] ) + fabs( rv1[i] ) ) ) );
    }
    for ( i = n-1; i >= 0; --i ) 
    { // Accumulation of right-hand transformations.
        if ( i < n ) 
        {
            if ( g ) 
            {
                for ( j = l; j < n; ++j )			// Double division to avoid possible underflow.
                    v[j][i] = (a[i][j] / a[i][l]) / g;
                for ( j = l; j < n; ++j ) 
                {
                    for ( s = 0.0, k = l; k < n; ++k )
                        s += a[i][k] * v[k][j];
                    for ( k = l; k < n; ++k )
                        v[k][j] += s * v[k][i];
                }
            }
            for ( j = l; j < n; ++j )
                v[i][j] = v[j][i] = 0.0;
        }
        v[i][i] = 1.0;
        g = rv1[i];
        l = i;
    }
    i = ( m < n ) ? m - 1 : n - 1; 
    for ( ; i >= 0; --i ) // IMIN 
    { // Accumulation of left-hand transformations.
        l = i + 1;
        g = w[i];
        for ( j = l; j < n; ++j )
            a[i][j] = 0.0;
        if ( g ) 
        {
            g = 1.0 / g;
            for ( j = l; j < n; ++j ) 
            {
                for ( s = 0.0, k = l; k < m; ++k )
                    s += a[k][i] * a[k][j];
                f = (s / a[i][i]) * g;
                for ( k = i; k < m; ++k )
                    a[k][j] += f * a[k][i];
            }
            for ( j = i; j < m; ++j )
                a[j][i] *= g;
        } 
        else
            for ( j = i; j < m; ++j )
                a[j][i] = 0.0;
        ++a[i][i];
    }
    for ( k = n-1; k >= 0; --k ) 
    { // Diagonalization of the bidiagonal form: Loop over singular values,
      // and over allowed iterations.
        for ( its = 0; its < 30; ++its ) 
        {
            flag = 1;
            for ( l = k; l >= 0; --l ) 
            { // Test for splitting.
                nm = l - 1; // Note that rv1[1] is always zero.
                if ( ( fabs( rv1[l] ) + anorm ) == anorm ) 
                {
                    flag = 0;
                    break;
                }
                if ( ( fabs( w[ nm ] ) + anorm ) == anorm )
                    break;
            }
            if ( flag ) 
            {
                c = 0.0; // Cancellation of rv1[l], if l > 1.
                s = 1.0;
                for ( i = l; i <= k; ++i ) 
                {
                    f = s * rv1[i];
                    rv1[i] = c * rv1[i];
                    if ( ( fabs(f) + anorm ) == anorm )
                        break;
                    g = w[i];
                    h = math::pythag(f, g);
                    w[i] = h;
                    h = 1.0 / h;
                    c = g * h;
                    s = -f * h;
                    for ( j = 0; j < m; ++j ) 
                    {
                        y = a[j][nm];
                        z = a[j][i];
                        a[j][nm] = y * c + z * s;
                        a[j][i] = z * c - y * s;
                    }
                }
            }
            z = w[k];
            if ( l == k ) 
            { // Convergence.
                if ( z < 0.0 ) 
                { // Singular value is made nonnegative.
                    w[k] = -z;
                    for ( j = 0; j < n; ++j )
                        v[j][k] = -v[j][k];
                }
                break;
            }
            if ( its == 30 )
            {    
                //fprintf(stderr, "Warning: no convergence in 30 svdcmp iterations\n");
                std::cerr << "SingularValueDecomposition - Warning: no convergence in 30 iterations." << std::endl;
            }
            x = w[l]; // Shift from bottom 2-by-2 minor.
            nm = k - 1;
            y = w[nm];
            g = rv1[nm];
            h = rv1[k];
            f = ( (y-z) * (y+z) + (g-h) * (g+h) ) / (2.0 * h * y );
            g = math::pythag( f, static_cast< T >( 1.0 ) );
            f = ( ( x - z ) * ( x + z ) + h * ( ( y / ( f + math::sign( g, f ) ) ) - h ) ) / x;
            c = s = 1.0;				  // Next QR transformation:
            
            for ( j = l; j <= nm; ++j ) 
            {
                i = j + 1;
                g = rv1[i];
                y = w[i];
                h = s * g;
                g = c * g;
                z = math::pythag( f, h );
                rv1[j] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = g * c - x * s;
                h = y * s;
                y *= c;
                for ( jj = 0; jj < n; ++jj ) 
                {
                    x = v[jj][j];
                    z = v[jj][i];
                    v[jj][j] = x * c + z * s;
                    v[jj][i] = z * c - x * s;
                }
                z = math::pythag( f, h );
                w[j] = z; // Rotation can be arbitrary if z = 0.
                if ( z ) 
                {
                    z = 1.0 / z;
                    c = f * z;
                    s = h * z;
                }
                f = c * g + s * y;
                x = c * y - s * g;
                for ( jj = 0; jj < m; ++jj ) 
                {
                    y = a[jj][j];
                    z = a[jj][i];
                    a[jj][j] = y * c + z * s;
                    a[jj][i] = z * c - y * s;
                }
            }
            rv1[l] = 0.0;
            rv1[k] = f;
            w[k] = x;
        }
		
    }
    //free(rv1);
	for ( i = 0; i < 3; ++i )
		for ( j = 0; j < 3; ++j )
			if ( i < j && w[i] < w[j] )
			{	
				double t = w[i];
				double u = w[j];
				w[i] = u;
				w[j] = t;
				
				for ( k = 0; k < 3; ++k )
				{
					t = v[i][k];
					u = v[j][k];
					v[i][k] = u * pow( -1, i );
					v[j][k] = t * pow( -1, j );
					t = a[k][i];
					u = a[k][j];
					a[k][i] = u * pow( -1, i );
					a[k][j] = t * pow( -1, j );
				}
			}
	
    }
			
}

#endif


