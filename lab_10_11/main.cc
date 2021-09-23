#include <iostream>
#include <algorithm>
#include <random>

#include <mpi.h>

// Имитация броска кубика.
uint16_t roll_dice()
{
    std::random_device rd;
    std::mt19937 generate( rd() );
    std::uniform_int_distribution<> dist( 1, 6 );

    return dist( generate );
}



int main( int argc, char** argv )
{
//    std::cout << roll_dice() << std::endl;
    int num_task {};
    int size     {};
    int master   {2};
    int buffer   {};

    // -------------------------------------------------------------------------

    MPI_Init( &argc, &argv );
    MPI_Status status;

    // -------------------------------------------------------------------------

    MPI_Comm_rank( MPI_COMM_WORLD, &num_task );
    MPI_Comm_size( MPI_COMM_WORLD, &size     );

    if ( num_task == master )
    {
        for ( std::size_t i = 0; i < master; ++i )
        {
            MPI_Recv( &buffer,
                      1,
                      MPI_INT,
                      i,
                      MPI_ANY_TAG,
                      MPI_COMM_WORLD,
                      &status );
            std::cout << "From: " << i
                      << "\nValue: " << buffer << std::endl;

        }
    }
    else
    {
        // Играем 5 партий.
        for ( std::size_t i = 0; i < 5; ++i )
        {
            buffer = roll_dice();
            MPI_Send( &buffer,
                      1,
                      MPI_INT,
                      master,
                      num_task,
                      MPI_COMM_WORLD );

            std::cout << "Send from slave: " << num_task
                      << " NUM: "  <<  buffer
                      << " PART: "  << i << std::endl;
        }
    }


    MPI_Finalize();

    // -------------------------------------------------------------------------

    std::cout << "END" << std::endl;
    return EXIT_SUCCESS;
}

