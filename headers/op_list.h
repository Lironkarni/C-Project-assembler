#ifndef OP_LIST_H
#define OP_LIST_H

#include <stdio.h>
#include <string.h>

#include "../headers/globals.h"
#include "../headers/line.h"
#include "../headers/label.h"


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


int which_instruction(char *word);

int check_if_operation(char *word);

int is_legal_method(Line *line, int method, int op_index, int num_args);

int is_register(char *ptr);


#endif
