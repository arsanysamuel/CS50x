#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Getting user input
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);


    // Printing the pyramid
    for (int i = 1; i <= height; i++)
    {
        for (int j = height - i; j > 0; j--)  // Printing spaces
        {
            printf(" ");
        }
        for (int j = 1; j <= i; j++)  // Printing hashes
        {
            printf("#");
        }
        printf("\n");  // Printing a newline
    }
}