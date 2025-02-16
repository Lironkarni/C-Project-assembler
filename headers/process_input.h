#ifndef PROCESS_INPUT_H
#define PROCESS_INPUT_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../headers/utils.h"
#include "../headers/memory_struct.h"

#define SUM_OPERATIONS 16
#define MAX_LABEL_LENGTH 31

typedef enum address_method_type
{
	IMMEDIATE,
	DIRECT,
	RELATIVE,
	DIRECT_REGISTER
}address_method_type;

typedef enum valid_address_method
{
	METHOD_0_1_3,
	METHOD_1,
	METHOD_1_3,
	METHOD_1_2,
	NONE
}valid_address_method;

typedef struct address_method
{
	int num_args;
	valid_address_method address_method_source;
	valid_address_method address_method_dest;
}address_method;

typedef struct op_code
{
	char* operation_name;
	int opcode;
	int funct;
	address_method address_method;

}op_code;


extern op_code operation_list[SUM_OPERATIONS];

int check_if_operation(char* first_word);
char* get_word(char* line);
int is_valid_label(char* label, Line *line);
void analyse_operation(Line *line, char *second_word, int is_label, char *first_word, int instruction_index, code_word *code_image);
int which_addressing_method(char *ptr, int op_index, Line *line);
#endif // PROCESS_INPUT_H