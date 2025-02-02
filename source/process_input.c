#include "process_input.h"
#include "../headers/error.h"

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

int is_empty_line(char* line)
{
	int i = 0;
	while (line[i] != '\0')
		if (isspace(line[i]))
			i++;
		else
			return 1;
	return 0;
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


int is_valid_label(char* label)
{
	/*V-check if fininsh with :*/
	/*check if this is not a name of a macro*/
	/*V-check its starts with alphbetic char*/
	/*V-check all chars are alphabetic or digits*/
	/*V-check if length is lower than 32 (MAX_LABEL_LENGTH)
	check if label name is saved assembly name
	check if label as been defind yet

	
	*/
	int i = 0, len;
	len = strlen(label);
	if (label[len - 1] != ':')
		return 1;
	if (!isalpha(label[0]))
		return 1;
	if (len > MAX_LABEL_LENGTH)
		return 1;
	while (label[i] != '\0')
		if(!isalpha(label[i]) && !isdigit(label[i]))
			return 1;

}

int main(int argc, char* argv[])
{
	FILE* input_file;
	int line_number = 0, num_args;
	int len, c;
	char first_word[MAX_LINE_LENGTH], line[MAX_LINE_LENGTH + 2]; /*for \0 and one more char for overflow check*/
	char* line_ptr;
	op_code curr_op;

	/*open the file*/
	input_file = fopen(argv[1], "r");
	if (!input_file)
	{
		print_system_error(ERROR_CODE_2);
		return 1;
	}

	while (fgets(line, sizeof(line), input_file))
	{
		line_number++;

		len = strlen(line); /*ge line length*/
		if (line[len - 1] == '\n')	/*put '\0' at the end of each line*/
			line[len - 1] = '\0';

		if (len > MAX_LINE_LENGTH)
			if (line[MAX_LINE_LENGTH] != '\0' && line[MAX_LINE_LENGTH] != '\n') {
				printf("Error, line %d is longer than %d\n", line_number, MAX_LINE_LENGTH); /*errr longer than 80*/
				while ((c = fgetc(input_file)) != '\n' && c != EOF); /*clear the rest of the line*/
				continue; /*don't check this row, move to the next one*/
			}

		if (line[0] == ';') /*if line starts with ";" this is comment and ignore it*/
			continue;
		if (is_empty_line(line) == 0)
			continue;

		/*get the fisrt word and check if this is guiding or instructive sentence*/
		get_word(line, first_word);
		/*check if its instructive sentence*/
		curr_op=check_if_instruction(first_word);
		if (strcmp(curr_op.operation_name, "0") == 0)
		{
			printf("not instruction");
			/*need to check if guiding command*/
			/*TODO*/
			/*if its label, need to check if its valid label's name*/
			is_valid_label(first_word);
		}
			

		else /*its instruction*/
		{
			/*need to check how many arguments*/
			line_ptr = line; // Create a pointer to the start of the line
			memcpy(line_ptr, line+ strlen(curr_op.operation_name),strlen(line));
			remove_spaces(line_ptr);
			/*TODO*/
			/*need to check for comma- if there is- error*/
			
			/*fond which case is this by number of arguments*/
			num_args = curr_op.address_method.num_args;
			switch (num_args)
			{
			case 0:
				printf("%s\n", curr_op.operation_name);
				printf("0 args\n");
				break;

			case 1:
				printf("%s\n", curr_op.operation_name);
				printf("1 args\n");
				break;
			
			case 2:
				printf("%s\n", curr_op.operation_name);
				printf("2 args\n");
				break;
			
			default:
				printf("%s\n", curr_op.operation_name);
				printf("some error\n");
				break;
			}			
		}
	}

	fclose(input_file);
	return 0;
}