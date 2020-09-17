#include <stdio.h>

#define TAB 8

int tab(int tSet, int tCount){
    int nSpc = TAB-(tCount % tSet);
    for (i=0;i<nSpc;i++)
        putchar(' ');
    tCount = tCount + nSpc;
    return tCount;
}

int back(int tCount){
    tCount--;
}

int main(){

    int tCount = 0; /* Variable for tracking the spaces */
    int tSet = 0; /* Variable for tracking how many sets of 8 have been used */

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
                // Some if else to count possible decrements
                break;
            case '\r': /* Do we care about return carriage? */
                break;
            default: /* Default will be any other characters*/
                putchar(c);
                tCount++;
                tSet = tCount/TAB;
                break;
        }
        putchar(); // Maybe
    }

    putchar(); // Maybe
}
