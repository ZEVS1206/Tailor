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
        return 1;
    }
    error = get_tree_from_file(&tree, "source/input.txt");
    if (error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", error);
        return 1;
    }
//
//     struct Value value1 = {.type = OPERATION, .operation = OP_ADD};
//     struct Value value2 = {.type = NUMBER, .number = 3};
//     struct Value value3 = {.type = NUMBER, .number = 5};
//     error = create_new_node(&(tree.root), &value1, NULL, NULL);
//     if (error != NO_ERRORS)
//     {
//         fprintf(stderr, "error = %d\n", error);
//         return 1;
//     }
//     error = create_new_node(&((tree.root)->left), &value2, NULL, NULL);
//     if (error != NO_ERRORS)
//     {
//         fprintf(stderr, "error = %d\n", error);
//         return 1;
//     }
//
//     error = create_new_node(&((tree.root)->right), &value3, NULL, NULL);
//     if (error != NO_ERRORS)
//     {
//         fprintf(stderr, "error = %d\n", error);
//         return 1;
//     }
    tree.error = NO_ERRORS;
    tree.tmp_root = tree.root;
    graphic_dump(&tree, "example");
    if (tree.error != NO_ERRORS)
    {
        fprintf(stderr, "error = %d\n", tree.error);
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
