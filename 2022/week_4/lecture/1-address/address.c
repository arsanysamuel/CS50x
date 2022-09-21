#include <stdio.h>

int main(void)
{
    /* Introducing pointers:
        * (astrisk): (uses: declaring pointer, dereferencing operator)
            1- declaring pointer:
                ex. int *n; : defining a pointer variable, telling the compiler to store an address.
            2- dereference operator:
                ex. printf("%i\n", *p); : going inside the address pointer p and printing what's stored inside (50).
        & (ampersand): (uses: referencing)
            1- reference operator: pointing to the address of a variable
                ex. int *n = &p; : storing the address of p in the pointer n;
    */
    int n = 50;
    int *p = &n;  // storing the address of n to a pointer variable p
    printf("%i\n", n);  // printing the value stored in n
    printf("%p\n", p);  // %p for printing pointers
    printf("%p\n", &n);  // same thing but using n address directly
    printf("%i\n", *p);  // going to the address p (the address of n) and printing the value inside it (50)
}