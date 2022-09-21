#include <stdio.h>
#include <stdlib.h>

// defining the new type node (node should be defined in this line), typedef {...} node; is abstracting (aliasing) using struct node with node only
typedef struct node
{
    int number;  // the actual element variable
    struct node *next;
    // The "node" pointer to the next "node" in the list, it's type node because it refers to the whole node not just the number (or whatever variable)
}
node;

int main(void)
{
    // List of size 0
    node *list = NULL;  // initializing pointers to NULL is a good for checking conditionals and avoiding garbage

    // Add a number to list
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        return 1;
    }
    //(*n).number = 1;  // () for dereferencing n first then using the struct member number
    n->number = 1;  // syntactic sugar for the line above
    n->next = NULL;

    // Update list to point to new node
    list = n;

    // Add another number to list
    n = malloc(sizeof(node));  // allocating new memory for the new node
    if (n == NULL)
    {
        free(list);
        return 1;
    }
    n->number = 2;
    n->next = NULL;
    list->next = n;  // adding it to the list

    // Add another number
    n = malloc(sizeof(node));
    if (n == NULL)
    {
        free(list->next);  // freeing the last node first
        free(list);  // free the first node at last (freeing this first node first won't allow freeing the linked node)
        return 1;
    }
    n->number = 3;
    n->next = NULL;
    list->next->next = n;  // adding to the list

    // Printing the numbers
    for (node *tmp = list; tmp != NULL; tmp = tmp->next)  // init tmp node to be list (first element pointer), condition: is this pointer is the last pointer?, update: assign tmp to the next node pointer
    {
        printf("%i\n", tmp->number);
    }

    // Free a whole linked list
    while (list != NULL)  // while not the last node
    {
        node *tmp = list->next;  // init tmp pointer to the next node
        free(list);  // free the current node (first to last during the loop)
        list = tmp;  // update the list pointer to be tmp
    }
}