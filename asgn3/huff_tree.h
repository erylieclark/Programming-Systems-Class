/*******************************************************************************
* File: huff_tree.h
*
* Description: These functions take the linked list and turn it into a 
* 	huffman tree. This file uses alot of functions from hist.c
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#ifndef _HUFF_TREE_H
#define _HUFF_TREE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hist.h"

extern char * code_table[HIST_TABLE_SIZE]; /* Array containg frequency of c */


node_t *create_new_node( void );
void create_tree( void );
void add_code_to_array( char *code, int count, unsigned char c );
void step_tree( char *code, node_t *head_pntr, int count);
void collect_codes( void );

#endif
