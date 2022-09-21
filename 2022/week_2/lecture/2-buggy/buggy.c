#include <stdio.h>
#include <cs50.h>

int get_negative_int(void);

int main(void)
{
    int i = get_negative_int();  // break point, use step into
    printf("%i\n", i);
}

int get_negative_int(void)
{
    int n;
    do
    {
        n = get_int("Negative int: ");
    }
    while (n < 0);  // bug here
    return n;
}