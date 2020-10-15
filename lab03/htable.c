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
* Function: init
*
* Description: Some simple initilizing of global variables.
*
*-----------------------------------------------------------------------------*/
/*void init(void){
    hist_table[HIST_TABLE_SIZE] = {0};
}
*/
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
            continue;
        }
        else{
            code = code_table[i];
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
* Description: 
*
* param:
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
    create_list();
    create_tree();
    collect_codes();
    print_output();
    
    return 0;
}
