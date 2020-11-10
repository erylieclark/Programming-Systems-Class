/*******************************************************************************
* File: read_header.c
*
* Description: this file contains the functions that help separate all of the
*   fields within the header block and convert them into a more computer
*   usable format
*
* Author: Erin Rylie Clark
*******************************************************************************/

#include "read_header.h"
#include "create_header.h"
#include "read_write.h"
#include "user_input.h"

/*------------------------------------------------------------------------------
* Function: check_null_header
*
* Description: check if this header is completely blank 
*
* return: returns 1 if a null header, 0 on not a null header
*-----------------------------------------------------------------------------*/
int check_null_header( void ){
    int result = 0;
    const char zero[BLOCK_SIZE] = {0}; /* Create a null buffer to compare to */
    if( !memcmp(readbuf, zero, BLOCK_SIZE) ){
        result = 1;
    }
    return result;
}
/*------------------------------------------------------------------------------
* Function: separate_header_fields 
*
* Description: a tedious function that pulls all of the data in order into the
*   header struct so that it can be converted to more useful formats later. It
*   also counts the values of each field and adds them up in order to check the
*   checksum value. The function will exit if chksum is not valid.
*
* param: header - the header struct to copy all the strings into
*-----------------------------------------------------------------------------*/
void separate_header_fields( header_t *header ){
    /* First clear out the wrbuf */
    int loc = 0;
    int chksum = 0;
    /* Now copy the contents of the header struct to the wrbuf */
        /* Very tedious.... */
    memcpy( header -> name, &readbuf[loc], NAME_W );
    loc += NAME_W;
    chksum += chksum_count(header -> name, NAME_W);
    memcpy( header -> mode, &readbuf[loc], MODE_W );
    loc += MODE_W;
    chksum += chksum_count(header -> mode, MODE_W);
    memcpy( header -> uid, &readbuf[loc], UID_W );
    loc += UID_W;
    chksum += chksum_count(header -> uid, UID_W);
    memcpy( header -> gid, &readbuf[loc], GID_W );
    loc += GID_W;
    chksum += chksum_count(header -> gid, GID_W);
    memcpy( header -> size, &readbuf[loc], SIZE_W );
    loc += SIZE_W;
    chksum += chksum_count(header -> size, SIZE_W);
    memcpy( header -> mtime, &readbuf[loc], MTIME_W );
    loc += MTIME_W;
    chksum += chksum_count(header -> mtime, MTIME_W);
    memcpy( header -> chksum, &readbuf[loc], CHKSUM_W );
    loc += CHKSUM_W;
    chksum += (' ')*CHKSUM_W; /* Add checksum as if it is only spaces */
    memcpy( header -> typeflag, &readbuf[loc], TYPEFLAG_W );
    loc += TYPEFLAG_W;
    chksum += chksum_count(header -> typeflag, TYPEFLAG_W);
    memcpy( header -> linkname, &readbuf[loc], LINKNAME_W );
    loc += LINKNAME_W;
    chksum += chksum_count(header -> linkname, LINKNAME_W);
    memcpy( header -> magic, &readbuf[loc], MAGIC_W );
    loc += MAGIC_W;
    chksum += chksum_count(header -> magic, MAGIC_W);
    memcpy( header -> version, &readbuf[loc], VERSION_W );
    loc += VERSION_W;
    chksum += chksum_count(header -> version, VERSION_W);
    memcpy( header -> uname, &readbuf[loc], UNAME_W );
    loc += UNAME_W;
    chksum += chksum_count(header -> uname, UNAME_W);
    memcpy( header -> gname, &readbuf[loc], GNAME_W );
    loc += GNAME_W;
    chksum += chksum_count(header -> gname, GNAME_W);
    memcpy( header -> devmajor, &readbuf[loc], DEVMAJ_W );
    loc += DEVMAJ_W;
    chksum += chksum_count(header -> devmajor, DEVMAJ_W);
    memcpy( header -> devminor, &readbuf[loc], DEVMIN_W );
    loc += DEVMIN_W;
    chksum += chksum_count(header -> devminor, DEVMIN_W);
    memcpy( header -> prefix, &readbuf[loc], PREFIX_W );
    loc += PREFIX_W;
    chksum += chksum_count(header -> prefix, PREFIX_W);

    while( loc != BLOCK_SIZE ){ /* Count the remaining bytes that do
        not belong to a specific field */
        chksum += readbuf[loc];
        loc++;
    }
    /* Convert chksum to an int and compare to counted chksum */
    if( chksum != (convert_octstr_to_int(header -> chksum, CHKSUM_W)) ){
        fprintf(stderr, "Invalid tar file - chksum does not match.\n");
        exit( EXIT_FAILURE );
    }
}
/*------------------------------------------------------------------------------
* Function: verify_header
*
* Description: this function checks the magic number for ustar and checks the
*   version if strict is enable 
*
* param: header - the header struct to look at for the magic and version
*-----------------------------------------------------------------------------*/
void verify_header( header_t *header ){
    /* First check magic number for ustar */
    if( strncmp( "ustar" , header -> magic, (MAGIC_W-1)) ){
        /* Check that the ustar matches, if it doesn't, report and exit */
        fprintf(stderr, "Invalid tar file - magic number not ustar.\n");
        exit( EXIT_FAILURE );
    }
     /* Only complete further checking if Strict (global variable) is enabled */
    if( strict ){
        /* Check that ustar is null terminated */
        if( ((header -> magic)[MAGIC_W-1]) != '\0' ){
            fprintf(stderr, "Invalid tar file -\
                magic number not null terminated.\n");
            exit( EXIT_FAILURE );
        }
        /* Now check the version */
        else if( strncmp( "00", header -> version, VERSION_W ) ){
            fprintf(stderr, "Invalid tar file - incorrect version.\n");
            exit( EXIT_FAILURE );
        }
    }
}
/*------------------------------------------------------------------------------
* Function: stitch_name_together 
*
* Description: take the prefix and name fields from the header struct and 
*   stitch them together with a '/' in between
*
* param: header - the header struct containing the prefix and name to read
* param: verbose - the verbose struct to transfer the full name into
*-----------------------------------------------------------------------------*/
void stitch_name_together( header_t *header, verbose_t *verbose ){
    int loc = 0;
    int length = 0;
    /* First check if there is anything in prefix - then check if it fills the
        buffer or not */
    if( (header -> prefix)[0] == '\0' ){ /* Nothing in the prefix */
        ; /* do nothing */
    }
    else{ /* If not empty, copy into the verbose buffer */
        length = strnlen( header -> prefix, PREFIX_W);
        strncpy( verbose -> name, header -> prefix, length );
        loc = length;
        /* Write a '/' into the next spot in the buffer */
        (verbose -> name)[loc] = '/';
        loc++;
    }
    /* Write the name into the buffer */
    if( (header -> name)[0] == '\0' ){ /* Nothing in the name */
        fprintf(stderr, "Invalid header file - nothing in name field.\n");
        exit( EXIT_FAILURE );
    }
    else{ /* If not empty, copy into verbose buffer */
        length = strnlen( header -> name, NAME_W);
        /* COULD BE AN ISSUE WITH NULL TERMINATING THIS */
        strncpy( &(verbose -> name)[loc], header -> name, length );
    }
}
        
/*------------------------------------------------------------------------------
* Function: convert_octstr_to_int 
*
* Description: convert the octal string back to an integer so that the value
*   is in a computer readable format 
*
* param: h_val - the string stored in the header struct
* param: width - the width of the field in the header struct
*-----------------------------------------------------------------------------*/
int convert_octstr_to_int( char *h_val, int width ){
    int res;
    char *endptr;
    errno = 0;

    /* Check if the first bit is set */ 
    if( h_val[0] & 0x80 ){
        res = extract_special_int( h_val, width );
    }
    else{
        res = strtol( h_val, &endptr, BASE );
        /* Do error checking as outlined in strtol man page */
        if( (errno == ERANGE && (res == LONG_MAX || res == LONG_MIN)) || \
            (errno != 0 && res == 0 )){
            perror("strtol");
            return -1; /* REMINDER: FOLLOW THROUGH ON ERROR CHECKING */
        }
        else if( endptr == h_val ){
            printf("No digits were found.\n");
            return -1; /* HERE TOO ^^^^^^ */
        }
    }
    return res;
}
/*------------------------------------------------------------------------------
* Function: get_type 
*
* Description: get the type of file and send back the character that is
*   supposed to be displayed when printing out the permissions of a file 
*
* param: flag - the typeflag from the header struct
* return: type - the new character representing the type
*-----------------------------------------------------------------------------*/
char get_type( char flag ){
    char type;
    switch( flag ){
        case '0':
            type = '-';
            break;
        case '\0':
            type = '-';
            break;
        case '2':
            type = 'l';
            break;
        case '5':
            type = 'd';
            break;
        default:
            printf("This file type is not supported by mytar.\n");
            return -1;
    }
    return type;
}
/*------------------------------------------------------------------------------
* Function: unpack_header_struct
*
* Description: this function takes out certain values and converts them from an
*   octal string to an integer value. It will also verify the other fields that
*   it does not necessarily need later for the creation of the file, but it
*   does help determine if this block is even a valid readable header block. 
*
* param: header - the header struct containing the separated strings
* param: verbose - the verbose struct to store the converted integers
*-----------------------------------------------------------------------------*/
int unpack_header_struct( header_t *header, verbose_t *verbose ){
    int length;
 
    /* Start by getting stitching the name together */
    stitch_name_together( header, verbose );

    /* Take the octal strings in the header struct and turn them into ints */
    verbose -> mode = convert_octstr_to_int( header -> mode, MODE_W );
    verbose -> uid = convert_octstr_to_int( header -> uid, UID_W );
    verbose -> gid = convert_octstr_to_int( header -> gid, GID_W );
    verbose -> size = convert_octstr_to_int( header -> size, SIZE_W );
    verbose -> mtime = convert_octstr_to_int( header -> mtime, MTIME_W );

    /* Get the type of file */
    if( (verbose -> type = get_type( *(header -> typeflag) )) == -1 ){
        fprintf(stderr, "%s: File type not supported by mytar.\n",\
            verbose -> name );
        exit( EXIT_FAILURE );
    }

    /* Check for a linkname */ /* POSSIBLY NEED TO CONVERT FROM OCTAL?? */
    if( (header -> linkname)[0] != '\0' ){ /* If there is something in it */
        length = strnlen( header -> linkname, LINKNAME_W);
        strncpy( verbose -> linkname, header -> linkname, length );
    }
    else{ /* If nothing there, do nothing */ 
        ; /* Do nothing */
    }

    /* Now transfer uname and gname */
    if( (header -> uname)[0] != '\0' ){ /* If there is something in it */
        length = strnlen( header -> uname, UNAME_W);
        strncpy( verbose -> uname, header -> uname, length );
    }
    else{ /* If nothing there, do nothing */ 
        ; /* Do nothing */
    }
    if( (header -> gname)[0] != '\0' ){ /* If there is something in it */
        length = strnlen( header -> gname, GNAME_W);
        strncpy( verbose -> gname, header -> gname, length );
    }
    else{ /* If nothing there, do nothing */ 
        ; /* Do nothing */
    }
    return 0;
}
/*------------------------------------------------------------------------------
* Function: read_header
*
* Description: this function is responsible for extracting the values out of 
*   the header block in the tar file and separating out the values before
*   returning them to a computer readable format. It will only grab out the
*   values it will later need, and verifies the remainder without moving them.
 
* param: verbose - the verbose struct containing the info needed to recreate
*   the file
* return: returns -1 if a null header, -2 if the file type is not supported,
*   and number of blocks otherwise 
*-----------------------------------------------------------------------------*/
int read_header( verbose_t *verbose ){
    int num_blocks = 0;
    header_t *header;

    /* Check for a Null header */    
    if( check_null_header() ){
        return -1; /* Return a 1 if it is a null header */
    }

    /* Create a new header struct to store the info */
    header = (header_t *) malloc(sizeof(header_t));
    if( header == NULL ){
        perror("malloc header");
        exit( EXIT_FAILURE );
    }

    /* Set all fields to empty */
    initialize_header_struct( header );
    /* Now start pulling all the contents into the header struct for easy
        parsing, validate the value of chcksum simultaneously */
    separate_header_fields( header );
    /* Verify Header - check magic number and version (if strict) */ 
    verify_header( header );
    /* Get info into computer friendly format */
    unpack_header_struct( header, verbose );
    /* Get the number of blocks to read for this file */
    num_blocks = get_content_size( verbose -> size , 0 );
    /* Pass back the header */
    return num_blocks;
}

/*------------------------------------------------------------------------------
* Function: extract_special_int 
*
* Description: For the interoperability with GNU tar, GNU seems to set the high-
*   order bit of the first byte, then treate the rest of the field as a binary
*   integer in network byte order. This only supports 32 bit int.
*
* param: where - where to read the string from
* param: len - the length of the buffer reading from 
* return: returns -1 on failure, and the integer on success
* This function was provided by Dr Nico - Cal Poly SLO
*-----------------------------------------------------------------------------*/
uint32_t extract_special_int( char *where, int len ){
    int32_t val = -1;

    if( (len >= sizeof(val)) && (where[0] & 0x80 )){
        /* The top bit is set and we have space to extract the last 4  bytes */
        val = *(int32_t *)(where + len - sizeof(val));
        val = ntohl(val);
    }
    return val;
}

