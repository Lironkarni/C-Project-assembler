#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"
#include "../headers/process_input.h"
#include "../headers/memory_struct.h"
#include "../headers/label.h"
#include "../headers/second_pass.h"

code_word code_image[MEM_SIZE]; /* Memory array for code instructions */
data_word data_image[MEM_SIZE]; /* Memory array for data instructions */

char *instruction_list[] = {".data", ".string", ".entry", ".extern"};

Symbol *symbol_table_head = NULL;
ext_list *ext_table_head=NULL;


int first_pass(char *file)
{
    process_line(file); /* Process each line from the file */
    /* Check if errors were found during the first pass */
    if (FOUND_ERROR_IN_FIRST_PASS)
    {
        FOUND_ERROR_IN_FIRST_PASS = 0; /* Reset error flag */
        return 1; /* Exit due to errors */
    }

    //update symbol table
    update_symbol_tabel();
    //test(DC,IC);
    second_pass(file, symbol_table_head, code_image,data_image); // second_pass
    // test(DC,IC);   
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
                /* TODO: need to free all memery*/
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

int get_data(Line *line, int inst_index, int **numbers)
{
    int expect_number = 1; // 0- expect number, 1- expect comma
    int capacity, count = 0;
    char *data_ptr = strstr(line->data, instruction_list[inst_index]);
    /* Move pointer past directive keyword */
    data_ptr += strlen(instruction_list[inst_index]);
    while (*data_ptr == ' ')
        data_ptr++;

    /* Allocate initial memory for numbers array */
    capacity = INIT_CAPACITY;
    *numbers = (int *)malloc(capacity * sizeof(int));
    if (*numbers == NULL)
    {
        print_system_error(ERROR_CODE_3);
        exit(1);
    }
    while (*data_ptr != NULL_CHAR)
    {
        if (expect_number)
        {
            char *end_ptr;
            /* Check if sign character is followed by a digit */
            if (*data_ptr == MINUS || *data_ptr == PLUS)
            {
                if (!isdigit(*(data_ptr + 1)))
                { // אם אחרי '-' או '+' לא בא מספר - שגיאה
                    print_syntax_error(22, line->file_name, line->line_number);
                    free(*numbers);
                    return 1;
                }
            }
            else if (!isdigit(*data_ptr))
            {
                print_syntax_error(ERROR_CODE_22, line->file_name, line->line_number);
                free(*numbers);
                return 1;
            }
            /* Convert string to integer */
            int num=strtol(data_ptr, &end_ptr, DECIMAL);
            if (*end_ptr == '.') { 
                print_syntax_error(ERROR_CODE_41, line->file_name, line->line_number);
                free(*numbers);
                return 1;
            }
            /* Resize numbers array if necessary */
            if (count >= capacity)
            {
                capacity *= 2;
                *numbers = (int *)realloc(*numbers, capacity * sizeof(int)); 
                if (*numbers == NULL)
                {
                    print_system_error(ERROR_CODE_3);
                    exit(1);
                }
            }
            (*numbers)[count++] = num; /* Store number in array */
            expect_number = 0;  /* Next character should be comma */
            data_ptr= end_ptr;
        }
        else
        {
            if (*data_ptr != COMMA)
            {
                print_syntax_error(ERROR_CODE_23, line->file_name, line->line_number);
                free(*numbers);
                return 1;
            }
            expect_number = 1; /* Next we expect a number */
            data_ptr++;
        }
        /* Skip spaces between numbers/commas */
        while (*data_ptr == SPACE)
            data_ptr++;
    }
    /* Check if data instruction ends incorrectly with a comma */
    if (expect_number)
    {
        print_syntax_error(ERROR_CODE_24, line->file_name, line->line_number);
        free(*numbers);
        return 1;
    }
    (*numbers)[count] = NULL_CHAR;
    return 0; /* Success */
}

int get_string_data(Line *line, int inst_index, char **characters)
{
    int capacity = INIT_CAPACITY, length = 0;
    char *char_array;
    char *data_ptr = strstr(line->data, instruction_list[inst_index]);

    /* Check if directive exists in the line */
    if (!data_ptr)
    {
        print_syntax_error(ERROR_CODE_22, line->file_name, line->line_number);
        return 1;
    }

    /* Move pointer past directive keyword */
    data_ptr += strlen(instruction_list[inst_index]);

    /* Skip initial spaces */
    while (*data_ptr == SPACE) // דילוג על רווחים
        data_ptr++;

    /* Check if string begins with quotation mark */
    if (*data_ptr != QUOTE)
    { 
        print_syntax_error(ERROR_CODE_20, line->file_name, line->line_number);
        return 1;
    }

    data_ptr++;  /* Skip opening quotation mark */

    /* Allocate initial memory for storing string */
    char_array = (char *)malloc(capacity * sizeof(char));
    if (!char_array)
    {
        print_system_error(ERROR_CODE_3);
        exit(1);
    }

    /* Read characters until closing quotation mark */
    while (*data_ptr && *data_ptr != QUOTE)
    {
        if (length >= capacity)
        {
            capacity *= 2;
            char_array = (char *)realloc(char_array, capacity * sizeof(char));
            if (!char_array)
            {
                print_system_error(ERROR_CODE_3);
                exit(1);
            }
        }
        char_array[length++] = *data_ptr++;
    }

    /* Check for missing closing quotation mark */
    if (*data_ptr != QUOTE)
    { 
        print_syntax_error(ERROR_CODE_20, line->file_name, line->line_number);
        free(char_array);
        return 1;
    }

    char_array[length] = NULL_CHAR;  /* Mark end of string */
    *characters = char_array;

    data_ptr++; /* Move past closing quotation mark */

    /* Skip trailing spaces */
    while (*data_ptr == SPACE)
        data_ptr++;

     /* Check for extraneous characters after the closing quotation mark */
    if (*data_ptr != NULL_CHAR)
    {
        print_syntax_error(ERROR_CODE_21, line->file_name, line->line_number);
        free(char_array);
        return 1;
    }

    return 0;/* Success */
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
        // בניית 24 ביטים מהמבנה
        // uint32_t full_word = (code_image[i].op_code << 18) |
        //                      (code_image[i].source_address << 16) |
        //                      (code_image[i].source_reg << 13) |
        //                      (code_image[i].target_address << 11) |
        //                      (code_image[i].target_reg << 8) |
        //                      (code_image[i].funct << 3) |
        //                      (code_image[i].A_R_E);
        uint32_t full_word = 0;
        memcpy(&full_word, &code_image[i], 3); // מעתיקים רק 3 בתים (24 ביטים)

        printf("Address %d: ", i);
        print_bits(full_word, 24); // הדפסת כל הביטים
        printf(" (0x%06X)\n", full_word);
    }
    printf("-----------------------------\n");

    // ext_ent_list *current_ext = ext_ent_list_head;
    // printf("extern entry Table:\n");
    // printf("-----------------------------\n");
    // while (current_ext)
    // {
    //     printf("Name: %s| Type: %d| line_number: %d\n", current_ext->label_name, current_ext->type, current_ext->line->line_number);
    //     current_ext = current_ext->next;
    // }
    // printf("-----------------------------\n\n");
}

void print_bits(uint32_t value, int bits)
{
    /* Iterate over each bit and print its value (0 or 1) */
    for (int i = bits - 1; i >= 0; i--)
    {
        printf("%d", (value >> i) & 1);
    }
}