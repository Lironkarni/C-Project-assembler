#include "../headers/first_pass.h"


/*start the first pass*/
int first_pass(char *file)
{
    process_line(file);
    if (FOUND_ERROR_IN_FIRST_PASS)
    {
        FOUND_ERROR_IN_FIRST_PASS = 0;
        return 1;
    }

    //update symbol table
    update_symbol_tabel();
    //test(DC,IC);
    second_pass(file, ext_list_head, 0, code_image,data_image); // second_pass
    // test(DC,IC);   
    return 0;
}

void process_line(char *file)
{
    FILE *input_file;
    char temp_line[MAX_LINE_LEN + 2], *first_word = NULL; /*for \0 and one more char for overflow check*/
    int line_number = 0, line_len, c;
    Line *line;

    input_file = fopen(file, "r");
    if (input_file == NULL)
    {
        print_system_error(ERROR_CODE_4);
    }
    else
    {
        while (fgets(temp_line, sizeof(temp_line), input_file))
        {
            printf("%s", temp_line);
            line_number++;
            line_len = strlen(temp_line);        /*get line length*/
            if (temp_line[line_len - 1] == '\n') /*put '\0' at the end of each line*/
                temp_line[line_len - 1] = NULL_CHAR;

            if (line_len > MAX_LINE_LEN)
                if (temp_line[MAX_LINE_LEN] != NULL_CHAR && temp_line[MAX_LINE_LEN] != '\n')
                {
                    print_syntax_error(ERROR_CODE_8, file, line_number); /*error longer than 80*/
                    while ((c = fgetc(input_file)) != '\n' && c != EOF)
                        ;     /*clear the rest of the line*/
                    continue; /*don't check this row, move to the next one*/
                }

            /*make line*/
            line = create_line(temp_line, file, line_number);

            if (line == NULL)
            {
                /*TODO-need to free all memery*/
                exit(1);
            }

            /*get the fisrt word and check if this is guiding or instructive sentence*/
            first_word = get_word(line->data);
            process_word(line, first_word);
        }
    }
    fclose(input_file);
}

void process_word(Line *line, char *first_word)
{
    int word_len, instruction_index, is_code = 0;
    char *second_word, *string_value;
    int *numbers = NULL;
    int is_label = 0;

    word_len = strlen(first_word);

    if (first_word[word_len - 1] == COLON)
    {
        is_label = 1;
        if (is_valid_label(first_word, line))
        {
            FOUND_ERROR_IN_FIRST_PASS = 1;
        }
        first_word[word_len - 1] = NULL_CHAR;
    }

    second_word = is_label ? get_word(NULL) : first_word;
    instruction_index = which_instruction(second_word);

    if (instruction_index != -1) // אם זו הנחיה
    {
        switch (instruction_index)
        {
        case 0: // data
            if (is_label)
            {
                add_symbol(line, first_word, instruction_index, is_code);
            }
            if (get_data(line, instruction_index, &numbers))
            {
                FOUND_ERROR_IN_FIRST_PASS = 1;
            }
            else
            {
                add_data(data_image, numbers, line);
            }

            break;
        case 1: // string

            if (is_label)
            {
                add_symbol(line, first_word, instruction_index, is_code);
            }
            if (get_string_data(line, instruction_index, &string_value))
            {
                FOUND_ERROR_IN_FIRST_PASS = 1;
            }
            else
            {
                add_string_data(data_image, string_value, line);
            }
            break;
        case 2: // entry
            second_word = get_word(NULL);
            add_to_ext_list(second_word, line->line_number);
            if (is_label)
            {
                printf("WARNING: label is ignored in entry line\n");
            }
            break; // handle in second pass
        case 3:    // extern
            second_word = get_word(NULL);
            add_to_ext_list(second_word, line->line_number);
            if (is_label)
            {
                printf("WARNING: label is ignored in extern line\n");
            }
            if (is_valid_label(second_word, line))
                FOUND_ERROR_IN_FIRST_PASS = 1;
            else
                add_symbol(line, second_word, instruction_index, is_code);
            break;
        }
        return;
    }

    else // not instruction. its operation
    {
        if(first_word[word_len-1]==COMMA)
        {
            print_syntax_error(ERROR_CODE_28, line->file_name, line->line_number);
            FOUND_ERROR_IN_FIRST_PASS=1;
            return;
        }

        analyse_operation(line, second_word, is_label, first_word, instruction_index, code_image); // ניתוח הקלט (מספר אופרנדים ושיטת מיעון וכו)
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
		add_to_code_image(code_image, line, num_args, operation_list[op_index].opcode,ZERO, ZERO,ZERO, NULL,NULL);
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

		add_to_code_image(code_image,line,num_args,operation_list[op_index].opcode,operation_list[op_index].funct,ZERO,address_method_des, NULL, second_operand);

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
			if(*ptr!=SPACE)
			{
			print_syntax_error(ERROR_CODE_26, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
			}
			print_syntax_error(ERROR_CODE_33, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		while (*ptr == SPACE) // דילוג על רווחים
			ptr++;

		second_operand = get_word(NULL);
		if(second_operand==NULL)
		{
			print_syntax_error(ERROR_CODE_26, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
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
		add_to_code_image(code_image,line,num_args,operation_list[op_index].opcode,operation_list[op_index].funct,address_method_src,address_method_des,first_operand,second_operand);
		break;
	}
}




int add_to_ext_list(char *label_name, int address)
{
	//ext_list *current= ext_list_head;
	ext_list *new_label = (ext_list *)malloc(sizeof(ext_list));
        if (!new_label)
        {
            print_system_error(ERROR_CODE_3);
            return 1;
        }
		new_label->label_name = malloc(strlen(label_name) + 1);
		if (!new_label->label_name) {
			free(new_label); // Free the struct to prevent memory leaks
			print_system_error(ERROR_CODE_3);
			return 1;
		}
		strcpy(new_label->label_name, label_name);
		new_label->address=address;
		new_label->next=ext_list_head;
		ext_list_head=new_label;

		return 0;
}





