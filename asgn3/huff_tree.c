/*******************************************************************************
* File : huff_tree.c
*
* Description: This file contains the functions that create the huffman tree
*   out of the linked list. It will also contain functions to read from the tree
*
* Author: Erin Rylie Clark
*
*******************************************************************************/
#include "huff_tree.h"
#include "hist.h"

char *code_table[HIST_TABLE_SIZE];
/*------------------------------------------------------------------------------
* Function: create_new_node
*
* Description: This function creates a new node that will store the count of
*   the first two nodes in the list, set the head pointer to the third node,
*   and pass back a pointer to the new node.
*
* return: new_node_pntr - a pointer to the new node
*-----------------------------------------------------------------------------*/
node_t *create_new_node( void ){
    node_t * new_node_pntr;
    node_t * left = head_pntr;
    node_t * right = head_pntr -> next;
    
    new_node_pntr = (node_t *) malloc(sizeof(node_t)); /* Allocate node space */
    if( !new_node_pntr ){ /* Check return value */
        perror("malloc new node\n");
        exit( EXIT_FAILURE ); /* Exit if malloc failed */
    }

    /* Initialize its values */
    new_node_pntr -> c = -1; /* To indicate not an actual char leaf */
    new_node_pntr -> count = (left -> count) + (right -> count);
        /* new node has the sum of its child nodes */
    new_node_pntr -> next = NULL; /* Set null before inserting back into list */
    new_node_pntr -> left = left;
    new_node_pntr -> right = right;
    new_node_pntr -> hcode = NULL; /* Don't have the code yet */
    
    head_pntr = head_pntr -> next -> next; 
        /* head pointer now points at third node */
    
    return new_node_pntr;
}
/*------------------------------------------------------------------------------
* Function: create_tree
* 
* Description: This function will take the linked list and create the tree
*-----------------------------------------------------------------------------*/
void create_tree( void ){
    node_t * new_node_pntr;
    if( head_pntr == NULL){
        return; /* The file was empty */
    }
    while ( (head_pntr -> next) != NULL){
        new_node_pntr = create_new_node();
        add_to_list( new_node_pntr );
    }
    return;
}
/*------------------------------------------------------------------------------
* Function: add_code_to_array
* 
* Description: This function will allocate space for the huffman code and give
*   the pointer to the appropriate spot in the code_table.
*-----------------------------------------------------------------------------*/
void add_code_to_array(char * code, int count, unsigned char c){
    code_table[c] = (char *) malloc( sizeof(char) * count );
    if( !code_table[c] ){ /* Check return value */
        perror("malloc string into code array\n");
        exit( EXIT_FAILURE ); /* Exit if malloc failed */
    }
    code_table[c] = strncpy( code_table[c], code, count );
    return;
}

/*------------------------------------------------------------------------------
* Function: step_tree
* 
* Description: This function will go through all of the branches of the tree
*   to get the huffman code associated with each leaf.
* Issues: checking the character against a zero could be misleading...
*-----------------------------------------------------------------------------*/
void step_tree( char * code, node_t *node_pntr, int count){
    
    /* Check for a left pointer */
    if ( (node_pntr -> left) == NULL ){ /* This might be a leaf */
        if ( (node_pntr -> c) == -1 ){ /* Not a leaf, tree is empty */
            return; /* empty file */
        }
        else{ /* This is a leaf */
            code[count] = '\0';
            add_code_to_array( code, count, (unsigned char) node_pntr -> c );
        }
    }
    else{ /* There is a left node to check */
        code[count] = '0'; /* Dropping into left branch, add a zero to code */
        step_tree( code , node_pntr -> left , (count + 1) ); /* Call again */
    }
    
    /* Now check the right pointer */
    if ( (node_pntr -> right) == NULL){ /* theoretically shouldn't get in here*/
        if ( (node_pntr -> c) == -1 ){
            return; /* empty file */
        }
        else{ /* This is a leaf */
            add_code_to_array( code, count, (unsigned char) node_pntr -> c );
        }
    }
    else{ /* There is a right node to check */
        code[count] = '1'; /* Dropping into right branch, add a one to code */
        step_tree( code, node_pntr -> right, (count + 1) ); /* Call again */
    }
    return;
}
/*------------------------------------------------------------------------------
* Function: collect_codes
* 
* Description: This function will take the huffman tree and generate the codes
*   for each leaf.
*-----------------------------------------------------------------------------*/
void collect_codes( void ){
    int count = 0;
    node_t *node_pntr = head_pntr;
    char * huff_code = (char *) malloc( sizeof(char) * HIST_TABLE_SIZE );
        /* Allocate a string that will hold the binary codes - this is the
            largest it will ever be */
    if( !huff_code ){ /* Check return value */
        perror("malloc string for collecting huffman code from tree\n");
        exit( EXIT_FAILURE ); /* Exit if malloc failed */
    }
    
    if ( node_pntr == NULL){
        return; /* The file was empty */
    }
    
    step_tree( huff_code, node_pntr, count);
    return;
}
