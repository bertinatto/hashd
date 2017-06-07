#include <glib.h>

#include "../../src/hashd.h"

typedef struct {
    struct dict *dd;
} dict_fixture;

typedef struct {
    struct node *np;
} node_fixture;

/* test_node.c */
void node_setup(node_fixture*, gconstpointer);
void node_teardown(node_fixture*, gconstpointer);
void test_node_new(node_fixture*, gconstpointer);
void test_node_insert(node_fixture*, gconstpointer);
void test_node_delete(node_fixture*, gconstpointer);

/* test_dict.c */
void dict_setup(dict_fixture*, gconstpointer);
void dict_teardown(dict_fixture*, gconstpointer);
void assert_dict_has_default_kvs(struct dict*);
void assert_dict_dont_have_default_kvs(struct dict*);
void test_new(dict_fixture*, gconstpointer);
void test_insert(dict_fixture*, gconstpointer);
void test_delete(dict_fixture*, gconstpointer);
