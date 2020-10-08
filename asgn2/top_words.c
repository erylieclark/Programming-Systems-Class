/*******************************************************************************
* File: top_words.c
*
* Description: This file mainly deals with steping through the hash table, and
*   determining which words belong in the top K words. It creates a linked list
*   containing the top words and returns a pointer to the linked list.
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#include "top_words.h"
#include "hashing.h"

/*------------------------------------------------------------------------------
* Function: top_k_words
*
* Description: This is the function called from main. It takes the number of
*   words requested from the user, or the default number, and steps through
*   the hash table, looking for the highest count words.
*
* param: k_words - number of words requested by the user
*-----------------------------------------------------------------------------*/

node_t * top_k_words( int k_words ){
    node_t *top_words_head_pntr = NULL; /* Pntr to linked list of top words*/
    node_t *cur_node_pntr; /* Node in hash table currently being compared */
    node_t *next_pntr; /* Next node to be compared */
    unsigned long i = 0; /* For stepping through the hash table */

    /* Step through hash table */
    while( i < HASH_TABLE_SIZE ){
        /* Step along array until find a spot that contains a pointer to node */
        if ( hash_table[i] == NULL ){
            /* do nothing if no node */
        }
        else { /* A node has been found */
            /* Store a pointer to the node, and compare count to top list */
            cur_node_pntr = hash_table[i];
            do{ 
                if ( compare_lowest_count(cur_node_pntr) ){
                    /* Do nothing with word, its count is not large enough */
                }
                else{
                    /* If count is higher or equal to the lowest count, step
                         through linked list of top words and compare counts */
                    next_pntr = cur_node_pntr -> next; /* Save a pointer to
                        next node in linked list of hash table in case top
                        words list changes it */
                    top_words_head_pntr = compare_top_k(cur_node_pntr, \
                        k_words, top_words_head_pntr);
                        /* Compare the count and word to the words in the top
                            words list */
                }
            }while( (cur_node_pntr = next_pntr) != NULL);
                /* As long as there is a node linked to the curent node */
                /* Returning the value of next to current for next loop */
        }
        i++;
    }

    return top_words_head_pntr; /* Return a pointer to the linked list 
            containing the top K words */
}

/*------------------------------------------------------------------------------
* Function: compare_top_k
* 
* Description: This function will step through the top words list and determine
*   where in the list the word belongs. It first compares the counts of the 
*   words to the words in the linked list and then compares lexicographically
*   if there is a tie.
*
* param: cur_node_pntr - pointer to the word to be inserted
* param: k_words - maximum number of words to be in the list
*-----------------------------------------------------------------------------*/
node_t * compare_top_k( node_t * cur_node_pntr , int k_words, \
         node_t *top_words_head_pntr){
    
    int k = 0;
    node_t * list_pntr;
    node_t * prev_node_pntr;
    if ( top_words_head_pntr == NULL){ /* First word in list */
        top_words_head_pntr = cur_node_pntr; /* Point to node of current word */
        top_words_head_pntr -> next = NULL; /* Next pointer points to nothing */
        prev_node_pntr = top_words_head_pntr; /* If the next word needs to
            know what the node before it was */
    }
    else{
        list_pntr = top_words_head_pntr; /* Start at top of list */
        while( k < k_words ){ /* Be sure to stay within range */ 
            if ( (cur_node_pntr -> count) < (list_pntr -> count) ){
                /* Count of node is less than count of current spot in list */
                prev_node_pntr = list_pntr; 
                    /* save in case we need to insert after */
                if( (check_list_end(k, k_words, cur_node_pntr, list_pntr)) \
                    == NULL){
                    break;
                } /* Check if we are at the end of the list, if the list is 
                    already full, dump the word. If the list is not full, put
                    the word in the next available spot*/
                list_pntr = list_pntr -> next;/*Go to next node */
            }
            else if ( (cur_node_pntr -> count) > (list_pntr -> count) ){
                /* Count of node is greater than current spot in list */
                if (k == 0){ /* node belongs at head, above first node */
                    top_words_head_pntr = insert_at_head(cur_node_pntr, \
                        list_pntr, top_words_head_pntr); 
                        /* Insert at top of list */
                    break;
                }
                /* If k > 0 node belongs in middle of list, so insert it */
                insert_node(prev_node_pntr, cur_node_pntr, list_pntr);
                break;
            }
            else{
                /* Counts are equal, compare lexigraphically */
                if ( compare_lexi( (cur_node_pntr -> word),\
                    (list_pntr -> word) ) < 0){ 
                        /* Current word belongs below list word */
                    prev_node_pntr = list_pntr;
                        /* Save in case we need to insert after */
                    if( (check_list_end(k, k_words, cur_node_pntr, list_pntr))\
                         == NULL){
                        break;
                    } /* Check if we are at the end of the list, if the list is 
                    already full, dump the word. If the list is not full, put
                    the word in the next available spot*/

                    list_pntr = list_pntr -> next;  /* Go to next node */
                }
                else if ( compare_lexi( (cur_node_pntr -> word),\
                    (list_pntr -> word) ) > 0){ 
                        /* Current word belongs above list word */
                    if (k == 0){ /* Node belongs at head, above first node */
                        top_words_head_pntr = insert_at_head(cur_node_pntr, \
                            list_pntr, top_words_head_pntr);
                            /* Insert at top of list */
                        break;
                    }
                    /* If k > 0 node belongs in middle of list, so insert it */
                    insert_node(prev_node_pntr, cur_node_pntr, list_pntr);
                    break; 
                }
            }
            k++;
        }
        dump_last_node(top_words_head_pntr, k_words);
    }
    return top_words_head_pntr;
}

/*------------------------------------------------------------------------------
* Function: insert_at_head
*
* Description: This function is told that the new node belongs above the first
*   node in the top words list and points the head pointer to this new node,
*   and sets the new node to point to the node that was previously at the top
*   of the list.
*
* param: cur_node_pntr - pointer to the word to be inserted
* param: list_pntr - pointer to the word that the new word will point to
*
* return a pointer to the head of the top words linked list
*-----------------------------------------------------------------------------*/
node_t * insert_at_head(node_t *cur_node_pntr, node_t *list_pntr,\
    node_t *top_words_head_pntr){
        top_words_head_pntr = cur_node_pntr; /* Point to node of current word */
        top_words_head_pntr -> next = list_pntr; /* Set the new node to 
            point to the node that was previously at the top of the list */
    return top_words_head_pntr;
}        
/*------------------------------------------------------------------------------
* Function: check_list_end
*
* Description: This word checks if the node about to be inserted is going to
*   be inserted at the end of the list, whether the list be full or not. If it
*   is at the end of the list, it will insert the word if the list is not full,
*   or it will not insert the word if the word belongs below the last word in
*   the list and the list is already full.
*
* param: k - tells the function how far into the list it is
* param: k_words - maximum number of words that belong in the list
* param: cur_node_pntr - pointer to the word to be inserted
* param: list_pntr - pointer to the node the word will be inserted after
*-----------------------------------------------------------------------------*/
node_t *check_list_end(int k, int k_words, node_t *cur_node_pntr, \
        node_t *list_pntr){
    if ( (list_pntr->next == NULL) && ( k < (k_words - 1)) ){
        /* Next pointer is null and not at end of list */
        /* List hasn't been filled yet */
        /* Insert word at this spot */
        list_pntr -> next = cur_node_pntr; /* Set the last node currently in
            the list to poitn to the new node */
        cur_node_pntr -> next = NULL; /* Make sure the new node doesn't point
            to anything for future reference */
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
*
* Description: This function only compares two strings lexicographically and
*   returns the result of the strcmp function. If the new word belongs before
*   the word in the list, it will return -1, and will return 1 if the new word
*   belongs after the word in the list.
*
* param: new_word - word to be inserted to the list
* param: list_word - word already in the list being compared to the new word
*-----------------------------------------------------------------------------*/
int compare_lexi(char *new_word, char * list_word){
    int result = strcmp(new_word, list_word);
    return result;
}

/*------------------------------------------------------------------------------
* Function: insert_node
*
* Description: This function inserts the node into the location that it belongs.
*   It takes a pointer to where it belongs, a pointer to the node before it,
*   and sets the next pointers accordingly.
*
* param: prev_node_pntr - the pointer to which the current node should follow
* param: cur_node_pntr - the pointer to the current node being inserted
* param: list_pntr - a pointer to the node that the current node should now
*   point to
*-----------------------------------------------------------------------------*/
    
void insert_node(node_t *prev_node_pntr, node_t *cur_node_pntr, \
        node_t *list_pntr){
    prev_node_pntr -> next = cur_node_pntr; /* Set the previous node to point
        to the current node */
    cur_node_pntr -> next = list_pntr; /* Set the current node to point to the
        node that it goes before in the list */
}

/*------------------------------------------------------------------------------
* Function: dump_last_node
*
* Description: When the top words list is full, this function removes the 
*   link at the end after a new word has been inserted into the table to keep
*   the list from containing more words than the user requested. If the list
*   is not full, it does not remove any nodes.
*
* param: top_words_head_pntr - a pointer to the head of the linked list
*   containing all of the top words
* param: k_words - the maximum number of words that belong in the list
*-----------------------------------------------------------------------------*/

void dump_last_node(node_t *top_words_head_pntr, int k_words){
    int k = 0; /* To keep track of how far down the list it is */
    node_t *list_pntr = top_words_head_pntr; /* Start at the top of the list */
    while( k < k_words){
        /* Step through list */
        if( (list_pntr -> next) == NULL){ /* If there aren't any more nodes
                after this one, and the list is not yet full, make sure the
                lowest count stays at zero */
            lowest_count = 0;
            return;
        }
        
        list_pntr = list_pntr -> next; /* Set the pointer to the next node */
        
        if (k == (k_words - 1) ){ /* If we are at the last node that should
                be in the list */
            lowest_count = list_pntr -> count; 
                /* Grab lowest count while we're at it */
        }
        k++;
    }
    list_pntr -> next = NULL; /* Drop last node off of list */
}

/*------------------------------------------------------------------------------
* Function: compare_lowest_count
*
* Description: This function will let the calling function know if the count
*   of the word in question is high enough to make it into the top words list.
*   The lowest count vairable is a global variable that maintains the lowest
*   count of words currently in the list.
*
* param: cur_node_pntr - pointer to the structure of the word in question
*
* returns 1 when the count is too low to go into the top list, return 0 
*   when the count is high enough
*-----------------------------------------------------------------------------*/

int compare_lowest_count( node_t *cur_node_pntr ){
    /* Compare count of current struct to lowest count */
    if ( (cur_node_pntr -> count) < lowest_count ){
        return 1; /* Do nothing with this word */
    }
    else{
        /* Do nothing, return the pointer */
    }
    return 0;
}
