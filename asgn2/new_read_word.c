#include "read_word.h"

char *read_word( FILE *file ){
    int i = 0; 
    int len = 0; /* Length of current line */
    char c; /* c to store cur char */
    char *mbuffer; /* mbuffer to store pntr to word */
    int num_bytes = 10;
    /* Allocate initial amount of space for the word */   
    mbuffer = (char *)malloc(num_bytes);
    if (mbuffer == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    while( ! isalpha(c = fgetc(file)) ){ /* Ignore any non letters */
        if ( c == EOF){ /* Check if non-letter is EOF and return if yes */ 
            return NULL;
        }
    }

    
    fseek( file, -1, SEEK_CUR ); /* Set it back one to account
      for fgetc call earlier */
    while( isalpha(c = fgetc(file)) ){
        mbuffer[len] = c; /* Place the character into mbuffer */ 
        len++; /* Increase length of line if not '\n' or EOF */
        if (len == num_bytes){
            num_bytes += num_bytes;
            mbuffer = (char *)realloc(mbuffer, num_bytes);
            if (mbuffer == NULL){
                perror("realloc");
            }
        }
    }
    mbuffer[len] = '\0'; /* Place end string character at end */

    /* Get rid of extra space */
    mbuffer = (char *)realloc(mbuffer, len);
    if (mbuffer == NULL){
        perror("realloc2");
    }


    fseek( file, -1, SEEK_CUR ); /* Set it back one to account to account for
                                    fgetc call earlier */

    for( i=0 ; i<len ; i++ ){ /* Format the word properly */
        if( isupper(mbuffer[i]) ){ /* Convert uppercase letters to lower case */
            mbuffer[i] = (char) tolower(mbuffer[i]); /* place back in buffer */
        }
    }
    return mbuffer;
}
