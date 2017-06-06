#include "common.h"

int
main(int argc, char **argv)
{
    g_test_init(&argc, &argv, NULL);
    
    /* node.c */
    g_test_add ("/node/new", node_fixture, NULL, node_setup, test_node_new, node_teardown);
    g_test_add ("/node/insert", node_fixture, NULL, node_setup, test_node_insert, node_teardown);
    g_test_add ("/node/delete", node_fixture, NULL, node_setup, test_node_delete, node_teardown);
    
    /* dict.c */
    g_test_add ("/dict/new", dict_fixture, NULL, dict_setup, test_new, dict_teardown);
    g_test_add ("/dict/insert", dict_fixture, NULL, dict_setup, test_insert, dict_teardown);
    g_test_add ("/dict/delete", dict_fixture, NULL, dict_setup, test_delete, dict_teardown);

    return g_test_run();
}

