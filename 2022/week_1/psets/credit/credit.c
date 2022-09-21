#include <cs50.h>
#include <stdio.h>
#include <math.h>

char check_validity(long card);

int main(void)
{
    // Getting user input
    long card = get_long("Number: ");

    // Determining card type and checking validity
    if (check_validity(card))
    {
        if (card / (long) pow(10, 13) == 34 || card / (long) pow(10, 13) == 37)
        {
            printf("AMEX\n");
        }
        else if (card / (long) pow(10, 14) == 51 ||
                 card / (long) pow(10, 14) == 52 ||
                 card / (long) pow(10, 14) == 53 ||
                 card / (long) pow(10, 14) == 54 ||
                 card / (long) pow(10, 14) == 55
                )
        {
            printf("MASTERCARD\n");
        }
        else if (card / (long) pow(10, 12) == 4 || card / (long) pow(10, 15) == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

char check_validity(long card)
{
    // Luhn's algorithm
    int sum = 0;
    int num;

    int i = 0;
    do
    {
        i += 2;
        num = ((card % (long) pow(10, i)) / (long) pow(10, i - 1)) * 2;
        sum += (num / 10 > 0) ? ((num / 10) + (num % 10)) : num;
    }
    while (card / (long) pow(10, i) > 0);

    i = -1;
    do
    {
        i += 2;
        sum += (card % (long) pow(10, i) / (long) pow(10, i - 1));
    }
    while (card / (long) pow(10, i) > 0);

    return (sum % 10 == 0) ? 1 : 0;
}