#include <stdlib.h>
#include <string.h>
#include <err.h>

#include "hashd.h"

struct node* node_new(const char*, const char*);
struct node* node_insert(struct node*, const char*, const char*);
struct node* node_delete(struct node*);

struct node*
node_new(const char *key, const char *value)
{
    struct node *np;

    if (key == NULL)
        return (NULL);

    if ((np = calloc(1, sizeof(*np))) == NULL) {
        err(1, NULL);
        return (NULL);
    }

    np->key = strdup(key);
    np->value = strdup(value);

    return (np);
}

struct node*
node_insert(struct node *head, const char *key, const char *value)
{
    struct node *np;

    if (head == NULL)
        return node_new(key, value);

    while (head->next != NULL)
        head = head->next;

    if ((np = node_new(key, value)) == NULL)
        return (NULL);

    np->prev = head;
    head->next = np;

    return (np);
}

struct node*
node_delete(struct node *e)
{
    struct node *next;

    if (e == NULL) {
        errx(1, "cannot delete invalid node");
        return NULL;
    }
    
    next = e->next;

    if (e->prev != NULL) {
        e->prev->next = e->next;
    }

    if (e->next != NULL) {
        e->next->prev = e->prev;
    }

    free(e->key); 
    free(e->value); 
    free(e); 

    return (next);
}

