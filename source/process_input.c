#include "../headers/process_input.h"
#include "../headers/error.h"
#include "../headers/globals.h"
#include "../headers/utils.h"

op_code operation_list[SUM_OPERATIONS] =
	{
		{"mov", 0, 0, {2, METHOD_0_1_3, METHOD_1_3}},
		{"cmp", 1, 0, {2, METHOD_0_1_3, METHOD_0_1_3}},
		{"add", 2, 1, {2, METHOD_0_1_3, METHOD_1_3}},
		{"sub", 2, 2, {2, METHOD_0_1_3, METHOD_1_3}},
		{"lea", 4, 0, {2, METHOD_1, METHOD_1_3}},
		{"clr", 5, 1, {1, NONE, METHOD_1_3}},
		{"not", 5, 2, {1, NONE, METHOD_1_3}},
		{"inc", 5, 3, {1, NONE, METHOD_1_3}},
		{"dec", 5, 4, {1, NONE, METHOD_1_3}},
		{"jmp", 9, 1, {1, NONE, METHOD_1_2}},
		{"bne", 9, 2, {1, NONE, METHOD_1_2}},
		{"jsr", 9, 3, {1, NONE, METHOD_1_2}},
		{"red", 12, 0, {1, NONE, METHOD_1_3}},
		{"prn", 13, 0, {1, NONE, METHOD_0_1_3}},
		{"rts", 14, 0, {0, NONE, NONE}},
		{"stop", 15, 0, {0, NONE, NONE}}};

int analyse_operation(Line *line, char *second_word, int is_label, char *first_word, int instruction_index)
{
	int num_args, is_code = 0, op_index;
	char *first_operand, *ptr;
	op_index = check_if_operation(second_word);
	if (op_index != -1) // אם זו פקודה מוכרת של אסמבלי
	{
		is_code = 1;
		if (is_label) // אם לפני כן הייתה תווית נוסיף לטבלת הסמלים
		{
			add_symbol(line, first_word, instruction_index, is_code);
		}
	}
	else // זה לא פקודה מוכרת
	{
		print_syntax_error(ERROR_CODE_25, line->file_name, line->line_number);
		FOUND_ERROR_IN_FIRST_PASS = 1;
	}

	num_args = operation_list[op_index].address_method.num_args; // מספר האופרנדים של הפקודה

	*ptr = strstr(line->data, operation_list[op_index].operation_name);
	ptr += strlen(operation_list[op_index].operation_name);
	while (*ptr == SPACE) // דילוג על רווחים
		ptr++;

	switch (num_args)
	{
	case 0:
		if (*ptr != NULL_CHAR) // extra chars after end of command
		{
			print_syntax_error(ERROR_CODE_21, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		// TODO- what else?
		break;

	case 1:
		if (*ptr == NULL_CHAR) // missing operand
		{
			print_syntax_error(ERROR_CODE_26, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		while (*ptr && isspace(*ptr)) /* Skipping whitespace */
			ptr++;
		if (*ptr == COMMA) // פסיק לא חוקי בין הוראה לאופרנד הראשון
		{
			print_syntax_error(ERROR_CODE_28, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		if (strchr(ptr, COMMA) != NULL) // אם יש פסיק נוסף בפקודה
		{
			print_syntax_error(ERROR_CODE_29, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		if (extraneous_text(ptr)) // אקסטרה תווים אחקי האופרנד היחיד
		{
			print_syntax_error(ERROR_CODE_21, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}

		/*need to check if addressing method is legal*/
			break;

	case 2:

		printf("2 args\n");
		break;

	default:

		printf("some error\n");
		break;
	}
}
// char* get_valid_address_method_name(valid_address_method method) {
//	switch (method) {
//	case METHOD_0_1_3: return "METHOD_0_1_3";
//	case METHOD_1: return "METHOD_1";
//	case METHOD_1_3: return "METHOD_1_3";
//	case METHOD_1_2: return "METHOD_1_2";
//	case NONE: return "NONE";
//	default: return "UNKNOWN";
//	}
// }

char *get_word(char *line)
{
	static char *current = NULL;
	// if its the first word in the line
	if (line != NULL)
	{
		current = line;
	}
	// got to the end of the row
	if (*current == '\0')
	{
		return NULL;
	}
	char *word = current;
	// skip spaces
	while (*current == ' ')
	{
		current++;
	}
	// size of the current word
	int len = 0;
	while (current[len] != '\0' && current[len] != ' ')
	{
		len++;
	}
	// allocate memory to the word
	word = (char *)malloc(len + 1);
	if (word == NULL)
	{
		print_system_error(ERROR_CODE_3);
		exit(1);
	}
	strncpy(word, current, len);
	word[len] = '\0';

	current += len; // put current on next word
	if (*current == ' ')
	{
		current++;
	}

	return word;
}

int check_if_operation(char *word)
{
	int i;
	// op_code not_found = {"0", 0, 0};
	for (i = 0; i < SUM_OPERATIONS; i++)
	{
		if (strcmp(word, operation_list[i].operation_name) == 0)
			i;
	}
	return -1;
}

int is_valid_label(char *label, Line *line)
{
	/*check its starts with alphbetic char
	check all chars are alphabetic or digits
	check if length is lower than 32 (MAX_LABEL_LENGTH)
	check if label name is saved assembly name
	check if label as been defind yet
	check if this is not a name of a macro*/
	int i = 0, len;
	int op;
	len = strlen(label);
	if (!isalpha(label[0]))
	{
		// label name is not starting with alphabet character
		print_syntax_error(ERROR_CODE_10, line->file_name, line->line_number);
		return 1;
	}
	if (len > MAX_LABEL_LENGTH)
	{
		print_syntax_error(ERROR_CODE_11, line->file_name, line->line_number);
		return 1;
	}
	while (i < len - 1)
		if (!isalnum(label[i++]))
		{
			// label name have non alpha-numeric character in it
			print_syntax_error(ERROR_CODE_12, line->file_name, line->line_number);
			return 1;
		}

	if (find_macro(label) != NULL)
	{
		// label name is equals to macro
		print_syntax_error(ERROR_CODE_17, line->file_name, line->line_number);
		return 1;
	}

	// see if its saved assembly name
	op = check_if_operation(label);
	if (op != -1)
	{
		// label name is equals to operation
		print_syntax_error(ERROR_CODE_9, line->file_name, line->line_number);
		return 1;
	}

	return 0;
}
