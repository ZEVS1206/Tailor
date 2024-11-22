// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>
//
// #include "tree.h"
// #include "tree_input.h"
//
// static void parse_information_from_file(struct Node *root, char **buffer, char *end_pointer, struct Parts_of_tree **parts);
// static size_t get_size_of_file(FILE *file_pointer);
// static char * save_root_data(struct Node *root, char *buffer);
// static char * skip_spaces(char *buffer, char *end_pointer);
//
// static char * skip_spaces(char *buffer, char *end_pointer)
// {
//     if (buffer == end_pointer)
//     {
//         return buffer;
//     }
//     while (isspace(*buffer))
//     {
//         buffer++;
//     }
//     return buffer;
// }
//
// static size_t get_size_of_file(FILE *file_pointer)
// {
//     fseek(file_pointer, 0L, SEEK_END);
//     size_t size_of_file = (size_t)ftell(file_pointer);
//     fseek(file_pointer, 0L, SEEK_SET);
//     return size_of_file;
// }
//
// Errors_of_tree get_tree_from_file(struct Tree *tree, const char *file_source_name)
// {
//     if (tree == NULL)
//     {
//         return ERROR_OF_READ_TREE;
//     }
//
//     FILE *file_pointer = fopen(file_source_name, "r");
//     if (file_pointer == NULL)
//     {
//         return ERROR_OF_READ_TREE;
//     }
//
//     size_t size_of_file = get_size_of_file(file_pointer);
//     char *buffer = (char *)calloc(size_of_file, sizeof(char));
//     if (buffer == NULL)
//     {
//         return ERROR_OF_READ_TREE;
//     }
//
//     char *end_pointer = &(buffer[size_of_file - 1]);
//
//     size_t result_of_reading = fread(buffer, sizeof(char), size_of_file, file_pointer);
//
//     if (result_of_reading != size_of_file)
//     {
//         return ERROR_OF_READ_FROM_FILE;
//     }
//
//     tree->tmp_root = tree->root;
//     parse_infromation_from_file(tree->tmp_root, &buffer, end_pointer);
//
//     return NO_ERRORS;
// }
//
// static void parse_information_from_file(struct Node *root, char **buffer, char *end_pointer, struct Parts_of_tree **parts)
// {
//     *buffer = skip_spaces(*buffer, end_pointer);
//
//     while (*buffer[0] == '(')
//     {
//         *buffer = skip_spaces(*buffer, end_pointer);
//     }
//     (*buffer)++;
//     (*parts)->left = (Node *) calloc(1, sizeof(Node));
//     (((*parts)->left)->value)
