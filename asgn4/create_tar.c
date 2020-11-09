/*******************************************************************************
* File: create_tar.c
*
* Description: this file contains the functions that handle turning all of the
*   paths that the user wants archived into a tarfile
*
* Author: Erin Rylie Clark
*******************************************************************************/

/* Local Header Files */
#include "create_tar.h"
#include "create_header.h"
#include "read_write.h"
#include "user_input.h"
#include "read_header.h"

/*------------------------------------------------------------------------------
* Function: read_dir 
*
* Description: this function is essentially just readdir but adds the error
*   checking. This function was made to condense create_tar slightly.
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
*   can lstat it and investigate further in the calling function
*
* param: path - the path to add the name to
* param: loc - where in the path buffer to write the name
* param: d_name - the name to write into the buffer
* return: -1 on failure, 0 on success
*-----------------------------------------------------------------------------*/
int add_to_path( char path[], int loc, const char *d_name){
    /* Check if it will fit in the total first */
    if( (loc + strlen(d_name) + 1) >= MAX_PATH_LENGTH ){ /* Add one for slash */
        printf("Path too long.\n");
        return -1; /* Failed */
    }
    else{
        path[loc] = '/'; /* First add a slash to the path before adding */
        loc++; /* Write in the next location */
        strcpy( &path[loc], d_name ); /* Now write the name */
    }
    return 0; /* Successful */
}
/*------------------------------------------------------------------------------
* Function: recurse_files_and_create
*
* Description: This function takes in a path to a file or directory, stats it
*   calls the header function to write the header, then checks to see if the
*   file is a directory or a file. If it is a directory, the function is
*   called again to get the contents inside of the directory. If it is not
*   a directory, the file contents are transferred to the tarfile.
*
* param: fd - the tarfile to write to 
* param: path - the path of the file to look at
* param: loc - the location to add another directory to if necessary
*-----------------------------------------------------------------------------*/

void recurse_files_and_create( FILE *fd, char *path, int loc ){
    struct stat file_st = {0};
    struct dirent *dir_st;
    DIR *d_pntr;
    int num_blocks; 
    FILE *rdfd;
    
    /* Stat the entry */
    if( lstat( path, &file_st ) == -1 ){
        perror(path);
        return; /* If we don't have permission for this one, check
                    the next one */
    }

    /* Collect info on the entry and write it to the output buffer */
    if( (num_blocks = create_header( file_st , path )) == -1 ){
        /* There was an error getting all the info, try the next file */
        return;
    }

    /* Check if it is a directory */
    if( (file_st.st_mode & S_IFMT) == S_IFDIR ){
        /* If it is a dir, write the header to the output and then start 
            reading the files in it */
        /* Write the header to the tar file */
        write_buffer_out( fd );
        if( verb_list ){ /* If verbose option was selected, print the path */
            printf("%s\n", path);
        }
        /* Open the given directory */
        if( (d_pntr = opendir( path )) == NULL ){
            perror(path); /* If can't open the directory, print an error */
            return; /* Return, try the next file*/
        }
        /* Read each entry in the directory */
        while( (dir_st = read_dir(d_pntr, path)) != NULL ){ 
            /* Stop on EOS or error */
            /* Skip parent and self directories */
            if( strcmp(dir_st -> d_name, ".") == 0 ||\
                strcmp(dir_st -> d_name, "..") == 0 ){
                continue;
            }
            /* Add the name to the path */
            if( (add_to_path( path, loc, dir_st -> d_name)) == -1 ){
                /* Name was not added to path, path too long */
                continue; /* If we proceeded with the rest of the loop, we would
                    recurse down further and the path would be even longer, so 
                    skip it even if it is a directory */
            }
            /* Call the function again to assess the files in the directory */
            recurse_files_and_create( fd, path, \
                (loc + strlen(dir_st -> d_name) + 1));
        }
        closedir( d_pntr );
    }
    else{ /* If not a directory, open the file */
        rdfd = fopen( path, "r" ); /* Open for read only */
        if( rdfd == NULL ){ /* If failed to open */
            perror("fopen");
            return; /* Return before writing anything to output */
        }
        else{ /* If opened successfully, write the header to ouput, then write
                    the contents of the file */
            /* Write the header to the tar file */
            write_buffer_out( fd );
            /* Write the contents of the file to the tar file */
            write_file_contents( fd, rdfd, num_blocks );
            if( verb_list ){ /* If verbose option is selected, print path */
                printf("%s\n", path);
            }
        }
    }
}
/*------------------------------------------------------------------------------
* Function: create_tar 
*
* Description: this function mainly calls the sub functions to create the 
*   whole tar file. It cycles through all of the paths that the user gave.
*
* param: fd - a file stream pointer to the tarfile to write to
* param: paths - the paths that the user requested
*-----------------------------------------------------------------------------*/

void create_tar( FILE *fd, char **paths ){
    int cur = 0;
    char path_buf[MAX_PATH_LENGTH];
    int loc = 0; /* Location to write to into the buffer */
    while( paths[cur] != NULL ){
        loc = 0; /* Write each given path to the beginning of the buffer */
        strcpy( &path_buf[loc], paths[cur] ); /* Add the path to the buffer */
        loc = strlen( paths[cur] ); /* Set location to write after the path */
        recurse_files_and_create( fd, path_buf, loc );
        cur++;
    }
    /* Finished by writing two blocks of null bytes */
    memset( writebuf, '\0', BLOCK_SIZE );
    write_buffer_out( fd );
    write_buffer_out( fd );

}

