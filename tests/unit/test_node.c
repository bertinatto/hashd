#include <stdlib.h>

#include "common.h"

void node_setup(node_fixture*, gconstpointer);
void node_teardown(node_fixture*, gconstpointer);
void test_node_new(node_fixture*, gconstpointer);
void test_node_insert(node_fixture*, gconstpointer);
void test_node_delete(node_fixture*, gconstpointer);

void
node_setup(node_fixture *nf, gconstpointer test_data)
{
    nf->np = node_new("x", "y");
}

void
node_teardown(node_fixture *nf, gconstpointer test_data)
{
    node_delete(nf->np);
}

void
test_node_new(node_fixture *nf, gconstpointer ignored)
{
    g_assert(nf->np != NULL);
    g_assert_cmpstr(nf->np->key, ==, "x");
    g_assert_cmpstr(nf->np->value, ==, "y");
    g_assert(node_new("x", "") != NULL);
}

void
test_node_insert(node_fixture *nf, gconstpointer ignored)
{
    struct node *np = node_insert(nf->np, "a", "b");
    g_assert(np != NULL);
    g_assert_cmpstr(nf->np->next->key, ==, "a");
    g_assert_cmpstr(nf->np->next->value, ==, "b");
    node_delete(np);
}

void
test_node_delete(node_fixture *nf, gconstpointer ignored)
{
    struct node *np0 = nf->np;
    struct node *np1 = node_insert(np0, "a", "b");
    struct node *np2 = node_insert(np0, "c", "d");

    g_assert(np1 != NULL);
    g_assert(np2 != NULL);
    
    struct node *np3 = node_delete(np1);
    g_assert_cmpstr(np3->key, ==, "c");
    g_assert_cmpstr(np3->value, ==, "d");
    g_assert_cmpstr(np0->next->key, ==, "c");
    g_assert_cmpstr(np0->next->value, ==, "d");
}

