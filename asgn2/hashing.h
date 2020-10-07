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

#define HASH_TABLE_SIZE 20

struct node{
    char *word;
    int count;
    struct node *next;
    struct node *top_word_node_pntr; /* Pointer to keep track of a linked list if this 
        word is tied for last place in top words list */
};

typedef struct node node_t;
node_t *hash_table[HASH_TABLE_SIZE]; /* Array of pointers to node
                                                structs */

unsigned long hash( char *word );
node_t *lookup_word( char *word ) ;
node_t *create_new_node( char *word );
void insert_word( node_t *node_pntr );
void handle_hash( char *word, int k_words);

#endif
