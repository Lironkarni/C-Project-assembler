#include "../headers/label.h"
#include "../headers/macro.h"
#include "../headers/op_list.h"
#include "../headers/error.h"




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
	if(label[len-1]==COLON || label[len-1]==COMMA){
        label[len-1]=NULL_CHAR;
        len--;
    }
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
	while (i < len)
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
	/* Check if symbol already exists */
    if (find_symbol(name) == NULL)
    {
		/* Allocate memory for new symbol */
        Symbol *new_symbol = (Symbol *)malloc(sizeof(Symbol));
        if (!new_symbol)
        {
            print_system_error(ERROR_CODE_3); /* Memory allocation failed */
            return;
        }
		 /* Copy symbol name safely */
        strncpy(new_symbol->name, name,sizeof(new_symbol->name) - 1);
		new_symbol->name[sizeof(new_symbol->name) - 1] = '\0';
		/* Set symbol type and add it to symbol table */
        new_symbol->type = type;
        new_symbol->next = symbol_table_head;
        symbol_table_head = new_symbol;
		/* Set symbol address based on its type */
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
        print_syntax_error(ERROR_CODE_18, line->file_name, line->line_number); /* Symbol already exists */
        return;
    }
}

Symbol *find_symbol(char *name)
{
    Symbol *current = symbol_table_head;

	/* Traverse the symbol table linked list */
    while (current)
    {
		/* Check if the current symbol's name matches the requested name */
        if (strcmp(current->name, name) == 0)
            return current; // Symbol found
        current = current->next;
    }
	/* Symbol not found */
    return NULL;
}

int add_to_ext_list(char *label_name, int address)
{
	/* Allocate memory for the new external label node */
	ext_list *new_label = (ext_list *)malloc(sizeof(ext_list));
        if (!new_label)
        {
            print_system_error(ERROR_CODE_3); // Memory allocation failed
            return 1;
        }
		/* Allocate memory for the label name string */
		new_label->label_name = malloc(strlen(label_name) + 1);
		if (!new_label->label_name) {
			free(new_label); // Free the struct to prevent memory leaks
			print_system_error(ERROR_CODE_3);
			return 1;
		}

		/* Copy the label name */
		strcpy(new_label->label_name, label_name);

		/* Set the address of the external label */
		new_label->address=address;

		/* Insert the new label at the head of the external list */
		new_label->next=ext_table_head;
		ext_table_head=new_label;

		return 0; // Success
}

void update_symbol_tabel()
{
	Symbol *current = symbol_table_head;
	/* Iterate through the symbol table and update symbol addresses */
    while (current)
    {
		/* Update addresses only for non-external symbols with valid types */
        if (current->type!=EXTERNAL && current->type!=-1){
            current->address+=IC; /* Update address with instruction counter offset */
	    }
        current = current->next;/* Move to next symbol */
    }
}

void free_symbols()
{
	Symbol *current = symbol_table_head;
    while (current) {
        Symbol *next = current->next;  // Store next symbol before freeing the current one
        free(current); // Free allocated symbol
        current = next;  // Move to the next macro in the list
    }
	symbol_table_head = NULL;   // Reset the head pointer to NULL after cleanup
}

void free_externs()
{
	ext_list *current= ext_table_head;
	ext_list *next;
	while (current)
	{
		next=current->next;
		free(current->label_name); // Free allocated extern name
		free(current); // Free allocated extern
		current=next;
	}
	ext_table_head = NULL;   // Reset the head pointer to NULL after cleanup
}