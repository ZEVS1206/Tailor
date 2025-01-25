#ifndef TREE_TEX_DUMP_H
#define TREE_TEX_DUMP_H

#include "tree.h"

Errors_of_tree create_latex_file(FILE *file_pointer);
Errors_of_tree close_latex_file(FILE *file_pointer);

void latex_dump(struct Node *root, FILE *file_pointer, const char *action);
void special_latex_dump(char *str, FILE *file_pointer, const char *action);

#endif
