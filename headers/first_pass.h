#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../headers/error.h"
#include "../headers/globals.h"
#include "../headers/line.h"
#include "../headers/error.h"
#include "../headers/memory_struct.h"
#include "../headers/label.h"
#include "../headers/op_list.h"
#include "../headers/second_pass.h"
#include "../headers/utils.h"





static const char *directive_words[] ={"data","string","extern","entry"}; //מילות הנחייה
static const int directive_count = sizeof(directive_words) / sizeof(directive_words[0]);


int first_pass(char *file);
void process_line(char* file);
void process_word(Line* line, char* first_word);
int which_addressing_method(char *ptr, int op_index, Line *line);
void analyse_operation(Line *line, char *second_word, int is_label, char *first_word, int instruction_index, code_word *code_image);
int add_to_ext_list(char *label_name, int address);

#endif