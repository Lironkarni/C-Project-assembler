#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"
#include "../headers/memory_struct.h"
#include "../headers/label.h"
#include "../headers/second_pass.h"
#include "../headers/op_list.h"


code_word code_image[MEM_SIZE]; /* Memory array for code instructions */
data_word data_image[MEM_SIZE]; /* Memory array for data instructions */


Symbol *symbol_table_head = NULL;   // pointer to the head of the symbol table
ext_list *ext_table_head=NULL;   // pointer to the head of the extern table


int first_pass(char *file)
{
    process_line(file); /* Process each line from the file */

    /* Check if errors were found during the first pass */
    if (FOUND_ERROR_IN_FIRST_PASS)
    {
        FOUND_ERROR_IN_FIRST_PASS = 0; /* Reset error flag */
        return 1; /* Exit due to errors */
    }

    update_symbol_tabel();
    second_pass(file, symbol_table_head, code_image,data_image); // start second_pass 
    return 0;
}

void process_line(char *file)
{
    FILE *input_file;
    char temp_line[MAX_LINE_LEN + 2], *first_word = NULL; /*for \0 and one more char for overflow check*/
    int line_number = 0, line_len, c;
    Line *line;

    input_file = fopen(file, "r");  /* Open the input file for reading */
    if (input_file == NULL)
    {
        print_system_error(ERROR_CODE_4); /* Report file open error */
    }
    else
    {
        while (fgets(temp_line, sizeof(temp_line), input_file)) /* Read file line by line */
        {
            printf("%s", temp_line); /* Print the current line (for debugging) */
            line_number++;
            line_len = strlen(temp_line);       /* Calculate line length */
            /* Replace newline character with NULL_CHAR */
            if (temp_line[line_len - 1] == '\n') 
                temp_line[line_len - 1] = NULL_CHAR;
            /* Check for line length overflow */
            if (line_len > MAX_LINE_LEN)
                if (temp_line[MAX_LINE_LEN] != NULL_CHAR && temp_line[MAX_LINE_LEN] != '\n')
                {
                    print_syntax_error(ERROR_CODE_8, file, line_number); /* Report line length error */

                    /* Clear remaining characters in the line */
                    while ((c = fgetc(input_file)) != '\n' && c != EOF)
                        ;    
                    continue; /*don't check this row, move to the next one*/
                }

            /*make line struct*/
            line = create_line(temp_line, file, line_number);

            if (line == NULL)
            {
                print_system_error(ERROR_CODE_3);
                exit(1);
            }

            
            first_word = get_word(line->data); /* Extract the first word from the line */
            process_word(line, first_word); /* Process extracted word */
        }
    }
    fclose(input_file); /* Close the input file */
}

void process_word(Line *line, char *first_word)
{
    int word_len, instruction_index, is_code = 0;
    char *second_word, *string_value;
    int *numbers = NULL;
    int is_label = 0;

    word_len = strlen(first_word);

    /* Check if the first word is a label (ends with ':') */
    if (first_word[word_len - 1] == COLON)
    {
        is_label = 1;
        if (is_valid_label(first_word, line))
        {
            FOUND_ERROR_IN_FIRST_PASS = 1;
        }
        first_word[word_len - 1] = NULL_CHAR; /* Remove ':' from label */
    }

    /* Get the next word if label, otherwise use first_word */
    second_word = is_label ? get_word(NULL) : first_word;
    if(second_word[strlen(second_word)-1]== COMMA)
    {
        print_syntax_error(ERROR_CODE_28, line->file_name, line->line_number);
        FOUND_ERROR_IN_FIRST_PASS=1;
        return;
    }
    instruction_index = which_instruction(second_word);

    /* Check if the word is an assembler directive */
    if (instruction_index != -1) // אם זו הנחיה
    {
        switch (instruction_index)
        {
        case 0: /* .data directive */
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
        case 1: /* .string directive */

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
        case 2: /* .entry directive (handled in second pass) */
            second_word = get_word(NULL);
            if (is_label)
            {
                printf("WARNING: label is ignored in entry line\n");
            }
            break; // handle in second pass
        case 3:    /* .extern directive */
            second_word = get_word(NULL);
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

    else /* The word is an assembly operation */
    {
        /* Check for illegal comma after operation */
        if(first_word[word_len-1]==COMMA)
        {
            print_syntax_error(ERROR_CODE_28, line->file_name, line->line_number);
            FOUND_ERROR_IN_FIRST_PASS=1;
            return;
        }

        analyse_operation(line, second_word, is_label, first_word, instruction_index, code_image); /* Analyse operands and addressing modes */
    }
}

void analyse_operation(Line *line, char *second_word, int is_label, char *first_word, int instruction_index, code_word *code_image)
{
	int num_args, is_code = 0, op_index, address_method_src, address_method_des, len, is_comma = 0;
	char *first_operand, *second_operand, *ptr;

    /* Identify operation index in the operation list */
	if (is_label)
		op_index = check_if_operation(second_word);
	else
		op_index = check_if_operation(first_word);

    /* Validate if the operation is recognized */    
	if (op_index != -1)
	{
		is_code = 1;
        /* If there was a label before the operation, add it to the symbol table */
		if (is_label)
		{
			add_symbol(line, first_word, instruction_index, is_code);
		}
	}
	else 
	{
		print_syntax_error(ERROR_CODE_25, line->file_name, line->line_number);  // Unrecognized operation
		FOUND_ERROR_IN_FIRST_PASS = 1;
		return;
	}

    /* Get the number of operands the operation expects */
	num_args = operation_list[op_index].address_method.num_args; 

    /* Move pointer after the operation name to process operands */
	ptr = strstr(line->data, operation_list[op_index].operation_name);
	ptr += strlen(operation_list[op_index].operation_name);
	while (*ptr == SPACE) 
		ptr++;

	switch (num_args)
	{
	case 0:
        /* If the operation expects no operands but extra text exists */
		if (*ptr != NULL_CHAR) 
		{
			print_syntax_error(ERROR_CODE_21, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
        /* Store the operation in the code image */
		add_to_code_image(code_image, line, num_args, operation_list[op_index].opcode,ZERO, ZERO,ZERO, NULL,NULL);
		return;

	case 1:
        /* Check for missing operand */
		if (*ptr == NULL_CHAR) 
		{
			print_syntax_error(ERROR_CODE_26, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
        /* Check if there is an illegal comma between operation and operand */
		while (*ptr && isspace(*ptr)) 
			ptr++;
		if (*ptr == COMMA) // פסיק לא חוקי בין הוראה לאופרנד הראשון
		{
			print_syntax_error(ERROR_CODE_28, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
        /* Check for extra commas */
		if (strchr(ptr, COMMA) != NULL) 
		{
			print_syntax_error(ERROR_CODE_29, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}

        /* Extract the operand */
		second_operand = get_word(NULL);
		len = strlen(second_operand);
		if (second_operand[len - 1] == COMMA)
		{
			print_syntax_error(ERROR_CODE_35, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		/* Validate addressing method */
		address_method_des = which_addressing_method(second_operand, op_index, line);
		if (address_method_des == -1)
        {
            FOUND_ERROR_IN_FIRST_PASS=1;
			return; // there was error. no need to keep analysing this line
        }
		ptr += len;
        /* Check for extraneous text after the operand */
		if (extraneous_text(ptr)) 
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

        /* Store the operation in the code image */
		add_to_code_image(code_image,line,num_args,operation_list[op_index].opcode,operation_list[op_index].funct,ZERO,address_method_des, NULL, second_operand);

		return;
	case 2:
    
		if (*ptr == NULL_CHAR) // missing operand
		{
			print_syntax_error(ERROR_CODE_30, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
		if (*ptr == COMMA) /* Illegal comma between operation and first operand */
		{
			print_syntax_error(ERROR_CODE_28, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
        /* Extract first operand */
		first_operand = get_word(NULL);
		len = strlen(first_operand);
		if (first_operand[len - 1] == COMMA)
		{
			is_comma = 1;
			first_operand[len - 1] = NULL_CHAR; // Remove the comma
		}
        /* Validate source addressing method */
		address_method_src = which_addressing_method(first_operand, op_index, line);
		if (address_method_src == -1)
		{
            FOUND_ERROR_IN_FIRST_PASS=1;
			return;
		}
		ptr += len;

		while (*ptr == SPACE) 
			ptr++;
        
        /* Check if the comma between operands is missing or misplaced */
		if (*ptr != COMMA && !is_comma) 
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
		while (*ptr == SPACE)
			ptr++;

        /* Extract second operand */
		second_operand = get_word(NULL);
		if(second_operand==NULL)
		{
			print_syntax_error(ERROR_CODE_26, line->file_name, line->line_number);
			FOUND_ERROR_IN_FIRST_PASS = 1;
			return;
		}
        /* Skip comma if exists */
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

        /* Validate destination addressing method */
		address_method_des = which_addressing_method(second_operand, op_index, line);
		if (address_method_des == -1)
		{
            FOUND_ERROR_IN_FIRST_PASS=1;
			return;
		}
		ptr += len;
		if (extraneous_text(ptr)) /* Check for extraneous text after both operands */
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

        /* Store the operation in the code image */
		add_to_code_image(code_image,line,num_args,operation_list[op_index].opcode,operation_list[op_index].funct,address_method_src,address_method_des,first_operand,second_operand);
		break;
	}
}

void test(int dc, int ic)
{
    printf("%d\n", dc);
    printf("%d\n", ic);
    // הדפסת טבלת הסמלים
    Symbol *current = symbol_table_head;
    printf("Symbol Table:\n");
    printf("-----------------------------\n");
    while (current)
    {
        printf("Name: %s| Address: %d| Type: %d\n", current->name, current->address, current->type);
        current = current->next;
    }
    printf("-----------------------------\n\n");

    // הדפסת תמונת הנתונים
    printf("Data Image:\n");
    printf("-----------------------------\n");
    for (int i = 0; i < dc; i++)
    {
        printf("Address %d: %06X\n", i+ic, data_image[i].data);
    }
    printf("-----------------------------\n");

    printf("Code Image\n");
    printf("%d\n", ic);
    // הדפסת תמונת הקוד
    for (int i = 100; i < ic; i++)
    {
        uint32_t full_word = 0;
        memcpy(&full_word, &code_image[i], 3); // מעתיקים רק 3 בתים (24 ביטים)

        printf("Address %d: ", i);
        print_bits(full_word, 24); // הדפסת כל הביטים
        printf(" (0x%06X)\n", full_word);
    }
    printf("-----------------------------\n");
}

void print_bits(uint32_t value, int bits)
{
    /* Iterate over each bit and print its value (0 or 1) */
    for (int i = bits - 1; i >= 0; i--)
    {
        printf("%d", (value >> i) & 1);
    }
}