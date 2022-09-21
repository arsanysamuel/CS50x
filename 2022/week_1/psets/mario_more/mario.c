#include <cs50.h>
#include <stdio.h>

void print_hashes(int iter);

int main(void)
{
    // Get height from user
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    // Print the pyramid
    for (int i = 0; i < n; i++)
    {
        for (int j = n - 1; j > i; j--)
        {
            printf(" ");
        }
        print_hashes(i);
        printf("  ");
        print_hashes(i);
        printf("\n");
    }
}

// will print hashes according to the iteration number
void print_hashes(int iter)
{
    for (int i = 0; i <= iter; i++)
    {
        printf("#");
    }
}