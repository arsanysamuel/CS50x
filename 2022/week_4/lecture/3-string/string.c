#include <stdio.h>

int main(void)
{
    // Pointer arethmetic in strings and arrays
    char *s = "HI!";
    printf("%c\n", *s);  // print what's inside the pointer address (here the first letter) equivalent to s[0]
    printf("%c\n", *(s + 1));  // equivalent to s[1]
    printf("%c\n", *(s + 2));  // equivalent to s[2]
    printf("%c\n", *(s + 3));  // equivalent to s[3]
}