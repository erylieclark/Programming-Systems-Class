/*****************************************************************************
* Filename: main.c
*
* Description: This program will take in a file and turn any tabs in the file
* into the appropriate number of spaces.
*
* Author: Erin Rylie Clark
*
* Revisions:
*
******************************************************************************/

#include <stdio.h>

#define TAB 8

/*------------------------------------------------------------------------------
* Function: tab
*
* Description:
*
*
* Param 1:
* Param 2:
*
* return: 
*-----------------------------------------------------------------------------*/
int tab(int tSet, int tCount){
    int nSpc = TAB-(tCount % tSet);
    int i;
    for (i=0;i<nSpc;i++)
        putchar(' ');
    tCount = tCount + nSpc;
    return tCount;
}

void back(int tCount){
    tCount--;
}

void main(){

    int tCount = 0; /* Variable for tracking the spaces */
    int tSet = 0; /* Variable for tracking how many sets of 8 have been used */
    int lCount = 0; /* Count the number of lines in the file */
    int c;
    while ((c = getchar()) != EOF){
        switch(c){
            case '\n':
                tCount = 0; /* Reset tCount and tSet with a new line */
                tSet = 0;
                break;
            case '\t':
                tCount = tab(tSet, tCount);
                tSet = tCount/TAB;
                break;
            case '\b':
                tCount--;
                tSet = tCount/TAB;
                /* Some if else to count possible decrements */
                break;
            case '\r': /* Do we care about return carriage? */
                break;
            default: /* Default will be any other characters*/
                putchar(c);
                tCount++;
                tSet = tCount/TAB;
                break;
        }
    }
}
