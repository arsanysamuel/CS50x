#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Casting data types (type conversion)
    int x = get_int("X: ");
    int y = get_int("Y: ");
    float z = (float) x / (float) y;  // casting ints to floats to give back a float result of division
    printf("Z: %.2f\n", z);
}