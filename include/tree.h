#ifndef TREE_H
#define TREE_H

#include <stdarg.h>

//#define DEBUG

#ifdef DEBUG
#define ON_DEBUG(...) __VA_ARGS__
#else
#define ON_DEBUG(...)
#endif

enum Errors_of_tree
{
    NO_ERRORS                  = 0,
    ERROR_OF_PUSH              = 1,
    ERROR_OF_DUMP              = 2,
    ERROR_OF_DESTRUCTOR        = 3,
    ERROR_OF_READ_TREE         = 4,
    ERROR_OF_READ_FROM_FILE    = 5,
    ERROR_OF_CONSTRUCTOR       = 6,
    ERROR_OF_RUN               = 7,
    ERROR_OF_NO_DEFINITION     = 8,
    ERROR_OF_DANGEROUS_COMMAND = 9
};

enum Value_type
{
    UNKNOWN_TYPE = 0,
    VARIABLE     = 1,
    NUMBER       = 2,
    OPERATION    = 3
};

enum Variables
{
    VAR_X     = 0,
    NOT_A_VAR = 1
};

enum Operations
{
    NOT_AN_OP = 0,
    OP_ADD    = 1,
    OP_MUL    = 2,
    OP_SUB    = 3,
    OP_DIV    = 4
};

struct Value
{
    Value_type type;
    union
    {
        int number;
        Operations operation;
        Variables variable;
    };
};

struct Node
{
    struct Value value;
    struct Node *parent_node;
    struct Node *left;
    struct Node *right;
};

struct Tree
{
    struct Node *root;
    struct Node *tmp_root;
    Errors_of_tree error;
};

Errors_of_tree tree_constructor(struct Tree *tree);
Errors_of_tree tree_destructor(struct Tree *tree);

Errors_of_tree create_new_node(struct Node **root, struct Value *value, struct Node *left, struct Node *right);

#endif
