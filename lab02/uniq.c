/*******************************************************************************
* Filename: uniq.c
* 
* Description: This file is inteneded to take a file input, and remove any
*	lines that are a duplicate of the previous line. File size and line
*	length should not be limited.*
* 
* Author: Erin Rylie Clark
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *stdin;
FILE *stdout;
/*------------------------------------------------------------------------------
* Function name: read_long_line
* 
* Description: This function will take in the file, and read the next line. It
* 	will allocate new space for the line, write the line into that buffer, and 
*   return a pointer to the beginning of the buffer.
* 
* param: FILE *file, a pointer called file to a FILE object given by *stdin
* 
* return: char *, a pointer to a character. This will contain NULL if the EOF
*   has been detected, or will return a pointer to the first character of 
*   the current line.
*-----------------------------------------------------------------------------*/
char *read_long_line(FILE *file){
    int len = 0; /* Length of current line */
    static int file_pos = 0; /* # of bytes(or chars) that have been accounted
                                for in the file. Static in order to keep place
                                of next line in between function calls */
    char c, *mbuffer; /* c to store cur char, mbuffer to store pntr to line */
    fseek(file, file_pos, SEEK_SET); /* Adjust file pointer to the correct spot
                                        in the file from last call. If first
                                        call, no adjustment will take place. */
    c = fgetc(file); /* Get the first character for the while loop to check */
    while( c != '\n' && c != EOF ){ /* Determine length of the line, do not
                                        inc len or get next char if curr char
                                        is '\n' or EOF  */
        len++;              /* Increase length of line if not '\n' or EOF */
        c = fgetc(file);    /* Get next character for while loop to check */
    }
    if ( c == EOF ){        /* If EOF, return a NULL pointer to main */
        return NULL;
    }
    else if ( c == '\n' ){
        len++;              /* Account for '\n' character at end of line */
        mbuffer = malloc( len );        /* Allocate a memory block long enough
                                            for the line */
        fseek(file, (-len), SEEK_CUR);  /* Return file pntr to beginning of
                                            line in order to write line to
                                            mbuffer using fgets */
        mbuffer = fgets(mbuffer, len, file); /* Write the line into mbuffer and
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
/*------------------------------------------------------------------------------
* Function: main
*-----------------------------------------------------------------------------*/
int main(){
    char *line_1, *line_2, *lnptr; /* 2 lines for compare, 1 storage pointer */
    int first_line = 1             /* Indicate if on first line */

    while(1){   /* Run this loop until reach EOF, indicated by NULL */
        
        if ( (lnptr = read_long_line(stdin)) == NULL ){
            break;      /* Retrieve the beginning of the next line in lnptr */
        }
        else if(first_line){    /* If this if first line in stdin file */
            line_1 = lnptr;     /* Store the pointer in line1 (free() later) */
            puts(line_1);       /* Print the line to stdout */
            first_line = 0;     /* First line has been taken care of */
            continue;           /* Skip comparison of line2 */
        }
        else{
            line_2 = lnptr;     /* If not first line in stdin file, always
                                    store new pointer to line2 */
        }

        if ( strcmp(line_1, line_2) == 0 ){ /* Compare line1 and line2 */
            free(line_2);   /* If they are the same, free the space
                                allocated to line2, and continue in next loop
                                to retrieve the next line into line_2*/
        }
        else{
            free(line_1);    /* If the lines are different, free line1 as
                                it is not needed for comparison anymore */
            puts(line_2);    /* Print 2nd line to stdout */
            line_1 = line_2; /* Line2 now stored in line1 for next comparison */
        }
    }   
    return 0;
}
