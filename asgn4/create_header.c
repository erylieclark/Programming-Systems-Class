/*******************************************************************************
* File: create_header.c
*
* Description:
*
* Author: Erin Rylie Clark
*******************************************************************************/

#include "create_header.h"
#include "read_write.h"
/*
#define MYOTCTAL
#define DEVS
*/

/*------------------------------------------------------------------------------
* Function: initialize_header_struct
*
* Description: 
*
* param:  
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

    return;
}

/*------------------------------------------------------------------------------
* Function: write_pathname
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/
void write_pathname( header_t *header, char path[] ){
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
    int err = 0;

    sprintf( buf, "%0*o", (width-1), val);
    if( buf[width-1] != '\0' ){ /* If the int is too big to write as octal */
        memset( buf, '\0', width ); /* Blank out the buffer again */
        if( insert_special_int( buf, width, val )){
            err++; /* Failed to insert the int */
        }
    }
        


#ifdef MYOCTAL    
    while( i < (width - 1) ){
        oct = val & mask; /* Get the first octal digit and convert to char */
        buf[i] = '0' + oct; /* Add the octal character into the buffer */
        val = val >> 3; /* Shift the next 3 bits into the LSB bits */
        i++;
        if( val < 1 ){ /* If zero, don't get any more octal digits */
            break;
        }
    }
    
    if( val > 0 ){ /* If the value is still greater than zero, it will not fit
        into the allotted number of octal digits, use given functions to 
        convert it into the appropriate format */
        if( insert_special_int( buf, width, orig_val )){
            err++; /* Failed to insert the int */
        }
    }
    else{ /* Place a '\0' into the rest of the buffer */
        while( i < width ){
            buf[i] = '\0';
            i++;
        } /* REMINDER: COULD DO MEMSET HERE?????? */
    }
#endif
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
*   spots. 
*
* param: file_st - the structure containing the lstat info on the file
* param: header - the header struct to place the collected info into
*-----------------------------------------------------------------------------*/
void get_uname_gname( struct stat file_st, header_t *header ){
    struct passwd *pw;
    struct group *gp;

    pw = getpwuid( file_st.st_uid );
    strncpy( header -> uname, pw->pw_name, (UNAME_W-1) );

    gp = getgrgid( file_st.st_gid );
    strncpy( header -> gname, gp->gr_name, (GNAME_W-1) );
}

/*------------------------------------------------------------------------------
* Function:  
*
* Description: This function will get the user id and the group id, and format
*   them correctly before placing them back into the header struct.
*
* param:  
*-----------------------------------------------------------------------------*/
int chksum_count( char buf[] , int width ){
    int i = 0;
    int count = 0;
    while( i < width ){
        count += (unsigned char ) buf[i];
        i++;
    }
    return count;
}

/*------------------------------------------------------------------------------
* Function:  
*
* Description: This function will get the user id and the group id, and format
*   them correctly before placing them back into the header struct.
*
* param:  
*-----------------------------------------------------------------------------*/
void get_info( struct stat file_st, header_t *header, char path[] ){
    int chksum = 0;
    /* Mode */
    printf("mode(oct): %0*o\n", (MODE_W-1), file_st.st_mode);
    printf("mode: %d\n", file_st.st_mode);
    convert_to_header_format( (file_st.st_mode & ~S_IFMT), MODE_W,\
        header -> mode ); 
    printf("mode(oct): %0*o\n", (MODE_W-1), file_st.st_mode);
    printf("mode: %d\n", file_st.st_mode);
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
    chksum += chksum_count( header -> devmajor, DEVMAJ_W );
    chksum += chksum_count( header -> devminor, DEVMIN_W );
#endif
    /* Be sure to add the path name to the chksum count */
    chksum += chksum_count( header -> name, NAME_W );
    chksum += chksum_count( header -> prefix, PREFIX_W );
    printf("chksum(oct): %0*o\n", (CHKSUM_W-1), chksum);
    printf("chksum: %d\n", chksum);
    /* Chksum */
    chksum += (' ')*CHKSUM_W; /* Assume chksum is all spaces and add to count */
    convert_to_header_format( chksum, CHKSUM_W, header -> chksum ); 
    printf("chksum(oct): %0*o\n", (CHKSUM_W-1), chksum);
    printf("chksum: %d\n", chksum);
}

/*------------------------------------------------------------------------------
* Function: create_header
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/
void create_header( struct stat file_st, char path[] ){
    header_t *header;

    /* Create a new header struct to store the info */
    header = (header_t *) malloc(sizeof(header_t));
    if( header == NULL ){
        perror("malloc header");
        exit( EXIT_FAILURE );
    }

    /* Set all fields to empty */
    initialize_header_struct( header );

    /* Put the path name into the header struct */
    write_pathname( header, path );

    /* Collect remaining info (except chksum), formtted, into the struct */
    get_info( file_st, header, path );

    /* Write all info to output buffer, output buffer is global */ 
    write_to_output_buffer( header );

}


/*------------------------------------------------------------------------------
* Function: write_to_output_buffer
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/
void write_to_output_buffer( header_t *header ){
    /* First clear out the wrbuf */
    char *where = memset( writebuf, '\0', 512 );
    /* Now copy the contents of the header struct to the wrbuf */
        /* Very tedious.... */
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
* Description: 
*
* param:  
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

