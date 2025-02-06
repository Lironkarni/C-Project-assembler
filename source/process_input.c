#include "../headers/process_input.h"
#include "../headers/error.h"
#include "../headers/globals.h"
#include "../headers/utils.h"

op_code operation_list[] =
{
	{"mov",0,0, {2, METHOD_0_1_3,METHOD_1_3}},
	{"cmp",1,0,{2,METHOD_0_1_3,METHOD_0_1_3}},
	{"add",2,1,{2,METHOD_0_1_3, METHOD_1_3}},
	{"sub",2,2, {2,METHOD_0_1_3,METHOD_1_3}},
	{"lea",4,0, {2, METHOD_1, METHOD_1_3}},
	{"clr",5,1, {1,NONE, METHOD_1_3}},
	{"not",5,2, {1, NONE, METHOD_1_3}},
	{"inc",5,3, {1,NONE, METHOD_1_3}},
	{"dec",5,4, {1,NONE, METHOD_1_3}},
	{"jmp",9,1, {1,NONE, METHOD_1_2}},
	{"bne",9,2, {1,NONE, METHOD_1_2}},
	{"jsr",9,3, {1,NONE, METHOD_1_2}},
	{"red",12,0, {1,NONE, METHOD_1_3}},
	{"prn",13,0, {1,NONE, METHOD_0_1_3}},
	{"rts",14,0, {0,NONE,NONE}},
	{"stop",15,0, {0,NONE,NONE}}
};

//char* get_valid_address_method_name(valid_address_method method) {
//	switch (method) {
//	case METHOD_0_1_3: return "METHOD_0_1_3";
//	case METHOD_1: return "METHOD_1";
//	case METHOD_1_3: return "METHOD_1_3";
//	case METHOD_1_2: return "METHOD_1_2";
//	case NONE: return "NONE";
//	default: return "UNKNOWN";
//	}
//}

void get_word(char* line, char* word)
{
	printf("got to get word");
	int i = 0;
	while (line[i] != '\0' && line[i] != ' ')
	{
		word[i] = line[i];
		i++;
	}
	word[i] = '\0';
}

op_code check_if_instruction(char* word)
{
	int i;
	op_code not_found = { "0",0,0 };
	for (i = 0;i < SUM_OPERATIONS;i++)
	{
		if (strcmp(word, operation_list[i].operation_name) == 0)
			return operation_list[i];
	}
	return not_found;
}


/*this function gets the command and removes all spaces from the beginning of the command*/
void remove_spaces(char* line)
{
	char* start = line;
	while (*start && (isspace((unsigned char)(*start)) || *start == '\t')) {
		start++;
	}
	/*move the rest of the string to the beginning*/
	memmove(line, start, strlen(start) + 1);
}


int is_valid_label(char* label, Line *line)
{
	
	//V-check its starts with alphbetic char
	//V-check all chars are alphabetic or digits
	//V-check if length is lower than 32 (MAX_LABEL_LENGTH)
	//check if label name is saved assembly name
	//check if label as been defind yet
	//check if this is not a name of a macro

	
	int i = 0, len;
	op_code op;
	len = strlen(label);
	if (!isalpha(label[0])){
		print_syntax_error(ERROR_CODE_10, line->file_name,line->line_number);
		return 1;
	}
	if (len > MAX_LABEL_LENGTH){
		print_syntax_error(ERROR_CODE_11, line->file_name,line->line_number);
		return 1;
	}
	while (label[i] != '\0')
		if(!isalpha(label[i]) && !isdigit(label[i])){
			print_syntax_error(ERROR_CODE_12,line->file_name,line->line_number);
			return 1;
		}

	//see if its saved assembly name
	op=check_if_instruction(label);
	if(strcmp(op.operation_name,"0")==0){
		print_syntax_error(ERROR_CODE_9,line->file_name,line->line_number);
		return 1;
	}
	return 0;
}
