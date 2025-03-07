#include "../headers/second_pass.h"
#include "../headers/label.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"

void second_pass(char *file, ext_ent_list *ext_ent_list_head, Symbol *symbol_table_head, code_word *code_image)
{
    FILE *input_file;
    char temp_line[MAX_LINE_LEN + 2];
    int line_number = 0, line_len, word_len, is_label = 0, op_index;
    Line *line;
    char *first_word, *second_word;
    Symbol *current_symbol;

    input_file = fopen(file, "r");
    if (input_file == NULL)
    {
        print_system_error(ERROR_CODE_4);
    }
    else
    {
        while (fgets(temp_line, sizeof(temp_line), input_file))
        {
            line_number++;
            line_len = strlen(temp_line);        /*get line length*/
            if (temp_line[line_len - 1] == '\n') /*put '\0' at the end of each line*/
                temp_line[line_len - 1] = NULL_CHAR;

            line = create_line(temp_line, file, line_number);
            first_word = get_word(line->data);
            word_len = strlen(first_word);

            if (first_word[word_len - 1] == COLON)
                is_label = 1;
            second_word = is_label ? get_word(NULL) : first_word;

            int inst_index = which_instruction(first_word);
            if (inst_index == TWO) // its entry
            {
                // need to make sure label is declared already in label list

                current_symbol = find_symbol(second_word);
                current_symbol = (Symbol *)malloc(sizeof(Symbol));
                if (!current_symbol)
                {
                    print_system_error(ERROR_CODE_3);
                    return;
                }
                if (current_symbol == NULL)
                {
                    print_syntax_error(ERROR_CODE_39, line->file_name, line->line_number);
                    FOUND_ERROR_IN_SECOND_PASS = 1;
                }
                // add type entry to the label we found in label list
                current_symbol->type = ENTRY;
            }

            // check if line has label- if yes, look if label exists in symbol table

            else if (op_index = check_if_operation(second_word) != -1) // its operation
            {
                printf("operation in second pass");

                //we can go over the code_image list, if line in 0, that means its a label. 
                for(int i=100;i<IC;i++)
                {
                    code_union code_u;
                    code_u.all_bits = code_image[i]; 
                    if(code_u.all_bits==ZERO)//this in NOT good, we have op_code 0- need to understand what to write here
                    {
                        //it can be in the line before or 2 before
                        //the line before can be a number for ex- add #6 HELLO
                    }
                }
            }
        }
    }
}

int check_externs(ext_ent_list *ext_ent_list_head, Symbol *symbol_table_head)
{
    ext_ent_list *current_ext = ext_ent_list_head;
    while (current_ext)
    {
        if (current_ext->type == TWO)
        {
            current_ext = current_ext->next;
            continue;
        }
        Symbol *current_symbol = symbol_table_head;
        while (current_symbol)
        {
            if (strcmp(current_ext->label_name, current_symbol->name) == 0)
            {
                print_syntax_error(ERROR_CODE_38, current_ext->line->file_name, current_ext->line->line_number);
                return 1;
            }
            current_symbol = current_symbol->next;
        }

        current_ext = current_ext->next;
    }

    return 0;
}
