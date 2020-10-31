/*******************************************************************************
* timeit.c
*
* Description: This function simply prints out Tick Tock in half second
*   increments for the specified amount of time from the user. It uses the UNIX
*   defined timers and alarms to indicate half seconds. 
*
* Author: Erin Rylie Clark
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>

#define NUM_INPUTS 2 /* What argc should be for correct usage */
#define SECONDS 1 /* The seconds input from the user */
#define STRING_MAX 15 /* The longest any of the strings printed will be */

/* These were for debugging when I thought one of them was causing an issue */
#define HANDLER
#define TIMER

/*------------------------------------------------------------------------------
* Function: check_input
*
* Description: This function checks the input that the user gave and makes sure
*   that the usage of the function is correct. If it is correct, the function
*   passes back the amount of time that the user requested. 
*-----------------------------------------------------------------------------*/
int check_input( int argc, char **argv ){
    int time, i;

    /* First check if there are too many or too few inputs */ 
    if ( argc > NUM_INPUTS || argc < NUM_INPUTS ){ 
        printf("usage: timeit <seconds> \n");
        exit( EXIT_FAILURE ); 
    }
    else{
        for(i = 0; i< strlen(argv[SECONDS]); i++ ){
            if( !isdigit(argv[SECONDS][i]) ){
                /* Check if the input is all digits */
                printf( "%s: malformed time\n", argv[SECONDS] );
                exit( EXIT_FAILURE ); 
            }
        }
    }
    
    time = atoi(argv[SECONDS]);
    return time;
}   
/*------------------------------------------------------------------------------
* Function: tt_handler
*
* Description: The handler figures out if the tick is odd or even by using the
*   modulus, then it writes the appropriate message to the write buffer and 
*   writes it to standard out. I could possibly have set buffers containing the
*   correct strings but I think I would have to define them in this function
*   every time anyway.
*-----------------------------------------------------------------------------*/
void tt_handler( int signum ){
    static int even_or_odd = 0;
    char wrbuf[STRING_MAX];
    int num_bytes;
    if( (++even_or_odd) % 2 ){
        strcpy( wrbuf, "Tick... \0" );
        num_bytes = 9;
    }
    else{
        strcpy( wrbuf, "Tock\n\0" );
        num_bytes = 6;
    }

    num_bytes = write( STDOUT_FILENO, wrbuf, num_bytes );
    if( num_bytes == -1 ){
        perror("Write to output.\n");
        exit( EXIT_FAILURE );
    }
}

/*------------------------------------------------------------------------------
* Function: main
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    int count, i;
    char wrbuf[STRING_MAX];
    int num_bytes;
    struct sigaction sa;
    struct itimerval itmr;
    count = check_input( argc, argv );
    
#ifdef HANDLER
    /* Set up the handler */
    sa.sa_handler = tt_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if( -1 == sigaction(SIGALRM, &sa, NULL) ){
        perror("sigaction");
        exit( EXIT_FAILURE );
    }
#endif
#ifdef TIMER
    /* Set up the timer */
        /* Interval is the time to count down from */
    itmr.it_interval.tv_sec = 0;
    itmr.it_interval.tv_usec = 500000;
        /* Value is the count that it is at */
    itmr.it_value.tv_sec = 0;
    itmr.it_value.tv_usec = 500000;
    if( -1 == setitimer(ITIMER_REAL, &itmr, NULL) ){
        perror("setitimer");
        exit( EXIT_FAILURE );
    }
#endif
    /* Catch the signal the right amount of times */
    for( i = 0; i < (count*2) ; i++ ){
        pause();
    }

    /* Print that time is up */
    strcpy( wrbuf, "Time\'s up!\n" );
    num_bytes = 11;
    num_bytes = write( STDOUT_FILENO, wrbuf, num_bytes );
    
    if( num_bytes == -1 ){
        perror("Write to output.\n");
        exit( EXIT_FAILURE );
    }
 
    return 0;
}
