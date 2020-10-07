/*******************************************************************************
* File: top_words.c
*
* Description:
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#include "top_words.h"

extern int lowest_count = 0; /* Init lowest count of a single word */


/*------------------------------------------------------------------------------
* Function: top_k_words
*-----------------------------------------------------------------------------*/

node_t * top_k_words( int k_words, node_t *hash_table[] ){
    node_t *top_words_head_pntr = NULL;
    node_t *cur_node_pntr;
    /* Step through hash table */
    while( i < HASH_TABLE_SIZE ){
        /* Step along array until find not a null pointer */
        if ( hash_table[i] == NULL ){
            /* do nothing */
        }
        else {
            /* Store a pointer to the node, and compare count to top list */
            cur_node_pntr = hash_table[i];
            cur_node_pntr = compare_lowest_count( cur_node_pntr);
            if ( cur_node_pntr == NULL ){
                /* Do nothing with word, its count is not large enough */
            else{
                /* If count is higher, step through linked list and compare counts */
                compare_top_k(cur_node_pntr, k_words);
               
    }
/*------------------------------------------------------------------------------
* Function: compare_top_k
*-----------------------------------------------------------------------------*/
void compare_top_k( node_t * cur_node_pntr , int k_words){
    int k = 0;
    node_t * list_pntr;
    node_t * prev_node_pntr;
    
    if ( top_words_head_pntr = NULL){ /* First word in list */
        top_words_head_pntr = cur_node_pntr; /* Point to node of current word */
        lowest_count = top_words_head_pntr -> count;
        top_words_head_pntr -> next = NULL;
    }
    else{
        list_pntr = top_words_head_pntr; /* Start at top of list */
        while( k < k_words ){ /* Be sure to stay within range */
            if ( (cur_node_pntr -> count) < (list_pntr -> count) ){
                /* Count of node is less than count of current spot in list */
                prev_node_pntr = list_pntr; 
                    /* save in case we need to insert after */
                if( (check_list_end(k, k_words, cur_node_pntr, list_pntr)) == NULL){
                    break;
                }
                list_pntr = list_pntr -> next;
                    /*Go to next node */
            }
            else if ( (cur_node_pntr -> count) > (list_pntr -> count) ){
                /* Count of node is greater than current spot in list */
                insert_node(prev_node_pntr, cur_node_pntr, list_pntr);
                break;
            }
            else{
                /* Counts are equal, compare lexigraphically */
                if ( compare_lexi( (cur_node_pntr -> word),\
                    (list_pntr -> word) ) < 0){ 
                        /* Current word is less than list word */
                    prev_node_pntr = list_pntr;
                        /* Save in case we need to insert after */
                    if( (check_list_end(k, k_words, cur_node_pntr, list_pntr)) == NULL){
                        break;
                    }
                    list_pntr = list_pntr -> next;
                        /* Go to next node */
                else if ( compare_lexi( (cur_node_pntr -> word),\
                    (list_pntr -> word) ) > 0){ 
                        /* Current word is greater than list word */
                    insert_node(prev_node_pntr, cur_node_pntr, list_pntr);
                    break; 
            }
            k++;
        }
        dump_last_node(top_words_head_pntr, k_words);
    }
}
/*------------------------------------------------------------------------------
* Function: check for end of list
*-----------------------------------------------------------------------------*/
node_t *check_list_end(int k, int k_words, node_t *cur_node_pntr, node_t *list_pntr){
    if ( (list_pntr->next == NULL) && ( k < (k_words - 1)) ){
        /* Next pointer is null and not at end of list */
        /* List hasn't been filled yet */
        /* Insert word at this spot */
        list_pntr -> next = cur_node_pntr;
        cur_node_pntr -> next = NULL;
        return NULL;
    }
    else if ( (list_pntr->next == NULL) && ( k == (k_words - 1)) ){
        /* Next pointer is null and at end of list */
        /* List is filled, do not insert word */
        cur_node_pntr = NULL;
    }
    else{
        /* Do nothing */
    }
    return cur_node_pntr;
}

/*------------------------------------------------------------------------------
* Function: compare_lexi
*-----------------------------------------------------------------------------*/
int compare_lexi(char *new_word, char * list_word){
    int result = strcmp(new_word, list_word);
    return result;
}

/*------------------------------------------------------------------------------
* Function: insert node
*-----------------------------------------------------------------------------*/
    
void insert_node(node_t *prev_node_pntr, node_t *cur_node_pntr, node_t *list_pntr){
    prev_node_pntr -> next = cur_node_pntr;
    cur_node_pntr -> next = list_pntr;
}

/*------------------------------------------------------------------------------
* Function: dump_last_node
*-----------------------------------------------------------------------------*/

void dump_last_node(node_t *top_words_head_pntr, int k_words){
    int k = 0;
    node_t *list_pntr = top_words_head_pntr;
    while( k < k_words){
        /* Step through list */
        list_pntr = list_pntr -> next;
        if(list_pntr == NULL){
            lowest_count = list_pntr -> count;
            return;
        if (k == (k_words - 1) ){
            lowest_count = list_pntr -> count; 
                /* Grab lowest count while we're at it */
        }
        k++;
    }
    list_pntr -> next = NULL; /* Drop last node off of list */
}

/*------------------------------------------------------------------------------
* Function: compare_lowest_count
*-----------------------------------------------------------------------------*/

node_t * compare_lowest_count( node_t *cur_node_pntr ){
    /* Compare count of current struct to lowest count */
    if ( (cur_node_pntr -> count) < lowest_count ){
        cur_node_pntr = NULL; /* Do nothing with this word */
    else{
        /* Do nothing, return the pointer */
    }
    return cur_node_pntr
}
