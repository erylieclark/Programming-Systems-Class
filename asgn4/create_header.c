/*******************************************************************************
* File: create_header.c
*
* Description:
*
* Author: Erin Rylie Clark
*******************************************************************************/

#include "create_header.h"

/*------------------------------------------------------------------------------
* Function:  
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/
void get_stat_info( struct stat file_st, header_t header ){
    

}

/*------------------------------------------------------------------------------
* Function:  
*
* Description: 
*
* param:  
*-----------------------------------------------------------------------------*/
void collect_h_info( struct stat file_st ){
    header_t header; /* Pointer to the header struct */

    /* Create a new header struct to store the info */
    header = (header_t) malloc(sizeof(header_t));
    if( header == NULL ){
        perror("malloc header");
        exit( EXIT_FAILURE );
    }

    /* 


}





