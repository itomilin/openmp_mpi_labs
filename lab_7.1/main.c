////////////////////////////////////////////////////////////////////////////////
//for( i = 2, j = 1; j < NMAX; i *= 2, j++ )
//a[j] = i * f;
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>

#include </usr/lib/gcc/x86_64-linux-gnu/8/include/omp.h>

#define NMAX 20
double a[NMAX];

int main()
{
    int i, j = 1;
    double f = 2.0;
    a[0] = 1.0;

# ifdef _OPENMP
    printf( "Compiled by an OpenMP-compliant implementation.\n" );
# endif


#pragma omp parallel for
    for( i = 1; i < NMAX; i++ )
    {
#pragma omp critical
        j *= 2;

        a[i] = j * f;

//        printf("Current thread %i : i %i : a[i] %g \n",
//               omp_get_thread_num(),
//               i,
//               a[i]);
    }

    for( int i = 0; i < NMAX; ++i )
    {
        printf( "%g\n", a[i] );
    }

    return 0;
}

