/*******************************************************************************
* File: hashing.c
*
* Description: This file contains all of the functions related to inserting
*   words into a hash table. It simply takes a word from main, evaluates its
*   hash value, checks if the word is there already, and inserts it into the
*   hash table if it is not.
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#include "hashing.h"
/*------------------------------------------------------------------------------
* Function: hash
* This function was found at: http://www.cse.yorku.ca/~oz/hash.html
*-----------------------------------------------------------------------------*/
unsigned long hash( char *word){ /* REMINDER: CHAR WAS PREV UNSIGNED */
    unsigned long hash = 5381;
    int c;
     
    while( (c = *word++) ){
        hash = ( (hash << 5) + hash ) + c;
    }
    hash = hash % HASH_TABLE_SIZE;
        /* Use modulus to fit this into the table */

    return hash;
}

/*------------------------------------------------------------------------------
* Function: lookup_word
*
* Description: This function takes the word in question, calls the hash function
*   to retrieve its hash value, and looks it up in the hash table. It will 
*   return a pointer to the location of the word if it is in the table, or will
*   return NULL if the word is not already in the table.
*
* param: word - word to be looked up
*
*-----------------------------------------------------------------------------*/

node_t * lookup_word( char *word) {
    node_t *node_pntr; /* Points to */
    head_pntr = &hash_table[hash(word)]; /* Points to spot in hash table
                                            to look for the word */
    node_pntr = *head_pntr; /* Stores the first node in the hash location */
 
    while( node_pntr != NULL){ /* If node_pntr is pointing to a node */

        if ( (strcmp( word, (node_pntr->word) )) == 0 ){
                /* Check the name value of the node */
            free(word); /* Free the space allocated to the word if already in
                            the table */
            return node_pntr; /* If the word is the same, return the current
                    pointer, this lets the calling function know that the word
                    is already in the table */
        }
        else{   /* If the word is not the same, find the next node */
            node_pntr = node_pntr->next; /* will be NULL if no more nodes */
        }
    }
    
    /* If reached the end of the list and the word did not match, send back the
        pointer to know where to insert the word */
    return node_pntr; /* Will be NULL if word was not matched */
}

/*------------------------------------------------------------------------------
* Function: create_new_node 
*
* Description: This function is only called if it has been determined that the
*   current word in question does not already exist in the hash table. It
*   allocate space to store the new structure and initialize the values. Word
*   already has allocated space, so this function simply sets a pointer to
*   that location. 
*-----------------------------------------------------------------------------*/
node_t *create_new_node(char *word ){
    node_t *new_node_pntr;
    
    new_node_pntr = (node_t *) malloc(sizeof(node_t)); /* Allocate space */
    new_node_pntr -> count = 1; /* First instance of this word */
    new_node_pntr -> word = word; /* Space already allocated for word */
    new_node_pntr -> next = NULL; /* To know when we have gotten to the end of
                                        a linked list */
    total_uniq_words++; /* Each time a node is created, means there is a new
                            unique word */
    return new_node_pntr; /* Return a pointer to the new node */
}


/*------------------------------------------------------------------------------
* Function: insert_word 
*
* Description: This function will recieve a pointer to the new node that was
*   created and insert it at the front of the linked list that corresponds
*   to its hash value. Because head_pntr is a global variable, it does not
*   need to be passed.
*
* param: node_pntr - pointer to the new node that was created
*
*-----------------------------------------------------------------------------*/
void insert_word( node_t *node_pntr ){   
    node_pntr -> next = *head_pntr; /* Set what the head pointer was previously
        pointing at to be the node that follows the inserted node */
    *head_pntr = node_pntr; /* Set the head pointer to point to the inserted
        node */
}

/*------------------------------------------------------------------------------
* Function: handle_hash
*
* Description: This function is called from main and calls the other functions
*   in this file. It looks up the word, creates a new node  and inserts it if
*   it isn't in the table, or increases the count of the word if it is already
*   in the table. It returns to main when it the word has been handled.
*
* param: word - a pointer to the first character of the word. This word 
*   already has space allocated for it.
*
*-----------------------------------------------------------------------------*/
void handle_hash(char *word){
    node_t *node_pntr;
    node_pntr = lookup_word(word); 
    
    if (node_pntr == NULL){ /* If word isn't already in hash table */
        node_pntr = create_new_node(word); /* Create the node */
        insert_word(node_pntr); /* Insert the node */
    }
    else{ /* If word is already in table */
        (node_pntr -> count)++; /* Increment the count */
    }
}




