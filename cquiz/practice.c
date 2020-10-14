#include <stdio.h>
#include <string.h>

int main(){
    int A = 2;
    int B = -1;
    char *cp = "abcdefg";
    int grade[5] = { 4, 3, 2, 1, 0};

    printf("A + B: %d\n", A+B);
    printf("My guess: -1 \n:");
    printf("+A++ + ++B: %d\n", +A++ + ++B);
    printf("My guess: 2 \n:");
    printf("grade[2]: %d\n", grade[2]);
    printf("My guess: 2\n:");
    printf("*(cp+4): %c\n", *(cp+4));
    printf("My guess: e\n:");
    printf("&grade[4]-&grade[2]: %ld\n", &grade[4]-&grade[2]);
    printf("My guess: 2\n:");
    printf("(int)&grade[4] - (int)&grade[2]: %d\n", (int)&grade[4] - (int)&grade[2]);
    printf("My guess: 16\n:");
    printf("(A != A)? A:B: %d\n", (A != A)? A:B);
    printf("My guess: 2\n:");
    printf("strlen(cp): %ld\n", strlen(cp));
    printf("My guess: 7\n:");
    printf("sizeof(grade): %ld\n", sizeof(grade));
    printf("My guess: \n:");
    printf("strlen...: %ld\n", (char*)(&grade[0]));
    printf("My guess: abcdefg\n:");

    return 0;
}
