/*******************************************************************************
* Filename: mytar.c
*
* Description: This program will take in the name of a tar file and additional
*   optional arguments to either unarchive, or archive the files specified 
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include "user_input.h"
#include "create_tar.h"
#include "create_header.h"
#include "read_write.h"
#include "read_header.h"
#include "read_tar.h"
/*------------------------------------------------------------------------------
* Function: print_output 
*
* Description: print_output does exactly that. It receives a pointer to the
*   linked list containing the now k words requested by the user.
*
* param: num_words - number of words requested by the user, or 10 by default
* param: top_words_pntr - pointer to a linked list containing the top words
*-----------------------------------------------------------------------------*/
void print_output(void){
}


/*------------------------------------------------------------------------------
* Function: main 
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[]){
    int options;
    FILE *fd;
    char **paths;

    /* First get the user input and see what they what */
    options = read_optional_input( argc, argv );

    /* Determine what needs to be done: extract, list, or create */
    switch( options & (CREATE | LIST | EXTRACT ) ){
        case CREATE:
            fd = open_file( argv[TAR_ARG], CREATE );
            /* NEED TO CHECK FOR RETURN VALUES!!!! */
            paths = get_paths( argc, argv, CREATE );
            create_tar( fd, paths );
            break;
        case LIST:
            fd = open_file( argv[TAR_ARG], LIST );
            paths = get_paths( argc, argv, LIST );
            read_tar( fd, paths, list_tar );
            break;
        case EXTRACT:
            fd = open_file( argv[TAR_ARG], EXTRACT );
            paths = get_paths( argc, argv, EXTRACT );
            break;
    }


    return 0;
}


