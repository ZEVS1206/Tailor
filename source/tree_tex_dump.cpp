// #include <stdio.h>
// #include <stdlib.h>
//
// #include "tree.h"
//
// Errors_of_tree create_latex_file(const char *file_name, FILE *file_pointer)
// {
//     if (file_name == NULL || file_pointer == NULL)
//     {
//         return ERROR_OF_LATEX_DUMP;
//     }
//     file_pointer = fopen(file_name, "a");
//     if (file_pointer == NULL)
//     {
//         return ERROR_OF_LATEX_DUMP;
//     }
//     fprintf(file_pointer, " \\documentclass{article}\n
//                             \\usepackage{graphicx}\n
//                             \\title{test}
//                             \\author{egor.buditsky56 }
// \date{December 2024}
//
// \begin{document}
//
// \maketitle
//
// \section{Introduction}
//
// \end{document}
