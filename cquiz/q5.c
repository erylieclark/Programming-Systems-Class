#include <stdio.h>

int sum(int x, int y){
    int result = 0;
    
    /* first check if x is the same as y */
    if ( x == y ){
        return x; /* don't add the values together b/c they are the same */
    }
    /* add x to the result until x is equal to y */
    for ( ; x < (y+1) ; x++){
        result += x;
    }
    return result;
}

int main(){
    int result;
    int x = 5;
    int y = 5;

    result = sum(x,y);
    printf("x = %d, y= %d, Result = %d\n", x, y,result);
    return 0;

}
