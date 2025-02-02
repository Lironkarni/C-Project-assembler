#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 80
#define SUM_OPERATIONS 16
#define MAX_LABEL_LENGTH 31

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

typedef enum guide_type
{
	DATA,
	STRING,
	ENTRY,
	EXTERNAL,
	OPERATION
}guide_type;

typedef struct label
{
	char* label_name;
	int address;
	guide_type type;
}label;


