#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <cstring>

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
    // Количество партий.
    constexpr int num_games = 5;

    int thread   {};
    int size     {};
    int master   {0};

    int games[num_games] {};

    int length {};
    char* temp {};

    // -------------------------------------------------------------------------

    MPI_Init( &argc, &argv );
    MPI_Status status;
    MPI_File fh;

    // -------------------------------------------------------------------------

    // Контекст обмена группы.
    MPI_Comm_rank( MPI_COMM_WORLD, &thread );
    MPI_Comm_size( MPI_COMM_WORLD, &size   );

    // -------------------------------------------------------------------------

    if ( thread == master ) // Посредник.
    {
        // Массив для хранения результатов для каждого игрока.
        int results[2] {};

        // Суммируем очки за все партии для каждого игрока.
        for ( int i = 1; i <= 2; ++i )
        {
            MPI_Recv( games,
                      num_games,
                      MPI_INT,
                      i,
                      MPI_ANY_TAG,
                      MPI_COMM_WORLD,
                      &status );
            results[i - 1] = std::accumulate( games,
                                              games + num_games,
                                              results[i - 1] );
        }

        // Определяем победителя.
        const char* winner {};
        if ( results[0] == results[1] )
            winner = "draw";
        else if ( results[0] < results[1] )
            winner = "Player 2";
        else if ( results[0] > results[1] )
            winner = "Player 1";

        // Формируем строку с суммой очков и победителем.
        std::stringstream ss;
        ss << "Player 1 result = " << results[0] << "\n" <<
              "Player 2 result = " << results[1] << "\n" <<
              "Winner: " << winner;
        const std::string tmp = ss.str();
        length = ss.str().length();
        temp = (char *) malloc( (length + 1) * sizeof(char) );
        std::snprintf( temp, length + 1, "%s", tmp.c_str() );
//        std::strcpy(temp, tmp.c_str());
    }
    else
    {
        const char* f_name = thread == 1 ? "player1.txt" : "player2.txt";

        MPI_File_open( MPI_COMM_SELF,
                       f_name,
                       MPI_MODE_CREATE | MPI_MODE_WRONLY,
                       MPI_INFO_NULL,
                       &fh );

        // Бросание куба 5 раз.
        for ( uint16_t i = 0; i < num_games; ++i )
        {
            char buf[50];
            games[i] = roll_dice();
            snprintf( buf, 50, "Game %i: %i \n", i + 1, games[i] );
            MPI_File_write( fh,
                            buf,
                            strlen(buf),
                            MPI_CHAR,
                            &status );
        }

        MPI_Send( games,
                  num_games,
                  MPI_INT,
                  master,
                  thread,
                  MPI_COMM_WORLD );

        MPI_File_close( &fh );
    }

    // -------------------------------------------------------------------------

    // Коллективная функция, должна быть вызвана всеми процессами.
    MPI_Bcast( &length, 1, MPI_INT, master, MPI_COMM_WORLD );

    // ** VERY IMPORTANT
    // Аллоцируем буфер под другие процессы.
    if (thread != master )
        temp = (char *) malloc( (length + 1) * sizeof(char) );
    // ** VERY IMPORTANT

    MPI_Bcast( temp, length + 1, MPI_BYTE, master, MPI_COMM_WORLD );
    MPI_Barrier( MPI_COMM_WORLD );

    // Добавляем результат игры, полученный после рассылке через broadcast.
    if ( thread != master )
    {
        const char* f_name = thread == 1 ? "player1.txt" : "player2.txt";

        MPI_File_open( MPI_COMM_SELF,
                       f_name,
                       MPI_MODE_APPEND | MPI_MODE_WRONLY,
                       MPI_INFO_NULL,
                       &fh );

        char buf[length + 50];
        snprintf( buf, length + 50, "Result: \n%s\n", temp );

        MPI_File_write( fh,
                        buf,
                        strlen(buf),
                        MPI_CHAR,
                        &status );

        MPI_File_close( &fh );
    }

    // -------------------------------------------------------------------------

    MPI_Finalize();

    // -------------------------------------------------------------------------

    return EXIT_SUCCESS;
}

