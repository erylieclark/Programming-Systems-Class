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

void top_k_words( int k_words, node_t *hash_table[] ){
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
               
    }

void compare_top_k( node_t * cur_node_pntr ){
    int k = 0;
    if ( top_words_head_pntr = NULL){ /* First word in list */
        top_words_head_pntr = cur_node_pntr; /* Point to node of current word */
        lowest_count = top_words_head_pntr -> count;
    }
    else{
        while( k < k_words ){ /* Be sure to stay within range */
            if ( cur_node_pntr ->
            
    


node_t * compare_lowest_count( node_t *cur_node_pntr ){
    /* Compare count of current struct to lowest count */
    if ( (cur_node_pntr -> count) < lowest_count ){
        cur_node_pntr = NULL; /* Do nothing with this word */
    else{
        /* Do nothing, return the pointer */
    }
    return cur_node_pntr
}
    /* Store node in between correct nodes */
    /* If count is same as another node, compare lexographically */
    /* Dump last node */
    /* If comparing last node in list, dump the struct that is lower */
    
    /* Compare the pointer of the current struct to the pointers stored
        in the table to see if the word is already in the table */
    for (i = 0 ; i < k_words ; i++ ){
        if (top_words_arr[i] == node_pntr){
            /* If pointer matches, the word is already in the table */
            cur_count = node_pntr -> count; /* Grab the count */
            if ( (cur_count - 1) == lowest_count){ /* Might be pointing to 
                                                    a linked list */
                dump_linked_list(low_ptr);
                replace_pointer_in_list();
        }
    }



    lowest_count = find_lowest_count(k_words, top_words_arr, low_ptr);

void replace_pointer_in_list(low_ptr){
    
}

void check_lowest_count( node_t *node_pntr){
    static int lowest_count;
    if ( (cur_count -1) == lowest_count){

void dump_linked_list(node_t **low_pntr){
    /* Clear the count pointers of each struct in linked list */
    node_t *temp;
    node_t *node;
    node = *low_pntr;
    while( (temp = node -> count_pntr) != NULL){
        node -> count_pntr = NULL;
        node = temp -> count_pntr;
    }
    

int find_lowest_count(int k_words, node_t *top_words_arr[], node_t **low_pntr){
    int i = 1 /* Count of first struct and start scanning at second struct */
    int lowest = (top_words_arr[i]) -> count;
    while( i < k_words ){ /* Scan the array to find the lowest count */
        if( ( (top_words_arr[i]) -> count) < lowest){
            lowest = (top_words_arr[i]) -> count; /* Store the lowest count*/
            low_pntr = top_words_arr[i]; /* Set the pointer to the location of
                                            the lowest count */
        }
    }
    return lowest;
}

    /* If pointer matches, the word is already in the table */
        /* For the case where pointer is pointing to linked list of lowest
            count, check if count is exactly one above lowest count. If yes,
            dump linked list of lowest count and replace with new pointer */

    /* If pointer doesn't match, check the value of count */

    /* Compare to static value of lowest count so far */

    /* If count is lower than lowest count, ignore */

    /* If count is higher than lowest count, replace lowest count pointer
        with current pointer */
    /* Find new lowest count and store */

    /* If count is equal to lowest count, add pointer to linked list */

void drop_lowest_count_list( struct *lowest){

    /* clear all count_ptr's */

}
