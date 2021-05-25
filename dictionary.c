// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

int global_counter;

// Number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int number = hash(word);
    node *tmp = table[number];
    while (tmp != NULL)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    /***************************************************************************************
    *    This function has been written by another user and as such is cited below.
    *    Title: djb2
    *    Author: Dan Bernstein
    *    Availability: http://www.cse.yorku.ca/~oz/hash.html
    *
    ***************************************************************************************/
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // initializes a word counter
    int count = 0;
    // opens dictionary in a read mode
    FILE *file = fopen(dictionary, "r");
    // returns false if file is corrupted
    if (file == NULL)
    {
        global_counter = count;
        return false;
    }
    // scans file for strings and stores them in a char array
    char word[LENGTH + 1];
    // initializes all elements of table to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    // loop runs as long as fscanf doesn't return EOF
    while (fscanf(file, "%s", word) != EOF)
    {
        count++;
        // memory allocation of node size
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        // copies characters from current word into the current node's (->word) element
        strcpy(n->word, word);
        // initializes node's next and all table's elements to NULL in order to prevent possible errors
        n->next = NULL;
        // receives a hash key from hash function
        int number = hash(word);
        // if number'th element of table is already occupied, change node's next to where number'th element of table is pointing
        if (table[number] != NULL)
        {
            n->next = table[number];
        }
        // make number'th element of table point at where current node (n) is pointing
        table[number] = n;
    }
    global_counter = count;
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return global_counter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    int counter = 0;
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            while (table[i] != NULL)
            {
                node *tmp = table[i]->next;
                free(table[i]);
                table[i] = tmp;
            }
            counter++;
        }
    }
    if (counter > 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}
