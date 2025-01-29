#ifndef TREE_H
#define TREE_H

#include <stdarg.h>
#include <math.h>

//#define DEBUG

#ifdef DEBUG
#define ON_DEBUG(...) __VA_ARGS__
#else
#define ON_DEBUG(...)
#endif

enum Mods_of_start
{
    UNKNOWN_MODE     = 0,
    DIFFERENTIATION  = 1,
    CALCULATION      = 2,
    TEST             = 3
};

enum Errors_of_tree
{
    NO_ERRORS                            =  0,
    ERROR_OF_PUSH                        =  1,
    ERROR_OF_DUMP                        =  2,
    ERROR_OF_DESTRUCTOR                  =  3,
    ERROR_OF_READ_TREE                   =  4,
    ERROR_OF_READ_FROM_FILE              =  5,
    ERROR_OF_CONSTRUCTOR                 =  6,
    ERROR_OF_RUN                         =  7,
    ERROR_OF_NO_DEFINITION               =  8,
    ERROR_OF_DANGEROUS_COMMAND           =  9,
    ERROR_OF_LATEX_DUMP                  = 10,
    ERROR_OF_CREATE_OPERATIONS_INTERFACE = 11
};

enum Function_name
{
    NOT_A_FUNC = 0,
    FUNC_SIN   = 1,
    FUNC_COS   = 2
};


enum Value_type
{
    UNKNOWN_TYPE = 0,
    VARIABLE     = 1,
    NUMBER       = 2,
    OPERATION    = 3,
    FUNCTION     = 4
};

enum Variables
{
    NOT_A_VAR = 0,
    VAR_X     = 1
};

enum Operations
{
    NOT_AN_OP = 0,
    OP_ADD    = 1,
    OP_MUL    = 2,
    OP_SUB    = 3,
    OP_DIV    = 4,
    OP_DEG    = 5
};

struct Value
{
    Value_type type;
    union
    {
        double number;
        Operations operation;
        Variables variable;
        Function_name function;
    };
};


struct Operation_interface
{
    void (*symplifying_tree)    (struct Node **root, FILE *file_pointer);
    void (*differentiation)     (struct Node **root, FILE *file_pointer);
    void (*calculation_of_tree) (struct Node **root, FILE *file_pointer);
    Operations operation_type;
};

struct Function
{
    char function_str_name[100];
    Function_name function_name;
    double (*proccess_function) (double parametr);
};

struct Function_interface
{
    Function_name function_name;
    void (*symplifying_tree)    (struct Node **root, FILE *file_pointer);
    void (*differentiation)     (struct Node **root, FILE *file_pointer);
    void (*calculation_of_tree) (struct Node **root, FILE *file_pointer);
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

const struct Function G_functions[] = {{"sin", FUNC_SIN, sin},
                                       {"cos", FUNC_COS, cos}};
const size_t size_of_functions = sizeof(G_functions) / sizeof(Function);


Errors_of_tree tree_constructor(struct Tree *tree);
Errors_of_tree tree_destructor(struct Tree *tree);
void differentiation(struct Node *root, FILE *file_pointer);
void calculation_of_tree(struct Node *root, FILE *file_pointer);
Errors_of_tree create_new_node(struct Node **root, struct Value *value, struct Node *left, struct Node *right);
//Errors_of_tree create_operations_interface(struct List_of_operations *interface, char *all_operations);
struct Node* copy_node(struct Node *root, struct Node *parent);
void symplifying_tree(struct Node **root, FILE *file_pointer);
#endif
