#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Gets the name from the user and prints a greeting message
    string name = get_string("What's your name? ");
    printf("hello, %s\n", name);
}