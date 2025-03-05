#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "../headers/label.h"

void second_pass(char *file, ext_ent_list *head, Symbol *symbol_table_head);
int check_externs(ext_ent_list *ext_ent_list_head, Symbol *symbol_table_head);


#endif