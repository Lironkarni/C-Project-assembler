#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../headers/label.h"
#include "../headers/line.h"


typedef struct ext_list
{
	char *label_name;
	int address;
	struct ext_list *next;
}ext_list;

extern ext_list *ext_list_head;

void second_pass(char *file, ext_list *head, code_word *code_image,data_word *data_image);
int check_externs(ext_list *ext_list_head, Symbol *symbol_table_head,Line *line);


#endif