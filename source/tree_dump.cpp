#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tree.h"
#include "tree_dump.h"


static void create_nodes_in_dump(struct Node *root, FILE *file_pointer);
static Errors_of_tree create_command_for_console(const char *file_in_name, const char *file_out_name);
static void create_connections(struct Node *root, FILE *file_pointer);
static Errors_of_tree check_command(const char *command, size_t size);
static size_t get_size_of_number(int number);
static char * get_type_name(Value_type type);
static void get_value(struct Value *value, char *str);
static char * get_operation(Operations operation);
static char * get_var(Variables var);

static Errors_of_tree create_command_for_console(const char *file_in_name, const char *file_out_name)
{
    if (file_in_name == NULL || file_out_name == NULL)
    {
        return ERROR_OF_DUMP;
    }

    char command_for_console[100] = "";
    snprintf(command_for_console, 100, "sudo dot -Tpng %s -o %s.png", file_in_name, file_out_name);
    Errors_of_tree error = check_command(command_for_console, 100);
    if (error != NO_ERRORS)
    {
        return error;
    }
    system(command_for_console);
    return NO_ERRORS;
}

static Errors_of_tree check_command(const char *command, size_t size)
{
    char not_dangerous_symbols[] = {'-', '.', '/', '_', '\0'};
    size_t size_of_not_dangersous_symbols = sizeof(not_dangerous_symbols) / sizeof(char);
    for (size_t i = 0; i < size; i++)
    {
        char symbol = command[i];
        if (!isalpha(symbol) && !isspace(symbol))
        {
            bool is_not_bad_symbol = false;
            for (size_t j = 0; j < size_of_not_dangersous_symbols; j++)
            {
                if (symbol == not_dangerous_symbols[j])
                {
                    is_not_bad_symbol = true;
                    break;
                }
            }
            if (!is_not_bad_symbol)
            {
                return ERROR_OF_DANGEROUS_COMMAND;
            }
        }
    }
    return NO_ERRORS;
}

static size_t get_size_of_number(int number)
{
    size_t ans = 0;
    while (number > 0)
    {
        number /= 10;
        ans++;
    }
    return ans;
}

static char * get_var(Variables var)
{
    switch(var)
    {
        case VAR_X:     return "x";
        case NOT_A_VAR: return "NOT A VARIABLE!";
        default:        return "UNKNOWN VARIABLE!";
    }
}

static char * get_operation(Operations operation)
{
    switch(operation)
    {
        case OP_ADD:    return "+";
        case OP_MUL:    return "*";
        case OP_SUB:    return "-";
        case OP_DIV:    return "/";
        case NOT_AN_OP: return "NOT AN OPERATION!";
        default:        return "UNKNOWN OPERATION";
    }
}

static char * get_type_name(Value_type type)
{
    switch(type)
    {
        case VARIABLE:  return "VARIABLE";
        case NUMBER:    return "NUMBER";
        case OPERATION: return "OPERATION";
        default:        return "UNKNOWN TYPE!";
    }
}

static void get_value(struct Value *value, char *str)
{
    if (value->type == NUMBER)
    {
        size_t size = get_size_of_number(value->number);
        char ans[100] = {0};
        sprintf(str, "%d", value->number);
        return;
    }
    else if (value->type == OPERATION)
    {
        char *ans = get_operation(value->operation);
        memcpy(str, ans, strlen(ans));
        return;
    }
    else if (value->type == VARIABLE)
    {
        char *ans = get_var(value->variable);
        memcpy(str, ans, strlen(ans));
        return;
    }
    else
    {
        str = "UNKNOWN VALUE!\0";
        return;
    }
}


static void create_nodes_in_dump(struct Node *root, FILE *file_pointer)
{
    if (root == NULL)
    {
        return;
    }


    if (root->left != NULL)
    {
        create_nodes_in_dump(root->left, file_pointer);
    }
    //printf("node = %p\n", root);
        //printf("node = %p\ndata = %s\n", root, str);
    char str[100] = {0};
    get_value(&(root->value), str);
    fprintf(file_pointer, "box%p "
                        "[shape = record,"
                        " label = \"{<node_par>parent = %p|<node_adr>address = %p|<node_t>type = %s|<node_v>value = %s|"
                        "{<node_l>left_node = %p|<node_r>right_node = %p}}\"];\n",
                        root, root->parent_node, root,
                        get_type_name((root->value).type),
                        str,
                        root->left, root->right);


    if (root->right != NULL)
    {
        create_nodes_in_dump(root->right, file_pointer);
    }
}

static void create_connections(struct Node *root, FILE *file_pointer)
{
    if (root == NULL)
    {
        return;
    }


    if (root->left != NULL)
    {
        fprintf(file_pointer, "box%p:<node_l>->box%p [color=red];\n", root, (root->left));
        create_connections(root->left, file_pointer);
    }


    if (root->right != NULL)
    {
        fprintf(file_pointer, "box%p:<node_r>->box%p [color=green];\n", root, (root->right));
        create_connections(root->right, file_pointer);
    }
}

void graphic_dump(struct Tree *tree, char *operation)
{
    if (tree == NULL || operation == NULL)
    {
        tree->error = ERROR_OF_DUMP;
        return;
    }
    tree->tmp_root = tree->root;
    const char file_name[] = "dump/dump.txt";
    FILE *file_pointer = fopen(file_name, "w");
    fprintf(file_pointer, "digraph Tree {\n");
    fprintf(file_pointer, "node [margin = \"0.01\"];\nrankdir = \"TB\";\n");

    if (file_pointer == NULL)
    {
        tree->error = ERROR_OF_DUMP;
        return;
    }

    char file_out_name[100] = "dump/";
    strncat(file_out_name, operation, 100);

    create_nodes_in_dump(tree->tmp_root, file_pointer);

    create_connections(tree->tmp_root, file_pointer);

    fprintf(file_pointer, "}\n");
    fclose(file_pointer);

    Errors_of_tree error = create_command_for_console(file_name, file_out_name);
    if (error != NO_ERRORS)
    {
        printf("Here\n");
        tree->error = ERROR_OF_DUMP;
        return;
    }
    return;
}


