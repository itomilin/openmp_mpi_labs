#include <iostream>

#include </usr/lib/gcc/x86_64-linux-gnu/8/include/omp.h>

#define N 6

int main( int argc, char** argv )
{
# ifdef _OPENMP
    printf( "Compiled by an OpenMP-compliant implementation.\n" );
    printf( "Num of threads: %i.\n", omp_get_max_threads() );
# endif

    int num_of_threads {omp_get_max_threads()};
    if ( argc > 1 )
        num_of_threads = std::stoi( argv[1] );

    int C[N][N] {};

    int A[N][N] =
    {
        { 1, 4, 3, 1, 6,  9  },
        { 2, 1, 5, 3, 5,  12 },
        { 3, 2, 1, 7, 11, 4  },
        { 1, 4, 3, 1, 6,  9  },
        { 4, 2, 7, 1, 6,  2  },
        { 5, 7, 8, 1, 3,  1  }
    };

    int B[N][N] =
    {
        { 1, 4, 3, 1, 6,  9  },
        { 2, 1, 5, 3, 5,  12 },
        { 3, 2, 1, 7, 11, 4  },
        { 5, 2, 1, 1, 6,  9  },
        { 4, 3, 2, 4, 11, 4  },
        { 2, 1, 5, 5, 5,  9  }
    };

#pragma omp parallel for num_threads( num_of_threads )
    for ( std::size_t i = 0; i < N; ++i )
    {
#ifdef DEBUG
        std::cout << "I thread: " << omp_get_thread_num() << std::endl;
#endif
        for ( std::size_t j = 0; j < N; ++j )
        {
//#ifdef DEBUG
//            std::cout << "J thread: " << omp_get_thread_num() << std::endl;
//#endif
            for ( std::size_t k = 0; k < N; ++k )
            {
//#ifdef DEBUG
//                std::cout << "C thread: " << omp_get_thread_num() << std::endl;
//#endif
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // Print the matrix.
    for ( std::size_t i = 0; i < N; ++i )
    {
        for ( std::size_t j = 0; j < N; ++j )
        {
            std::cout << C[i][j];
            std::cout << " ";
        }
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}

