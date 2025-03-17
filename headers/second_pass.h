#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "../headers/label.h"

/*
 * Function: second_pass
 * ----------------------------
 *   Performs the second pass of assembly processing.
 *
 *   file: name of assembly source file
 *   head: pointer to external labels list head
 *   symbol_table_head: pointer to symbol table head
 *   code_image: pointer to code segment memory
 *   data_image: pointer to data segment memory
 */
void second_pass(char *file, ext_list *head, Symbol *symbol_table_head, code_word *code_image, data_word *data_image);

/*
 * Function: check_externs
 * ----------------------------
 *   Checks if operand is external label and updates external label list.
 *
 *   ext_list_head: pointer to external labels list head
 *   operand_name: operand to check
 *   line: pointer to parsed line information
 */
void check_externs(ext_list *ext_list_head, char operand_name, Line *line);

/*
 * Function: make_ent_file
 * ----------------------------
 *   Creates .ent file with entry labels and their addresses.
 *
 *   filename: name of output file (without extension)
 *   symbol_table_head: pointer to symbol table head
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

#endif /* SECOND_PASS_H */