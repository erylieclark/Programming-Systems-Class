#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*------------------------------------------------------------------------------
* Function name: read_long_line
*
* Description: This function will take in the file, and read the next line. It
*   will allocate new space for the line, write the line into that buffer,
*   return a pointer to the beginning of the buffer.
*
* param: FILE *file, a pointer called file to a FILE object given by *stdin
*
* return: char *, a pointer to a character. This will contain NULL if the EOF
*   has been detected, or will return a pointer to the first character of
*   the current line.
*-----------------------------------------------------------------------------*/
char *rll(FILE *file){
    int len = 0; /* Length of current line */
    static int file_pos = 0; /* # of bytes(or chars) that have been accounted
                                for in the file. Static in order to keep place
                               of next line in between function calls */
    char c, *mbuffer; /* c to store cur char, mbuffer to store pntr to line */
    fseek(file, file_pos, SEEK_SET); /* Adjust file pointer to the correct spot
                                        in the file from last call. If first
                                        call, no adjustment will take place. */

    while( (c = getc(file)) != '\n' && c != EOF ){ /* Determine length of the line, do not
                                        inc len or get next char if curr char
                                        is '\n' or EOF  */
        len++;              /* Increase length of line if not '\n' or EOF */
    }
    if ( c == EOF ){        /* If EOF, return a NULL pointer to main */
        return NULL;
    }
    else if ( c == '\n' ){
        len++;              /* Account for '\n' character at end of line */
        mbuffer = malloc(len);        /* Allocate a memory block long enough
                                        for the line */
        fseek(file, -len, SEEK_CUR);  /* Return file pntr to beginning of
                                            line in order to write line to
                                            mbuffer using fgets */
        mbuffer = fgets(mbuffer, len, file); /* Write line into mbuffer and
                                                return the pointer to mbuffer */
        file_pos += len;    /* Store the cur file pntr position in the static
                                variable to save for the next function call */
        return mbuffer;     /* Return pntr to line to main for comparison */
    }
    else {
        exit(EXIT_FAILURE);
    }
    return mbuffer;
}

int main(){

    char *lnptr; /* what is recieved back from rll */
    char **line_arr; /* an array of pointer to char pointers */
    int i = 0;
    int space = 10;

    /* Initialize an array for the pointer to lines */
    line_arr = malloc(space*sizeof(lnptr));
    if (line_arr == NULL){ /* Check for errors */
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    /* Assume that rll will throw an error if there is no input*/
    /* Read the file and store the lines */
    while( (lnptr = rll(stdin)) != NULL){
        /* Insert the pointer into the next spot in the table */
        line_arr[i] = lnptr;
        i++; 
        if ( i == space ){ /* Allocate more space if out of space */
            space += space;
            line_arr = (char **)realloc(line_arr, space*sizeof(lnptr));
            if (line_arr == NULL){ /* Check for errors */
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* Now print the lines backwards */
    while( i > 0 ){
        i--;
        puts( line_arr[i] );
    }
    free(line_arr);
    return EXIT_SUCCESS;
}

