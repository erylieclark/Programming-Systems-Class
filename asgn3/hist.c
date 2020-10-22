/*******************************************************************************
* File : hist.c
*
* Description: This file contains the functions that count the number of each
*   character that is in the file given by the user and sorts them. It will
*   create a linked list of all the words in ascending frequency and order.
*
* Author: Erin Rylie Clark
*******************************************************************************/

# include "hist.h"

/* Global Variables */
int hist_table[HIST_TABLE_SIZE]; /* To store count of each char */
node_t * head_pntr; /* To point to the front of the linked list */

/*------------------------------------------------------------------------------
* Function: count_chars
* 
* Description: This function will take in a char and increment the count of that
*   char indexed by its ascii value. hist_table is a global array.
*
* param: c - the char whos count should be incremented
*-----------------------------------------------------------------------------*/
void count_chars( unsigned char c ){
    hist_table[c] += 1; /* Add one to the count for the corresponding char*/    
}

/*------------------------------------------------------------------------------
* Function: compare_char
* 
* Description: This function will take two nodes, compare their char values, and
*   return a -1 or 1. -1 means the first node is less than the second. 1 means
*   the first nodes is greater than the second. If the nodes are the same, place
*   the new one in front of the one it is being compared to (ie. return -1).
*   This function is only called if the count is also the same.
*
* param: first - pointer to the node to be inserted (newest node)
* param: second - pointer to the node already in the list
*-----------------------------------------------------------------------------*/
int compare_char( node_t *first , node_t *second ){
    int result;
    int a = first -> c;
    int b = second -> c;
    result = ( a < b )? -1 : ( a > b );
    if ( result == 0 ){
        result = -1;
    }
    return result;
}

/*------------------------------------------------------------------------------
* Function: compare_count
* 
* Description: This function will take two nodes, compare their counts, and
*   return a -1, or 1. -1 means the first node is less than the second. 0
*   means they are the same, and the function will then call the compare_char
*   function to determine where to place it. 1 means the first nodes is greater
*   than the second.
*
* param: first - pointer to the node to be inserted (newest node)
* param: second - pointer to the node already in the list
*-----------------------------------------------------------------------------*/
int compare_count( node_t *first, node_t *second ){
    int result;
    int a = first -> count;
    int b = second -> count;

    result = ( a < b )? -1 : ( a > b );
    if ( result == 0 ){
        result = compare_char( first , second );
    }
    return result;
}

/*------------------------------------------------------------------------------
* Function: create_new_leaf
*
* Description: This function creates a new leaf node for any char in the hist
*   table that has a count greater than 0, and initializes its values.
*
* param: c - character to make a struct for
* param: count - count of character in the file
*
* return: new_node_pntr - a pointer to the node created
*-----------------------------------------------------------------------------*/
node_t * create_new_leaf( unsigned char c , int count ){
    node_t * new_node_pntr;

    new_node_pntr = (node_t *) malloc(sizeof(node_t)); /* Allocate node space */
    if( !new_node_pntr ){ /* Check return value */
        perror("malloc new leaf\n");
        exit( EXIT_FAILURE ); /* Exit if malloc failed */
    }

    /* Initialize its values */
    new_node_pntr -> c = c;
    new_node_pntr -> count = count;
    new_node_pntr -> next = NULL;
    new_node_pntr -> left = NULL;
    new_node_pntr -> right = NULL;
    new_node_pntr -> hcode = NULL;

    return new_node_pntr;
}
/*------------------------------------------------------------------------------
* Function: insert_head
* 
* Description: This function will take the head_pntr and the next_pntr and set
*   the new node to be the front of the linked list. head_pntr is a global
*   variable and does not need to be returned.
*
* param: new - the new node to be inserted at the head of the list
* param: cur - the node that the new node is replacing as the head of the list
*-----------------------------------------------------------------------------*/
void insert_head(node_t *new, node_t *cur){
    head_pntr = new; /* Set new node to top of list */
    head_pntr -> next = cur; /* Set new node to point to previous head node */
}

/*------------------------------------------------------------------------------
* Function: insert_between
* 
* Description: This function will take a pointer to a node, and place it between
*   the appropriate two nodes passed to the function.
*
* param: prev - the node before where the new node should be
* param: new - the node to be inserted
* param: cur - the node after where the new node should be 
*-----------------------------------------------------------------------------*/
void insert_between( node_t *prev, node_t *new, node_t *cur ){
    prev -> next = new; /* Set the prev node to point to the new one */
    new -> next = cur; /* Set the new one to point to the following node */
    return;
}

/*------------------------------------------------------------------------------
* Function: insert_node
* 
* Description: This function will takes a few pointers and determines where
*   the new node needs to be place. It will call the appropriate function to
*   complete the insertion.
*
* param: prev - the node before where the new node should be
* param: new - the node to be inserted
* param: cur - the node after where the new node should be 
*-----------------------------------------------------------------------------*/
void insert_node( node_t *prev , node_t *new, node_t *cur ){
    if ( cur == head_pntr ) { /* We are at front of list still */
        insert_head( new, cur );
    }
    else{ /* In middle of list */
        insert_between( prev, new, cur);
    }        
    return;
}
/*------------------------------------------------------------------------------
* Function: add_to_list
* 
* Description: This function will step through the list to find out where the
*   new node belongs. It will then call the appropriate function to insert it.
*
* param: new_node_pntr - pointer to node to be inserted
*-----------------------------------------------------------------------------*/
void add_to_list( node_t * new_node_pntr ){
    node_t * cur_node_pntr = NULL;
    node_t * prev_node_pntr = NULL;
    int location;

    if ( head_pntr == NULL ) { /* First node to be inserted */
        insert_head(new_node_pntr, cur_node_pntr);
    }
    else{ /* Subsequent nodes */
        cur_node_pntr = head_pntr; /* Start at top of list */
        while (1){ /* Continue until the node is inserted */
            location = compare_count( new_node_pntr, cur_node_pntr );
            if ( location > 0 ){ /* New node belongs after the current node */
                prev_node_pntr = cur_node_pntr; /* Save if inserting after */
                if ( (cur_node_pntr -> next) == NULL){ /* At end of list */
                    insert_node(prev_node_pntr, new_node_pntr, NULL);
                    return; /* Inserted at end of list, now leave */
                }
                cur_node_pntr = cur_node_pntr -> next; /* Go to next pointer */
            }
            else if ( location < 0 ){ /* Node belongs before the current node */
                insert_node(prev_node_pntr, new_node_pntr, cur_node_pntr);
                return;
            } /* done inserting the node, now leave */
        }
    }
}
/*------------------------------------------------------------------------------
* Function: create_list
* 
* Description: This function will take the hist_table, create a struct for any
*   character that has a count of at least 1 and order the list by count in
*   ascending order.
*
* return: uniq_bytes - the number of characters with a count greater than 0 
*-----------------------------------------------------------------------------*/
int create_list( void ){
    node_t * new_node_pntr = NULL;
    int i;
    int uniq_bytes = 0; /* Initialize as no unique bytes */
    
    for( i = 0 ; i < HIST_TABLE_SIZE ; i++ ){
        if (hist_table[i] == 0){ /* The entry has a count of 0 */
            continue; /* Do not make a node for it if it has no count */
        }
        else{ /* The entry has a count. Make a new node and add to list */
            new_node_pntr = create_new_leaf( i , hist_table[i] );
            add_to_list(new_node_pntr); 
           /* printf("Hex: 0x%02x, Count = %d\n", new_node_pntr -> c, \
                new_node_pntr -> count); */ /* Helpful for debugging */
            uniq_bytes++; /* Increment uniq bytes with each new node */
        }
    }

    return uniq_bytes;
}
