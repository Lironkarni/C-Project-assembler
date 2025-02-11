#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"
#include "../headers/process_input.h"

static int IC = 100;
static int DC = 0;

char *instruction_list[] = {".data", ".string", ".entry", ".extern"};

Symbol *symbol_table_head = NULL;

/*start the first pass*/
int first_pass(char *file)
{
    Symbol *symbol_table_head = NULL;

    if (process_line(file) != 0)
    {
        /*if errors found, dont make obj file and entries, externals but continue to go over this file*/
        printf("found errors, don't move to next file\n");
    }
    else
    {
        printf("start second pass");
    }
    return 0;
}

int process_line(char *file)
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
    }
    fclose(input_file);
    return 0;
}

void process_word(Line *line, char *first_word)
{
    op_code curr_op;
    int word_len, is_label, num_args, instruction_index;
    char *line_ptr, *second_word, *string_value;
    int *numbers = NULL;
    int is_label = 0;

    printf("first word is %s\n", first_word);
    word_len = strlen(first_word);

    if (first_word[word_len - 1] == COLON){
        is_label = 1;
        if(!is_valid_label(first_word, line)){
            //ERROR invalid label - return
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
            case 0: //data
                if (is_label) {
                    add_symbol(line, first_word, DC, (guide_type)instruction_index);
                }
                get_data(line, instruction_index, &numbers);
                break;
            case 1: //string
                string_value = get_word(NULL);
                if (is_label) {
                    add_symbol(line, first_word, DC, (guide_type)instruction_index);
                }
                if (if_valid_string(string_value, line))
                {
                    FOUND_ERROR_IN_FIRST_PASS = 1;
                }
                break;
            case 2://entry
            case 3://extern
                /* code */
                add_symbol(line, second_word, DC, (guide_type)instruction_index);
                // need here to allocate memory to the data and update DC
                break;
            }
        return;
    }


    /*check if its instructive sentence*/
    curr_op = check_if_instruction(second_word);
    if (strcmp(curr_op.operation_name, "0") != 0)
    {
        if (is_label) {
            add_symbol(line, first_word, IC, (guide_type)instruction_index);
        }    

        /*find which case is this by number of arguments*/
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

void add_symbol(Line *line, char *name, int address, guide_type type)
{
    printf("%s %d %d\n", name, address, type);
    if (find_symbol(name) != NULL)
    {
        Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
        if (!new_symbol)
        {
            print_system_error(ERROR_CODE_3);
            return;
        }
        strcpy(new_symbol->name, name);
        new_symbol->address = address;
        new_symbol->type = type;
        new_symbol->next = symbol_table_head;
        symbol_table_head = new_symbol;
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

int if_valid_string(char *word, Line *line)
{
    int len;
    len = strlen(word);
    if (word[0] != QUOTE || word[len - 1] != QUOTE)
    {
        print_syntax_error(ERROR_CODE_20, line->file_name, line->line_number);
        return 1;
    }
    if (get_word(NULL) != NULL)
    {
        print_syntax_error(ERROR_CODE_21, line->file_name, line->line_number);
        return 1;
    }
    // TODO- more possible errors  ??
    return 0;
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
            if (!isdigit(*data_ptr))
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
    return 0;
}