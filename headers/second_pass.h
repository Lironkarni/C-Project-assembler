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

/*
 * Function: make_ent_file
 * ----------------------------
 *   Creates .ent file containing entry labels and their addresses.
 *
 *   filename: name of output file (without extension)
 *   symbol_table_head: pointer to the symbol table head
 */
void make_ent_file(const char *filename, Symbol *symbol_table_head);

/*
 * Function: make_ob_file
 * ----------------------------
 *   Creates .ob file containing code and data segments in machine hex code.
 *
 *   filename: name of output file (without extension)
 *   code_image: pointer to code segment memory
 *   ic: final instruction counter
 *   data_image: pointer to data segment memory
 *   dc: final data counter
 */
void make_ob_file(const char *filename, code_word *code_image, int ic, data_word *data_image, int dc);

/*
 * Function: make_ext_file
 * ----------------------------
 *   Creates .ext file containing external labels and their reference addresses.
 *
 *   filename: name of output file (without extension)
 *   ext_table_head: pointer to the external labels list head
 */
void make_ext_file(const char *filename, ext_list *ext_table_head);

#endif /* SECOND_PASS_H */