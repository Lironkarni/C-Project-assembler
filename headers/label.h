#ifndef LABEL_H
#define LABEL_H
#include "../headers/utils.h"
#include "../headers/error.h"
#include "../headers/process_input.h"

#define MAX_LABEL_LENGTH 31


typedef enum guide_type
{
	DATA,
	STRING,
	ENTRY,
	EXTERNAL,
	OPERATION
}guide_type;

typedef struct symbol {
    char name[MAX_LABEL_LENGTH]; 
    int address;                 
    guide_type type;                
    struct symbol *next;           
} Symbol;

typedef struct ext_list
{
	char *label_name;
	int address;
	struct ext_list *next;
}ext_list;

extern Symbol *symbol_table_head;

int is_valid_label(char *label, Line *line);
void add_symbol(Line *line, char *name, int instruction_index, int is_code);
Symbol *find_symbol(char *name);
void update_symbol_tabel();
int add_to_ext_list(ext_list *ext_list_head, char *label_name, int address);

#endif