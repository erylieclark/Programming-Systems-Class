/*******************************************************************************
* File: read_word.c
*
* Description: This file only contains the read_word function and returns a
*  pointer to a buffer containing that word.
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#include "read_word.h"

/*------------------------------------------------------------------------------
* Function: read_word
*
* Description: This function will separate out a word from the file and return
*   it to main. It will skip past any non alphabetic characters and get a
*   string of letters until it hits the next non alphabetic character, then
*   return the word. The function also keeps track of the length of the word
*   and allocates space in memory to store it. The function will return a
*   pointer to the first character of the word.
*
* param: file - file to get the word from
*
*-----------------------------------------------------------------------------*/
char *read_word( FILE *file ){
    static int file_pos = 0;
    int i, len = 0; /* Length of current line */
    char c, *mbuffer; /* c to store cur char, mbuffer to store pntr to line */
    fseek( file, file_pos, SEEK_SET ); /* Adjust file ind to the correct spot
                                        in the file from last call. If first
                                        call, no adjustment will take place. */
    while( ! isalpha(c = fgetc(file)) ){ /* Ignore any non letters */
        if ( c == EOF){ /* Check if non-letter is EOF and return if yes */
            file_pos = 0; /* Reset for next file */
            return NULL;
        }
        else{    
            len++;  /* Keep track of file indicator */
        }
    }
    
    file_pos += len; /* add len of non letters before resetting for a word */
    len = 1; /* Last check of while loop was a letter */
    
    while( isalpha(c = fgetc(file)) ){
        len++;      /* Increase length of line if not '\n' or EOF */
    }
    /* Len is the length of the word at this point */
    len++; /* Inc len to account for last check of while loop - fseek needs to
                know how many spaces to move back from current place, and 
                malloc needs to account for the '\0' that fgets will add to 
                the end of the word */
    mbuffer = malloc(len); /* Allocate enough memory for the word */
    fseek(file, -len, SEEK_CUR); /* Return file indicator to start of word */
    mbuffer = fgets(mbuffer, len, file); /* Copy word into mbuffer */
    file_pos += len; /* Store file indicator pos to set on next call */
    
    for( i=0 ; i<len ; i++ ){ /* Format the word properly */
        if( isupper(mbuffer[i]) ){ /* Convert uppercase letters to lower case */
            mbuffer[i] = (char) tolower(mbuffer[i]); /* place back in buffer */
        }
    }
    return mbuffer;
}
