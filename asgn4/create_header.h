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

#define OCTAL 8

/* Widths of each field */
#define NAME_W 100
#define MODE_W 8
#define UID_W 8
#define GID_W 8
#define SIZE_W 12
#define MTIME_W 12
#define CHKSUM_W 8
#define TYPEFLAG_W 1
#define LINKNAME_W 100
#define MAGIC_W 6
#define VERSION_W 2
#define UNAME_W 32
#define GNAME_W 32
#define DEVMAJ_W 8
#define DEVMIN_W 8
#define PREFIX_W 155

/* Structure containing each field */
typedef struct header header_t;
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
};

void initialize_header_struct( header_t *header );
int write_pathname( header_t *header, char path[], struct stat file_st );
void convert_to_header_format( uint32_t val, int width, char buf[] );
int set_typeflag_and_linkname( struct stat, header_t*, char path[] );
void set_magic_and_v( header_t *header );
void get_uname_gname( struct stat file_st, header_t *header );
int chksum_count( char buf[], int width );
void write_size( struct stat file_st, header_t *header );
int get_info( struct stat file_st, header_t *header, char path[] );
int create_header( struct stat file_st, char path[] );
int get_content_size( int size, int mode );
void write_to_output_buffer( header_t *header );
int insert_special_int( char *where, size_t size, int32_t val );

#endif /* _CREATE_HEADER_H */
