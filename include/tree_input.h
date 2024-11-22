#ifndef TREE_INPUT_H
#define TREE_INPUT_H

#include "tree.h"

struct Parts_of_tree
{
    struct Node *root;
    struct Node *left;
    struct Node *right;
}

Errors_of_tree get_tree_from_file(struct Tree *tree, const char *file_source_name);
#endif
