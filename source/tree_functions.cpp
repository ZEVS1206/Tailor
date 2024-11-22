#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

Errors_of_tree create_new_node(struct Node **root, struct Value *value, struct Node *left, struct Node *right)
{
    if (*root == NULL)
    {
        *root = (Node *) calloc(1, sizeof(Node));
        if (*root == NULL)
        {
            return ERROR_OF_PUSH;
        }
    }
    (*root)->value = *value;
    (*root)->left = left;
    (*root)->right = right;
    return NO_ERRORS;
}


