/*******************************************************************************
* File: parseline.h
*
* Description: parseline is used to take the input from a command line and
*  separate out the stages of a pipeline, and determine where all of the inputs
*  and outputs are. 
*
* Author: Erin Rylie Clark
*******************************************************************************/

#ifndef _PARSELINE_H 
#define _PARSELINE_H

/* System Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Local Header Files */
#include "check_valid_input.h"
#include "store_stage_info.h"
#include "fork_pipe.h"

extern stage_t *head_pntr;
extern int sig_flag;
int parseline( void );
void free_tokens( char *stage_tokens[], int num_tokens );
void free_stages( char *stages[], int num_stages, int cur_stage );

#endif /* _PARSELINE_H */

