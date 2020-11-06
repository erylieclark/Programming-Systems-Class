/*******************************************************************************
* File: read_header.h
*
* Description: 
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _READ_HEADER_H
#define _READ_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <ctype.h>
#include <sys/sysmacros.h>
#include <arpa/inet.h>
#include <grp.h>
#include "create_header.h"

/* Structure containing each field for verbose kjj*/
/*typedef struct header header_t;
struct header{
	char name[NAME_W];
	char mode[MODE_W];
	char uid[UID_W];
	char gid[GID_W];
	char size[SIZE_W];
	char mtime[MTIME_W];
	char chksum[CHKSUM_W];
	char typeflag[TYPEFLAG_W];
	char linkname[LINKNAME_W];
	char magic[MAGIC_W];
	char version[VERSION_W];
	char uname[UNAME_W];
	char gname[GNAME_W];
	char devmajor[DEVMAJ_W];
	char devminor[DEVMIN_W];
	char prefix[PREFIX_W];
}; */

int check_null_header( void );
void separate_header_fields( header_t *header );
void read_header( void );




#endif /* _READ_HEADER_H */




