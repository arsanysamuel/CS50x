#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    //int list[3];  // list memory will be on the stack (the bottom part)
    int *list = malloc(3 * sizeof(int));  // dynamically allocating memory on the heap (the top part)
    if (list == NULL)
    {
        return 1;
    }

    list[0] = 1;
    list[1] = 2;
    list[2] = 3;

    // an additional variable needs to be added to the list array

    // allocating memory with the size of the new data
    int *tmp = malloc(4 * sizeof(int));
    if (tmp == NULL)
    {
        free(list);
        return 1;
    }

    // Copying values from the old array to the new one
    for (int i = 0; i < 3; i++)
    {
        tmp[i] = list[i];
    }

    // Adding the extra value
    tmp[3] = 4;

    // Free the old array address and
    free(list);
    list = tmp;  // updating the list pointer to be the tmp array pointer

    // Printing values
    for (int i = 0; i < 4; i++)
    {
        printf("%i\n", list[i]);
    }

    // Freeing the new array pointer before exit
    free(list);

    return 0;
}