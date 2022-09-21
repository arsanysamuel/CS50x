#include <stdio.h>
#include <math.h>
#include <cs50.h>

int main(void)
{
    // showing the float imprecision issue, rounding for accurate results

    float dollars = get_float("Amount in dollars: ");

    int pennies = dollars * 100;
    printf("Pennies, unrounded: %i\n", pennies);

    pennies = round(dollars * 100);
    printf("Pennies, rounded: %i\n", pennies);
}