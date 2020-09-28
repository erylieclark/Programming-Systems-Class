/*******************************************************************************
* Filename: uniq.c
*
* Description: This file is inteneded to take a file input, and remove any
*	lines that are a duplicate of the previous line. File size and line
*	length should not be limited.
*
*
* Author: Erin Rylie Clark
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *stdin;
FILE *stdout;

/*------------------------------------------------------------------------------
* Funtion name: safe_malloc
* This code was mostly taken from lecture_05 notes
*-----------------------------------------------------------------------------*/
/*void safe_malloc(size_t size){
    void *new;
    if ( !(new=malloc(size)) ){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return new;
} */

/*------------------------------------------------------------------------------
* Function name: read_long_line
*
* Description: This function will take in the file, and read the next line. It
* 	will allocate new space for the long line and then free the space when
*	it is done with the information.
*
* param a:
* param b:
*
* return:
*
*-----------------------------------------------------------------------------*/

char *read_long_line(FILE *file){
    
    /* Figure out how long this line is */
    int len = 0;
    char *mbuffer;
    while( getchar() != '\n' && getchar() != EOF ){ /* Determine length of line, stopped by '\n' character*/
        len++;
    }
    if ( getchar() == EOF ){
        return NULL;
    }
    else if ( getchar() == '\n' ){

        /* Allocate the necessary amount of space using malloc */
        mbuffer = malloc( len + 1 );
    
        /* Write the line into the buffer */
        mbuffer = fgets(mbuffer, len, file);
        /* Return the pointer to the buffer	*/
        return mbuffer;
    }
    else {
        exit(EXIT_FAILURE);
    }
    return mbuffer;
}

/*------------------------------------------------------------------------------
* Function name: print_line
*-----------------------------------------------------------------------------*/
void print_line(char* c){
    while( *c != '\0'){
        putchar(*c);
        c++;
    }
}

/*------------------------------------------------------------------------------
* Function: main
*-----------------------------------------------------------------------------*/

int main(){
    char *line_1, *line_2, *lnptr;
    int first_line, dup_line;
    first_line = 1;
    /* Call to read long line */
    
    while(1){
        
        if ( (lnptr = read_long_line(stdin)) == NULL ){
            break;
        }
        else if(first_line){
            line_1 = lnptr;
            print_line(line_1);
            first_line = 0;
        }
        else if(dup_line){
            line_2 = lnptr;
            dup_line = 0;
        }

        if ( strcmp(line_1, line_2) == 0 ){
            dup_line = 1;
            free(line_2);
        }
        else{
            dup_line = 0;
            free(line_1);
            print_line(line_2);
            line_1 = line_2;
        }
    }   

    /* Store the first pointer for first line */
    /* Print first line */
    
    /* If EOF, do nothing */
    /* Call RLL again */
    /* Store second pointer for second line */
    /* Compare lines */
    /* If second line is different, print second line, and free buffer for first line */
    /* If second line is same, do not print, free buffer for second line */



    return 0;
}

