#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "tree_dump.h"
#include "tree_tex_dump.h"
#include "tree_input.h"

static void get_mode(Mods_of_start *mode);

// const char *s = "(2*3)*(1+3)+50$";
//
// int get_n();
// int get_e();
// int get_p();
// int get_t();
// void syntax_error();
//
// int p = 0;
//
// void syntax_error()
// {
//     printf("Syntax Error! At index %d wait %c\n", p, s[p]);
//     return;
// }
//
// int get_n()
// {
//     int val = 0;
//     int old_p = p;
//     while ('0' <= s[p] && s[p] <= '9')
//     {
//         val = val * 10 + (s[p] - '0');
//         p++;
//     }
//     if (p == old_p)
//     {
//         syntax_error();
//     }
//     return val;
// }
//
// int get_e()
// {
//     int val = get_t();
//     while (s[p] == '+' || s[p] == '-')
//     {
//         int op = s[p];
//         p++;
//         int val2 = get_t();
//         if (op == '+')
//         {
//             val += val2;
//         }
//         else if (op == '-')
//         {
//             val -= val2;
//         }
//     }
//     return val;
// }
//
// int get_g()
// {
//     int val = get_e();
//     if (s[p] != '$')
//     {
//         p++;
//         syntax_error();
//     }
//     return val;
// }
//
// int get_t()
// {
//     int val = get_p();
//     while (s[p] == '*' || s[p] == '/')
//     {
//         int op = s[p];
//         p++;
//         int val2 = get_p();
//         if (op == '*')
//         {
//             val *= val2;
//         }
//         else if (op == '/')
//         {
//             val /= val2;
//         }
//     }
//     return val;
// }
//
// int get_p()
// {
//     if (s[p] == '(')
//     {
//         p++;
//         int val = get_e();
//         if (s[p] != ')')
//         {
//             syntax_error();
//         }
//         p++;
//         return val;
//     }
//     else
//     {
//         return get_n();
//     }
// }

static void get_mode(Mods_of_start *mode)
{
    printf("Please, choose mode for smart calculator:\n");
    printf("differentation - 1\ncalculation - 2\n");
    scanf("%d", mode);
    while (*mode != DIFFERENTIATION && *mode != CALCULATION)
    {
        printf("Please, enter another variant!\n");
        while(getchar() != '\n');
        scanf("%d", mode);
    }
    return;
}


int main()
{
    Mods_of_start mode = UNKNOWN_MODE;
    get_mode(&mode);
    // int ans = get_g();
    // printf("answer = %d\n", ans);
    struct Tree tree = {0};
    Errors_of_tree error = NO_ERRORS;
    //char all_operations[] = {'+', '-', '*', '/', '^'}; // diff_allowed_operation.txt: '+', '-', '*', '/', '^'
    //size_t size_of_all_operations = sizeof(all_operations) / sizeof(all_operations[0]);
    error = tree_constructor(&tree);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return error;
    }
    const char *latex_file_name = "tex_dump/example.tex";
    FILE *latex_file_pointer = fopen(latex_file_name, "w");
    fclose(latex_file_pointer);
    latex_file_pointer = fopen(latex_file_name, "a");
    error = create_latex_file(latex_file_pointer);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    error = get_tree_from_file(&tree, "source/input.txt", latex_file_pointer, mode);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    //printf("root information:\nroot.number = %d\nroot.operation = %d, root.variable = %d\n", (((tree.root)->left)->value).number, (((tree.root)->left)->value).operation, (((tree.root)->left)->value).variable);
    //error = close_latex_file(latex_file_pointer);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    //create_operations_interface(tree.interface, all_operations);
    tree.tmp_root = tree.root;
    // struct Value new_value = {.type = OPERATION,operations .operation = OP_ADD};
    // struct Node *new_left_node = (Node *) calloc(1, sizeof(Node));
    // struct Node *new_right_node = (Node *) calloc(1, sizeof(Node));
    // new_left_node->parent_node = (tree.tmp_root)->left;
    // new_right_node->parent_node = (tree.tmp_root)->left;
    // (new_left_node->value).type = NUMBER;
    // (new_right_node->value).type = VARIABLE;
    // (new_left_node->value).number = 2;
    // (new_right_node->value).number = VAR_X;
    // error = create_new_node(&(tree.tmp_root)->left, &new_value, new_left_node, new_right_node);
    tree.error = NO_ERRORS;
    graphic_dump(&tree, "example");
    if (tree.error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", tree.error);
        return 1;
    }
    //differentiation(tree.root, latex_file_pointer);
    calculation_of_tree(tree.tmp_root, latex_file_pointer);
    if (tree.error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", tree.error);
        return 1;
    }
    graphic_dump(&tree, "after_calculation");
    //graphic_dump(&tree, "after_differentiation");
    //symplifying_tree(&(tree.root));
    //graphic_dump(&tree, "after_symplifying");
    error = close_latex_file(latex_file_pointer);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    error = tree_destructor(&tree);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    return 0;
}
