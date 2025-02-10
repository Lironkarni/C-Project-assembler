#include "../headers/globals.h"
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"
#include "../headers/process_input.h"

static int IC = 100;
static int DC = 0; 

char* instruction_list[]={".data", ".string", ".entry", ".extern"};

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
    char *line_ptr, *second_word;

    printf("first word is %s\n", first_word);
    /*check if its instructive sentence*/
    curr_op = check_if_instruction(first_word);
    if (strcmp(curr_op.operation_name, "0") == 0)
    {
        printf("not operation, its or giuding or label\n");
        /*if its label, need to check if its valid label's name*/
        word_len = strlen(first_word);
        if (first_word[word_len - 1] == COLON)
        {                                                /*if its label*/
            is_label = is_valid_label(first_word, line); /* if its valid label- is_label=0*/

            second_word = get_word(NULL);
            printf("second word is: %s\n", second_word);
            instruction_index=which_instruction(second_word);
            switch (instruction_index)
            {
            case 0:
            case 1:
            case 2:
            case 3:
                /* code */
                add_symbol(line,second_word,DC,(guide_type)instruction_index);
                break;
            case -1:
                printf("first word- label, second word- NOT instruction, maybe a operation\n");
                break;    
            default:
                break;
            }
        }
    }
    else /*its operation*/
    {
        /*need to check how many arguments*/
        line_ptr = line->data; // Create a pointer to the start of the line
        // memcpy(line_ptr, line + strlen(curr_op.operation_name), strlen(line));
        // remove_spaces(line_ptr);
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

int which_instruction(char *word)
{
    int i;
    for(i=0;i<INSTRUCTION_COUNT;i++){
        if(strcmp(word, instruction_list[i])==0)
        return i;
    }
    return -1;
}

void add_symbol(Line *line,char *name, int address, guide_type type)
{
    printf("%s %d %d\n",name,address, type);
    if(find_symbol(name)!=NULL){
        Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (!new_symbol)
    {
        print_system_error(ERROR_CODE_3);
        return;
    }
    strcpy(new_symbol->name, name);
    new_symbol->address = address;
    new_symbol->type= type;
    new_symbol->next = symbol_table_head;
    symbol_table_head = new_symbol;
    }
    else{
        print_syntax_error(ERROR_CODE_18,line->file_name, line->line_number);
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