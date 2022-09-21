#include <stdio.h>

void swap(int a, int b);

int main(void)
{
    int x = 1;
    int y = 2;

    printf("x is %i y is %i\n", x, y);
    swap(x, y);  // calling by value: copying the values of x and y into new addresses within the function scope
    printf("x is %i y is %i\n", x, y);  // printing the same unswapped
}

void swap(int a, int b)
{
    printf("a is %i b is %i\n", a, b);
    int temp = a;
    a = b;
    b = temp;
    printf("a is %i b is %i\n", a, b);
}