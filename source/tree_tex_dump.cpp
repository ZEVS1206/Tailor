#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "tree_tex_dump.h"

static void latex_bypass_of_tree(struct Node *root, FILE *file_pointer);
static void analyze_root(struct Node *root, FILE *file_pointer);
static void print_fraction(struct Node *root, FILE *file_pointer);
static void print_mul(struct Node *root, FILE *file_pointer);
static void print_add_or_sub(struct Node *root, FILE *file_pointer);
static void print_pow(struct Node *root, FILE *file_pointer);
static void print_one_node(struct Node *root, FILE *file_pointer);


const char * transform_variable_to_str(struct Value value)
{
    switch(value.variable)
    {
        case VAR_X:     return "x";    break;
        case NOT_A_VAR: return "NOT A VAR"; break;
    }
    return "NOT A VAR";
}

const char transform_operation_to_str(struct Value value)
{
    switch (value.operation)
    {
        case OP_ADD:    return '+';  break;
        case OP_SUB:    return '-';  break;
        case OP_MUL:    return '*';  break;
        case OP_DIV:    return '/';  break;
        case OP_DEG:    return '^';  break;
        case NOT_AN_OP: return '\n'; break;
        default:        return '\0'; break;
    }
    return '\n';
}


void special_latex_dump(const char *str, FILE *file_pointer, const char *action)
{
    if (file_pointer == NULL)
    {
        return;
    }
    fprintf(file_pointer, "%s:  \\texbf{\\large %s \\normalsize}\n", action, str);
    return;
}

static void print_one_node(struct Node *root, FILE *file_pointer)
{
    if (root == NULL || file_pointer == NULL)
    {
        return;
    }
    // if ((root->parent_node) != NULL)
    // {
    //     return;
    // }
    if ((root->value).type == NUMBER && (root->parent_node) == NULL)
    {
        fprintf(file_pointer, " %d\\\\\n", ((root)->value).number);
    }
    // else if ((root->value).type == VARIABLE)
    // {
    //     fprintf(file_pointer, " %s\\\\\n", transform_variable_to_str(root->value));
    // }
    return;
}

static void print_mul(struct Node *root, FILE *file_pointer)
{
    if (root == NULL || file_pointer == NULL)
    {
        return;
    }
    fprintf(file_pointer, "\\begin{equation}\n");
    if (((root->left)->value).type == NUMBER)
    {
        fprintf(file_pointer, " %d ", ((root->left)->value).number);
    }
    else if (((root->left)->value).type == VARIABLE)
    {
        fprintf(file_pointer, " %s ", transform_variable_to_str((root->left)->value));
    }
    // else if (((root->left)->value).type == OPERATION)
    // {
    //     fprintf(file_pointer, " %c ", transform_operation_to_str((root->left)->value));
    // }

    fprintf(file_pointer, "\\cdot");

    if (((root->right)->value).type == NUMBER)
    {
        fprintf(file_pointer, " %d ", ((root->right)->value).number);
    }
    else if (((root->right)->value).type == VARIABLE)
    {
        fprintf(file_pointer, " %s ", transform_variable_to_str((root->right)->value));
    }
    // else if (((root->left)->value).type == OPERATION)
    // {
    //     fprintf(file_pointer, " %c ", transform_operation_to_str((root->right)->value));
    // }

    fprintf(file_pointer, "\n \\end{equation}\n");
    // fprintf(file_pointer, "\\begin{equation}\n %d \\cdot %d\n \\end{equation}\n",
    //                       ((root->left)->value).number,
    //                       ((root->right)->value).number);
    return;
}

static void print_fraction(struct Node *root, FILE *file_pointer)
{
    if (root == NULL || file_pointer == NULL)
    {
        return;
    }
    fprintf(file_pointer, "\\begin{equation}\n\\frac{");
    if (((root->left)->value).type == NUMBER)
    {
        fprintf(file_pointer, " %d }", ((root->left)->value).number);
    }
    else if (((root->left)->value).type == VARIABLE)
    {
        fprintf(file_pointer, " %s }", transform_variable_to_str((root->left)->value));
    }
    // else if (((root->left)->value).type == OPERATION)
    // {
    //     fprintf(file_pointer, " %c ", transform_operation_to_str((root->left)->value));
    // }

    if (((root->right)->value).type == NUMBER)
    {
        fprintf(file_pointer, "{ %d", ((root->right)->value).number);
    }
    else if (((root->right)->value).type == VARIABLE)
    {
        fprintf(file_pointer, "{ %s", transform_variable_to_str((root->right)->value));
    }
    // else if (((root->left)->value).type == OPERATION)
    // {
    //     fprintf(file_pointer, " %c ", transform_operation_to_str((root->left)->value));
    // }

    fprintf(file_pointer, "}\n \\end{equation}\n");
    // fprintf(file_pointer, "\\begin{equation}\n\\frac{ %d }{ %d }\n \\end{equation}\n",
    //                       ((root->left)->value).number,
    //                       ((root->right)->value).number);
    return;
}

static void print_pow(struct Node *root, FILE *file_pointer)
{
    if (root == NULL || file_pointer == NULL)
    {
        return;
    }
    fprintf(file_pointer, "\\begin{equation}\n");
    if (((root->left)->value).type == NUMBER)
    {
        fprintf(file_pointer, " %d ^", ((root->left)->value).number);
    }
    else if (((root->left)->value).type == VARIABLE)
    {
        fprintf(file_pointer, " %s ^", transform_variable_to_str((root->left)->value));
    }
    // else if (((root->left)->value).type == OPERATION)
    // {
    //     fprintf(file_pointer, " %c ", transform_operation_to_str((root->left)->value));
    // }

    if (((root->right)->value).type == NUMBER)
    {
        fprintf(file_pointer, "{ %d }", ((root->right)->value).number);
    }
    else if (((root->right)->value).type == VARIABLE)
    {
        fprintf(file_pointer, "{ %s }", transform_variable_to_str((root->right)->value));
    }
    // else if (((root->left)->value).type == OPERATION)
    // {
    //     fprintf(file_pointer, " %c ", transform_operation_to_str((root->left)->value));
    // }

    fprintf(file_pointer, "\n \\end{equation}\n");
    // fprintf(file_pointer, "\\begin{equation}\n%d ^ { %d }\n \\end{equation}\n",
    //                       ((root->left)->value).number,
    //                       ((root->right)->value).number);
    return;
}


static void print_add_or_sub(struct Node *root, FILE *file_pointer)
{
    if (root == NULL || file_pointer == NULL)
    {
        return;
    }
    fprintf(file_pointer, "\\begin{equation}\n");
    if (((root->left)->value).type == NUMBER)
    {
        fprintf(file_pointer, " %d ", ((root->left)->value).number);
    }
    else if (((root->left)->value).type == VARIABLE)
    {
        fprintf(file_pointer, " %s ", transform_variable_to_str((root->left)->value));
    }
    // else if (((root->left)->value).type == OPERATION)
    // {
    //     fprintf(file_pointer, " %c ", transform_operation_to_str((root->left)->value));
    // }

    fprintf(file_pointer, "%c", (((root->value).operation) == OP_ADD) ? '+' : '-');

    if (((root->right)->value).type == NUMBER)
    {
        fprintf(file_pointer, " %d ", ((root->right)->value).number);
    }
    else if (((root->right)->value).type == VARIABLE)
    {
        fprintf(file_pointer, " %s ", transform_variable_to_str((root->right)->value));
    }
    // else if (((root->left)->value).type == OPERATION)
    // {
    //     fprintf(file_pointer, " %c ", transform_operation_to_str((root->left)->value));
    // }

    fprintf(file_pointer, "\n \\end{equation}\n");
    // fprintf(file_pointer, "\\begin{equation}\n %d %c %d\n \\end{equation}\n",
    //                       ((root->left)->value).number,
    //                       (((root->value).operation) == OP_ADD) ? '+' : '-',
    //                       ((root->right)->value).number);
    return;
}


static void analyze_root(struct Node *root, FILE *file_pointer)
{
    if (root == NULL || file_pointer == NULL)
    {
        return;
    }
    //printf("root address = %p\n", root);
    //printf("root operation = %d\n", (root->value).operation);
    if ((root->value).type == OPERATION)
    {
        //printf("root address = %p    root value type = %d\n", root, (root->value).type);
        switch ((root->value).operation)
        {
            case OP_ADD:    print_add_or_sub(root, file_pointer); break;
            case OP_SUB:    print_add_or_sub(root, file_pointer); break;
            case OP_MUL:    print_mul       (root, file_pointer); break;
            case OP_DIV:    print_fraction  (root, file_pointer); break;
            case OP_DEG:    print_pow       (root, file_pointer); break;
            case NOT_AN_OP:                                       break;
            default: break;
        }
    }
    else
    {
        print_one_node(root, file_pointer);
    }

    return;
}

static void latex_bypass_of_tree(struct Node *root, FILE *file_pointer)
{
    if (root == NULL)
    {
        return;
    }
    latex_bypass_of_tree(root->left, file_pointer);
    analyze_root(root, file_pointer);
    latex_bypass_of_tree(root->right, file_pointer);
}


Errors_of_tree create_latex_file(FILE *file_pointer)
{
    if (file_pointer == NULL)
    {
        return ERROR_OF_LATEX_DUMP;
    }
    fprintf(file_pointer,   "\\documentclass{article}\n"
                            "\\usepackage{graphicx}\n"
                            "\\title{test}\n"
                            "\\author{Zevs Grom}\n"
                            "\\date{January 2025}\n"
                            "\\begin{document}\n");
                            //"\\end{document}\n");
    return NO_ERRORS;
}

void latex_dump(struct Node *root, FILE *file_pointer, const char *action)
{
    if (root == NULL || file_pointer == NULL)
    {
        return;
    }
    fprintf(file_pointer, "\n%s:", action);
    latex_bypass_of_tree(root, file_pointer);
    return;
}

Errors_of_tree close_latex_file(FILE *file_pointer)
{
    if (file_pointer == NULL)
    {
        return ERROR_OF_LATEX_DUMP;
    }
    fprintf(file_pointer, "\\end{document}\n");
    fclose(file_pointer);
    return NO_ERRORS;
}
