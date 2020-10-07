/*******************************************************************************
* File: top_words.h
*
* Description: 
* 
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#ifndef _TOP_WORDS
#define _TOP_WORDS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashing.h"

node_t *top_k_words(int k_words, node_t *hash_table[]);

void compare_top_k( node_t * cur_node_pntr , int k_words){
node_t *check_list_end(int k, int k_words, node_t *cur_node_pntr, node_t *list_pntr){
int compare_lexi(char *new_word, char * list_word){
void insert_node(node_t *prev_node_pntr, node_t *cur_node_pntr, node_t *list_pntr){
void dump_last_node(node_t *top_words_head_pntr, int k_words){
node_t * compare_lowest_count( node_t *cur_node_pntr ){



#endif /*_TOP_WORDS */
