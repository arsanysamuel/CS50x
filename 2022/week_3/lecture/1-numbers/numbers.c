#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Linear search implementation
    int numbers[] = {4, 6, 8, 2, 7, 5, 0};

    for (int i = 0; i < 7; i++)
    {
        if (numbers[i] == 0)
        {
            printf("Found at %i\n", i);
            return 0;
        }
    }
    printf("Not found\n");
    return 1;
}