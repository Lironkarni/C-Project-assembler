#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "../headers/label.h"

void second_pass(char *file, Symbol *symbol_table_head, code_word *code_image,data_word *data_image);
void make_ent_file(const char *filename, Symbol *symbol_table_head);
void make_ob_file(const char *filename, code_word *code_image, int ic, data_word *data_image, int dc);
void make_ext_file(const char *filename, ext_list *ext_table_head);


#endif