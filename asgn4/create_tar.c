/*******************************************************************************
* File: create_tar.c
*
* Description:
*
* Author: Erin Rylie Clark
*******************************************************************************/

#include "create_tar.h"
#include "create_header.h"
#include "read_write.h"
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
int add_to_path( char path[], int loc, const char *d_name){
    /* Check if it will fit in the total first */
    if( (loc + strlen(d_name) + 1) >= HEADER_PATH_MAX ){ /* Add one for slash */
        printf("Path too long.\n");
        return 0; /* Failed */
    }
    else{
        path[loc] = '/'; /* First add a slash to the path before adding */
        loc++; /* Write in the next location */
        strcpy( &path[loc], d_name ); /* Now write the name */
    }
    return 1; /* Successful */
}
/*------------------------------------------------------------------------------
* Function:  
*
* Description: 
*
* param:  
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
        /* Failed to get the size correct, skip to the next file */
        return; /* REMINDER: THIS MAY NOT BE THE RIGHT THING TO DO */
    } /* num_blocks should be zero if it is a directory */

    /* Check if it is a directory */
    if( (file_st.st_mode & S_IFMT) == S_IFDIR ){
        /* If it is a dir, write the header to the output and then start 
            reading the files in it */
        /* Write the header to the tar file */
        write_buffer_out( fd );
        /* Open the given directory */
        if( (d_pntr = opendir( path )) == NULL ){
            perror(path); /* If can't open the directory, print an error */
            return; /* Return */
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
            if( (add_to_path( path, loc, dir_st -> d_name)) == 0 ){
                /* Name was not added to path, path too long */
                continue; /* If we proceeded with the rest of the loop, we would
                    recurse down further and the path would be even longer, so 
                    skip it even if it is a directory */
            }
            recurse_files_and_create( fd, path, \
                (loc + strlen(dir_st -> d_name) + 1));
        }
        closedir( d_pntr );
    }
    else{ /* If not a directory, open the file*/
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
        }
    }
}


/*------------------------------------------------------------------------------
* Function:  
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/

void create_tar( FILE *fd, char **paths ){
    int cur = 0;
    char path_buf[HEADER_PATH_MAX];
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







