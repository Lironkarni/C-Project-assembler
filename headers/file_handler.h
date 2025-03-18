#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "../headers/label.h"


/*
 * Function: load_file
 * ----------------------------
 *   Loads file content into memory.
 *
 *   filename: file name
 *
 *   returns: pointer to loaded content
 */
char *load_file(char *filename);

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

void make_ent_file(const char *filename, Symbol *symbol_table_head);

void make_ext_file(const char *filename, ext_list *ext_table_head);


#endif