#include <stdio.h>



int main(void){
    char *cp;
    *cp = 'c';
    printf("cp points to %c", *cp);
    char *ap;
    printf("ap points to %c", *ap);
    int **pp;
    printf("value of pp is %p\n", pp);
    return 0;
    
}
