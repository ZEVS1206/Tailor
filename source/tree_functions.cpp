#include <stdio.h>
#include <stdlib.h>

#include "tree.h"

static void destructor_recursive(struct Node *root);
static void calculation_of_subtree(struct Node **root);
static void calculation_of_nodes(struct Value *left_value, struct Value *right_value, Operations operation, int *result);
static void differentiation_of_constant(struct Node **root);
static void differentiation_of_variable(struct Node **root);
static void differentiation_of_operation(struct Node **root);
static void differentiation_sum_sub(struct Node **root);
static void differentiation_mul(struct Node **root);
static void differentiation_mul_variables(struct Node **root);
static void differentiation_mul_variable_and_constant(struct Node **root);
static void differentiation_div(struct Node **root);
static void replace_node_with_number(struct Node **root, int number);
static bool try_differentiation_constant(struct Node **root);
static bool try_differentiation_variable(struct Node **root);
static bool try_differentiation_operation(struct Node **root);
static void try_differentiation_of_constant_and_variable(struct Node **root);


static void replace_node_with_number(struct Node **root, int number)
{
    if ((*root) == NULL)
    {
        return;
    }
    struct Value new_value = {.type = NUMBER, .number = number};
    Errors_of_tree error = create_new_node(root, &new_value, NULL, NULL);
    return;
}

static void try_differentiation_of_constant_and_variable(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    bool attempt = try_differentiation_constant(root);
    if (!attempt)
    {
        attempt = try_differentiation_variable(root);
    }
    return;
}

static void differentiation_div(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    struct Node *old_left = (*root)->left;
    struct Node *old_right = (*root)->right;
    try_differentiation_of_constant_and_variable(&((*root)->left));
    try_differentiation_of_constant_and_variable(&((*root)->right));
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
    try_differentiation_of_constant_and_variable(&((*root)->left));
    try_differentiation_of_constant_and_variable(&((*root)->right));
    calculation_of_subtree(root);
}

static void differentiation_mul_variable_and_constant(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }

    int result_differentiation = 0;
    if ((((*root)->left)->value).type == NUMBER)
    {
        result_differentiation = (((*root)->left)->value).number;
    }
    else
    {
        result_differentiation = (((*root)->right)->value).number;
    }
    replace_node_with_number(root, result_differentiation);
    return;
}


static void differentiation_mul_variables(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    struct Node *old_left = (Node *) calloc(1, sizeof(Node));
    struct Node *old_right = (Node *) calloc(1, sizeof(Node));
    old_left->value = (*root)->left->value;
    old_right->value = (*root)->right->value;
    old_left->parent_node = (*root)->left->parent_node;
    old_right->parent_node = (*root)->right->parent_node;
    bool attempt = try_differentiation_variable(&(old_left));
    attempt = try_differentiation_variable(&(old_right));
    struct Node *new_left_node = (Node *) calloc(1, sizeof(Node));
    struct Node *new_right_node = (Node *) calloc(1, sizeof(Node));
    if (new_left_node == NULL || new_right_node == NULL)
    {
        return;
    }
    new_left_node->parent_node = (*root);
    new_right_node->parent_node = (*root);
    new_left_node->left = (Node *) calloc(1, sizeof(Node));
    new_left_node->right = (Node *) calloc(1, sizeof(Node));
    new_right_node->left = (Node *) calloc(1, sizeof(Node));
    new_right_node->right = (Node *) calloc(1, sizeof(Node));
    struct Value new_root_value = {.type = OPERATION, .operation = OP_ADD};
    (new_left_node->value).type = OPERATION;
    (new_left_node->value).operation = OP_MUL;
    (new_right_node->value).type = OPERATION;
    (new_right_node->value).operation = OP_MUL;
    (((new_left_node)->left)->value).type = NUMBER;
    (((new_left_node)->left)->value).number = ((old_left)->value).number;
    (((new_left_node)->right)->value).type = VARIABLE;
    (((new_left_node)->right)->value).variable = (((*root)->right)->value).variable;
    (((new_right_node)->right)->value).type = NUMBER;
    (((new_right_node)->right)->value).number = ((old_right)->value).number;
    (((new_right_node)->left)->value).type = VARIABLE;
    (((new_right_node)->left)->value).variable = (((*root)->left)->value).variable;
    Errors_of_tree error = create_new_node(root, &new_root_value, new_left_node, new_right_node);
    free(old_left);
    free(old_right);
    return;
}

static void differentiation_mul(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    if ((((*root)->left)->value).type == VARIABLE && (((*root)->right)->value).type == VARIABLE)
    {
        differentiation_mul_variables(root);
    }
    else
    {
        differentiation_mul_variable_and_constant(root);
    }
    return;
}


static void differentiation_of_operation(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    switch (((*root)->value).operation)
    {
        case OP_ADD: differentiation_sum_sub    (root); break;
        case OP_SUB: differentiation_sum_sub    (root); break;
        case OP_MUL: differentiation_mul        (root); break;
        case OP_DIV: differentiation_div        (root); break;
        default: break;
    }
    return;
}

static bool try_differentiation_constant(struct Node **root)
{
    if ((*root) == NULL)
    {
        return false;
    }
    if (((*root)->value).type == NUMBER)
    {
        differentiation_of_constant(root);
        return true;
    }
    return false;
}

static bool try_differentiation_variable(struct Node **root)
{
    if ((*root) == NULL)
    {
        return false;
    }
    if (((*root)->value).type == VARIABLE)
    {
        differentiation_of_variable(root);
        return true;
    }
    return false;
}

static bool try_differentiation_operation(struct Node **root)
{
    if ((*root) == NULL)
    {
        return false;
    }
    if (((*root)->value).type == OPERATION)
    {
        differentiation_of_operation(root);
        return true;
    }
    return false;
}

static void differentiation_of_constant(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    if (((*root)->value).type == NUMBER)
    {
        replace_node_with_number(root, 0);
    }
    return;
}

static void differentiation_of_variable(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    if (((*root)->value).type == VARIABLE)
    {
        replace_node_with_number(root, 1);
    }
    return;
}


void differentiation(struct Node *root)
{
    if (root == NULL)
    {
        return;
    }
    bool attempt = try_differentiation_constant(&root);
    if (attempt)
    {
        return;
    }
    attempt = try_differentiation_variable(&root);
    if (attempt)
    {
        return;
    }
    attempt = try_differentiation_operation(&root);
}

static void calculation_of_nodes(struct Value *left_value, struct Value *right_value, Operations operation, int *result)
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
        if ((*root)->left != NULL && (*root)->right != NULL && (((*root)->left )->value).type == NUMBER
                                                            && (((*root)->right)->value).type == NUMBER)
        {
            int result = 0;
            calculation_of_nodes(&(((*root)->left)->value), &(((*root)->right)->value), ((*root)->value).operation, &result);
            replace_node_with_number(root, result);
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
    if ((*root)->left != NULL)
    {
        free((*root)->left);
        (*root)->left = left;
        ((*root)->left)->parent_node = *root;
    }
    if ((*root)->right != NULL)
    {
        free((*root)->right);
        (*root)->right = right;
        ((*root)->right)->parent_node = *root;
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
