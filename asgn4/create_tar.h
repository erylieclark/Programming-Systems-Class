/*******************************************************************************
* File: create_tar.h
*
* Description: 
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _CREATE_TAR_H
#define _CREATE_TAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#define HEADER_PATH_MAX 100

struct dirent *read_dir( DIR *d_pntr, const char *path );
int add_to_path( char path[], int loc, const char *d_name );
void recurse_files_and_create( FILE *fd, char *path, int loc );
void create_tar( FILE *fd, char **paths );

#endif /* _CREATE_TAR_H */
