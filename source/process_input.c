#include "../headers/process_input.h"
#include "../headers/error.h"
#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/first_pass.h"
#include "../headers/memory_struct.h"

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

char *REGISTERS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

void analyse_operation(Line *line, char *second_word, int is_label, char *first_word, int instruction_index, code_word *code_image)
{
	int num_args, is_code = 0, op_index, address_method_src, address_method_des, len, is_comma = 0;
	char *first_operand, *second_operand, *ptr;
	if (is_label)
		op_index = check_if_operation(second_word);
	else
		op_index = check_if_operation(first_word);

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
		return;
	}

	num_args = operation_list[op_index].address_method.num_args; // מספר האופרנדים של הפקודה

	ptr = strstr(line->data, operation_list[op_index].operation_name);
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
		add_to_code_image(code_image, line, num_args, op_index,ZERO, ZERO,ZERO, NULL,NULL);
		return;

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

		second_operand = get_word(NULL);
		len = strlen(second_operand);
		if (second_operand[len - 1] == COMMA)
		{
			print_syntax_error(ERROR_CODE_35, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		/*need to check if addressing method is legal*/
		address_method_des = which_addressing_method(second_operand, op_index, line);
		if (address_method_des == -1)
			return; // there was error. no need to keep analysing this line

		ptr += len;
		if (extraneous_text(ptr)) // אקסטרה תווים אחרי האופרנד היחיד
		{
			print_syntax_error(ERROR_CODE_21, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		// check if the method is legal
		if (is_legal_method(line, address_method_des, op_index, num_args))
		{
			return;
		}

		add_to_code_image(code_image,line,num_args,op_index,operation_list[op_index].funct,ZERO,address_method_des, NULL, second_operand);

		return;
	case 2:
		if (*ptr == NULL_CHAR) // missing operand
		{
			print_syntax_error(ERROR_CODE_30, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		if (*ptr == COMMA) // פסיק לא חוקי בין הוראה לאופרנד הראשון
		{
			print_syntax_error(ERROR_CODE_28, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		first_operand = get_word(NULL);
		len = strlen(first_operand);
		if (first_operand[len - 1] == COMMA)
		{
			is_comma = 1;
			first_operand[len - 1] = NULL_CHAR; // need to word w/o the comma
		}
		address_method_src = which_addressing_method(first_operand, op_index, line);
		if (address_method_src == -1)
		{
			return;
		}
		ptr += len;

		while (*ptr == SPACE) // דילוג על רווחים
			ptr++;
		if (*ptr != COMMA && !is_comma) // expected comma
		{
			print_syntax_error(ERROR_CODE_33, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		while (*ptr == SPACE) // דילוג על רווחים
			ptr++;

		second_operand = get_word(NULL);
		if (second_operand[0] == COMMA)
		{
			ptr++;
			second_operand += 1;
		}

		len = strlen(second_operand);
		if (second_operand[len - 1] == COMMA)
		{
			print_syntax_error(ERROR_CODE_35, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		address_method_des = which_addressing_method(second_operand, op_index, line);
		if (address_method_des == -1)
		{
			return;
		}
		ptr += len;
		if (extraneous_text(ptr)) // אקסטרה תווים אחרי האופרנד היחיד
		{
			print_syntax_error(ERROR_CODE_21, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		// check if the method is legal
		if (is_legal_method(line, address_method_des, op_index - 1, num_args) || is_legal_method(line, address_method_src, op_index, num_args))
		{
			return;
		}
		add_to_code_image(code_image,line,num_args,op_index,operation_list[op_index].funct,address_method_src,address_method_des,first_operand,second_operand);
		break;

	default:

		printf("some error\n");
		break;
	}
}
int which_addressing_method(char *ptr, int op_index, Line *line)
{
	long num_ptr;
	if (*ptr == NUMBER_SIGN) // if start with #
	{
		ptr++;
		if (*ptr == MINUS || *ptr == PLUS)
		{
			if (!isdigit(*(ptr + 1)))
			{ // אם אחרי '-' או '+' לא בא מספר - שגיאה
				print_syntax_error(ERROR_CODE_31, line->file_name, line->line_number);
				return -1;
			}
		}
		ptr++;
		while (*ptr != SPACE && *ptr != NULL_CHAR)
		{

			if (!isdigit(*ptr++))
			{
				print_syntax_error(ERROR_CODE_22, line->file_name, line->line_number);
				return -1;
			}
		}
		num_ptr = strtol(ptr, NULL, DECIMAL);
		if (num_ptr < MIN_21BIT || num_ptr > MAX_21BIT)
		{
			print_syntax_error(ERROR_CODE_36, line->file_name, line->line_number);
			return -1;
		}
		return IMMEDIATE;
	}
	if (is_register(ptr)) // check if register
	{
		return DIRECT_REGISTER;
	}

	if (*ptr == AMPERSAND) //&next
	{
		// next word after & must be label that defined or will be define later
		if (is_valid_label(ptr, line))
			return -1;

		// this is used only for this operations: jmp, bne, jsr
		if (op_index != 9 && op_index != 10 && op_index != 11)
		{
			print_syntax_error(ERROR_CODE_32, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return -1;
		}

		return RELATIVE; // יחסי
	}
	if (!is_valid_label(ptr, line)) //  אם המילה הבאה אחרי הפעולה זה תווית שהוגדרה או תוגדר בהמשך
	{
		return DIRECT;
	}
	else // המילה היא לא אף אחת מהאפשרויות הנל- ז"א שיש שגיאה
	{
		return -1;
	}
}

int is_legal_method(Line *line, int method, int op_index, int num_args)
{
	if (num_args == 1) // need to check only destination
	{
		switch (operation_list[op_index].address_method.address_method_dest)
		{
		case METHOD_0_1_3:
			if (method == RELATIVE)
			{
				print_syntax_error(ERROR_CODE_34, line->file_name, line->line_number);
				FOUND_ERROR_IN_FIRST_PASS = 1;
				return 1;
			}
			return 0; // METHOD IS LEGAL

		case METHOD_1_2:
			if (method == IMMEDIATE || method == DIRECT_REGISTER)
			{
				print_syntax_error(ERROR_CODE_34, line->file_name, line->line_number);
				FOUND_ERROR_IN_FIRST_PASS = 1;
				return 1;
			}
			return 0;

		case METHOD_1_3:
			if (method == IMMEDIATE || method == RELATIVE)
			{
				print_syntax_error(ERROR_CODE_34, line->file_name, line->line_number);
				FOUND_ERROR_IN_FIRST_PASS = 1;
				return 1;
			}
			return 0;

		default:
			return 0;
		}
	}
	// its 2 args- need to check only the src (dest already checked in last if)
	switch (operation_list[op_index].address_method.address_method_source)
	{
	case METHOD_0_1_3:
		if (method == RELATIVE)
		{
			print_syntax_error(ERROR_CODE_34, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return 1;
		}
		return 0;

	case METHOD_1:
		if (method == DIRECT || method == RELATIVE || method == DIRECT_REGISTER)
		{
			print_syntax_error(ERROR_CODE_34, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return 1;
		}
		return 0;
	default:
		return 0;
	}
}

int is_register(char *ptr)
{
	for (int i = 0; i < NUM_OF_REG; i++)
	{
		if (strncmp(ptr, REGISTERS[i], TWO) == 0)
		{
			return 1;
		}
	}
	return 0;
}
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
		{
			return i;
		}
	}
	return -1;
}
