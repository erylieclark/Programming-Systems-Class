/*****************************************************************************
* Filename: main.c
*
* Description: This program will take in a file and turn any tabs in the file
* into the appropriate number of spaces. It will additionally account for any
* special characters including newline, backspace, and return carriage.
*
* Author: Erin Rylie Clark
*
******************************************************************************/

#include <stdio.h>

#define TAB 8 /* A tab is defined as 8 spaces */

/*------------------------------------------------------------------------------
* Function: tab
*
* Description: This function takes in the column count and the number of tab
* sets, and outputs the correct number of spaces when the tab key is detected.
* It will return the resulting column count to the main function.
* 
* Param 1: tSet - how many sets of 8 spaces currently in tCount (starts at 0)
* Param 2: tCount - current column count
*
* return: tCount - updated column count after tab key is detected
*-----------------------------------------------------------------------------*/
int tab(int tSet, int tCount){

    int nSpc = TAB-(tCount - TAB*tSet);     /* Number of spaces to next tab
                                                stop */ 
    int i = 0;
    
        /* Output the appropriate number of spaces. Skip if none are needed. */
    while(i<nSpc){
        putchar(' ');
        i++;
    }
    
    tCount += nSpc;     /* New tCount is prev tCount + spaces added */
    return tCount;
}

/*------------------------------------------------------------------------------
* Function: main
*-----------------------------------------------------------------------------*/

void main(){

    int tCount = 0;     /* Variable for tracking the current column */
    int tSet = 0;       /* Sets of 8 in the current column count */
    int c;              /* Store the current character */
    while ((c = getchar()) != EOF){ /* Loop until end of file */
        switch(c){
            case '\n':
                tCount = 0;         /* Newline resets Count and Sets */
                tSet = tCount/TAB;
                break;
            case '\t':
                tCount = tab(tSet, tCount); /* Call tab function for '\t'*/
                tSet = tCount/TAB;  /* Recalc set after function call */
                continue;           /* tab function takes care of output, so
                                        skip to next loop without hitting
                                        putchar() at bottom */
            case '\b':
                if (tCount > 0)     /* Check if tCount will go negative */
                    tCount--;
                else
                    tCount = 0;     /* Keep tCount at 0 if already 0 */
                tSet = tCount/TAB;  /* Recalc tSet based on new tCount */
                break;
            case '\r':
                tCount = 0;         /* '\r' sets the column back to beginning
                                        of line, so reset tCount */
                tSet = 0;
                break;
            default:                /* Default will be any other characters*/
                tCount++;           /* Increment tCount and recalc tSet */
                tSet = tCount/TAB;
                break;
        }
        putchar(c); /* For all cases apart from tab, output the character */
    }
}
