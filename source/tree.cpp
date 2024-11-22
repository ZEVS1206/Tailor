#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "tree_dump.h"

int main()
{
    struct Tree tree = {0};
    Errors_of_tree error = NO_ERRORS;
    struct Value value1 = {.type = OPERATION, .operation = OP_ADD};
    struct Value value2 = {.type = NUMBER, .number = 3};
    struct Value value3 = {.type = NUMBER, .number = 5};
    error = create_new_node(&(tree.root), &value1, NULL, NULL);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    error = create_new_node(&((tree.root)->left), &value2, NULL, NULL);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }

    error = create_new_node(&((tree.root)->right), &value3, NULL, NULL);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    tree.error = NO_ERRORS;
    tree.tmp_root = tree.root;
    graphic_dump(&tree, "example");
    if (tree.error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", tree.error);
        return 1;
    }
    free((tree.root)->right);
    free((tree.root)->left);
    free(tree.root);
    return 0;
}
