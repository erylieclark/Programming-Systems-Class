/*******************************************************************************
* File: read_tar.h
*
* Description: 
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _READ_TAR_H
#define _READ_TAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include "read_header.h"

#define NUM_PERM_BITS 9
#define LIST_USRGRP_WIDTH 17
#define LIST_SIZE_WIDTH 8
#define OG_YEAR 1900

void write_permission( int mode );
void print_uname_gname( char *uname, char *gname );
void print_time( time_t *mtime );
void list_tar( verbose_t *verbose, FILE *fd, int num_blocks );
void initialize_verbose_struct( verbose_t *verbose );
int restore_attributes( verbose_t *verbose );
FILE *create_item_type( verbose_t *verbose );
void extract_tar( verbose_t *verbose, FILE *rdfd, int num_blocks );
void read_tar( FILE *fd, char **paths, void(*option)(verbose_t *verbose,\
	FILE *fd, int num_blocks ) );



#endif /* _READ_TAR_H */
