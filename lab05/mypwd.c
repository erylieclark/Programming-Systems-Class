/*******************************************************************************
* mypwd.c
*
* Description: This function will print the current working directory.
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

extern int dev_id;

/*------------------------------------------------------------------------------
* Function: print_dir
*
* Description: 
*
* param:
*-----------------------------------------------------------------------------*/
void print_dir( char *pathname ){
    
}

/*------------------------------------------------------------------------------
* Function: add_backpath_level
*
* Description: 
*
* param:
*-----------------------------------------------------------------------------*/
/*char *add_backpath_level( char backpath[], int loc){
    backpath[loc] = "../";
    return backpath;
}*/
/*------------------------------------------------------------------------------
* Function: add_dir_to_backpath
*
* Description: 
*
* param:
*-----------------------------------------------------------------------------*/
void add_to_backpath( char backpath[], int loc, const char *dir_name){
    strcpy( &backpath[loc], dir_name );
}
/*------------------------------------------------------------------------------
* Function: check_path_length 
*
* Description: 
*
* param:
*-----------------------------------------------------------------------------*/
void check_path_length(){

}
/*------------------------------------------------------------------------------
* Function: read_dir 
*
* Description: 
*
* param:
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
    return dir;
}

/*------------------------------------------------------------------------------
* Function: main
*
* Description: backpath is used so that we do not need to change directories
*   and so that we do not need to leave many directories open at a time.
*-----------------------------------------------------------------------------*/

int main( void ){

    /* Initialize Variables */
    struct stat fs = {0};
    struct dirent *dir;
    DIR *d_pntr;
    char path[PATH_MAX]; /* Store the whole path */
    char *pathnames[PATH_MAX]; /* Store individual strings of each directory */
    char backpath[PATH_MAX*2]; /* A string to step a number of dir levels up */
    int dev_id; /* Device ID */
    int my_id; /* My Current ID */
    int test_id; /* Testing the ID in my parent directory to cmp to myself */
    int loc = 0; /* Place in backpath to write to */
    int path_length = 0; /* Keep track of how long the name will be */

    /* First find out who I am, and get my own info */
    if( lstat(".", &fs ) == -1 ){
        perror("lstat1");
        exit( EXIT_FAILURE );
    }
    dev_id = fs.st_dev;
    printf("Device ID: %d\n", dev_id);
    my_id = fs.st_ino;
    printf("My ID: %d\n", my_id);
    
    /* Get string for parent directory */    
    add_to_backpath( backpath, loc, "../");
    loc += PARENT_DIR_STR;
    
    /* Loop until we find root */
    while( my_id != fs.st_ino ){

        /* Open my parent directory */
        if( (d_pntr = opendir( backpath )) == NULL ){
            perror("opendir");
            exit( EXIT_FAILURE );
        }
        while( my_id != test_id && fs.st_dev == dev_id){ 
                /* Read all the entries in this directory */
            /* Get the next entry in the directory */
            if( (dir = read_dir(d_pntr)) == NULL ){
                break; /* End of stream */
            }
            /* Skip parent and self directories */
            if( strcmp(dir -> d_name, ".") == 0 || strcmp(dir -> d_name, "..") == 0 ){
                continue;
            }

            printf( "Directory Name: %s\n", dir -> d_name );            
            add_to_backpath( backpath, loc, dir -> d_name);
                /* Add the read directory to backpath and check its info */
                /* Do not move loc so that we can write over this dir later */
            if( lstat( backpath, &fs ) == -1 ){
                perror("lstat2");
                continue; /* If we don't have permission for this file, check
                            the next one */
            }
            test_id = fs.st_ino;
            printf("Test ID: %d\n", test_id);
            printf("MyDev ID: %d\n", dev_id);
            printf("Dev ID: %ld\n", fs.st_dev);
        }
        closedir(d_pntr);
        printf("Test ID: %d\n", test_id);
        
        /* Get my new information */ 
        add_to_backpath( backpath, loc, ".");
        if( lstat( backpath, &fs ) == -1 ){
            perror("lstat1");
            exit( EXIT_FAILURE );
        }
        my_id = fs.st_ino;
        printf("My ID: %d\n", my_id);

        /* Set the new path to my parent */
        add_to_backpath( backpath, loc, "../." );
        loc += PARENT_DIR_STR; /* Set to write after last "../" now */

        /* Check that my parent isn't also me */
        if( lstat( backpath, &fs ) == -1 ){
            perror("lstat1");
            exit( EXIT_FAILURE );
        }
        printf("Dev ID: %ld\n", fs.st_dev);
        add_to_backpath( backpath, loc, "\0" );
    }    
    return 0;
}




