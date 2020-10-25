/*******************************************************************************
* mypwd.c
*
* Description: This function is intended to print the current working directory
*   by traversing the filesystem. It looks to its parent directory and compares
*   i nodes until it knows where it came from, then goes to the parent's parent
*   directory and repeats. This program also checks device numbers.
*
* Author: Erin Rylie Clark
*******************************************************************************/
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 2048
#endif

#define PARENT_DIR_STR 3 /* Length of "../" to get next parent directory */
#define ROOT_INODE 2 /* Root will always have an inode of 2 */


/*------------------------------------------------------------------------------
* Function: print_dir
*
* Description: print_dir takes in the array of character pointers to each of the
*   path names that were identified as being in the path as well as the count
*   of path names and prints a '/' followed by the path name. The array is in
*   reverse order so print_dir starts at the end and works back towards the
*   beginning. 
*
* param: loc - the number of names in the path
* param: pathnames - an array of character pointers pointing to each dir name
*-----------------------------------------------------------------------------*/
void print_dir( int loc , char *pathnames[]){
    for( loc = (loc-1) ; loc >= 0 ; loc-- ){
        printf("/");
        printf("%s", pathnames[loc] );
    }
    printf("\n");
}

/*------------------------------------------------------------------------------
* Function: add_dir_to_backpath
*
* Description: this function adds the specified string in dir_name to the end
*   of the path. The back path consists mostly of "../../"s because this is the
*   way to identifiy the parent of each directory without having to change the
*   directory.
*
* param: backpath - the array containing the path to step up the path
* param: loc - the location to print the string in the backpath array
* param: dir_name - the string to be added to backpath
*-----------------------------------------------------------------------------*/
void add_to_backpath( char backpath[], int loc, const char *dir_name){
    strcpy( &backpath[loc], dir_name );
}

/*------------------------------------------------------------------------------
* Function: read_dir 
*
* Description: this function is essentially just readdir but adds the error
*   checking. This function was made to condense main slightly.
*
* param: d_pntr - pointer to the directory to read from
*-----------------------------------------------------------------------------*/
struct dirent *read_dir(DIR *d_pntr){
    struct dirent *dir;

    errno = 0; /* Set errno to see if it has changed after readdir */
    if( (dir = readdir(d_pntr)) == NULL ){
        if( errno == 0 ){
            return NULL; /* End of stream, no errors */
        }
        else{ /* An error has occured and errno has changed */
            perror("mypwd");
            exit( EXIT_FAILURE );
        }
    }
    return dir; /* Return a pointer to the struct if it was read w/o error */
}

/*------------------------------------------------------------------------------
* Function: check_path_length 
*
* Description: check the potential length of the path to be sure it does not
*   exceed PATH_MAX, exit if it does exceed
*
* param: add_length - the length of the directory name to be added to the path,
*   not including the '/' that will come before it
*-----------------------------------------------------------------------------*/
void check_path_length(int add_length){
    static int path_length = 0; /* so do not have to pass back and forth */
    path_length += (add_length + 1); /* One for the '/' before each dir name */
    if( path_length > PATH_MAX ){
        printf("Path too long.\n");
        exit( EXIT_FAILURE );
    }
}
/*------------------------------------------------------------------------------
* Function: add_dir_to_list
*
* Description: add the identified directory to the pathnames array for later
*   printing
*
* param: dir_name - the string to be added
* param: pathnames - array of character pointers that dir_name will be added to
*-----------------------------------------------------------------------------*/
int add_dir_to_list( const char *dir_name, char *pathnames[]){
    static int loc = 0; /* Location in pathnames to add the string */
    int length; /* Length of the string without the '/' or '\0' */
    length = strlen(dir_name);
    check_path_length( length );
    pathnames[loc] = (char *) malloc(length + 1); /* One for null byte */
    if( !pathnames[loc] ){ /* Check return value of malloc */
        perror("mypwd");
        exit( EXIT_FAILURE );
    }

    strcpy( pathnames[loc], dir_name ); /* Copy the name into the array */
    loc++; /* Write the next directory name to the following spot */
    return loc;
}
/*------------------------------------------------------------------------------
* Function: main
*
* Description: the basic steps of this function are:
*   1. Get the information of the directory I am currently in.
*   2. Open the parent directory and read all of the directories within it until
*       the one I am currently in is found.
*   3. Write a path to the parent directory and repeat the first 3 steps.
*   4. Check if the parent directory is the same as the child directory and quit
*       if yes, this means we found root.
*   5. print out the pathname
*-----------------------------------------------------------------------------*/

int main( void ){

    /* Initialize Variables */
    struct stat fs = {0};
    struct dirent *dir;
    DIR *d_pntr;
    char *pathnames[PATH_MAX];
    char backpath[PATH_MAX*2]; /* A string to step a number of dir levels up */
    int dev_id; /* Device ID */
    int my_id; /* My Current ID */
    int test_id = 0; /* Testing the ID in my parent directory to cmp to myself */
    int loc = 0; /* Place in backpath to write to */
    int num_dirs = 0;

    /* First find out who I am, and get my own info */
    if( lstat(".", &fs ) == -1 ){
        perror("mypwd");
        exit( EXIT_FAILURE );
    }
    dev_id = fs.st_dev;
    my_id = fs.st_ino;
    
    /* Get back path string for parent directory */    
    add_to_backpath( backpath, loc, "../");
    loc += PARENT_DIR_STR;
    
    /* Loop until we find root */
    while( 1 ){

        /* Open my parent directory */
        if( (d_pntr = opendir( backpath )) == NULL ){
            perror("mypwd");
            exit( EXIT_FAILURE );
        }
        /* Loop until both my_id and dev_id match each other.
            Read all of the entries in the directory */
        while( my_id != test_id || fs.st_dev != dev_id){ 
            /* Get the next entry in the directory */
            if( (dir = read_dir(d_pntr)) == NULL ){
                break; /* End of stream */
            }
            /* Skip parent and self directories */
            if( strcmp(dir -> d_name, ".") == 0 ||\
                strcmp(dir -> d_name, "..") == 0 ){
                continue;
            }
            /* Add the read directory to backpath and check its info */
            /* Do not move loc so that we can write over this dir later */
            add_to_backpath( backpath, loc, dir -> d_name);
            /* Get the information on the read directory */
            if( lstat( backpath, &fs ) == -1 ){
                perror("mypwd");
                continue; /* If we don't have permission for this one, check
                            the next one */
            }
            test_id = fs.st_ino; /* Set for next loop */
        }
        closedir(d_pntr);
        
        /* Exited loop because reached end of stream without finding myself */
        if( dir == NULL ){ 
            printf("Cannot get current directory.\n");
            exit( EXIT_FAILURE );
        }
        else{ /* Otherwise, I found myself */
            num_dirs = add_dir_to_list( dir -> d_name, pathnames );
        }

        /* Now I am my parent directory, Get my new information */ 
        add_to_backpath( backpath, loc, ".");
        if( lstat( backpath, &fs ) == -1 ){
            perror("mypwd");
            exit( EXIT_FAILURE );
        }
        my_id = fs.st_ino;
        dev_id = fs.st_dev;

        /* Set a path to my parent */
        add_to_backpath( backpath, loc, "../." );
        loc += PARENT_DIR_STR; /* Set to write after last "../" now */

        /* Check that my parent isn't also me */
        if( lstat( backpath, &fs ) == -1 ){
            perror("mypwd");
            exit( EXIT_FAILURE );
        }
        test_id = fs.st_ino;
        if( my_id == test_id && dev_id == fs.st_dev ){ /* Parent is me */
            break; /* Leave */
        }
        /* Parent is not me, terminate the ../ for the next opendir */
        add_to_backpath( backpath, loc, "\0" );
    }
    print_dir( num_dirs, pathnames );
    return 0;
}
