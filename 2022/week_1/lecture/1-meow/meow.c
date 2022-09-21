#include <stdio.h>
#include <cs50.h>

// Prototype for the fuction (just to define it before usage) as a header
void meow(int n);

int main(void)
{
    meow(3);
}

void meow(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("meow\n");
    }
}