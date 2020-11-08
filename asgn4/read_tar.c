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
/*------------------------------------------------------------------------------
* Function: write_permissions 
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/
void write_permissions( int mode ){
    char perms[] = {'r','w','x','r','w','x','r','w','x'};
    int mask[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP,\
        S_IROTH, S_IWOTH, S_IXOTH };
    int i = 0;

    while( i < NUM_PERM_BITS ){
        if( mode & mask[i] ){
            printf("%c", perms[i]);
        }
        i++;
    }
    printf(" "); /* Print the space between the permissions and the user */
}
/*------------------------------------------------------------------------------
* Function: print_uname_gname 
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/
void print_uname_gname( char *uname, char *gname ){
    int u_length, g_length, width;
    u_length = strlen( uname );
    g_length = strlen( gname );

    if( (u_length + g_length) < LIST_USRGRP_WIDTH ){
        printf("%s/", uname );
        width = LIST_USRGRP_WIDTH - u_length;
        printf("%-*s ", width, gname);
    }
    else{
        printf("%s/%s ", uname, gname);
    }
}
/*------------------------------------------------------------------------------
* Function: print_time
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/
void print_time( time_t *mtime ){
    struct tm *t_st;
    t_st = localtime( mtime );
    printf("%4d-%02d-%02d %02d:%02d ", (t_st -> tm_year) + OG_YEAR,\
        t_st -> tm_mon, t_st -> tm_mday, t_st -> tm_hour, t_st -> tm_min );
}
/*------------------------------------------------------------------------------
* Function: list_tar 
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/
void list_tar( verbose_t *verbose ){
    printf("Made it into list function.\n");
    if( verbose ){
        printf("%c", verbose -> type ); /* Print the type first */
        write_permissions( verbose -> mode );
        print_uname_gname( verbose -> uname, verbose -> gname );
        printf("%*d ", LIST_SIZE_WIDTH, verbose -> size);
        print_time( &(verbose -> mtime) );
    }
    printf("%s\n", verbose -> name );

}
/*------------------------------------------------------------------------------
* Function: initialize_verbose_struct
*
* Description: 
*
* param:  
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
* Function: read_tar 
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/

void read_tar( FILE *fd, char **paths, void(*option)(verbose_t *verbose) ){
    int cur = 0;
    char path_buf[MAX_PATH_LENGTH];
    int loc = 0; /* Location to write to into the buffer */
    int null_blocks = 0;
    int num_blocks = 0;
    verbose_t *verbose;
    
    /* Create a new header struct to store the info */
    verbose = (verbose_t *) malloc(sizeof(verbose_t));
    if( verbose == NULL ){
        perror("malloc verbose");
        exit( EXIT_FAILURE );
    }

    initialize_verbose_struct( verbose );
    
    /* Start reading the tar file in blocks */
    while( null_blocks != 2 ){
        read_into_buffer( fd );
        printf("read the first block into the buffer.\n");
        if( (num_blocks = read_header( verbose )) == -1 ){
            null_blocks++;
            printf("Came back from read_header.\n");
            continue;
        }
        else{ /* The header is valid and can be used */
            /* REMINDER: ISSUE HERE WHERE NULL BLOCKS COULD NOT BE SUCCESSIVE */
            null_blocks = 0; /* Null blocks must be together */
            printf("Calling list function now.");
            option( verbose );

        }
    }
}






