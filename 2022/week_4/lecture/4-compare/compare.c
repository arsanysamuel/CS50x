#include <stdio.h>
#include <cs50.h>  // for get int etc
#include <string.h>

int main(void)
{
    char *i = get_string("i: ");
    char *j = get_string("j: ");

    //if (i == j)  // won't work because different address
    if (strcmp(i, j) == 0)
    {
        printf("Same\n");
    }
    else
    {
        printf("different\n");
    }
}