#include "../headers/second_pass.h"
#include "../headers/label.h"
#include "../headers/error.h"
#include "../headers/first_pass.h"

void second_pass(char *file, ext_ent_list *ext_ent_list_head, Symbol *symbol_table_head, code_word *code_image)
{
    A_R_E a_r_e = {4, 2, 1};
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
        // go through the file and look if there is .entry
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
                second_word = get_word(NULL);
                // need to make sure label is declared already in label list
                current_symbol = (Symbol *)malloc(sizeof(Symbol));
                current_symbol = find_symbol(second_word);

                if (current_symbol == NULL)
                {
                    print_syntax_error(ERROR_CODE_39, line->file_name, line->line_number);
                    FOUND_ERROR_IN_SECOND_PASS = 1;
                }
                // add type entry to the label we found in label list
                current_symbol->type = ENTRY;
            }
        }

        // check if line has label- if yes, look if label exists in symbol table
        //  we can go over the code_image list, if line in 0, that means its a label.
        for (int i = 100; i < IC; i++)
        {
            code_union code_u;
            code_u.code_w = code_image[i];

            if (code_image[i].op_code == ZERO && code_image[i].funct == ZERO && code_image[i].A_R_E == ZERO && code_image[i].source_reg == ZERO && code_image[i].target_reg == ZERO)
            {
                // it can be in the line before or 2 before
                // the line before can be a number for ex- add #6 HELLO
                if (code_image[i].place == 1)
                {
                    // need to check if this label is in symbol table
                    current_symbol = find_symbol(code_image[i].first_operand);

                    if (current_symbol == NULL)
                    {
                        print_syntax_error(ERROR_CODE_40, line->file_name, line->line_number);
                        FOUND_ERROR_IN_SECOND_PASS = 1;
                        continue;
                    }
                    else
                    {
                        // change the row from ZERO to the address of the label, check if extern or internal,
                        // use address method that we found

                        // need the addressing method of first operand
                        if (code_image[i].source_address == 1)
                        {
                            code_u.all_bits = current_symbol->address;
                            code_u.all_bits <<= THREE_BITS_SHIFT;
                            if (current_symbol->type == EXTERNAL)
                            {
                                code_u.all_bits |= a_r_e.E;
                            }
                            else // if DATA/STRING or entry. not extern
                            {
                                code_u.all_bits |= a_r_e.R;
                            }
                            code_image[i] = code_u.code_w;
                        }
                        // for 2 operand address method 2 is not valid
                    }
                }
                else if (code_image[i].place == TWO) // second operand
                {
                    // need to check if this label is in symbol table
                    if (code_image[i].target_address == 2)
                    {
                        code_image[i].second_operand += 1;
                    }

                    current_symbol = find_symbol(code_image[i].second_operand);
                    if (current_symbol == NULL)
                    {
                        print_syntax_error(ERROR_CODE_40, line->file_name, line->line_number);
                        FOUND_ERROR_IN_SECOND_PASS = 1;
                        continue;
                    }
                    else
                    {
                        if (code_image[i].target_address == 1)
                        {
                            code_u.code_w.target_address = ZERO;
                            code_u.code_w.source_address = ZERO;

                            code_u.all_bits = current_symbol->address;
                            code_u.all_bits <<= THREE_BITS_SHIFT;
                            if (current_symbol->type == EXTERNAL)
                            {
                                code_u.all_bits |= a_r_e.E;
                            }
                            else // if DATA/STRING or entry. not extern
                            {
                                code_u.all_bits |= a_r_e.R;
                            }

                            code_image[i] = code_u.code_w;
                        }

                        else if (code_image[i].target_address == 2)
                        {
                            int num = (current_symbol->address) - (i - 1); // caculate the between the addresses

                            code_u.code_w.target_address = ZERO;
                            code_u.code_w.source_address = ZERO;
                            code_u.all_bits = ZERO;
                            code_u.all_bits = num;
                            code_u.all_bits <<= THREE_BITS_SHIFT;
                            code_u.all_bits |= a_r_e.A;

                            code_image[i] = code_u.code_w;
                        }
                    }
                }
            }
        }
    }

    fclose(input_file);
    test(DC, IC);

    if (FOUND_ERROR_IN_SECOND_PASS != 0)
    {
        return;
    }

    // need to build the output files
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
