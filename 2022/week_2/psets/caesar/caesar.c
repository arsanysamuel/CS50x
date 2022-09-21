#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool only_digits(string arg);
char rotate(char letter, int key);

int main(int argc, string argv[])
{
    // limiting arguments to 2 args and key to int
    if (argc != 2 || !only_digits(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int key = atoi(argv[1]);  // using the key
    string text = get_string("plaintext: ");  // getting the plain text
    int len = strlen(text);

    // Cyphering text
    printf("ciphertext: ");
    for (int i = 0; i < len; i++)
    {
        printf("%c", rotate(text[i], key));
    }
    printf("\n");
}

bool only_digits(string arg)
{
    // return true if the arg is only digits
    int len = strlen(arg);
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(arg[i]))
        {
            return false;
        }
    }
    return true;
}

char rotate(char letter, int key)
{
    // Cypher the letter using the key
    if (isalpha(letter))
    {
        if (isupper(letter))
        {
            char cypher = ((letter + key) % 'A') % 26 + 'A';
            return cypher;
        }
        else if (islower(letter))
        {
            char cypher = ((letter + key) % 'a') % 26 + 'a';
            return cypher;
        }
    }
    return letter;
}