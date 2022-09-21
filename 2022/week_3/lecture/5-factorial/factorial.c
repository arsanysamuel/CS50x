#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int fact(int num);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./factorial number\n");
        return 1;
    }

    int len = strlen(argv[1]);
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("The number must be an integer\n");
            return 1;
        }
    }

    int number = atoi(argv[1]);
    if (number <= 0)
    {
        printf("The number must be positive\n");
        return 1;
    }

    printf("Result = %i\n", fact(number));
}

int fact(int num)
{
    if (num == 1)
    {
        return 1;
    }

    return num * fact(num - 1);
}