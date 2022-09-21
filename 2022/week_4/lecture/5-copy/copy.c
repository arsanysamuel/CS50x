#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // for malloc and free

int main(void)
{
    char *s = get_string("s: ");

    //char *t = s;  // this will assign the address of s into t, the toupper change will happen on both s and t
    char *t = malloc(strlen(s) + 1);  // allocating memory for the string t, the + 1 for the null character

    if (t == NULL)  // 0 in terms of memory, malloc returns NULL if no memroy allocated
    {
        printf("malloc() failed, not enough memory!\n");
        return 1;
    }

    for (int i = 0, n = strlen(s) + 1; i < n; i++)
    {
        //*(t + i) = *(s + i);  // less readable
        t[i] = s[i];
    }
    //strcpy(t, s); // easier solution, copies string s to t

    if (strlen(t) > 0)
    {
        t[0] = toupper(t[0]);
    }

    printf("s: %s\n", s);
    printf("t: %s\n", t);

    free(t);  // freeing allocated memory
    // no need to free s because get_string does that at the end of excution

    return 0;
}