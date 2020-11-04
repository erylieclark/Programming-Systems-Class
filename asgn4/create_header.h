/*******************************************************************************
* File: create_header.h
*
* Description: 
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _CREATE_HEADER_H
#define _CREATE_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
typedef struct header header_t;

header_t{
	char name[HEADER_PATH_MAX];
	int mode;
	int uid;
	int gid;
	int size;
	int mtime;
	int chksum;
	char typeflag;
	char linkname[100];
	int magic;
	uint16_t version;
	char uname[32];
	char gname[32];
	uint64_t devmajor;
	uint64_t devminor;
	char prefix[155];
}

void collect_h_info( struct stat file_st );

#endif /* _CREATE_HEADER_H */
