#include <stdlib.h>

#include "common.h"

void dict_setup(dict_fixture*, gconstpointer);
void dict_teardown(dict_fixture*, gconstpointer);
void test_new(dict_fixture*, gconstpointer);
void test_insert(dict_fixture*, gconstpointer);
void test_delete(dict_fixture*, gconstpointer);
void assert_dict_has_default_kvs(struct dict*);
void assert_dict_dont_have_default_kvs(struct dict*);

void
dict_setup(dict_fixture *df, gconstpointer test_data)
{
    df->dd = dict_new(1000);
}

void
dict_teardown(dict_fixture *df, gconstpointer test_data)
{
    free(df->dd);
}

void
assert_dict_has_default_kvs(struct dict *d)
{
    struct node *node1 = dict_find(d, "key1");
    struct node *node2 = dict_find(d, "key2");
    g_assert_cmpstr(node1->key, ==, "key1");
    g_assert_cmpstr(node1->value, ==, "val1");
    g_assert_cmpstr(node2->key, ==, "key2");
    g_assert_cmpstr(node2->value, ==, "");
}

void
assert_dict_dont_have_default_kvs(struct dict *d)
{
    g_assert(dict_find(d, "key1") == NULL);
    g_assert(dict_find(d, "key2") == NULL);
}

void
test_new(dict_fixture *df, gconstpointer ignored)
{
    g_assert(df->dd != NULL);
    assert_dict_dont_have_default_kvs(df->dd);
}

void
test_insert(dict_fixture *df, gconstpointer ignored)
{
    /*Valid input*/
    g_assert_cmpint(dict_insert(df->dd, "key1", "val1"), ==, 1);
    g_assert_cmpint(dict_insert(df->dd, "key2", ""), ==, 1);
    /*Invalid input*/
    g_assert_cmpint(dict_insert(df->dd, "key3", NULL), ==, -1);
    g_assert_cmpint(dict_insert(df->dd, NULL, "val3"), ==, -1);

    assert_dict_has_default_kvs(df->dd);
}

void
test_delete(dict_fixture *df, gconstpointer ignored)
{
    assert_dict_dont_have_default_kvs(df->dd);
    dict_insert(df->dd, "key1", "val1");
    dict_insert(df->dd, "key2", "");
    assert_dict_has_default_kvs(df->dd);
    dict_delete(df->dd, "key1");
    dict_delete(df->dd, "key2");
    assert_dict_dont_have_default_kvs(df->dd);
}
