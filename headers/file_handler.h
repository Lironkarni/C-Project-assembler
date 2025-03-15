#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../headers/memory_struct.h"
#include "../headers/label.h"
#include "../headers/error.h"

void delete_am_file(const char *filename);

char *load_file(char *filename);

void make_ob_file(const char *filename, code_word *code_image, int ic, data_word *data_image, int dc);

void make_ent_file(const char *filename, Symbol *symbol_table_head);
#endif