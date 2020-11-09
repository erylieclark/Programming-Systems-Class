/*******************************************************************************
* File: create_header.c
*
* Description: this file deals primarily with getting the info from the stat
*   struct of a file and formatting the values before transferring them into
*   a header struct that makes it easier to write the full header to the 
*   tarfile.
*
* Author: Erin Rylie Clark
*******************************************************************************/

#include "create_header.h"
#include "read_write.h"

/* if wanting to get devmajor and devminor values. For this program, leave alone
#define DEVS
*/

/*------------------------------------------------------------------------------
* Function: initialize_header_struct
*
* Description: Clear out all of the fields in the header struct, so that if a
*   value does not take up its alotted width, the remainder will be nulls as
*   opposed to garbage values. This also helps with string termination if
*   necessary
*
* param: header - the header struct containing the fields
*-----------------------------------------------------------------------------*/
void initialize_header_struct( header_t *header ){

    memset( header -> name, '\0', NAME_W );
    memset( header -> mode, '\0', MODE_W ); 
    memset( header -> uid, '\0', UID_W );
    memset( header -> gid, '\0', GID_W );
    memset( header -> size, '\0', SIZE_W );
    memset( header -> mtime, '\0', MTIME_W );
    memset( header -> chksum, '\0', CHKSUM_W );
    memset( header -> typeflag, '\0', TYPEFLAG_W );
    memset( header -> linkname, '\0', LINKNAME_W );
    memset( header -> magic, '\0', MAGIC_W );
    memset( header -> version, '\0', VERSION_W );
    memset( header -> uname, '\0', UNAME_W );
    memset( header -> gname, '\0', GNAME_W );
    memset( header -> devmajor, '\0', DEVMAJ_W );
    memset( header -> devminor, '\0', DEVMIN_W );
    memset( header -> prefix, '\0', PREFIX_W );
}
/*------------------------------------------------------------------------------
* Function: write_pathname
*
* Description: this function takes in the path name and splits it into the
*   prefix and name header fields as necessary. It fills the name field as much
*   as it can while still splitting on a '/' 
*
* param: header - the header struct to write the path into
* param: path - the path to copy into the header struct
* return: -1 on failure, 0 on success
*-----------------------------------------------------------------------------*/
int write_pathname( header_t *header, char path[] ){
    strncpy( header -> name, path, NAME_W );
}

/*------------------------------------------------------------------------------
* Function: convert_to_header_format
*
* Description: This function takes an integer and converts it to an octal fomrat
*   as an ASCII string. It will check that the integer will fit into its
*   alotted space as an octal string, and if it doesnt, the function will call
*   the given insert special int function to modify the format to the one that
*   GNUs tar uses.
*
* param:  
*
* REMINDER: NEED TO MAKE SURE THE SPECIAL INT FUNCTION IS NOT CALLED WHEN 
*   THE STRICT FLAG IS SET
*-----------------------------------------------------------------------------*/
void convert_to_header_format( uint32_t val, int width, char buf[] ){
    
    /* Try putting the value into the buffer as an octal string */
    sprintf( buf, "%0*o", (width-1), val); 

    /* If it wrote into the last spot in the buffer, its too big */
    if( buf[width-1] != '\0' ){ /* Too big to write as octal */
        memset( buf, '\0', width ); /* Blank out the buffer again */
        if( !strict ){
            if( insert_special_int( buf, width, val )){
                printf("Val cannot be written to the header field.\n");
                exit( EXIT_FAILURE );
            }
        }
        else{
            printf("Val cannot be written to the header\
                field in strict mode.\n");
            exit( EXIT_FAILURE );
        }
    }
} 
/*------------------------------------------------------------------------------
* Function: set_typeflag_and_linkname
* 
* Description: This function checks the mode of the stat struct and checks if
*   it matches with any of the acceptable types for mytar. If it does match, it
*   writes the appropriate value to typeflag in the header struct. If it does
*   not match, it returns a 1 to the calling function. If the type is a symlink,
*   the function will write the linkname to linkname in the header struct.
* 
* param: file_st - the structure containing the lstat info on the file
* param: header - the header struct to place the collected info into
* param: path - the path of the file we are currently looking at
* return: 0 on success, nonzero on failure
*-----------------------------------------------------------------------------*/ 
int set_typeflag_and_linkname( struct stat file_st,\
        header_t *header, char path[] ){ 
    int err = 0; 

    /* Get the type of file */ 
    switch( file_st.st_mode & S_IFMT ){ 
        case S_IFREG: /* Write a 0 if a regular file */ 
            *header -> typeflag = '0'; 
            break; 
        case S_IFLNK: /* Write a 2 if a symbolic link */ 
            *header -> typeflag = '2'; 
            if( ( readlink( path, header -> linkname, LINKNAME_W )) \
                    == -1 ){ 
                    perror("readlink"); /* Check return values */ 
                    err++; /* Report to the calling function that it failed */ 
            }
            /* Unless the buffer is filled, linkname will already be null
                terminated due to the initialization of the buffer */  
            break;
        case S_IFDIR: /* Write a 5 if a directory */
            *header -> typeflag = '5';
            break;
        default: /* Report an error to calling function if the file is not one
                    of the acceptable types of files */
            printf("%s: Not a regular file, symlink, or directory.\n", path );
            err++;
    }
    return err;
} 
/*------------------------------------------------------------------------------
* Function: set_magic_and_v 
*
* Description: This function simply writes "ustar" into the magic buffer in the
*   header stat, and writes "00" in the version buffer. These are always 
*   constant, so they are hard coded in. Magic will be automatically null
*   terminated due to the initialization of the header struct.
*
* param: header - the header struct that contains the buffers to write to
*-----------------------------------------------------------------------------*/
void set_magic_and_v( header_t *header ){
    memcpy(header -> magic, "ustar", (MAGIC_W-1));
    memcpy(header -> version, "00", VERSION_W);
}
/*------------------------------------------------------------------------------
* Function: get_uname_gname
*
* Description: This function will get the user name based on the user id as well
*   as the group name, and copy them into the header struct in their appropriate
*   spots. If it fails to get the symbolic names, it will use the numeric ones. 
*
* param: file_st - the structure containing the lstat info on the file
* param: header - the header struct to place the collected info into
*-----------------------------------------------------------------------------*/
void get_uname_gname( struct stat file_st, header_t *header ){
    struct passwd *pw;
    struct group *gp;
    errno = 0; /* Set errno to zero before the call to check if it changed */
    pw = getpwuid( file_st.st_uid );
    if( pw == NULL ){ /* If we can't get the symbolic name, use numeric */
        perror("getpwuid");
        convert_to_header_format( file_st.st_uid, UNAME_W, header -> uname );
    }
    else{   
        strncpy( header -> uname, pw->pw_name, (UNAME_W-1) );
    }

    gp = getgrgid( file_st.st_gid );
    if( gp == NULL ){ /* If we can't get the symbolic name, use numeric */
        perror("getgrgid");
        convert_to_header_format( file_st.st_gid, GNAME_W, header -> gname );
    }
    else{
        strncpy( header -> gname, gp->gr_name, (GNAME_W-1) );
    }
}
/*------------------------------------------------------------------------------
* Function: chksum_count 
*
* Description: This function counts the value of all the chars in each field of
*   the header struct in order to determine the chksum value.
*
* param: buf - the buffer to count the chars in
* param: width - the width of the buffer, to be able to count all of the chars
*-----------------------------------------------------------------------------*/
int chksum_count( char buf[] , int width ){
    int i = 0;
    int count = 0;
    while( i < width ){
        count += (unsigned char) buf[i];
        i++;
    }
    return count;
}
/*------------------------------------------------------------------------------
* Function: get_info 
*
* Description: 
*
* param: file_st - the stat structure returned by lstat-ing the file
* param: header - the header struct to transfer all of the stat info to
* param: path - the path of the file
*-----------------------------------------------------------------------------*/
int get_info( struct stat file_st, header_t *header, char path[] ){
    int chksum = 0;
    int err = 0;
    /* Mode */
    convert_to_header_format( (file_st.st_mode & ~S_IFMT), MODE_W,\
        header -> mode ); 
    chksum += chksum_count( header -> mode, MODE_W );
    /* User ID */
    convert_to_header_format( file_st.st_uid, UID_W, header -> uid ); 
    chksum += chksum_count( header -> uid, UID_W );
    /* Group ID */
    convert_to_header_format( file_st.st_gid, GID_W, header -> gid ); 
    chksum += chksum_count( header -> gid, GID_W );
    /* Size */
    convert_to_header_format( file_st.st_size, SIZE_W, header -> size );
    chksum += chksum_count( header -> size, SIZE_W );
    /* mtime */    
    convert_to_header_format( file_st.st_mtime, MTIME_W, header -> mtime ); 
    chksum += chksum_count( header -> mtime, MTIME_W );
    /* Skip cksum until end */
    /* Typeflag */
    if( set_typeflag_and_linkname( file_st, header, path )){
        /* There was an error */
        printf("Error with typeflag and linkname.\n");
        err++;
    }
    chksum += chksum_count( header -> typeflag, TYPEFLAG_W );
    chksum += chksum_count( header -> linkname, LINKNAME_W );
    /* Magic and Version */
    set_magic_and_v( header );
    chksum += chksum_count( header -> magic, MAGIC_W );
    chksum += chksum_count( header -> version, VERSION_W );
    /* Uname and Gname */
    get_uname_gname( file_st, header );
    chksum += chksum_count( header -> uname, UNAME_W );
    chksum += chksum_count( header -> gname, GNAME_W );
#ifdef DEVS
    /* Devmajor and Devminor */
    convert_to_header_format( major(file_st.st_dev),\
        DEVMAJ_W, header -> devmajor);
    convert_to_header_format( minor(file_st.st_dev),\
        DEVMIN_W, header -> devminor);
#endif
    chksum += chksum_count( header -> devmajor, DEVMAJ_W );
    chksum += chksum_count( header -> devminor, DEVMIN_W );
    /* Be sure to add the path name to the chksum count */
    chksum += chksum_count( header -> name, NAME_W );
    chksum += chksum_count( header -> prefix, PREFIX_W );
    /* Chksum */
    chksum += (' ')*CHKSUM_W; /* Assume chksum is all spaces and add to count */
    convert_to_header_format( chksum, CHKSUM_W, header -> chksum ); 
    
    return err;
}

/*------------------------------------------------------------------------------
* Function: create_header
*
* Description: this function takes in the stat structure on a file and transfers
*   the information into a header struct in the appropriate format for a tar
*   file. It will transfer the information to the buffer to be written to the
*   tarfile but does not write it - the calling function will first verify the
*   validity of everything else before writing it.
*
* param: file_st - the struct stat returned from lstat-ing the file
* param: path - the file path
* return: num_blocks on success, -1 on failure
*-----------------------------------------------------------------------------*/
int create_header( struct stat file_st, char path[] ){
    header_t *header;
    int num_blocks;
    int err = 0;

    /* Create a new header struct to store the info */
    header = (header_t *) malloc(sizeof(header_t));
    if( header == NULL ){
        perror("malloc header");
        exit( EXIT_FAILURE );
    }

    /* Set all fields to empty */
    initialize_header_struct( header );

    /* Put the path name into the header struct */
    if( (write_pathname( header, path )) == -1 ){
        err = -1;
    }
    
    /* Collect remaining info, formatted, into the struct */
    if( get_info( file_st, header, path ) ){
        err = -1;
    }

    /* Write all info to output buffer, output buffer is global */ 
    write_to_output_buffer( header );

    /* Get the number of blocks to be written before freeing the struct */
    num_blocks = get_content_size( file_st.st_size );
    
    free( header ); /* No value is returned on free */

    return num_blocks;
}

/*------------------------------------------------------------------------------
* Function: get_content_size
*
* Description: this function is intended to read the size of the file, determine
*   how many blocks of data need to be written, and returns the value
*
* param: size - size of the file
* return: num_blocks - the number of blocks needed for the file contents
*-----------------------------------------------------------------------------*/
int get_content_size( int size ){
    int num_blocks;

    /* Find num blocks based on file size from stat first */
    num_blocks = size / BLOCK_SIZE;
    if( (size % BLOCK_SIZE) > 0 ){
        num_blocks++; /* Add one if there are leftover bytes that do not fill
            a full block */
    }
    return num_blocks;
}
/*------------------------------------------------------------------------------
* Function: read_write_buffer_header
*
* Description: tedious function to copy all of the fields in the header struct
*   to the output buffer before writing it to the tarfile 
*
* param: header - header struct containing all of the data to be written
*-----------------------------------------------------------------------------*/
void write_to_output_buffer( header_t *header ){

    /* First clear out the wrbuf */
    char *where = memset( writebuf, '\0', 512 );

    /* Copy the contents... */
    where = memcpy( where, header -> name, NAME_W );
    where = where + NAME_W;
    where = memcpy( where, header -> mode, MODE_W );
    where = where + MODE_W;
    where = memcpy( where, header -> uid, UID_W );
    where = where + UID_W;
    where = memcpy( where, header -> gid, GID_W );
    where = where + GID_W;
    where = memcpy( where, header -> size, SIZE_W );
    where = where + SIZE_W;
    where = memcpy( where, header -> mtime, MTIME_W );
    where = where + MTIME_W;
    where = memcpy( where, header -> chksum, CHKSUM_W );
    where = where + CHKSUM_W;
    where = memcpy( where, header -> typeflag, TYPEFLAG_W );
    where = where + TYPEFLAG_W;
    where = memcpy( where, header -> linkname, LINKNAME_W );
    where = where + LINKNAME_W;
    where = memcpy( where, header -> magic, MAGIC_W );
    where = where + MAGIC_W;
    where = memcpy( where, header -> version, VERSION_W );
    where = where + VERSION_W;
    where = memcpy( where, header -> uname, UNAME_W );
    where = where + UNAME_W;
    where = memcpy( where, header -> gname, GNAME_W );
    where = where + GNAME_W;
    where = memcpy( where, header -> devmajor, DEVMAJ_W );
    where = where + DEVMAJ_W;
    where = memcpy( where, header -> devminor, DEVMIN_W );
    where = where + DEVMIN_W;
    where = memcpy( where, header -> prefix, PREFIX_W );
    where = where + PREFIX_W;

    return;
}

/*------------------------------------------------------------------------------
* Function: insert_special_int 
*
* Description: "For interoperability with GNU tar. GNU seeems to set the high
*   order bit of the first byte, then treat the rest of the field as a binary
*   integer in network byte order. Insert the given integer into the given
*   field using this technique." 
*
* param: where - where to write the value
* param: size - the size of the location to write to
* param: val - the value to write
* return: 0 on success, nonzero otherwise
*
* This function was provide by Dr. Nico - Cal Poly SLO
*-----------------------------------------------------------------------------*/
int insert_special_int( char *where, size_t size, int32_t val ){
    int err = 0;
    if( val < 0  || (size < sizeof(val)) ){
        /* If it's negative, bit 31 is set and we can't use the flag. If len is
            too small, we can't write it */
        err++;
    }
    else{
        memset( where, 0, size ); /* Clear out the buffer */
        *(int32_t *)(where+size-sizeof(val)) = htonl(val); /* Place the int */
        *where |= 0x80; /* Set the high order bit */
    }
    return err;
}

