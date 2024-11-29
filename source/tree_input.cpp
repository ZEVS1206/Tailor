#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tree.h"
#include "tree_input.h"

static void parse_information_from_file(struct Node *root, char **buffer, char *end_pointer);
static size_t get_size_of_file(FILE *file_pointer);
static char * get_value_from_file(char *str, size_t size_of_str, char *buffer, char *end_pointer);
static char * save_root_data(struct Node *root, char *buffer);
static char * skip_spaces(char *buffer, char *end_pointer);
static void transform_to_variable(const char *str, struct Value *value);
static void transform_to_operation(const char symbol, struct Value *value);

static char * skip_spaces(char *buffer, char *end_pointer)
{
    if (buffer == end_pointer)
    {
        return buffer;
    }
    while (buffer < end_pointer && isspace(*buffer))
    {
        buffer++;
    }
    return buffer;
}


static void transform_to_variable(const char *str, struct Value *value)
{
    if (value == NULL || str == NULL)
    {
        return;
    }

    if (strcasecmp("x", str) == 0)
    {
        value->variable = VAR_X;
    }
    else
    {
        value->variable = NOT_A_VAR;
    }
    return;
}

static void transform_to_operation(const char symbol, struct Value *value)
{
    if (value == NULL)
    {
        return;
    }
    if (symbol == '+')
    {
        value->operation = OP_ADD;
    }
    else if (symbol == '-')
    {
        value->operation = OP_SUB;
    }
    else if (symbol == '*')
    {
        value->operation = OP_MUL;
    }
    else if (symbol == '/')
    {
        value->operation = OP_DIV;
    }
    return;
}

static size_t get_size_of_file(FILE *file_pointer)
{
    fseek(file_pointer, 0L, SEEK_END);
    size_t size_of_file = (size_t)ftell(file_pointer);
    fseek(file_pointer, 0L, SEEK_SET);
    return size_of_file;
}

Errors_of_tree get_tree_from_file(struct Tree *tree, const char *file_source_name)
{
    if (tree == NULL)
    {
        return ERROR_OF_READ_TREE;
    }

    FILE *file_pointer = fopen(file_source_name, "rb");
    if (file_pointer == NULL)
    {
        return ERROR_OF_READ_TREE;
    }

    size_t size_of_file = get_size_of_file(file_pointer);
    char *buffer = (char *)calloc(size_of_file, sizeof(char));
    if (buffer == NULL)
    {
        return ERROR_OF_READ_TREE;
    }

    char *end_pointer = &(buffer[size_of_file - 1]);
    char *old_buffer = &(buffer[0]);

    size_t result_of_reading = fread(buffer, sizeof(char), size_of_file, file_pointer);

    if (result_of_reading != size_of_file)
    {
        return ERROR_OF_READ_FROM_FILE;
    }
    fclose(file_pointer);

    tree->tmp_root = tree->root;
    parse_information_from_file(tree->tmp_root, &buffer, end_pointer);
    free(old_buffer);
    return NO_ERRORS;
}

static char * get_value_from_file(char *str, size_t size_of_str, char *buffer, char *end_pointer)
{
    size_t index = 0;
    while (buffer < end_pointer && isalnum(*buffer) && index < size_of_str)
    {
        str[index] = *buffer;
        index++;
        buffer++;
    }
    if (index < size_of_str - 1)
    {
        str[index] = '\0';
    }
    return buffer;
}

static void parse_information_from_file(struct Node *root, char **buffer, char *end_pointer)
{
    *buffer = skip_spaces(*buffer, end_pointer);
    if (*buffer >= end_pointer)
    {
        return;
    }

    if (*buffer[0] == '(')
    {
        (*buffer)++;
        root->left = (Node *) calloc(1, sizeof(Node));
        if (root->left == NULL)
        {
            printf("Error of creating node!\n");
            return;
        }
        (root->left)->parent_node = root;
        ON_DEBUG(printf("go to left\n");)
        ON_DEBUG(getchar();)
        parse_information_from_file(root->left, buffer, end_pointer);
        ON_DEBUG(printf("leave left\n");)
        if (*buffer >= end_pointer)
        {
            return;
        }
        if (*buffer[0] == '\n')
        {
            return;
        }
        *buffer = skip_spaces(*buffer, end_pointer);
        ON_DEBUG(getchar();)
        ON_DEBUG(printf("processing center\n");)
        if (!isalnum(*buffer[0]) && *buffer[0] != ')' && *buffer[0] != '(')
        {
            transform_to_operation(*buffer[0], &(root->value));
            ON_DEBUG(printf("operation = %c\n", *buffer[0]);)
            (root->value).type = OPERATION;
            (*buffer)++;
        }
        *buffer = skip_spaces(*buffer, end_pointer);
        (*buffer)++;
        root->right = (Node *) calloc(1, sizeof(Node));
        if (root->right == NULL)
        {
            printf("Error of creating node!\n");
            return;
        }
        (root->right)->parent_node = root;
        ON_DEBUG(printf("go to right\n");)
        ON_DEBUG(getchar();)
        parse_information_from_file(root->right, buffer, end_pointer);
        ON_DEBUG(printf("leave right\n");)
        ON_DEBUG(getchar();)
        if (*buffer >= end_pointer)
        {
            return;
        }
        if (*buffer[0] == '\n')
        {
            return;
        }
    }

    if (isdigit(*buffer[0]))
    {
        char str[100];
        *buffer = get_value_from_file(str, 100, *buffer, end_pointer);
        ON_DEBUG(printf("value = %s\n", str);)
        (root->value).number = atoi(str);
        (root->value).type = NUMBER;
    }
    else if (isalpha(*buffer[0]))
    {
        char str[100];
        *buffer = get_value_from_file(str, 100, *buffer, end_pointer);
        ON_DEBUG(printf("variable = %s\n", str);)
        transform_to_variable(str, &(root->value));
        (root->value).type = VARIABLE;
    }

    if (*buffer[0] == ')')
    {
        (*buffer)++;
        return;
    }


}
