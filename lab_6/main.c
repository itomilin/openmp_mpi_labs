#include <stdio.h>
#include <string.h>

#include </usr/lib/gcc/x86_64-linux-gnu/8/include/omp.h>

int main( int argc, char** argv )
{
    if ( omp_get_max_threads () < 4 )
        printf( "omp_get_max_threads < 4. Set maximum threads. \n" );
    else
        omp_set_num_threads( 4 );

    const char string_a[] = "te12st_Strzzstring";
    const char string_b[] = "test_Strzzcdfaq";

//    const char string_a[] = "test_Str";
//    const char string_b[] = "test_Str";

    int min_size = strlen( string_a ) < strlen( string_b ) ?
                strlen( string_a ) : strlen( string_b ) ;

    int mismatch = 0;
#pragma omp parallel for
    for ( int i = 0; i < min_size; ++i )
    {
        if ( string_a[i] != string_b[i] )
            ++mismatch;
        printf( "%c : %c : thread = %i\n", string_a[i], string_b[i],
                omp_get_thread_num() );
    }

    if ( mismatch != 0 )
        printf( "Strings are ne. Count of mismatch: %i\n", mismatch );
    else
        printf( "Strings are equal.\n" );

    return 0;
}

