#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

static void destructor_recursive(struct Node *root);
static void calculation_of_subtree(struct Node **root);
static void calculate_nodes(struct Value *left_value, struct Value *right_value, Operations operation, int *result);
static void differentiation_of_constant_and_variable(struct Node **root);
static void differentiation_of_operation(struct Node **root);
static void differentiation_sum_sub(struct Node **root);
static void differentiation_mul(struct Node **root);
static void differentiation_div(struct Node **root);


static void differentiation_div(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    struct Node *old_left = (*root)->left;
    struct Node *old_right = (*root)->right;
    differentiation_of_constant_and_variable(&((*root)->left));
    differentiation_of_constant_and_variable(&((*root)->right));
    int result = ((((*root)->left)->value).number * (old_right->value).number -
                  (old_left->value).number * (((*root)->right)->value).number) / ((old_right->value).number *
                                                                                  (old_right->value).number);
    struct Value new_value = {.type = NUMBER, .number = result};
    Errors_of_tree error = create_new_node(root, &new_value, NULL, NULL);
    return;
}

static void differentiation_sum_sub(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    differentiation_of_constant_and_variable(&((*root)->left));
    differentiation_of_constant_and_variable(&((*root)->right));
    calculation_of_subtree(root);
}

static void differentiation_mul(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    struct Node *old_left = (*root)->left;
    struct Node *old_right = (*root)->right;
    differentiation_of_constant_and_variable(&((*root)->left));
    differentiation_of_constant_and_variable(&((*root)->right));
    int result = (((*root)->left)->value).number * (old_right->value).number +
                 (old_left->value).number * (((*root)->right)->value).number;
    struct Value new_value = {.type = NUMBER, .number = result};
    Errors_of_tree error = create_new_node(root, &new_value, NULL, NULL);
}


static void differentiation_of_operation(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    switch (((*root)->value).operation)
    {
        case OP_ADD: differentiation_sum_sub(root); break;
        case OP_SUB: differentiation_sum_sub(root); break;
        case OP_MUL: differentiation_mul(root); break;
        case OP_DIV: differentiation_div(root); break;
        default: break;
    }
    return;
}


static void differentiation_of_constant_and_variable(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    int result_differentiation = 0;
    if (((*root)->value).type == VARIABLE)
    {
        result_differentiation = 1;
    }
    struct Value new_value = {.type = NUMBER, .number = result_differentiation};
    Errors_of_tree error = create_new_node(root, &new_value, NULL, NULL);
    return;
}

void differentiation(struct Node *root)
{
    if (root == NULL)
    {
        return;
    }
    if ((root->value).type == NUMBER || (root->value).type == VARIABLE)
    {
        differentiation_of_constant_and_variable(&root);
    }
    else if ((root->value).type == OPERATION)
    {
        differentiation_of_operation(&root);
    }
}

static void calculate_nodes(struct Value *left_value, struct Value *right_value, Operations operation, int *result)
{
    if (left_value == NULL || right_value == NULL)
    {
        return;
    }
    switch (operation)
    {
        case OP_ADD: *result = left_value->number + right_value->number; break;
        case OP_SUB: *result = left_value->number - right_value->number; break;
        case OP_MUL: *result = left_value->number * right_value->number; break;
        case OP_DIV: *result = left_value->number / right_value->number; break;
        default: break;
    }
    return;
}


static void calculation_of_subtree(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    if (((*root)->value).type == OPERATION)
    {
        if ((*root)->left != NULL && (*root)->right != NULL && (((*root)->left)->value).type == NUMBER
                                                            && (((*root)->right)->value).type == NUMBER)
        {
            int result = 0;
            calculate_nodes(&(((*root)->left)->value), &(((*root)->right)->value), ((*root)->value).operation, &result);
            struct Value new_value = {.type = NUMBER, .number = result};
            Errors_of_tree error = create_new_node(root, &new_value, NULL, NULL);
        }
    }
    return;
}

void calculation_of_tree(struct Node *root)
{
    if (root == NULL)
    {
        return;
    }
    calculation_of_tree(root->left);
    calculation_of_tree(root->right);
    calculation_of_subtree(&root);
}


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
    // (*root)->left = left;
    // (*root)->right = right;
    if (left == NULL && (*root)->left != NULL)
    {
        free((*root)->left);
        (*root)->left = left;
    }
    if (right == NULL && (*root)->right != NULL)
    {
        free((*root)->right);
        (*root)->right = right;
    }
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
