#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char cipher(char letter, string key);

int main(int argc, string argv[])
{
    // Command line args limiting
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    int keylen = strlen(argv[1]);
    if (keylen != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    for (int i = 0; i < keylen; i++)
    {
        if (!isalpha(argv[1][i]))
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        for (int j = 0; j < keylen; j++)
        {
            if (argv[1][i] == argv[1][j] && i != j)
            {
                printf("Key must contain 26 characters.\n");
                return 1;
            }
        }
    }

    // Getting user input
    string plaintext = get_string("plaintext:  ");

    // Ciphering
    int textlen = strlen(plaintext);
    printf("ciphertext: ");
    for (int i = 0; i < textlen; i++)
    {
        char cipher_char;
        if (isalpha(plaintext[i]))
        {
            cipher_char = cipher(plaintext[i], argv[1]);
        }
        else
        {
            cipher_char = plaintext[i];
        }
        printf("%c", cipher_char);
    }
    printf("\n");
    return 0;
}

char cipher(char letter, string key)
{
    if (isupper(letter))
    {
        return toupper(key[letter % 'A']);
    }
    else
    {
        return tolower(key[letter % 'a']);
    }
}