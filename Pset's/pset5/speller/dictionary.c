// Implements a dictionary's functionality

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = LENGTH * 7;

// when you load words, keep track of them
int words_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // hash word to obtain hash value
    int index = hash(word);
    
    // access linked list at that index in hash table
    node *cursor = table[index];
    
    // keep moving cursor untill u get to NULL, checking each node for the word
    while (cursor != NULL)
    {
        // traverse linked list looking for the word (strcasecmp)
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int value = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        // make it case insensitive
        value += tolower(word[i]);
    }
    return value % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *file = fopen(dictionary, "r");
    
    if (file == NULL)
    {
        return false;
    }
    
    // read strings from file one at a time
    char word[45];
    
    while (fscanf(file, "%s", word) != EOF)
    {
        // create a new word for each node using malloc
        node *new_node = malloc(sizeof(node));
        
        if (new_node == NULL)
        {
            return false;
        }
        
        // copy word into node use strcpy
        strcpy(new_node->word, word);
        new_node->next = NULL;
        
        // hash word to obtain a hash value, function takes a string and returns an index
        int index = hash(word);
        
        // insert node into hash table at that location
        if (table[index] == NULL)
        {
            table[index] = new_node;
        }
        else
        {
            new_node->next = table[index];
            table[index] = new_node;
        }
        // counting words for "size" function below
        words_count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return words_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // set tmp = cursor; move cursor; free temp; repeat
    for (int i = 0; i < N; i++)
    {
        node *head = table[i];
        node *cursor = head;
        node *temp = head;
        
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(temp);
            temp = cursor;
        }
    }
    return true;
}
