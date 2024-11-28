#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

static void destructor_recursive(struct Node *root);

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


Errors_of_tree tree_constructor(struct Tree *tree)
{
    if (tree == NULL)
    {
        return ERROR_OF_CONSTRUCTOR;
    }
    tree->error = NO_ERRORS;
    tree->root = (Node *) calloc(1, sizeof(Node));
    if (tree->root == NULL)
    {
        return ERROR_OF_CONSTRUCTOR;
    }
    tree->tmp_root = tree->root;
    return NO_ERRORS;
}

Errors_of_tree tree_destructor(struct Tree *tree)
{
    if (tree == NULL)
    {
        return ERROR_OF_DESTRUCTOR;
    }
    destructor_recursive(tree->root);
    if (tree->error != NO_ERRORS)
    {
        return tree->error;
    }
    return NO_ERRORS;
}


static void destructor_recursive(struct Node *root)
{
    if (root == NULL)
    {
        return;
    }

    destructor_recursive(root->left);
    destructor_recursive(root->right);
    free(root);
}
