#include <stdio.h>

#define CHAR_VALUES 255


/*------------------------------------------------------------------------------
* Function: strspn
*-----------------------------------------------------------------------------*/

size_t strspn(const char *s, const char *accept){
    size_t count = 0;
    unsigned char char_table[CHAR_VALUES] = {'\0'}; 
        /* Create a lookup table, initialize values to NULL */
    /* Place the characters of accept into the table */
    while( *accept != '\0' ){ /* While accept is not end of string */
        /* Make sure we aren't trying to access values outside of the table */
        if (*accept > CHAR_VALUES || *accept < 0){
            return -1;
        }

        if ( char_table[(unsigned char)*accept] == '\0' ){ 
            /* If the character is not already in the table */
                char_table[(unsigned char)*accept] = *accept; 
                    /* Place it in the table */
        }
        else{
            /* Do nothing, the character is already in the table */
        }
        accept++; /* Go to the next character in the string */
    }

    /* Now go through s */
    while ( *s != '\0' ){ /* While s is not end of string */
        /* Make sure we aren't trying to access values outside of the table */
        if ( *s > CHAR_VALUES || *s < 0){
            return -1;
        }

        if ( char_table[(unsigned char)*s] != '\0' ){ 
            count++; /* If character from s is in the table, inc count */
        }
        else{
            break; /* If character is not in the table, exit loop */
        }
        s++;
    }
    return count;


}

/*------------------------------------------------------------------------------
* Function: main
*-----------------------------------------------------------------------------*/
int main(){
    int result;
    char *str1 = "ABC*$##?E2F14-GHI";
    char *str2 = "ABC--()*&#$D123";

    result = strspn(str1,str2);

    printf("Result: %d\n", result);
    
    return 0;
}
