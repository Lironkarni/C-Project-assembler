#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"
#include "../headers/process_input.h"
#include "../headers/memory_struct.h"

code_word code_image[MEM_SIZE];
data_word data_image[MEM_SIZE];

char *instruction_list[] = {".data", ".string", ".entry", ".extern"};

Symbol *symbol_table_head = NULL;

/*start the first pass*/
void first_pass(char *file)
{
    process_line(file);
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
                temp_line[line_len - 1] = '\0';

            if (line_len > MAX_LINE_LEN)
                if (temp_line[MAX_LINE_LEN] != '\0' && temp_line[MAX_LINE_LEN] != '\n')
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
        test(DC, IC);
    }
    fclose(input_file);
}

void process_word(Line *line, char *first_word)
{
    op_code curr_op;
    int word_len, instruction_index, is_code = 0;
    char *second_word, *string_value;
    int *numbers = NULL;
    int is_label = 0;

    printf("first word is %s\n", first_word);
    word_len = strlen(first_word);

    if (first_word[word_len - 1] == COLON)
    {
        is_label = 1;
        if (is_valid_label(first_word, line))
        {
            printf("found error in label %s\n", line->data);
            FOUND_ERROR_IN_FIRST_PASS = 1;
        }
        first_word[word_len - 1] = '\0';
    }

    second_word = is_label ? get_word(NULL) : first_word;
    printf("second word is: %s\n", second_word);
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
            if (is_label)
            {
                printf("WARNING: label is ignored in entry line\n");
            }
            break; // handle in second pass
        case 3:    // extern
            if (is_label)
            {
                printf("WARNING: label is ignored in extern line\n");
            }
            second_word = get_word(NULL);
            add_symbol(line, second_word, instruction_index, is_code);
            break;
        }
        return;
    }

    else // not instruction. its operation
    {
        analyse_operation(line, second_word, is_label, first_word, instruction_index, code_image); // ניתוח הקלט (מספר אופרנדים ושיטת מיעון וכו)
    }
}

int which_instruction(char *word)
{
    int i;
    for (i = 0; i < INSTRUCTION_COUNT; i++)
    {
        if (strcmp(word, instruction_list[i]) == 0)
            return i;
    }
    return -1;
}

void add_symbol(Line *line, char *name, int instruction_index, int is_code)
{
    guide_type type = (guide_type)instruction_index;
    if (find_symbol(name) == NULL)
    {
        Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
        if (!new_symbol)
        {
            print_system_error(ERROR_CODE_3);
            return;
        }

        strcpy(new_symbol->name, name);
        new_symbol->type = type;
        new_symbol->next = symbol_table_head;
        symbol_table_head = new_symbol;
        if (instruction_index == EXTERN_INDEX)
        {
            new_symbol->address = -100; // sholdn't it be 0?
        }
        else if (is_code)
        {
            new_symbol->address = IC;
        }
        else
        {
            new_symbol->address = DC;
        }
    }
    else
    {
        print_syntax_error(ERROR_CODE_18, line->file_name, line->line_number);
        return;
    }
}

Symbol *find_symbol(char *name)
{
    Symbol *current = symbol_table_head;
    while (current)
    {
        if (strcmp(current->name, name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

/*this function get the data if its ".data", and check if its valid or there are errors*/
int get_data(Line *line, int inst_index, int **numbers)
{
    int expect_number = 1; // 0- expect number, 1- expect comma
    int capacity, count = 0;
    char *data_ptr = strstr(line->data, instruction_list[inst_index]);
    data_ptr += strlen(instruction_list[inst_index]);
    while (*data_ptr == ' ')
        data_ptr++;

    capacity = INIT_CAPACITY;
    *numbers = (int *)malloc(capacity * sizeof(int));
    if (*numbers == NULL)
    {
        print_system_error(ERROR_CODE_3);
        exit(1);
    }
    while (*data_ptr != '\0')
    {
        if (expect_number)
        {
            if (*data_ptr == '-' || *data_ptr == '+')
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
            (*numbers)[count++] = strtol(data_ptr, &data_ptr, DECIMAL);
            expect_number = 0; // expecting now a comma
        }

        else
        {
            if (*data_ptr != COMMA)
            {
                print_syntax_error(ERROR_CODE_23, line->file_name, line->line_number);
                free(*numbers);
                return 1;
            }
            expect_number = 1;
            data_ptr++;
        }
        while (*data_ptr == ' ')
            data_ptr++;
    }
    // check if line ended with comma
    if (expect_number)
    {
        print_syntax_error(ERROR_CODE_24, line->file_name, line->line_number);
        free(*numbers);
        return 1;
    }
    (*numbers)[count] = '\0';
    return 0;
}

int get_string_data(Line *line, int inst_index, char **characters)
{
    int capacity = INIT_CAPACITY, length = 0;
    char *char_array;
    char *data_ptr = strstr(line->data, instruction_list[inst_index]);

    if (!data_ptr)
    {
        print_syntax_error(ERROR_CODE_22, line->file_name, line->line_number);
        return 1;
    }

    data_ptr += strlen(instruction_list[inst_index]);

    while (*data_ptr == SPACE) // דילוג על רווחים
        data_ptr++;

    if (*data_ptr != QUOTE)
    { // אם המחרוזת לא מתחילה בגרשיים - שגיאה
        print_syntax_error(ERROR_CODE_20, line->file_name, line->line_number);
        return 1;
    }

    data_ptr++; // מעבר אחרי המרכאה הפותחת

    char_array = (char *)malloc(capacity * sizeof(char));
    if (!char_array)
    {
        print_system_error(ERROR_CODE_3);
        exit(1);
    }

    // קריאת המחרוזת עד המרכאה הסוגרת
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

    if (*data_ptr != QUOTE)
    { // אם אין מרכאה סוגרת - שגיאה
        print_syntax_error(ERROR_CODE_20, line->file_name, line->line_number);
        free(char_array);
        return 1;
    }

    char_array[length] = NULL_CHAR; // סימון סוף מחרוזת
    *characters = char_array;

    data_ptr++; // מעבר אחרי המרכאה הסוגרת

    // דילוג על רווחים אחרי המחרוזת
    while (*data_ptr == SPACE)
        data_ptr++;

    // אם יש עוד תווים אחרי המחרוזת - שגיאה
    if (*data_ptr != NULL_CHAR)
    {
        print_syntax_error(ERROR_CODE_21, line->file_name, line->line_number);
        free(char_array);
        return 1;
    }

    return 0; // הצלחה
}

void test(int dc, int ic)
{
    printf("%d\n", dc);
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
        printf("Address %d: %06X\n", i, data_image[i].data);
    }
    printf("-----------------------------\n");

    printf("Code Image\n");
    printf("%d\n", ic);
    // הדפסת תמונת הקוד
    for (int i = 100; i < ic; i++)
    {
        // בניית 24 ביטים מהמבנה
        uint32_t full_word = (code_image[i].op_code << 18) |
                             (code_image[i].source_address << 16) |
                             (code_image[i].source_reg << 13) |
                             (code_image[i].target_address << 11) |
                             (code_image[i].target_reg << 8) |
                             (code_image[i].funct << 3) |
                             (code_image[i].A_R_E);

        printf("Address %d: ", i);
        print_bits(full_word, 24); // הדפסת כל הביטים
        printf(" (0x%06X)\n", full_word);
    }
    printf("-----------------------------\n");
}

void print_bits(uint32_t value, int bits)
{
    for (int i = bits - 1; i >= 0; i--)
    {
        printf("%d", (value >> i) & 1);
    }
}