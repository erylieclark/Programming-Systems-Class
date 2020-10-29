

#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
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
int same_file( const char *src, const char *dst ){
    struct stat source = {0};
    struct stat dest = {0};
    /* Get information on both files */
    if( stat(src, &source) == -1 ){
        perror("stat");
        return 0;
    }
    if( stat(dst, &dest) == -1 ){
        perror("stat");
        return 0;
    }
    /* Compare the information */
    if( source.st_dev != dest.st_dev ){
        return 0; /* Not the same */
    }
    else if( source.st_ino != dest.st_ino ){
        return 0; /* Not the same */
    }
    else{
        return 1; /* They are the same */
    }
}

/*------------------------------------------------------------------------------
* Function: main
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    int result = same_file( argv[1], argv[2] );
    printf("Result: %d\n", result);
    return 0;
}









