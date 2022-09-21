#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int x;
    printf("x: ");  // Prompt
    scanf("%i", &x);  // scanf takes the pointer (address) of the variable to put the scanned value in
    printf("x: %i\n", x);

    char *s = malloc(4 * sizeof(char));
    //char s[4];  // simpler way but a local variable
    printf("s: ");  // Prompt
    scanf("%s", s);  // s itself is a pointer
    printf("s: %s\n", s);
    free(s);
}