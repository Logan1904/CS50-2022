// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 100000;

// Initialise number of dictionary words
unsigned int count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash the word to check
    unsigned int index = hash(word);

    // Set a pointer to point at the head of the linked list at the hash value
    node *my_pointer = table[index];

    // Iterate through the linked list
    while(my_pointer != NULL)
    {
        if(strcasecmp(word,my_pointer->word) == 0)
        {
            return true;
        }

        my_pointer = my_pointer->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Custom hash function using prime numbers

    unsigned long hash = 5381;
    int c;

    while ((c = toupper(*word++)))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *my_dict = fopen(dictionary, "r");

    // Check if dictionary opened ok
    if(my_dict == NULL)
    {
        return false;
    }

    // Scan dictionary
    char scanned_word[LENGTH+1];
    while(fscanf(my_dict, "%s", scanned_word) != EOF)
    {
        // Create node to store scanned word
        node *tmp = malloc(sizeof(node));
        if (tmp == NULL)
        {
            printf("Malloc failed\n");
            return false;
        }

        // Populate node with scanned word
        strcpy(tmp->word,scanned_word);

        // Get hash index number
        unsigned int index = hash(scanned_word);

        // Assign node pointer to table head pointer
        tmp->next = table[index];

        // Reassign table head pointer to node
        table[index] = tmp;

        // Add to count
        count++;
    }

    // Close file
    fclose(my_dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate through buckets in hash table
    for (int i = 0; i < N; ++i)
    {
        // Set pointer to head of linked list
        node *my_pointer = table[i];

        // Iterate and free linked list
        while(my_pointer != NULL)
        {
            node *tmp = my_pointer;
            my_pointer = my_pointer->next;
            free(tmp);
        }

        // Check if we have reached end of bucket & pointer is NULL
        if (i == N-1 && my_pointer == NULL)
        {
            return true;
        }
    }

    return false;
}
