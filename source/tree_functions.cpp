#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "tree.h"

static void destructor_recursive(struct Node *root);
static void calculation_of_subtree(struct Node **root);
static void calculation_of_nodes(struct Value *left_value, struct Value *right_value, Operations operation, int *result);
static void differentiation_of_constant(struct Node **root);
static void differentiation_of_variable(struct Node **root);
static void differentiation_of_operation(struct Node **root);
static void differentiation_sum_sub(struct Node **root);
static void differentiation_mul(struct Node **root);
static void differentiation_mul_expressions(struct Node **root);
//static void differentiation_mul_expression_and_constant(struct Node **root);
static void differentiation_div(struct Node **root);
static void replace_node_with_number(struct Node **root, int number);
static bool try_differentiation_constant(struct Node **root);
static bool try_differentiation_variable(struct Node **root);
static bool try_differentiation_operation(struct Node **root);
static void try_differentiation_of_constant_and_variable(struct Node **root);
static struct Node* copy_node(struct Node *root, struct Node *parent);

static struct Node* copy_node(struct Node *root, struct Node *parent)
{
    if (root == NULL)
    {
        return NULL;
    }
    struct Node* new_node = (Node *) calloc(1, sizeof(Node));
    new_node->left = copy_node(root->left, root);
    new_node->right = copy_node(root->right, root);
    new_node->value = root->value;
    new_node->parent_node = parent;
    return new_node;
}



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

    struct Node *old_left = copy_node((*root)->left, *root);
    struct Node *old_right = copy_node((*root)->right, *root);
    struct Node *denominator_node_left = copy_node((*root)->right, *root);
    struct Node *denominator_node_right = copy_node((*root)->right, *root);
    differentiation((*root)->left);
    differentiation((*root)->right);
    struct Node *diffirentiation_left = copy_node((*root)->left, *root);
    struct Node *diffirentiation_right = copy_node((*root)->right, *root);
    struct Value new_root_value = {.type = OPERATION, .operation = OP_DIV};
    struct Value new_numerator_value = {.type = OPERATION, .operation = OP_SUB};
    struct Value new_denominator_value = {.type = OPERATION, .operation = OP_MUL};
    struct Value new_numerator_left_and_right_value = {.type = OPERATION, .operation = OP_MUL};
    struct Node *new_numerator_left_node = NULL;
    struct Node *new_numerator_right_node = NULL;
    struct Node *new_numerator_node = NULL;
    struct Node *new_denominator_node = NULL;
    Errors_of_tree error = NO_ERRORS;
    error = create_new_node(&new_numerator_left_node, &new_numerator_left_and_right_value, diffirentiation_left, old_right);
    if (error != NO_ERRORS)
    {
        return;
    }
    error = create_new_node(&new_numerator_right_node, &new_numerator_left_and_right_value, old_left, diffirentiation_right);
    if (error != NO_ERRORS)
    {
        return;
    }
    error = create_new_node(&new_numerator_node, &new_numerator_value, new_numerator_left_node, new_numerator_right_node);
    if (error != NO_ERRORS)
    {
        return;
    }
    error = create_new_node(&new_denominator_node, &new_denominator_value, denominator_node_left, denominator_node_right);
    if (error != NO_ERRORS)
    {
        return;
    }
    error = create_new_node(root, &new_root_value, new_numerator_node, new_denominator_node);
    return;
}

static void differentiation_sum_sub(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    bool attempt = true;
    if ((((*root)->left)->value).type == OPERATION)
    {
        attempt = try_differentiation_operation(&((*root)->left));
    }
    else
    {
        try_differentiation_of_constant_and_variable(&((*root)->left));
    }

    if ((((*root)->right)->value).type == OPERATION)
    {
        attempt = try_differentiation_operation(&((*root)->right));
    }
    else
    {
        try_differentiation_of_constant_and_variable(&((*root)->right));
    }
    calculation_of_subtree(root);
}

/*static void differentiation_mul_expression_and_constant(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    int result_differentiation = 0;
    bool attempt = true;
    if ((((*root)->left)->value).type == OPERATION)
    {
        attempt = try_differentiation_operation(&((*root)->left));
        if ((((*root)->right)->value).type == NUMBER)
        {
            calculation_of_subtree(root);
        }
    }
    else if ((((*root)->right)->value).type == OPERATION)
    {
        attempt = try_differentiation_operation(&((*root)->right));
        if ((((*root)->left)->value).type == NUMBER)
        {
            calculation_of_subtree(root);
        }
    }
    else
    {
        if ((((*root)->left)->value).type == NUMBER)
        {
            result_differentiation = (((*root)->left)->value).number;
        }
        else
        {
            result_differentiation = (((*root)->right)->value).number;
        }
        replace_node_with_number(root, result_differentiation);
    }
    return;
}*/

static void differentiation_mul_expressions(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    Errors_of_tree error = NO_ERRORS;
    struct Node *old_left = copy_node((*root)->left, *root);
    struct Node *old_right = copy_node((*root)->right, *root);
    differentiation(((*root)->left));
    differentiation(((*root)->right));
    struct Node *diffirentiation_left = copy_node((*root)->left, *root);
    struct Node *diffirentiation_right = copy_node((*root)->right, *root);
    struct Value new_root_value = {.type = OPERATION, .operation = OP_ADD};
    struct Value new_nodes_value = {.type = OPERATION, .operation = OP_MUL};
    struct Node *new_left_node = NULL;
    struct Node *new_right_node = NULL;
    error = create_new_node(&(new_left_node), &new_nodes_value, diffirentiation_left, old_right);
    if (error != NO_ERRORS)
    {
        return;
    }
    error = create_new_node(&(new_right_node), &new_nodes_value, old_left, diffirentiation_right);
    if (error != NO_ERRORS)
    {
        return;
    }
    error = create_new_node(root, &new_root_value, new_left_node, new_right_node);
    return;
}




static void differentiation_mul(struct Node **root)
{
    if ((*root) == NULL)
    {
        return;
    }
    // if ((((*root)->left)->value).type == NUMBER || (((*root)->right)->value).type == NUMBER)
    // {
    //     differentiation_mul_expression_and_constant(root);
    // }
    // else if (((((*root)->left)->value).type == VARIABLE && (((*root)->right)->value).type == VARIABLE) ||
    //          (((*root)->left)->value).type == OPERATION || (((*root)->right)->value).type == OPERATION)
    // {
    differentiation_mul_expressions(root);
    // else if ((((*root)->left)->value).type == VARIABLE && (((*root)->right)->value).type == NUMBER ||
    //          (((*root)->left)->value).type == NUMBER && (((*root)->right)->value).type == VARIABLE)
    // {
    //     differentiation_mul_variable_and_constant(root);
    // }
    if ((((*root)->left)->value).type == NUMBER && (((*root)->right)->value).type == NUMBER)
    {
        calculation_of_subtree(root);
        bool attempt = try_differentiation_constant(root);
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
    if ((*root)->left != NULL && (*root)->left != left)
    {
        // printf("left = %p\n", left);
        // printf("root = %p\n", *root);
        free((*root)->left);
        (*root)->left = left;
        if (left != NULL)
        {
            ((*root)->left)->parent_node = *root;
        }
    }
    else
    {
        (*root)->left = left;
        if (left != NULL)
        {
            ((*root)->left)->parent_node = *root;
        }
    }
    if ((*root)->right != NULL && (*root)->right != right)
    {
        free((*root)->right);
        (*root)->right = right;
        if (right != NULL)
        {
            ((*root)->right)->parent_node = *root;
        }
    }
    else
    {
        (*root)->right = right;
        if (right != NULL)
        {
            ((*root)->right)->parent_node = *root;
        }
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
    //printf("Here\n");
    if (root != NULL)
    {
        free(root);
    }
    root = NULL;
}
