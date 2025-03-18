#include "../headers/op_list.h"
#include "../headers/globals.h"
#include "../headers/error.h"
#include "../headers/line.h"
#include "../headers/label.h"


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
char *instruction_list[] = {".data", ".string", ".entry", ".extern"};


int which_instruction(char *word)
{
    int i;
    /* Check if the word matches any known directive */
    for (i = 0; i < INSTRUCTION_COUNT; i++)
    {
        if (strcmp(word, instruction_list[i]) == 0)
            return i; /* Return index of the directive if found */
    }
    return -1; /* Word is not a recognized directive */
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
			ptr++;
		}
		char *end_ptr;
		num_ptr = strtol(ptr, &end_ptr, DECIMAL);

		 // **Check for decimal point**
		 if (*end_ptr == POINT) {  
            print_syntax_error(ERROR_CODE_42, line->file_name,line->line_number); // Error: Number must be an integer
            return -1;
        }

		//ptr++;
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
		if (is_valid_label(ptr+1, line))
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
		if (method == IMMEDIATE || method == RELATIVE || method == DIRECT_REGISTER)
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