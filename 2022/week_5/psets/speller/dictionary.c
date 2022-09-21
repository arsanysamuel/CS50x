// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
//const unsigned int N = 26;
const unsigned int N = 1129;  // prime number + slightly more than ('z' - 'a') * 45

// Hash table
node *table[N];

// Prototypes
void free_node(node *n);
unsigned int list_size(node *n);
bool in_dict(const char *word, node *n);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Calculate hash of the word
    unsigned int hash_code = hash(word);

    // Check if word in dictionary node
    return in_dict(word, table[hash_code]);
}

// Check if word in the dictionary bucket (recursively)
bool in_dict(const char *word, node *n)
{
    if (n == NULL)
    {
        return false;
    }
    else if (strcasecmp(word, n->word) == 0)
    {
        return true;
    }
    else
    {
        return in_dict(word, n->next);
    }
}

// Hashes word to a number, could be optimized
unsigned int hash(const char *word)
{
    // Return the hash code of the string
    unsigned int sum = 0, i = 0;

    while (word[i] != '\0')
    {
        sum += tolower(word[i]);
        i++;
    }

    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Opening dictionary file
    FILE *dict_file = fopen(dictionary, "r");
    if (dict_file == NULL)
    {
        return false;
    }

    // Initializing the table
    for (int i = 0; i > N; i++)
    {
        table[i]->next = NULL;
    }

    // Reading words (lines) from the file, hashing each word and appending to the table
    char *word = malloc(sizeof(char) * (LENGTH + 1));
    unsigned int hash_code, i = 0;
    char c;
    while (fread(&c, sizeof(char), 1, dict_file))  // While the file hasn't ended
    {
        if (isalpha(c) || (c == '\'' && i != 0))  //  Add letters and apostrophies only to word
        {
            word[i] = c;
            i++;
        }

        else if (c == '\n' && i > 0)  // Whole word (line) read
        {
            word[i] = '\0';  // End word
            i = 0;

            // Get hash
            hash_code = hash(word);

            // Insert word to the hash table: inserting a new node to the top of the bucket
            node *new = malloc(sizeof(node));
            strcpy(new->word, word);
            new->next = table[hash_code];
            table[hash_code] = new;
        }
    }

    // Closing the file and freeing memory
    fclose(dict_file);
    free(word);

    // Successfully read file
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    unsigned long n = 0;

    for (int i = 0; i < N; i++)
    {
        n += list_size(table[i]);
    }

    return n;
}

// Return the number of nodes in a linked-list
unsigned int list_size(node *n)
{
    //printf("%p\n", n);
    if (n == NULL)  // Base case
    {
        return 0;
    }
    return list_size(n->next) + 1;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        free_node(table[i]);
    }

    return true;
}

void free_node(node *n)
{
    if (n == NULL)  // Base case: no more nodes
    {
        return;
    }
    free_node(n->next);
    free(n);
}
