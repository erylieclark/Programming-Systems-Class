
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------------------
* Function: 
*
* Description: 
*
* param:
*-----------------------------------------------------------------------------*/
int bstr2int( const char *s ){
    int num = 0;
    int length;
    /* First find the length of the string */
    length = strlen(s);
    /* Check if NULL */
    if( s == NULL ){
        printf("NULL given.\n");
        return 0;
    }
    else if( length > (sizeof(int)*8) ){ /* Check if too big */
        return -1;
    }
    /* Now filter out any 0s */
    while( *s == '0' ){
        s++; /* Get the next 0 or 1 */
        length--;
    }
    if( length < 1 ){ /* No more bits left */
        printf("All zeroes.\n");
        return 0; /* They were all zeros */
    }

    /* Get the remaining bits and shift them into an int */
    while( *s != '\0' ){
        
        if( *s == '0' ){
            num = num << 1; /* Shift a zero into the spot */
        }
        else if( *s == '1' ){
            num = num << 1; /* Get the zero into the spot */
            num = num | 0x01; /* Replace the zero with a one */
        }
        else{ /* Not a valid input */
            printf("Not a valid input. Use 0's and 1's only.\n");
            return -1;
        }
        s++;
    }
    return num;
}

/*------------------------------------------------------------------------------
* Function: main
*
* Description: main simply handles the file reading and then calls the 
*   remainder of the functions.
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    char *s = (char *)malloc(35);
    if( s == NULL ){
        perror("malloc");
        exit( EXIT_FAILURE );
    }

    s = fgets(s, 35, stdin);
    printf("Value: %d\n", bstr2int( s ));
    return 0;
}






