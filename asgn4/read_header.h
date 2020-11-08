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
#include <limits.h>
#include "create_header.h"

#define BASE 8
#define MAX_PATH_LENGTH 256
#define SYM_LINK_LENGTH 100

typedef struct verbose verbose_t;
struct verbose{
	char name[MAX_PATH_LENGTH];
	int mode;
	int uid;
	int gid;
	int size; /* SOME OF THESE SIZES COULD BE TOO SMALL */
	time_t mtime;
	char type;
	char linkname[SYM_LINK_LENGTH];
	char uname[UNAME_W];
	char gname[GNAME_W];
};


int check_null_header( void );
void separate_header_fields( header_t *header );
void verify_header( header_t *header );
void stitch_name_together( header_t *header, verbose_t *verbose );
int convert_octstr_to_int( char *h_val, int width );
char get_type( char flag );
int unpack_header_struct( header_t *header, verbose_t *verbose );
int read_header( verbose_t *verbose );
uint32_t extract_special_int( char *where, int len );




#endif /* _READ_HEADER_H */




