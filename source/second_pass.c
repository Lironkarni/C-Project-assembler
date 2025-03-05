#include "../headers/second_pass.h"
#include "../headers/label.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"

void second_pass(char *file, ext_ent_list *ext_ent_list_head, Symbol *symbol_table_head)
{
    // if (check_externs(ext_ent_list_head, symbol_table_head))
    // {
    //     FOUND_ERROR_IN_SECOND_PASS = 1;
    // }

    FILE *input_file;
    char temp_line[MAX_LINE_LEN + 2];
    int line_number=0, line_len;
    Line *line;
    char *first_word, *second_word;

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
            second_word = get_word(NULL);

            int inst_index=which_instruction(first_word);
            if(inst_index==TWO)// its entry
            {
                //need to make sure label is declared already in label list
                if(find_symbol(second_word)!=NULL)
                {
                    print_syntax_error(ERROR_CODE_39, line->file_name, line->line_number);
                    FOUND_ERROR_IN_SECOND_PASS=1;
                }
            }
            //add type entry to the label in label list
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
