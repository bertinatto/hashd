#include <err.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "hashd.h"

unsigned int hash_func(const char*, int);
struct dict* dict_new(unsigned long);
int dict_insert(struct dict*, const char*, const char*);
int dict_delete(struct dict*, const char*);
struct node* dict_find(struct dict*, const char*);

/*DJB hash borrowed from an old Redis version*/
unsigned int
hash_func(const char *buf, int len)
{
    unsigned int hash = 5381;

    while (len--)
        hash = ((hash << 5) + hash) + (tolower(*buf++)); /* hash * 33 + c */
    return hash;
}

unsigned int
dict_get_index(struct dict *table, const char *buf, int len)
{
    return hash_func(buf, strlen(buf)) % table->size;
}

struct dict*
dict_new(unsigned long size)
{
    struct dict *table;

    if ((table = calloc(1, sizeof(struct dict))) == NULL) {
        err(1, NULL);
        return (NULL);
    }

    if ((table->nodes = calloc(size, sizeof(struct node*))) == NULL) {
        err(1, NULL);
        free(table);
        return (NULL);
    }
    
    table->size = size;
    table->used = 0;

    return (table);
}

int
dict_insert(struct dict *table, const char *key, const char *value)
{
    struct node *np;
    struct node *head;
    unsigned int hash;

    hash = dict_get_index(table, key, strlen(key));
    head = table->nodes[hash];

    if ((np = node_insert(head, key, value)) == NULL) {
        err(1, NULL);
        return (-1);
    }
    
    if (head == NULL)
        table->nodes[hash] = np;
    (table->used)++;
    return (1);
}

struct node*
dict_find(struct dict *table, const char *key)
{
    struct node *np;
    unsigned int hash;

    hash = dict_get_index(table, key, strlen(key));
    np = table->nodes[hash];
       
    while (np != NULL) {
        if (strcasecmp(np->key, key) == 0)
            return (np);
        np = np->next;                
    }

    return (NULL);
}

int
dict_delete(struct dict *table, const char *key)
{
    unsigned int hash;
    struct node *np;

    if ((np = dict_find(table, key)) == NULL) {
        return (0);
    }
    
    hash = dict_get_index(table, key, strlen(key));

    table->nodes[hash] = node_delete(np);
    
    return (1);
}

