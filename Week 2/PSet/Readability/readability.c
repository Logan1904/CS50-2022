#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string);
int count_words(string);
int count_sentences(string);

int main(void)
{
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    float L = (float) letters  / words * 100;
    float S = (float) sentences / words * 100;

    float index = 0.0588 * L - 0.296 * S - 15.8;

    if (index >= 16)
    {
        printf("Grade 16+\n");
    } else if (index < 1)
    {
        printf("Before Grade 1\n");
    } else
    {
        int new_index = round(index);
        printf("Grade %i\n", new_index);
    }
}

int count_letters(string text)
{
    int letters = 0;
    int n = strlen(text);

    for (int i = 0; i < n; ++i)
    {
        text[i] = toupper(text[i]);

        if (text[i] >= 65 && text[i] <=90)
        {
            letters++;
        }
    }

    return letters;
}

int count_words(string text)
{
    int words = 1;
    int n = strlen(text);

    for (int i = 0; i < n; ++i)
    {
        if (text[i] == ' ')
        {
            words++;
        }
    }

    return words;
}

int count_sentences(string text)
{
    int sentences = 0;
    int n = strlen(text);

    for (int i = 0; i < n; ++i)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }

    return sentences;
}