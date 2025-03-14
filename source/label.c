#include "../headers/label.h"

char *instruction_list[] = {".data", ".string", ".entry", ".extern"};

Symbol *symbol_table_head = NULL;


int is_valid_label(char *label, Line *line)
{
	/*check its starts with alphbetic char
	check all chars are alphabetic or digits
	check if length is lower than 32 (MAX_LABEL_LENGTH)
	check if label name is saved assembly name
	check if label as been defind yet
	check if this is not a name of a macro or name of register*/
	int i = 0, len;
	int op;
	len = strlen(label);
	if (!isalpha(label[0]))
	{
		// label name is not starting with alphabet character
		print_syntax_error(ERROR_CODE_10, line->file_name, line->line_number);
		return 1;
	}
	if (len > MAX_LABEL_LENGTH)
	{
		print_syntax_error(ERROR_CODE_11, line->file_name, line->line_number);
		return 1;
	}
	while (i < len - 1)
		if (!isalnum(label[i++]))
		{
			// label name have non alpha-numeric character in it
			print_syntax_error(ERROR_CODE_12, line->file_name, line->line_number);
			return 1;
		}

	if (find_macro(label) != NULL)
	{
		// label name is equals to macro
		print_syntax_error(ERROR_CODE_17, line->file_name, line->line_number);
		return 1;
	}

	// see if its saved assembly name
	op = check_if_operation(label);
	if (op != -1)
	{
		// label name is equals to operation
		print_syntax_error(ERROR_CODE_9, line->file_name, line->line_number);
		return 1;
	}
	if(is_register(label))
	{
		print_syntax_error(ERROR_CODE_19, line->file_name, line->line_number);
		return 1;
	}
	return 0;
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
        strncpy(new_symbol->name, name,sizeof(new_symbol->name) - 1);
		new_symbol->name[sizeof(new_symbol->name) - 1] = '\0';
        new_symbol->type = type;
        new_symbol->next = symbol_table_head;
        symbol_table_head = new_symbol;
        if (instruction_index == EXTERN_INDEX)
        {
            new_symbol->address = ZERO; 
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

void update_symbol_tabel()
{
	Symbol *current = symbol_table_head;
    while (current)
    {
        if (current->type!=EXTERNAL && current->type!=-1)
            current->address+=IC;
        current = current->next;
    }
}