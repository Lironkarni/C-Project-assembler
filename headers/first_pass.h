#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#include "../headers/utils.h"
#include "../headers/process_input.h"
#include "../headers/label.h"
#define INSTRUCTION_COUNT 4


int first_pass(char *file);
void process_line(char* file);
void process_word(Line* line, char* first_word);
int which_instruction(char *word);
int get_data(Line *line, int inst_index, int **numbers);
int get_string_data( Line *line, int inst_index, char **string_value);

void test(int dc, int ic);
void print_bits(uint32_t value, int bits);


#endif