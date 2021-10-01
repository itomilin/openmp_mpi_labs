#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <cstring>
#include <cmath>

#include <mpi.h>

int main( int argc, char** argv )
{
    int thread {};
    int size   {};
    int master {0};

    // Начальное выравнивание для векторов.
    std::vector<int> xi  = { 1 };
    std::vector<int> xir = { 1 };
    auto constexpr A = 5;
    auto constexpr x = 2;
    double res {};
    auto constexpr MAX {5};

//    for ( std::size_t i = 1; i <= 5; ++i )
//    {
//        xir.emplace_back( std::pow( x, (i - 1) * i ) );
//        xi.emplace_back( std::pow( x, i ) );
//    }

//    for ( std::size_t i = 0; i < xi.size(); ++i )
//    {
//        res += A * xi[i] * xir[i];
//    }
//    std::cout << res << std::endl;
    // -------------------------------------------------------------------------

    MPI_Init( &argc, &argv );
    MPI_Status status;

    // -------------------------------------------------------------------------

    // Контекст обмена группы.
    MPI_Comm_rank( MPI_COMM_WORLD, &thread );
    MPI_Comm_size( MPI_COMM_WORLD, &size   );

    // -------------------------------------------------------------------------

    if ( thread == master )
    {
        xi.resize( MAX );
        MPI_Recv( &xi[0],
                  MAX,
                  MPI_INT,
                  MPI_ANY_SOURCE,
                  MPI_ANY_TAG,
                  MPI_COMM_WORLD,
                  &status );

        xir.resize( MAX );
        MPI_Recv( &xir[0],
                  MAX,
                  MPI_INT,
                  1,
                  MPI_ANY_TAG,
                  MPI_COMM_WORLD,
                  &status );

        // Умножаем параллельно полученные вектора.
#pragma omp parallel for
        for ( std::size_t i = 0; i < xi.size(); ++i )
        {
            res += A * xi[i] * xir[i];
        }

        std::cout << res << std::endl;
    }
    else if ( thread == 1 )
    {
        // Начинаем вычисление x^i и x^ir.
        for ( std::size_t i = 1; i < MAX; ++i )
        {
            xir.emplace_back( std::pow( x, (i - 1) * i ) );
            xi.emplace_back( std::pow( x, i ) );
        }

        MPI_Send( &xi[0],
                  MAX,
                  MPI_INT,
                  master,
                  thread,
                  MPI_COMM_WORLD );

        MPI_Send( &xir[0],
                  MAX,
                  MPI_INT,
                  master,
                  thread,
                  MPI_COMM_WORLD );
    }

    // -------------------------------------------------------------------------

    MPI_Finalize();

    // -------------------------------------------------------------------------

    return EXIT_SUCCESS;
}

