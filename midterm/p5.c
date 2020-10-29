#define _DEFAULT_SOURCE

#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PATH_MAX
#define PATH_MAX 2048
#endif
/*------------------------------------------------------------------------------
* Function: read_dir 
*
* Description: this function is essentially just readdir but adds the error
*   checking. This function was made to condense main slightly.
*
* param: d_pntr - pointer to the directory to read from
* param: path - this is only for printing the path that could not be read
*-----------------------------------------------------------------------------*/
struct dirent *read_dir(DIR *d_pntr, const char *path){
    struct dirent *dir;

    errno = 0; /* Set errno to see if it has changed after readdir */
    if( (dir = readdir(d_pntr)) == NULL ){
        if( errno == 0 ){
            return NULL; /* End of stream, no errors */
        }
        else{ /* An error has occured and errno has changed */
            perror("path");
            return NULL;
        }
    }
    return dir; /* Return a pointer to the struct if it was read w/o error */
}
/*------------------------------------------------------------------------------
* Function: add_to_path
*
* Description: add the name of the file or directory to the path so that we
*   can lstat it and investigate further 
*-----------------------------------------------------------------------------*/
int add_to_path( char path[], int loc, const char *name){
    /* Check if it will fit in buffer first */
    if( (loc + strlen(name) + 1) >= PATH_MAX ){ /* Add one for slash */
        printf("Path too long.\n");
        return 0; /* Failed */
    }
    else{
        path[loc] = '/'; /* First add a slash to the path before adding */
        loc++; /* Write in the next location */
        strcpy( &path[loc], name ); /* Now write the name */
    }
    return 1; /* Successful */
}
/*------------------------------------------------------------------------------
* Function: print_dir
*-----------------------------------------------------------------------------*/
void print_dir( char *path_buf){
    printf("%s\n", path_buf);
}
/*------------------------------------------------------------------------------
* Function: my_rec_func 
*-----------------------------------------------------------------------------*/
int my_rec_func( char path_buf[], const char *name, int loc){
    int count = 0;
    struct stat file_st = {0};
    struct dirent *dir_st;
    DIR *d_pntr;
    /* Open the given directory */
    if( (d_pntr = opendir( path_buf )) == NULL ){
        perror(path_buf); /* If can't open the directory, print an error */
        return 0; /* Return a zero count, did not find matches */
    }
    /* Read each entry in the directory */
    while( (dir_st = read_dir(d_pntr, path_buf)) != NULL ){ /* EOS or error */
        /* Skip parent and self directories */
        if( strcmp(dir_st -> d_name, ".") == 0 ||\
            strcmp(dir_st -> d_name, "..") == 0 ){
            continue;
        }
        /* Add the name to the path */
        if( (add_to_path( path_buf, loc, dir_st -> d_name)) == 0 ){
            /* Name was not added to path, path too long */
            continue; /* If we proceeded with the rest of the loop, we would
                recurse down further and the path would be even longer, so 
                skip it even if it is a directory */
        }

        /* Check the name of the entry */
        if( strcmp(dir_st -> d_name, name) == 0 ){ /* A name match */
            count++; /* Add the match to the count */
            print_dir(path_buf);
        }
        
        /* Stat the entry */
        if( lstat( path_buf, &file_st ) == -1 ){
            perror(path_buf);
            continue; /* If we don't have permission for this one, check
                            the next one */
        }
        /* Check if it is a directory */
        if( (file_st.st_mode & S_IFMT) == S_IFDIR ){
            /* If it is a dir, call the function and add the returned count to
                the current count */
            count += my_rec_func( path_buf, name,\
                 (loc + strlen(dir_st -> d_name) + 1) ); /* Add 1 for slash */
        }
    }
    closedir(d_pntr); /* Close the directory when done */
    return count;        
}
/*------------------------------------------------------------------------------
* Function: 
*-----------------------------------------------------------------------------*/
int myfind( const char *path, const char *name ){
    int count = 0;
    char path_buf[PATH_MAX];
    int loc = 0; /* Location to write into the buffer */

    strcpy( &path_buf[loc], path ); /* Add the given path to the buffer */
    loc = strlen( path ); /* Set location to write after the given path */
    /* Call the recursive function to search the directory */
    count = my_rec_func( path_buf, name, loc );
    printf("Count: %d\n", count);
    return count;   
}
/*------------------------------------------------------------------------------
* Function: main
*-----------------------------------------------------------------------------*/

int main( int argc, char *argv[] ){
    myfind( argv[1], argv[2] );
    return 0;
}
