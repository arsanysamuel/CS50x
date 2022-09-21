#include <stdio.h>

void swap(int *a, int *b);

int main(void)
{
    int x = 1;
    int y = 2;

    printf("x is %i y is %i\n", x, y);
    swap(&x, &y);  // calling by reference: passing the addresses of x and y
    printf("x is %i y is %i\n", x, y);
}

void swap(int *a, int *b)  // will take pointer args
{
    int temp = *a;  // putting the value inside the address a into temp
    *a = *b;  // putting the value inside the address b into inside the address a
    *b = temp;  // putting the value of temp inside the address b
}