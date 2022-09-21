//#include <cs50.h>
#include <stdio.h>
#include <string.h>  // just for strlen()

int main(void)
{
    //string s = "HI!";  // in the library it's typedef char* string;
    char *s = "HI!";  // declaring string (string is a pointer to the first char in the string)
    char *p = &s[0];
    char c = s[0];
    char *p_c = &c;
    printf("s: %s\n", s);
    printf("s address: %p\n", s);
    printf("p address: %p\n", p);
    printf("c address: %p\n", p_c);  // different address for the different variable

    printf("\n");

    printf("Now printing the addresses of the whole string: \n");
    for (int i = 0; i < strlen(s) + 1; i++)  // + 1 for printing the null character
    {
        printf("address of %c is %p\n", s[i], &s[i]);
    }
}