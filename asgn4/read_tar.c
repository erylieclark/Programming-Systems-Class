/*******************************************************************************
* File: read_tar.c
*
* Description:
*
* Author: Erin Rylie Clark
*******************************************************************************/
#include "read_tar.h"
#include "create_tar.h"
#include "create_header.h"
#include "read_write.h"
#include "user_input.h"
/*------------------------------------------------------------------------------
* Function: write_permissions 
*
* Description: this function mask the mode with each type of permission and 
*   prints out each type of permission that is enabled. Otherwise it prints -
*   This function is if verbose and list are requested together 
*
* param: mode - the mode from the verbose struct
*-----------------------------------------------------------------------------*/
void write_permissions( int mode ){
    char perms[] = {'r','w','x','r','w','x','r','w','x'};
    int mask[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP,\
        S_IROTH, S_IWOTH, S_IXOTH };
    int i = 0;

    while( i < NUM_PERM_BITS ){ /* Cycle through all the fields */
        if( mode & mask[i] ){ /* Mask the mode with each type of permission */
            printf("%c", perms[i]); /* Print the letter if enabled */
        }
        else{
            printf("-"); /* Print - if not enabled */
        }
        i++;
    }
    printf(" "); /* Print the space between the permissions and the user */
}
/*------------------------------------------------------------------------------
* Function: print_uname_gname 
*
* Description: this function simply takes the uname and gname fields from the
*   verbose struct and prints them out according to the formatting specified
*   in the assignment. This function is also only used if verbose and list are
*   requested together
*
* param: uname
* param: gname
*-----------------------------------------------------------------------------*/
void print_uname_gname( char *uname, char *gname ){
    int u_length, g_length, width;
    /* Get the lengths to see if they'll fit in the alotted space */
    u_length = strlen( uname );
    g_length = strlen( gname );

    if( (u_length + g_length) < LIST_USRGRP_WIDTH ){ /* check if they fit */
        printf("%s/", uname ); /* Print uname if they do */
        width = LIST_USRGRP_WIDTH - u_length;
        printf("%-*s ", width, gname); /* Print gname such that it fills out
            the remainder of the field */
    }
    else{ /* If they don't fit, just print the names out, no matter how much
                space they take up */
        printf("%s/%s ", uname, gname);
    }
}
/*------------------------------------------------------------------------------
* Function: print_time
*
* Description: this function takes the time in seconds and uses local time
*   to separate out the fields and print them. This function is only used when
*   the list and verbose options are requested together
*
* param: mtime - the mtime from the verbose struct
*-----------------------------------------------------------------------------*/
void print_time( time_t *mtime ){
    struct tm *t_st;
    t_st = localtime( mtime ); /* Get the struct with separated values */
    printf("%4d-%02d-%02d %02d:%02d ", (t_st -> tm_year) + OG_YEAR,\
        (t_st -> tm_mon)+1, t_st -> tm_mday, t_st -> tm_hour, t_st -> tm_min );
}
/*------------------------------------------------------------------------------
* Function: check_requested_paths 
*
* Description: this function is used to make sure that the path that we have
*   pulled from the tarfile falls under the paths that have been requested by
*   the user
*
* param: cur - the current path we are looking at
* param: req - the array of requested paths from the user 
* return: check - return 1 if the path is in it, 2 if the path is longer, 0
*   if the path does not match a requested one.
*-----------------------------------------------------------------------------*/
int check_requested_paths( char *cur, char **req ){
    int len, len_req, i = 0;
    int len_cur = strlen( cur ); /* Get the length of the current path */
    int check = 0;
    /* First check if there are any paths */
    if( *req == NULL ){
        check = 2; /* The current path is a valid one , because the user did
            not request any specific ones */
        return check; 
    }
    /* Now Cycle through the paths to see if the current path matches any of
        the requested paths. We want to compare only the least amount of chars
        in either path to tell if this file/dir is a part of the requested
        path or possibly an extension of it */
    while( req[i] != NULL ){
        len_req = strlen( req[i] );
        len = len_req > len_cur ? len_cur : len_req; /* get the shortest */
        if( strncmp( cur, req[i], len ) ){
            /* The strings do not match, check the next requested path */
            i++;
        }
        else{ /* The paths do match, so continue working with this file */
            if( len_cur >= len_req ){
                /* This is a part of the requested path */
                check++;
            }
            else{
                ; /* This is a parent of the path, do nothing */
            }
            check++;
            break;
        }
    }
    return check;
}
/*------------------------------------------------------------------------------
* Function: list_tar 
*
* Description: this is the function that handles the printing of the requested
*   paths from the tarfile. It also deals with the verbose option if that is
*   requested. This function fseeks past the contents of all of the files to
*   find each of the headers
*
* param: verbose - verbose struct to get all the values from
* param: fd - the file stream that we are reading from
* param: num_blocks - the number of blocks that the file takes up
* param: paths - the paths requested by the user
*-----------------------------------------------------------------------------*/
void list_tar( verbose_t *verbose, FILE *fd, int num_blocks, char **paths ){
    /* Compare the current path with the requested paths */
    if( check_requested_paths(verbose -> name, paths) < 2 ){
        /* No match, or the path is only a parent of a requested path,
            Skip to the next header based on num_blocks */
        if( (fseek( fd, BLOCK_SIZE*num_blocks , SEEK_CUR )) ){
            perror("fseek");
            exit( EXIT_FAILURE );
        }
        return; /* Go back without extracting this file or directory */
    }
    
    if( verb_list ){ /* If verbose is set */
        printf("%c", verbose -> type ); /* Print the type first */
        write_permissions( verbose -> mode );
        print_uname_gname( verbose -> uname, verbose -> gname );
        printf("%*d ", LIST_SIZE_WIDTH, verbose -> size);
        print_time( &(verbose -> mtime) );
    }
    /* Print the name whether or not verbose is selected */
    printf("%s\n", verbose -> name );

    /* Now skip to the next header based on num_blocks */
    if( (fseek( fd, BLOCK_SIZE*num_blocks, SEEK_CUR )) ){
        perror("fseek");
        exit( EXIT_FAILURE );
    } 
}
/*------------------------------------------------------------------------------
* Function: initialize_verbose_struct
*
* Description: set all of the verbose fields to null to help eliminate garbage
*   values
*
* param: verbose - the struct to read from
*-----------------------------------------------------------------------------*/
void initialize_verbose_struct( verbose_t *verbose ){
    memset( verbose -> name, '\0', MAX_PATH_LENGTH );
    verbose -> mode = 0;
    verbose -> uid = 0;
    verbose -> gid = 0;
    verbose -> size = 0;
    verbose -> mtime = 0;
    verbose -> type = 0;
    memset( verbose -> linkname, '\0', SYM_LINK_LENGTH );
    memset( verbose -> uname, '\0', UNAME_W );
    memset( verbose -> gname, '\0', GNAME_W );

}
/*------------------------------------------------------------------------------
* Function: restore_attributes
*
* Description: this function is intended to help set the permissions of the file
*   or directory and to restore the modified time.
*
* param: verbose - the struct to get the values from
*-----------------------------------------------------------------------------*/
int restore_attributes( verbose_t *verbose ){
    int x_mask = S_IXUSR | S_IXGRP | S_IXOTH;
    int rw_perms = 0666;
    int rwx_perms = 0777;
    struct stat statbuf;

    /* Start with permissions */
    if( x_mask & verbose -> mode ){ /* If any of the execute bits are set */
        chmod( verbose -> name, rwx_perms ); /* Offer read/write/execute */
    }
    else{ /* If no execute bits are set */
        chmod( verbose -> name, rw_perms ); /* Offer only read/write */
    }
    /* Now restore the modified time - THIS MIGHT CHANGE AFTER WRITING TO IT */
    if( (lstat( verbose -> name, &statbuf )) == -1 ){ /* Stat to access time */
        perror("lstat new file");
        return -1; /* CHECK RETURN VALUES WHEN THIS IS USED */
    }
    else{
        statbuf.st_mtime = verbose -> mtime;
    }
    return 0;
}
/*------------------------------------------------------------------------------
* Function: create_item_type 
*
* Description: this function is intended to create a file, directory, or
*   symlink depending on the type of thing we are looking at
*
* param: verbose - the struct containing the info on the item
* return: wrfd - a file stream pointing to the file to write to. This is null
*   unless it is a regular file.
*-----------------------------------------------------------------------------*/
FILE *create_item_type( verbose_t *verbose ){
    FILE *wrfd; /* Stream to write to */
    switch( verbose -> type ){
        case '-': /* A regular file */
            wrfd = fopen( verbose -> name, "w" ); /* Open file for writing */
            if( wrfd == NULL ){ /* Check return values */
                perror("fopen new file");
                exit( EXIT_FAILURE );
            }
            break;
        case 'l':
            if( symlink( verbose -> linkname, verbose -> name ) ){ 
                perror("symlink"); 
                exit( EXIT_FAILURE ); 
            }
            wrfd = NULL;
            break;
        case 'd': /* Create the directory */
            if (mkdir( verbose -> name, (verbose -> mode) &\
                (S_IXUSR | S_IXGRP | S_IXOTH )? 0777 : 0666 )){
                if( errno == EEXIST ){
                    wrfd = NULL; /* Don't create if it already exist, but no
                                    need to exit for this */
                }
                else{
                    perror("mkdir"); /* Exit on other errors though */
                    exit( EXIT_FAILURE );
                }
            }
            wrfd = NULL;
            break;
        default:
            wrfd = NULL; /* If it is somehow something else, return without
                            doing anything */
            break;
    }
    return wrfd;
}
/*------------------------------------------------------------------------------
* Function: extract_tar
*
* Description: this function is intended to extract the contents of the tarfile
*   based on the paths given by the user. If no paths were specified, all of 
*   the files are extracted 
*
* param: verbose - the struct to get all necessary file info from
* param: rdfd - the file stream pointed to the tarfile to read from
* param: num_block - the number of blocks that the file takes up
* param: paths - the paths requested by the user
*-----------------------------------------------------------------------------*/
void extract_tar( verbose_t *verbose, FILE *rdfd, int num_blocks,\
        char **paths ){
    FILE *wrfd;
    int check;

    /* Compare the current path with the requested paths */
    if( (check = check_requested_paths(verbose -> name, paths)) == 0 ){
        /* No match, Skip to the next header based on num_blocks */
        if( (fseek( rdfd, BLOCK_SIZE*num_blocks , SEEK_CUR )) ){
            perror("fseek");
            exit( EXIT_FAILURE );
        }
        return; /* Go back without extracting this file or directory */
    }
    else if( check == 1 ){ /* If its in the path, check if its a dir */
        if( !(verbose -> type == 'd') ){
            if( (fseek( rdfd, BLOCK_SIZE*num_blocks , SEEK_CUR )) ){
                perror("fseek");
                exit( EXIT_FAILURE );
            }
        return;
        }
    }
        
    /* otherwise, this is either a parent of a requested path, or it is part
        of a requested path, proceed with extracting this file */
    wrfd = create_item_type( verbose );
     
    /* Check if the item is a directory, file, or symlink */
    if( wrfd == NULL ){ /* Null if it is a directory or symlink */
        /* Just in case it is something else that has a size, seek to the next
            header */
        if( verb_list ){
            printf("%s\n", verbose -> name );
        }
        if( (fseek( rdfd, BLOCK_SIZE*num_blocks , SEEK_CUR )) ){
            perror("fseek");
            exit( EXIT_FAILURE );
        }
    }
    else{ /* If a file, write all of the contents from the tarfile */
        restore_file_contents( wrfd, rdfd, num_blocks, verbose -> size );
        if( verb_list && check == 2 ){ /* Only print if this is a part of the
                requested path, and not just a parent */
            printf("%s\n", verbose -> name );
        }
        fclose( wrfd ); /* Close the file when done */
    }
    /* RESTORE MTIME HERE????? */
}
/*------------------------------------------------------------------------------
* Function: read_tar 
*
* Description: this file reads the tarfile and uses the user input to send
*   each header of the tarfile to the correct function - either extract or list.
*   It also determines the end of the tarfile. 
*
* param: fd - the filestream to read from
* param: paths - paths requested by the user
* param: option - a pointer to either the extract_tar function or the list_tar
*   function
*-----------------------------------------------------------------------------*/

void read_tar( FILE *fd, char **paths, void(*option)(verbose_t *verbose,\
        FILE *fd, int num_blocks, char **paths) ){
    int null_blocks = 0;
    int num_blocks = 0;
    verbose_t *verbose;
    
    /* Malloc the verbose struct to store the info */
    verbose = (verbose_t *) malloc(sizeof(verbose_t));
    if( verbose == NULL ){
        perror("malloc verbose");
        exit( EXIT_FAILURE );
    }
    
    /* Start reading the tar file in blocks */
    while( null_blocks != 2 ){
        initialize_verbose_struct( verbose );
        read_into_buffer( fd );
        if( (num_blocks = read_header( verbose )) == -1 ){
            null_blocks++;
            continue;
        }
        else{ /* The header is valid and can be used */
            /* REMINDER: ISSUE HERE WHERE NULL BLOCKS COULD NOT BE SUCCESSIVE */
            null_blocks = 0; /* Null blocks must be together */
            option( verbose, fd, num_blocks, paths );
        }
    }
    free( verbose );
}

