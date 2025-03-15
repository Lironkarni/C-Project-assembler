#ifndef FILE_COPY_H
#define FILE_COPY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>


#include "../headers/globals.h"
#include "../headers/error.h"
#include "../headers/label.h"
#include "../headers/memory_struct.h"



void remove_extra_spaces(char *line);
int is_empty_line(const char *line);
char *get_word(char *line);
void print_bits(uint32_t value, int bits);
int extraneous_text(char *command);
void test(int dc, int ic);
#endif // FILE_COPY_H
