#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "../headers/label.h"

/*
 * Function: second_pass
 * ----------------------------
 *   Performs the second pass of assembly processing.
 *
 *   file: name of the assembly source file
 *   symbol_table_head: pointer to the head of the symbol table
 *   code_image: pointer to the code segment memory
 *   data_image: pointer to the data segment memory
 */
void second_pass(char *file, Symbol *symbol_table_head, code_word *code_image,data_word *data_image);

#endif /* SECOND_PASS_H */