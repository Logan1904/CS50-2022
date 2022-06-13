#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // Error-handling
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    } else
    {
        if (strlen(argv[1]) != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        } else
        {
            for (int i = 0, n = strlen(argv[1]); i < n; ++i)
            {

                if (argv[1][i] < 65 || (argv[1][i] > 90 && argv[1][i] < 97) || argv[1][i] > 122)
                {
                    printf("Key contains invalid charactes.\n");
                    return 1;
                }

                for (int j = i+1; j < n; ++j)
                {
                    if (argv[1][i] == argv[1][j])
                    {
                        printf("Key must not contain duplicate(s).\n");
                        return 1;
                    }
                }
            }
        }
    }

    // Convert key to upper-case
    string key = argv[1];
    for (int i = 0, n = strlen(key); i < n; ++i)
    {
        key[i] = toupper(key[i]);
    }

    // Convert plaintext to ciphertext
    string plaintext = get_string("plaintext: ");
    int n = strlen(plaintext);

    for (int i = 0; i < n; ++i)
    {
        if (plaintext[i] >= 65 && plaintext[i] <= 90)
        {
            plaintext[i] = key[plaintext[i] - 65];
        } else if (plaintext[i] >= 97 && plaintext[i] <= 122)
        {
            plaintext[i] = tolower(key[plaintext[i] - 97]);
        }
    }

    // Print ciphertext
    printf("ciphertext: %s\n", plaintext);

}