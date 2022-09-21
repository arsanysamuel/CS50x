#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Getting user input
    string text = get_string("Text: ");

    // Calculating characters, words and sentences
    int chars = count_letters(text);
    int words = count_words(text);
    int sents = count_sentences(text);

    // Calculating Coleman-Liau index
    float L = (float) chars / (float) words * 100;
    float S = (float) sents / (float) words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);

    // Printing out the result
    if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

int count_letters(string text)
{
    // Calculate letters in text
    int len = strlen(text);
    int chars = 0;
    for (int i = 0; i < len; i++)
    {
        if (isalpha(text[i]))
        {
            chars++;
        }
    }
    return chars;
}

int count_words(string text)
{
    // Calculate words in text
    int len = strlen(text);
    int words = 0;
    for (int i = 0; i < len; i++)
    {
        if ((text[i] == ' ' || text[i] == '.' || text[i] == '?' || text[i] == '!') && text[i + 1] != ' ')
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string text)
{
    // Calculate sentences in text
    int len = strlen(text);
    int sents = 0;
    for (int i = 0; i < len; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sents++;
        }
    }
    return sents;
}