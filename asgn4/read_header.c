/*******************************************************************************
* File: read_header.c
*
* Description:
*
* Author: Erin Rylie Clark
*******************************************************************************/

#include "read_header.h"
#include "create_header.h"
#include "read_write.h"

/*------------------------------------------------------------------------------
* Function: check_null_header
*
* Description: 
*
* param: 
* return: returns 1 if a null header, 0 on not a null header
*-----------------------------------------------------------------------------*/
int check_null_header( void ){
    int result = 0;
    const char zero[BLOCK_SIZE] = {0}; /* Create a null buffer to compare to */
    if( !memcpy(readbuf, zero, BLOCK_SIZE) ){
        result = 1;
    }
    return result;
}
/*------------------------------------------------------------------------------
* Function: separate_header_fields 
*
* Description: a tedious function that pulls all of the data in order into the
*   header struct so that it can be converted to more useful formats later
*
* param: 
* return:
*-----------------------------------------------------------------------------*/
void separate_header_fields( header_t *header ){
    /* First clear out the wrbuf */
    char *where = readbuf;
    /* Now copy the contents of the header struct to the wrbuf */
        /* Very tedious.... */
    where = memcpy( header -> name, *where, NAME_W );
    where = where + NAME_W;
    where = memcpy( header -> mode, where, MODE_W );
    where = where + MODE_W;
    where = memcpy( header -> uid, where, UID_W );
    where = where + UID_W;
    where = memcpy( header -> gid, where, GID_W );
    where = where + GID_W;
    where = memcpy( header -> size, where, SIZE_W );
    where = where + SIZE_W;
    where = memcpy( header -> mtime, where, MTIME_W );
    where = where + MTIME_W;
    where = memcpy( header -> chksum, where, CHKSUM_W );
    where = where + CHKSUM_W;
    where = memcpy( header -> typeflag, where, TYPEFLAG_W );
    where = where + TYPEFLAG_W;
    where = memcpy( header -> linkname, where, LINKNAME_W );
    where = where + LINKNAME_W;
    where = memcpy( header -> magic, where, MAGIC_W );
    where = where + MAGIC_W;
    where = memcpy( header -> version, where, VERSION_W );
    where = where + VERSION_W;
    where = memcpy( header -> uname, where, UNAME_W );
    where = where + UNAME_W;
    where = memcpy( header -> gname, where, GNAME_W );
    where = where + GNAME_W;
    where = memcpy( header -> devmajor, where, DEVMAJ_W );
    where = where + DEVMAJ_W;
    where = memcpy( header -> devminor, where, DEVMIN_W );
    where = where + DEVMIN_W;
    where = memcpy( header -> prefix, where, PREFIX_W );
    where = where + PREFIX_W;

    return;
}
    


}
/*------------------------------------------------------------------------------
* Function: read_header
*
* Description: this function takes int a pointer to a function that it should
*   send the header off to.
*
* param: option - 
* return: returns 1 if a null header, 0 on success, -1 if it is invalid 
*-----------------------------------------------------------------------------*/
void read_header( void /*void(*option)(header_t header) */ ){
    header_t *header;

    /* Create a new header struct to store the info */
    header = (header_t *) malloc(sizeof(header_t));
    if( header == NULL ){
        perror("malloc header");
        exit( EXIT_FAILURE );
    }

    /* Set all fields to empty */
    initialize_header_struct( header );

    /* Check for a Null header */    
    if( check_null_header() ){
        return 1; /* Return a 1 if it is a null header */
    }
    
    /* Now start pulling all the contents into the header struct for easy
        parsing */
    separate_header_fields();

    /* Perform the next part: either list or extract */ 
/*    (*option)( header ); */ /* Call whichever function was given to read_header */

}



