/*******************************************************************************
* htable.c
*
* Description: This program will take in a file or other input, produce a 
*   huffman tree, and output the codes of any character in the file.
*
* Author: Erin Rylie Clark
*
*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hist.h"
#include "huff_tree.h"

/*------------------------------------------------------------------------------
* Function: print_output
*
* Description: This function will take an array of pointers to strings and 
*   print out the hex value of each character in the file given by the user
*   along with its corresponding huffman code.
*
* param: array - the array that holds the pointers to characters
*
*-----------------------------------------------------------------------------*/

void print_output( void ){
    int i;
    char * code;
    for( i = 0 ; i < (HIST_TABLE_SIZE - 1) ; i++ ){
        if (code_table[i] == NULL){
            continue; /* Skip if the entry is empty */
        }
        else{
            code = code_table[i]; /* print the corresponding string */
            printf("0x%02X: %s\n", i , code);
        } 
    }
}


/*------------------------------------------------------------------------------
* Function: open_file
*
* Description: 
*
* param:
*-----------------------------------------------------------------------------*/
FILE *open_file(int cur, int argc, char **argv ){
    FILE *file;
    
    while( !(file = fopen(argv[cur],"r")) ){ /* Check for successful
                                                        opening */
        perror(argv[argc]); /* Find the next file that can be opened */
        argc++; /* Inc to check if there is a next argument */
        if ( (argc-cur) < 1){
            file = NULL; /* Last file in list could not be opene*/
            break;
        }
    }
    return file;
}

/*------------------------------------------------------------------------------
* Function: main
*
* Description: main simply handles the file reading and then calls the 
*   remainder of the functions.
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    int c, i = 1;
    FILE * file;
   /* init();*/ /* Initialize global variable */
    while( i < argc){
        file = open_file(i, argc, argv);

        if( file == NULL){
            break;
        }
        c = fgetc(file);
        while( !feof(file) ){
            c = (unsigned char) c;
            count_chars(c);
            c = fgetc(file);
        }
        if( fclose(file) ){
            perror("fclose");
        }
        i++;
    }
    create_list(); /* Initial linked list */
    create_tree(); /* Readjust list into tree */
    collect_codes(); /* Go through the tree and collect codes for each char */
    print_output();
    
    return 0;
}
