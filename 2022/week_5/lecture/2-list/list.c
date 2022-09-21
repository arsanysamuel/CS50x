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

    printf("%p\n", list);

    // an additional variable needs to be added to the list array

    // allocating memory with the size of the new data and moving the values from old array (list) to new one (tmp)
    int *tmp = realloc(list, 4 * sizeof(int));
    if (tmp == NULL)
    {
        free(list);
        return 1;
    }

    // The address of tmp equals the address of list if allocated to the same position in memory (not always the case)
    printf("%p\n", list);
    printf("%p\n", tmp);

    // Adding the extra value
    tmp[3] = 4;

    if (list != tmp)
    {
        free(list);
        list = tmp;  // updating the address of list
    }

    // Printing values
    for (int i = 0; i < 4; i++)
    {
        printf("%i\n", list[i]);
    }

    // Freeing the new array pointer before exit
    free(list);

    return 0;
}