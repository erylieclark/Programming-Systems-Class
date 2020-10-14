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
    node_t *cur = head_pntr;
    while( (cur -> next) != NULL){
        printf("Hex code: 0x%02X, Decimal: %d, Count: %d\n", cur -> c ,cur -> c, cur -> count);
        cur = cur -> next;
    }
}

/*------------------------------------------------------------------------------
* Function: main
*
* Description: 
*
* param:
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    int c;
    /*node_t * head_pntr;*/

   /* init();*/ /* Initialize global variable */

    while( (c = fgetc(stdin)) ){
        if (!feof(stdin)){
            c = (unsigned char) c;
            count_chars(c);
        }
        else{
            perror("End of file, or file error");
            break;
        }
    }
    create_list();
    print_output();

    exit(EXIT_SUCCESS );

}
