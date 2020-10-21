/*******************************************************************************
* File: hist.h
*
* Description: These functions relate to counting the frequency of each 
*   character from the input and creating an ordered linked list.
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#ifndef _HIST_H
#define _HIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HIST_TABLE_SIZE 256
extern int hist_table[HIST_TABLE_SIZE]; /* Array containg frequency of c */
extern struct node * head_pntr;

typedef struct node node_t;
struct node{
    int c;
    int count;
    node_t * next;
    node_t * left;
    node_t * right;
    char * hcode;
};

void count_chars( unsigned char c );
int compare_char( node_t *first, node_t *second );
int compare_count( node_t *first, node_t *second );
node_t *create_new_leaf( unsigned char c, int count );
void insert_head( node_t *, node_t * );
void insert_between( node_t *prev, node_t *new, node_t *cur );
void insert_node( node_t *prev, node_t *new, node_t *cur );
void add_to_list ( node_t *new_node_pntr );
unsigned char create_list( void );

#endif
