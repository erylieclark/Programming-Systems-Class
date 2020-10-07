/*******************************************************************************
* File: hashing.c
*
* Description:
*
* Author: Erin Rylie Clark
*
*******************************************************************************/

#include "hashing.h"

extern node_t **head_pntr = NULL;

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
    return hash;
}

/*------------------------------------------------------------------------------
* Function: lookup_word 
*-----------------------------------------------------------------------------*/

node_t * lookup_word( char *word) {
    node_t *node_pntr;
    head_pntr = &hash_table[hash(word)];
    node_pntr = *head_pntr;
 
    while( node_pntr != NULL){ /* While node_pntr is pointing to a node */
        printf("Comparing the pointer\n");
        if ( (strcmp( word, (node_pntr->word) )) == 0 ){
                /* Check the name value of the node */
            printf("Word is found in the array.\n");
            free(word);
            return node_pntr; /* If the word is the same, return the current pointer */
        }
        else{   /* If the word is not the same, find the next node */
            printf("Word was not found.\n");
            node_pntr = node_pntr->next; /* will be NULL if no more structs */
        }
    }
    
    /* If reached the end of the list and the word did not match, send back the
        pointer to know where to insert the word */
    return node_pntr; /* Will be NULL if word was not matched */
}

/*------------------------------------------------------------------------------
* Function: create_new_node 
*-----------------------------------------------------------------------------*/
node_t *create_new_node(char *word ){
    node_t *new_node_pntr;
    
    new_node_pntr = (node_t *) malloc(sizeof(node_t)); 
    new_node_pntr -> count = 1;
    new_node_pntr -> word = word;
    new_node_pntr -> next = NULL;
    new_node_pntr -> count_pntr = NULL;
    return new_node_pntr;
}


/*------------------------------------------------------------------------------
* Function: insert_word 
*-----------------------------------------------------------------------------*/
void insert_word( node_t *node_pntr ){
    
    node_pntr -> next = *head_pntr;
    *head_pntr = node_pntr;
    printf("%9d %s\n", node_pntr->count, node_pntr->word);
  
}

/*------------------------------------------------------------------------------
* Function: handle_hash
*-----------------------------------------------------------------------------*/
void handle_hash(char *word, int k_words){
    node_t *node_pntr;
    
    node_pntr = lookup_word(word); 
    
    if (node_pntr == NULL){ /* If word isn't already in hash table */
        node_pntr = create_new_node(word);
        insert_word(node_pntr); 
    }
    else{
        (node_pntr -> count)++;
        printf("%9d %s\n", node_pntr->count, node_pntr->word);
    }
}




