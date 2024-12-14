#ifndef TREE_TEX_DUMP_H
#define TREE_TEX_DUMP_H

#include "tree.h"

Errors_of_tree create_latex_file(const char *file_name, FILE *file_pointer);
void latex_dump(struct Tree *tree, FILE_*file_pointer);
Errors_of_tree close_latex_file(FILE *file_pointer);

#endif
