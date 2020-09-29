
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *stdin;
FILE *stdout;

char *read_long_line(FILE *file){
    /* Figure out how long this line is */
    int len = 0;
    static int file_pos = 0;
    char c, *mbuffer;
    fseek(file, file_pos, SEEK_SET);
    c = fgetc(file);
    while( c != '\n' && c != EOF ){
    /* Determine length of line, stopped by '\n' character*/
        len++;
        c = fgetc(file);
    }
    if ( c == EOF ){
        printf("EOF\n");
        return NULL;
    }
    else if ( c == '\n'){
        printf("Newline\n");
        printf("Length is: %d\n", len);
        fseek(file, (-len-1), SEEK_CUR);
        mbuffer = malloc(len+1);
        mbuffer = fgets(mbuffer, len+1 , file);
        printf("Mbuffer: %s\n", mbuffer);
        file_pos += len + 1;
    }
    return mbuffer;
}

int main(){
    char *lnptr;
    /* Call to read long line */
    while(1){

        if ( (lnptr = read_long_line(stdin)) == NULL)
            break;
    }
    return 0;
}
