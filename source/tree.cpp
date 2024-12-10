#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "tree_dump.h"
#include "tree_input.h"

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

int main()
{
    // int ans = get_g();
    // printf("answer = %d\n", ans);
    struct Tree tree = {0};
    Errors_of_tree error = NO_ERRORS;
    error = tree_constructor(&tree);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return error;
    }
    error = get_tree_from_file(&tree, "source/input.txt");
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    tree.tmp_root = tree.root;
    // struct Value new_value = {.type = OPERATION, .operation = OP_ADD};
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
    differentiation(tree.root);
    //calculation_of_tree(tree.tmp_root);
    if (tree.error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", tree.error);
        return 1;
    }
    //graphic_dump(&tree, "after_calculation");
    graphic_dump(&tree, "after_differentiation");
    symplifying_tree(&(tree.root));
    graphic_dump(&tree, "after_symplifying");
    error = tree_destructor(&tree);
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
    return 0;
}
