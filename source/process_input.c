#include "../headers/process_input.h"
#include "../headers/error.h"
#include "../headers/globals.h"
#include "../headers/utils.h"

op_code operation_list[SUM_OPERATIONS] =
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

char* get_word(char* line)
{
	static char* current=NULL;
	//if its the first word in the line
	if(line!=NULL){
		current=line;
	}
	//got to the end of the row
	if(*current=='\0'){
		return NULL;
	}
	char *word=current;
	    // skip spaces
    while (*current == ' ') {
        current++;
    }
	   // size of the current word
    int len = 0;
    while (current[len] != '\0' && current[len] != ' ') {
        len++;
    }
	//allocate memory to the word
	word=(char *)malloc(len+ 1);
	if(word==NULL){
		print_system_error(ERROR_CODE_3);
		exit(1);
	}
	strncpy(word,current,len);
	word[len]='\0';

	current+=len; //put current on next word
	if(*current==' '){
		current++;
	}
	
	return word;
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

int is_valid_label(char* label, Line *line)
{
	/*check its starts with alphbetic char
	check all chars are alphabetic or digits
	check if length is lower than 32 (MAX_LABEL_LENGTH)
	check if label name is saved assembly name
	check if label as been defind yet
	check if this is not a name of a macro*/
	int i = 0, len;
	op_code op;
	len = strlen(label);
	if (!isalpha(label[0])){
		//label name is not starting with alphabet character
		print_syntax_error(ERROR_CODE_10, line->file_name,line->line_number);
		return 1;
	}
	if (len > MAX_LABEL_LENGTH){
		print_syntax_error(ERROR_CODE_11, line->file_name,line->line_number);
		return 1;
	}
	while (i< len-1)
		if(!isalnum(label[i++])){
			//label name have non alpha-numeric character in it
			print_syntax_error(ERROR_CODE_12,line->file_name,line->line_number);
			return 1;
		}

	 if (find_macro(label) != NULL) {
		//label name is equals to macro
        print_syntax_error(ERROR_CODE_17, line->file_name, line->line_number);
        return 1;
    }

	//see if its saved assembly name
	op=check_if_instruction(label);
	if(strcmp(op.operation_name,"0")!=0){
		//label name is equals to operation
		print_syntax_error(ERROR_CODE_9,line->file_name,line->line_number);
		return 1;
	}

	return 0;
}
