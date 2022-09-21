// Implement a list of numbers as a binary search tree

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Defining the node
typedef struct node
{
    int number;
    struct node *left;
    struct node *right;
}
node;

// Prototypes
void free_tree (node *root);
void print_tree (node *root);
bool search(node *tree, int number);  // unused here but for demonstration

int main(void)
{
    // Tree of size zero
    node *tree = NULL;

    // Adding a number to tree
    node *n = malloc(sizeof(node));
    if (n == NULL)
    {
        return 1;
    }
    n->number = 2;
    n->left = NULL;
    n->right = NULL;
    tree = n;  // adding the root node

    // Adding another number to tree
    n = malloc(sizeof(node));
    if (n == NULL)
    {
        return 1;
    }
    n->number = 1;
    n->left = NULL;
    n->right = NULL;
    tree->left = n;  // adding left of the root node

    // Adding another number to tree
    n = malloc(sizeof(node));
    if (n == NULL)
    {
        return 1;
    }
    n->number = 3;
    n->left = NULL;
    n->right = NULL;
    tree->right = n;

    // printing the tree
    print_tree(tree);

    // freeing the tree memory
    free_tree(tree);
}

void print_tree(node *root)
{
    // Printing the tree nodes one at a time (using recursion)
    if (root == NULL)  // Base case: no more nodes
    {
        return;
    }
    print_tree(root->left);  // recursing through the left branch
    printf("%i\n", root->number);  // printing the number of this node
    print_tree(root->right);  // recursing through the right branch
}

void free_tree(node *root)
{
    // free the whole tree (using recursion)
    if (root == NULL)  // Base case: no more nodes
    {
        return;
    }
    free_tree(root->left);  // recursively free the left branch
    free_tree(root->right);  // recursively free the right branch
    free(root);  // free the root node after recursively freeing children
}

bool search(node *tree, int number)
{
    // Recursively search a binary search sorted tree
    if (tree == NULL)  // Base case: no more nodes to search
    {
        return false;
    }
    else if (number < tree->number)
    {
        return search(tree->left, number);
    }
    else if (number > tree->number)
    {
        return search(tree->right, number);
    }
    else
    {
        return true;
    }
}