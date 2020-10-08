/*******************************************************************************
* File: hashing.h
*
* Description: all of these files will relate to the hash function and the hash
*  table. It will insert words into the table and return the count of the word.
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#ifndef _HASHING
#define _HASHING

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 32000
/* Below is the structure for the nodes that will be inserted into the hash
	table, it simply contains the word, its count, and a pointer to the
	next node */
struct node{
    char *word;
    int count;
    struct node *next;
};

typedef struct node node_t;
extern node_t *hash_table[HASH_TABLE_SIZE]; /* Array of pointers to node
                                                structs */
extern unsigned long total_uniq_words;
extern node_t **head_pntr; /* Keep track of where to insert the new node */

unsigned long hash( char *word );
node_t *lookup_word( char *word ) ;
node_t *create_new_node( char *word );
void insert_word( node_t *node_pntr );
void handle_hash( char *word);

#endif
