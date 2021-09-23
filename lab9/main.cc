#include <iostream>
#include <algorithm>
#include <ctime>

//#include <stddef.h>  /* подключаем макрос 'offsetof()' */

#include <mpi.h>

// Координаты с кладом.
#define N_X 10
#define N_Y 5

// Структура с координатами клада.
struct Coordinate final
{
    int x {};
    int y {};
} coordinate;

int main( int argc, char** argv )
{
    // -------------------------------------------------------------------------

    // Возможные координаты клада.
    Coordinate c1, c2, c3, c4, c5, c6;
    c1.x = 4;
    c1.y = 6;

    c2.x = 7;
    c2.y = 3;

    c3.x = 5;
    c3.y = 5;

    c4.x = 10;
    c4.y = 5;

    c5.x = 8;
    c5.y = 5;

    c6.x = 9;
    c6.y = 9;

    Coordinate coordinates[] = { c1, c2, c3, c4, c5, c6 };

    std::srand ( unsigned ( std::time(NULL) ) );
    std::random_shuffle( std::begin( coordinates ), std::end( coordinates ) );

    // -------------------------------------------------------------------------

    Coordinate buffer;
    int num_task {};
    int size     {};
    int master   {5};

    // -------------------------------------------------------------------------

    MPI_Init( &argc, &argv );
    MPI_Status status;

    // -------------------------------------------------------------------------

    // Создаем собственный тип.
    MPI_Datatype my_custom_datatype;

    // Длины полей структуры.
    int len[3] = { 1, 1, 1 };

    // Смещения полей структуры.
    MPI_Aint pos[3] =
    {
        offsetof( Coordinate, x ),
        offsetof( Coordinate, y ),
        sizeof( Coordinate )
    };

    // Описатель полей структуры.
    MPI_Datatype mpi_type[3] = { MPI_INT, MPI_INT, MPI_UB };

    // Создаем структуру.
    MPI_Type_struct( 2, /* Count of structure fields. */
                     len,
                     pos,
                     mpi_type,
                     &my_custom_datatype );

    //
    MPI_Type_commit( &my_custom_datatype );

    // -------------------------------------------------------------------------

    MPI_Comm_rank( MPI_COMM_WORLD, &num_task );
    MPI_Comm_size( MPI_COMM_WORLD, &size     );

    // Master == thread 5.
    if ( num_task == master )
    {
        for ( std::size_t i = 0; i < master; ++i )
        {
            MPI_Recv( &buffer,
                      1,
                      my_custom_datatype,
                      i,
                      MPI_ANY_TAG,
                      MPI_COMM_WORLD,
                      &status );

//            std::cout << "\n==Recieve from: " << i
//                      << "\n==Coordinate X: " << buffer.x
//                      << " Y: "               << buffer.y << std::endl;

            if ( buffer.x == N_X && buffer.y == N_Y )
            {
                std::cout << "==== WINNER ====" << std::endl;
                std::cout << "SLAVE: "  << i << std::endl;
                std::cout << "==== WINNER ====" << std::endl;
            }
        }
    }
    else
    {
        buffer = coordinates[num_task];
        MPI_Send( &buffer,
                  1,
                  my_custom_datatype,
                  master,
                  num_task,
                  MPI_COMM_WORLD );

        std::cout << "Send from slave: " << num_task
                  << "\nCoordinate X: "  <<  buffer.x
                  << " Y: "              <<  buffer.y << std::endl;
    }


    MPI_Finalize();

    // -------------------------------------------------------------------------

    return EXIT_SUCCESS;
}

