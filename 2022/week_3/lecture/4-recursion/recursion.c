#include <stdio.h>
#include <cs50.h>

void draw(int n);

int main(void)
{
    // solving a problem using recursion
    int height = get_int("Height: ");

    draw(height);
}

void draw(int n)
{
    if (n <= 0)  // Base case: return when no more rows
    {
        return;
    }

    draw(n - 1);  // call the function one more time with one less row

    for (int i = 0; i < n; i++)  // draw a line of hashes
    {
        printf("#");
    }
    printf("\n");
}

/*
What will happen here if n = 3 is:
    in recurse 0:
        1- check the base case (false)
        2- call the function with 2
    in recurse 1:
        3- check the base case (false)
        4- call the function with 1
    in recurse 2:
        5- check the base case (false)
        6- call the function with 0
    in recurse 3:
        7- check the base case (true)
        8- return
    in recurse 2:
        9- draw a line of 1 hash
    in recurse 1:
        10- draw a line of 2 hashs
    in recurse 0:
        11- draw a line of 3 hashs
*/